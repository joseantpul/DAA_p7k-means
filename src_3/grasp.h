#ifndef GRASP
#define GRASP

#include "matrixPoints.h"
#include "solution.h"

class Grasp {
 public:
  Grasp() {
    max_iterations = 1000;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
  }
  void load(string filename);
  Solution grasp_algorithm(int k, int LRCsize);
  int number_of_points() {
    return this->points.number_of_points();
  }

 private:
  int max_iterations;
  int LRCsize;
  MatrixPoints points;
  vector<point> LRC;
  vector<point> service_points;
  Solution construction_phase(int k, int LRCsize);
  point takeRandomFromLRC();
  void generateLRC(Solution current_solution);
};

point Grasp::takeRandomFromLRC() {
  if (LRC.size() > 0) {
    int random_index = std::rand() % this->LRC.size();
    return LRC[random_index];
  } else {
    throw std::runtime_error("La LRC está vacía");
  }
}

// LRC con los puntos más alejados de la solución
// Distancia de un punto al conjunto DE SOLUCIONES distancia mínima a uno de los puntos
void Grasp::generateLRC(Solution current_solution) {
  vector<point> current_service_points = current_solution.get_service_points();
  this->LRC.clear();
  this->LRC = points.farthestPoints(current_service_points, this->LRCsize);
}

Solution Grasp::construction_phase(int k, int LRCsize) {
  Solution sol;
  point inicial_point = points.takeRandomPoint();
  sol.add_service_point(inicial_point);
  for(int i = 0; i < k - 1; i++) { // Ya tenemos un punto de servicio, por lo que recorremos k - 1
    this->generateLRC(sol);
    sol.add_service_point(this->takeRandomFromLRC());
  }
}

  //Para hacer la fase constructiva descomentar todo menos sol = this->busqueda_local(sol)
Solution Grasp::grasp_algorithm(int k, int LRCsize) {
  Solution mejor_solucion;
  this->LRCsize = LRCsize;
  for(int i = 0; i < max_iterations; i++) {
    Solution sol = this->construction_phase(k, LRCsize);
      //sol = this->busqueda_local(sol);
      //this->actualizar(sol, mejor_solucion);
  }
    //return mejor_solucion;
}

void Grasp::load(string filename) {
  points.load(filename);
}



#endif