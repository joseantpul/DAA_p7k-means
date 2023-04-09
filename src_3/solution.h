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

  Solution(vector<point> sp, MatrixPoints mp, double Pmedian) {
    this->service_points = sp;
    this->generate_groupings(mp);
    this->p_median = Pmedian;
  }

  vector<point> get_service_points() {
    return this->service_points;
  }

  void add_service_point(point service_point) {
    this->service_points.push_back(service_point);
  }

  void generate_groupings(MatrixPoints points) {
    this->grouping = points.buildGroupings(this->service_points);
  }

  int getNumPointsOfCluster(int clusterInd) {
    if (clusterInd < 0 || clusterInd >= this->grouping.size()) {
      throw std::runtime_error("The index of the cluster is not valid");
    }
    return this->grouping[clusterInd].size();
  }

  int getLRC_size() {
    if (this->LRCsize == -1) {
      throw std::runtime_error("You cant access the LRCsize from a kmeans solution");
    }
    return this->LRCsize;
  }

  double getP_median() {
    return this->p_median;
  }

  void setP_median(double pmedian) {
    this->p_median = pmedian;
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

  void show_groupings() {
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