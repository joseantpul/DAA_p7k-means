#ifndef KMEANS
#define KMEANS

#include "solution.h"
#include "matrixPoints.h"

class Kmeans {
 private:
  MatrixPoints matrix_points;
  std::vector<std::vector<point> >  clusters; // Para cada cluster (0, 1, numClusters - 1) sus elementos
  int numClusters; // número de clusters
  std::vector<point> centroids; // Centroide para cada cluster (cada cluster 0, 1 ... numClusters - 1)

 public:
  Kmeans() {}

  int number_of_points() {
    //return this->matrix_points.size();
    return 5;
  }

  void load(string filename) {
    //matrix_points.load(filename);
  }

  Solution kmeans_algorithm(int k) {
    this->numClusters = k;
    /*
    this->generarPrimerosCentroides();
    int iteracion = 1;
    std::vector<std::vector<punto> > clusters_antiguos;
    do {
      clusters_antiguos = this->clusters;
      this->construirClusters();
      //mostrar_clusters();
      this->recalcularCentroides();
      std::cout << "Iteración: " << iteracion << std::endl;
      iteracion++;
    } while(!this->esIgual(clusters_antiguos));*/
    this->clusters = {{}}
    Solution sol(this->clusters, this->centroids, 89.2);
    return sol;
  }

};





#endif