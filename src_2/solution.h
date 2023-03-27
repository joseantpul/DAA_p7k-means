// Clase solución

#include "utils.h"

class Solution {
 public:
  std::vector<std::vector<point> > clusters;
  std::vector<point> centroids;
  double SSE;

  Solution(const std::vector<std::vector<point> >& input_clusters, const std::vector<point>& input_centroids, double SSE) {
    clusters = input_clusters;
    centroids = input_centroids;
    this->SSE = SSE;
  }

  int getNumPointsOfCluster(int clusterInd) {
    return this->clusters[clusterInd].size();
  }

  double getSSE() {
    return this->SSE;
  }

  /*
  void mostrar_centroides() {
    int contador = 0;
    for (const punto& centroide : centroids) {
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
    for (const std::vector<punto>& cluster : clusters) {
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
  }*/
};