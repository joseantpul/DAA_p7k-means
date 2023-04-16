#ifndef INTERFAZ
#define INTERFAZ

#include "grasp.h"

using namespace std;

class interfaz_algoritmos {
 public:
  interfaz_algoritmos() {
    this->lrcsize = 2;
    this->min_agrupamientos = 2;
  }
  void show_grasp_table(std::string filename);
  int get_num_points(std::string filename);
 private:
  int lrcsize;
  int min_agrupamientos;
  int max_agrupamientos;
};

void interfaz_algoritmos::show_grasp_table(std::string filename) {
  int np = get_num_points(filename);
  max_agrupamientos = np * 0.3; 
  if (max_agrupamientos < min_agrupamientos) {
    max_agrupamientos = 2;
  } 

  vector<int> times;
  vector<double> SSEs;
  vector<int> numberK;
  int number_of_points;
  cout << "Número de puntos: " << np << endl;
  for(int i = min_agrupamientos; i <= max_agrupamientos; i++) {
    Grasp grasp;
    grasp.load(filename);
    auto startTime = high_resolution_clock::now();
    Solution sol = grasp.grasp_algorithm(i, lrcsize);
    auto finalTime = high_resolution_clock::now();
    cout << "K inicial: " << i << endl;
    cout << "Tiempo de ejecución (en microsegundos): " << duration_cast<microseconds>(finalTime - startTime).count() << endl;
    cout << "P mediana de la solución " << sol.getP_median() << endl;
    sol.show_service_points();
    sol.show_groupings();
  }
}

int interfaz_algoritmos::get_num_points(std::string filename) {
  std::ifstream ifile(filename);
  int m;
  ifile >> m;
  return m;
}

#endif