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

 private:
  int lrcsize;
  int min_agrupamientos;
  int max_agrupamientos;
  Grasp grasp;
};

void interfaz_algoritmos::show_grasp_table(std::string filename) {
  this->grasp.load(filename);
  max_agrupamientos = grasp.number_of_points() * 0.3; 
  if (max_agrupamientos < min_agrupamientos) {
    max_agrupamientos = 2;
  } 

  vector<int> times;
  vector<double> SSEs;
  //vector<vector<int> > puntos_de_servicio; 
  vector<int> numberK;
  int number_of_points;
  for(int i = min_agrupamientos; i <= max_agrupamientos; i++) {
    //numberK.push_back(i);
    //auto startTime = high_resolution_clock::now();
    Solution sol = grasp.grasp_algorithm(i, lrcsize);
    //auto finalTime = high_resolution_clock::now();
    //times.push_back(duration_cast<microseconds>(finalTime - startTime).count());
    //number_of_points = sol.number_of_points();
    //SSEs.push_back(sol.getP_median());
    //sol.show_service_points();
  }
  /*
  cout << "Instance        | " << "Points | " << "K | " << " LRC | " << "SSE | " << "time" << endl; 
  for(int i = 0; i < numberK.size(); i++) {
    cout << filename << " | " << number_of_points << " | " 
    << numberK[i] << " |  " << lrcsize << " | " << SSEs[i] << " | " << times[i] << endl; 
  }*/
}


#endif