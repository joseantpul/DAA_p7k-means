#ifndef GRASP
#define GRASP

#include "matrixPoints.h"
#include "solution.h"

class Grasp {
 public:
  Grasp() {
    max_iterations = 1000;
  }
  void load(string filename);
  Solution grasp_algorithm(int k, int LRCsize);
  int number_of_points() {
    return this->points.number_of_points();
  }

 private:
  int max_iterations;
  MatrixPoints points;
  Solution fase_constructiva(int k, int LRCsize);
};

Solution Grasp::fase_constructiva(int k, int LRCsize) {
  
}

  //Para hacer la fase constructiva descomentar todo menos sol = this->busqueda_local(sol)
Solution Grasp::grasp_algorithm(int k, int LRCsize) {
    //Solution mejor_solucion;
  for(int i = 0; i < max_iterations; i++) {
      //Solution sol = this->fase_constructiva();
      //sol = this->busqueda_local(sol);
      //this->actualizar(sol, mejor_solucion);
  }
    //return mejor_solucion;
}

void Grasp::load(string filename) {
  points.load(filename);
}



#endif