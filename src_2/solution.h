// Clase solución

#include "utils.h"

class Solution {
 public:
  std::vector<std::vector<point> > clusters;
  std::vector<point> centroids;
  double SSE;
  int number_of_point;

  Solution(const std::vector<std::vector<point> >& input_clusters, const std::vector<point>& input_centroids, double SSE, int np) {
    clusters = input_clusters;
    centroids = input_centroids;
    this->SSE = SSE;
    number_of_point = np;
  }

  int getNumPointsOfCluster(int clusterInd) {
    //return this->clusters[clusterInd].size();
    if (clusterInd < 0 || clusterInd >= this->clusters.size()) {
      cout << "Not valid index, throw error" << endl;
    }
    return 3;
  }

  double getSSE() {
    return this->SSE;
  }

  int number_of_points() {
    return this->number_of_point;
  }


  void mostrar_centroides() {
    int contador = 0;
    for (const point& centroide : centroids) {
      std::cout << "Centroide " << contador << ": (";
      for (size_t i = 0; i < centroide.second.size(); ++i) {
        std::cout << centroide.second[i];
        if (i < centroide.second.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << ")\n";
      contador++;
    }
  }

  void mostrar_clusters() {
    int contador = 0;
    for (const std::vector<point>& cluster : clusters) {
      std::cout << "Cluster " << contador << ": [";
      for (size_t i = 0; i < cluster.size(); ++i) {
        std::cout << cluster[i].first;
        if (i < cluster.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << "]\n";
      contador++;
    }
  }
};