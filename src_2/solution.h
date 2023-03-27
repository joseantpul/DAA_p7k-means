// Clase solución
#ifndef SOLUTION
#define SOLUTION

#include "utils.h"

class Solution {
 public:
  std::vector<std::vector<point> > clusters;
  std::vector<point> centroids;
  double SSE;
  int number_of_point;
  int LRCsize; // only for grasp

  Solution(const std::vector<std::vector<point> >& input_clusters, const std::vector<point>& input_centroids, double SSE, int np, int LRCsize = -1) {
    clusters = input_clusters;
    centroids = input_centroids;
    this->SSE = SSE;
    number_of_point = np;
    this->LRCsize = LRCsize;
  }

  int getNumPointsOfCluster(int clusterInd) {
    if (clusterInd < 0 || clusterInd >= this->clusters.size()) {
      throw std::runtime_error("The index of the cluster is not valid");
    }
    return this->clusters[clusterInd].size();
  }

  int getLRC() {
    if (this->LRCsize == -1) {
      throw std::runtime_error("You cant access the LRCsize from a kmeans solution");
    }
    return this->LRCsize;
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



#endif