#ifndef GRASP
#define GRASP

#include "matrixPoints.h"
#include "solution.h"

class Grasp {

 public:
  Grasp() {}

  int number_of_points() {
    return this->matrix_points.number_of_points();
  }

  void load(string filename) {
    this->centroids.clear();
    this->clusters.clear();
    matrix_points.load(filename);
  }

  Solution grasp_algorithm(int k, int LRCsize) {
    this->numClusters = k;
    this->LRCsize = LRCsize;
    this->isInSolution.resize(number_of_points(), false);
    for(int i = 0; i < numClusters; i++) {
      this->generateLRC();
      this->centroids.push_back(this->takeRandomFromLRC());
      this->construirClusters();
    }
    Solution sol(this->clusters, this->centroids, this->calculateSSE(), number_of_points(), this->LRCsize);
    return sol;
  }
 private:

  MatrixPoints matrix_points;
  std::vector<std::vector<point> >  clusters; // For each cluster its elements
  int numClusters; // number of cluster
  std::vector<point> centroids; // Centroid for each cluster
  std::vector<point> LRC;
  int LRCsize;
  vector<bool> isInSolution;

  void construirClusters() {
    // Vaciar atributo clusters
    clusters.clear();
    clusters.resize(this->centroids.size(), {});
    // Para cada punto de la matriz de coordenadas de instancia
    for (const point& puntoActual : matrix_points.pointsMatrix) {
      //if (isInSolution[puntoActual.first]) continue;
      int indiceCentroideMasCercano = -1;
      double menorDistancia = std::numeric_limits<double>::max();
      // Para cada centroide i
      for (int i = 0; i < this->centroids.size(); i++) {
        double distancia = matrix_points.distanceBetweenPoints(this->centroids[i], puntoActual);
        // Si la distancia calculada es menor que la menorDistancia actual
        if (distancia < menorDistancia) {
          indiceCentroideMasCercano = i;
          menorDistancia = distancia;
        }
      }
      // Suponemos que los atributos clusters y centroides están en concordancia
      int indiceClusterAlquepertenece = indiceCentroideMasCercano;
      if (indiceClusterAlquepertenece != -1) {
        this->clusters[indiceClusterAlquepertenece].push_back(puntoActual);
      } else {
        std::cerr << "Error: No se encontró el centroide más cercano" << std::endl;
      }
    }
  }

  void generateLRC() {
    LRC.clear();
    // the ones that are already out and the ones inside the LRC on the iteration so it doesnt repeat the same minimum point
    vector<bool> inLocalSolution(number_of_points(), false); 
    for (int i = 0; i < LRCsize; i++) {
      double minSSE = std::numeric_limits<double>::max();
      int minSSEIndex = -1;
      for (int j = 0; j < matrix_points.number_of_points(); ++j) {
        if (!inLocalSolution[j] && !isInSolution[j]) {
          double sse = calculateSSE(matrix_points.pointsMatrix[j], matrix_points.pointsMatrix);
          if (sse < minSSE) {
            minSSE = sse;
            minSSEIndex = j;
          }
        }
      }
      if (minSSEIndex != -1) {
        inLocalSolution[minSSEIndex] = true;
        LRC.push_back(matrix_points.pointsMatrix[minSSEIndex]);
      } else {
        throw std::runtime_error("No se pudo encontrar el mejor SSE para la LRC");
      }
    }
  }

  point takeRandomFromLRC() {
    if (LRC.size() > 0) {
      int randomIndex = rand() % (LRC.size());
      point randomPoint = LRC[randomIndex];
      isInSolution[randomPoint.first] = true;
      return randomPoint;
    } else {
      throw std::runtime_error("La LRC está vacía");
    }
  }

  double calculateSSE(const point& centroid, const std::vector<point>& cluster) {
    double sse = 0;
    for (const point& currentPoint : cluster) {
      double distance = matrix_points.distanceBetweenPoints(centroid, currentPoint);
      sse += distance * distance;
    }
    return sse;
  }

  // Calcula la suma de errores al cuadrado para varios clusters
  double calculateSSE(const std::vector<point>& newCentroids, const std::vector<std::vector<point> >& newClusters) {
    double sse = 0;
    for (int i = 0; i < newCentroids.size(); i++) {
      const point& currentCentroid = newCentroids[i];
      const std::vector<point>& currentCluster = newClusters[i];
      for (const point& currentPoint : currentCluster) {
        double distance = matrix_points.distanceBetweenPoints(currentCentroid, currentPoint);
        sse += distance * distance;
      }
    }
    return sse;
  }

  double calculateSSE() {
    return calculateSSE(this->centroids, this->clusters);
  }

};






#endif