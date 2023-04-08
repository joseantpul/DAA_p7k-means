#ifndef MATRIXPOINTS
#define MATRIXPOINTS

#include "utils.h"


class MatrixPoints {
 private:
  int num_points;
  int dimension;
 public:
  std::vector<point> pointsMatrix;

  int get_dimension() {
    return this->dimension;
  }
  void load(string filename);
  int number_of_points() {
    return this->num_points;
  }
  double distanceBetweenPoints(point otherp, point otherp2); // distancia euclidea
  std::vector<std::vector<point> > buildGroupings(std::vector<point> service_points);
};


void MatrixPoints::load(string filename) {
  pointsMatrix.clear();
  std::ifstream ifile(filename);
  if (!ifile) {
    std::cerr << "Error, could not open the file with the instance" << std::endl;
    return;
  }
  int m, n;
  ifile >> m >> n;
  this->num_points = m;
  this->dimension = n;
  std::string line;
  std::getline(ifile, line); // Leer el salto de línea después de n

  for (int i = 0; i < m; ++i) {
    std::getline(ifile, line);
    std::replace(line.begin(), line.end(), ',', '.');
    std::istringstream iss(line);
    point_values valores(n);
    for (int j = 0; j < n; ++j) {
      iss >> valores[j];
    }
    this->pointsMatrix.push_back(std::make_pair(i, valores));
  }
}

double MatrixPoints::distanceBetweenPoints(point otherp, point otherp2) { // Distancia euclidea
  double distance = 0;
  for (int k = 0; k < otherp.second.size(); k++) {
    distance += pow(otherp.second[k] - otherp2.second[k], 2);
  }
  return sqrt(distance);
}

// Given the service points calculates the groupings by assigning each point to 
// its nearest service point
std::vector<std::vector<point> > MatrixPoints::buildGroupings(std::vector<point> service_points) {
  std::vector<std::vector<point> > grouping;
  grouping.resize(service_points.size(), {});
    // Para cada punto de la matriz de coordenadas de instancia
  for (const point& puntoActual : this->pointsMatrix) {
    if (std::find(service_points.begin(), service_points.end(), puntoActual) != service_points.end()) continue;//<---------Si el punto esta en service points
    int indiceCentroideMasCercano = -1;
    double menorDistancia = std::numeric_limits<double>::max();
      // Para cada punto de servicio i
    for (int i = 0; i < service_points.size(); i++) {
      double distancia = distanceBetweenPoints(service_points[i], puntoActual);
        // Si la distancia calculada es menor que la menorDistancia actual
      if (distancia < menorDistancia) {
        indiceCentroideMasCercano = i;
        menorDistancia = distancia;
      }
    }
      // Suponemos que los atributos clusters y centroides están en concordancia
    int indiceClusterAlquepertenece = indiceCentroideMasCercano;
    if (indiceClusterAlquepertenece != -1) {
      grouping[indiceClusterAlquepertenece].push_back(puntoActual);
    } else {
      std::cerr << "Error: No se encontró el centroide más cercano" << std::endl;
    }
  }
  return grouping;
}

#endif