#ifndef SOLUTION
#define SOLUTION

#include "utils.h"
#include "matrixPoints.h"

class Solution {
 public:
  std::vector<std::vector<point> > grouping;
  std::vector<point> service_points;
  double p_median;
  int number_of_points;
  int LRCsize; 

  Solution() {}

  Solution(const std::vector<std::vector<point> >& input_clusters, const std::vector<point>& input_centroids, double SSE, int np, int LRCsize = -1) {
    grouping = input_clusters;
    service_points = input_centroids;
    this->p_median = SSE;
    number_of_points = np;
    this->LRCsize = LRCsize;
  }

  void add_service_point(point service_point) {
    service_points.push_back(service_point);
  }

  void generate_groupings(MatrixPoints points) {
    grouping = points.buildGroupings(this->service_points);
  }

  void calculatePmedian(MatrixPoints points) {

  }

  int getNumPointsOfCluster(int clusterInd) {
    if (clusterInd < 0 || clusterInd >= this->grouping.size()) {
      throw std::runtime_error("The index of the cluster is not valid");
    }
    return this->grouping[clusterInd].size();
  }

  int getLRC() {
    if (this->LRCsize == -1) {
      throw std::runtime_error("You cant access the LRCsize from a kmeans solution");
    }
    return this->LRCsize;
  }

  double getP_median() {
    return this->p_median;
  }

  int get_number_of_points() {
    return this->number_of_points;
  }


  void show_service_points() {
    int contador = 0;
    for (const point& centroide : service_points) {
      std::cout << "Punto de servicio " << contador << ": (";
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
    for (const std::vector<point>& cluster : grouping) {
      std::cout << "Agrupamiento " << contador << ": [";
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