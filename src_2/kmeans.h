#ifndef KMEANS
#define KMEANS

#include "solution.h"
#include "matrixPoints.h"

class Kmeans {

 public:
  Kmeans() {}

  int number_of_points() {
    return this->matrix_points.number_of_points();
  }

  void load(string filename) {
    matrix_points.load(filename);
  }

  Solution kmeans_algorithm(int k) {
    this->numClusters = k;
    this->generateFirstCentroids();
    int iteracion = 1;
    std::vector<std::vector<point> > clusters_antiguos;
    do {
      clusters_antiguos = this->clusters;
      this->construirClusters();
      this->recalcularCentroides();
      std::cout << "Iteración: " << iteracion << std::endl;
      iteracion++;
    } while(!this->esIgual(clusters_antiguos));
    Solution sol(this->clusters, this->centroids, this->calculateSSE(), number_of_points());
    return sol;
  }
 private:

  MatrixPoints matrix_points;
  std::vector<std::vector<point> >  clusters; // For each cluster its elements
  int numClusters; // number of cluster
  std::vector<point> centroids; // Centroid for each cluster

  void generateFirstCentroids() {
    int n = matrix_points.pointsMatrix.size();
    if (numClusters > n) {
      throw std::runtime_error("The number of cluster cant be greater than the number of points");
    }
    std::vector<bool> points_used(n, false); // Indica si un punto ya ha sido utilizado como centroide
    this->centroids.clear(); // Limpiar centroides existentes
    for (int k = 0; k < numClusters; ++k) {
      double menorSSE = std::numeric_limits<double>::max();
      int indiceMenorSSE = -1;
      for (int i = 0; i < n; ++i) {
        if (!points_used[i]) { // Si el punto no ha sido utilizado como centroide
          double sse = calculateSSE(matrix_points.pointsMatrix[i], matrix_points.pointsMatrix);
          if (sse < menorSSE) {
            menorSSE = sse;
            indiceMenorSSE = i;
          }
        }
      }
      if (indiceMenorSSE != -1) {
        points_used[indiceMenorSSE] = true; // Marcar el punto seleccionado como utilizado
        this->centroids.push_back(matrix_points.pointsMatrix[indiceMenorSSE]);
      } else {
        throw std::runtime_error("Could not find a valid centroid");
      }
    }
  }

  void construirClusters() {
    // Vaciar atributo clusters
    clusters.clear();
    clusters.resize(this->numClusters, {});
    // Para cada punto de la matriz de coordenadas de instancia
    for (const point& puntoActual : matrix_points.pointsMatrix) {
      int indiceCentroideMasCercano = -1;
      double menorDistancia = std::numeric_limits<double>::max();
      // Para cada centroide i
      for (int i = 0; i < numClusters; i++) {
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

  void recalcularCentroides() {
    this->centroids.clear();
    for(int indiceCluster = 0; indiceCluster < numClusters; indiceCluster++) {
      std::vector<point> clusterActual = this->clusters[indiceCluster];
      point centroideActual;
      centroideActual.first = -1; // no tiene índice por que no pertenece realmente al conjunto de puntos original
      centroideActual.second.resize(matrix_points.gdimension(), 0);
      for(const point& puntoActual: clusterActual) {
        for(int i = 0; i < matrix_points.gdimension(); i++) { //sumar cada punto
          centroideActual.second[i] += puntoActual.second[i];
        }
      }
      for(int i = 0; i < matrix_points.gdimension(); i++) { //dividir por el número de puntos en el cluster
        centroideActual.second[i] /= clusterActual.size();
      }
      this->centroids.push_back(centroideActual);
    } 
  }

  bool esIgual(const std::vector<std::vector<point> >& cluster_antiguo) {
    if (clusters.size() != cluster_antiguo.size()) { // No debería de suceder nunca en kmeans
      return false;
    }
    for (int indiceCluster = 0; indiceCluster < this->clusters.size(); indiceCluster++) {
      const std::vector<point>& puntosClusterActual = clusters[indiceCluster];
      const std::vector<point>& puntos_antiguos = cluster_antiguo[indiceCluster];
      if (puntosClusterActual.size() != puntos_antiguos.size()) { // si tienen distinto tamaño ya no son iguales
        return false;
      }
      std::vector<point> copia_puntos(puntosClusterActual);
      std::vector<point> copia_puntos_antiguos(puntos_antiguos);
      std::sort(copia_puntos.begin(), copia_puntos.end());
      std::sort(copia_puntos_antiguos.begin(), copia_puntos_antiguos.end());
      for (size_t i = 0; i < copia_puntos.size(); ++i) {
        if (copia_puntos[i].first != copia_puntos_antiguos[i].first) {
          return false;
        }
      }
    }
    return true;
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