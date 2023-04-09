#ifndef MATRIXPOINTS
#define MATRIXPOINTS

#include "utils.h"


class MatrixPoints {
 private:
  int num_points;
  int dimension;
  double minDistanceToPointSet(const point& p, const std::vector<point>& point_set);
 public:
  MatrixPoints() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
  }
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
  point takeRandomPoint();
  vector<point> farthestPoints(vector<point> service_points, int LRCsize);
  double calculate_Pmedian(vector<point> solution_service_points);
};

// La distancia de cada punto a service_points es la distancia mínima con un punto
// de service_points
double MatrixPoints::minDistanceToPointSet(const point& p, const std::vector<point>& point_set) {
  double min_distance = std::numeric_limits<double>::max(); 
  for (const point& other_point : point_set) {
    double current_distance = distanceBetweenPoints(p, other_point);   
      if (current_distance < min_distance) {
        min_distance = current_distance;
      }
  }
  return min_distance;
}

// Devuelve un vector con los LRCsize puntos mas alejados de service_points
std::vector<point> MatrixPoints::farthestPoints(std::vector<point> service_points, int LRCsize) {
  std::vector<point> farthest_points;
  // Itera sobre los puntos en pointsMatrix y calcula las distancias mínimas a service_points
  std::vector<double> min_distances;
  vector<int> id_points;
  for (const point& current_point : this->pointsMatrix) {
    if (std::find(service_points.begin(), service_points.end(), current_point) != service_points.end()) continue;
    //cout << "Punto " << current_point.first << " con distancia minima: " <<  minDistanceToPointSet(current_point, service_points) << endl;
    // Calcula la distancia mínima al conjunto de puntos service_points
    double min_distance = minDistanceToPointSet(current_point, service_points);
    // Almacena la distancia en el vector min_distances
    min_distances.push_back(min_distance);
    id_points.push_back(current_point.first);
  }
  // Encuentra los índices de los LRCsize puntos más alejados
  for (int i = 0; i < LRCsize; i++) {
    double max_distance = -1;
    int max_distance_index = -1;
    for (size_t j = 0; j < min_distances.size(); j++) {
      if (min_distances[j] > max_distance) {
        max_distance = min_distances[j];
        max_distance_index = id_points[j];
      }
    }
    if (max_distance_index != -1) {
      farthest_points.push_back(pointsMatrix[max_distance_index]);
      // Establece la distancia del punto seleccionado en -1 para que no se vuelva a seleccionar
      min_distances[max_distance_index] = -1;
    }
  }
  return farthest_points;
}

point MatrixPoints::takeRandomPoint() {
  int random_index = std::rand() % pointsMatrix.size();
  return pointsMatrix[random_index];
}

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
  if (service_points.size() != grouping.size()) throw std::runtime_error("The number of groupings must be the same as the number of service points");
  return grouping;
}

// Calcula la P mediana para los puntos de servicio pasados por parámetro
double MatrixPoints::calculate_Pmedian(vector<point> solution_service_points) {
  // Suma de la distancia de cada punto a su servicio asociado
  // Construir agrupamiento
  // Para cada punto calcular la distancia a su punto de servicio y sumar todas distancias
  vector<vector<point> > groupings = this->buildGroupings(solution_service_points);
  double p_median = 0;
  for(int i = 0; i < groupings.size(); i++) {
    for(const point& current_point: groupings[i]) {
      p_median += this->distanceBetweenPoints(solution_service_points[i], current_point);
    }
  }
  return p_median;
}

#endif