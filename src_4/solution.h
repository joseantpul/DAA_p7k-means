#ifndef SOLUTION
#define SOLUTION

#include "utils.h"
#include "matrixPoints.h"

class Solution {
 private:
  std::vector<std::vector<point> > grouping;
  std::vector<point> service_points;
  double p_median;
  void generate_groupings(MatrixPoints points) {
    this->grouping = points.buildGroupings(this->service_points);
  }
 public:

  Solution() {}

  Solution(vector<point> sp, MatrixPoints mp, double Pmedian) {
    this->service_points = sp;
    this->generate_groupings(mp);
    this->p_median = Pmedian;
  }

  void clear() {
    service_points.clear();
    grouping.clear();
  }
  
  // Da valor a p_median, grouping por optimización puedes elegir si generar groupings o no (por defecto si se generan)
  void update_data(MatrixPoints points, bool onlypmedian = 0) {
    if (this->service_points.empty()) {
      throw std::runtime_error("Se ha intentado actualizar la solución sin tener puntos de servicio");
    }
    this->p_median = points.calculate_Pmedian(this->service_points);
    if(onlypmedian) return;
    this->generate_groupings(points);
  }

  vector<point> get_service_points() {
    return this->service_points;
  }

  void add_service_point(point service_point) {
    this->service_points.push_back(service_point);
  }

  int getNumPointsOfCluster(int clusterInd) {
    if (clusterInd < 0 || clusterInd >= this->grouping.size()) {
      throw std::runtime_error("The index of the cluster is not valid");
    }
    return this->grouping[clusterInd].size();
  }

  double getP_median() {
    return this->p_median;
  }

  bool isEmpty() {
    return this->service_points.empty();
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