//#include "interfaz.h"
#include "grasp.h"

int main() {
  /*
  bool leave = false;
  interfaz_algoritmos interfaz; // clase que contiene un atributo para cada algoritmo y genera su tabla
  while(!leave) {
    string filename;
    cout << "Introduce el fichero que quieres usar" << endl;
    cin >> filename;
    interfaz.show_grasp_table(filename);
    int op = 0;
    cout << "¿Quieres salir del programa? (Introduce 1 en caso afirmativo)" << endl;
    cin >> op;
    if (op == 1) {
      leave = true;
    }
  }
  MatrixPoints p;
  p.load("../data/pr1.txt");
  vector<double> p0val = {5.0, 5.0};
  point p0;
  p0.first = 2;
  p0.second = p0val;
  point p1;
  p0val = {15.0, 15.0};
  p1.second = p0val;
  p1.first = 5;
  vector<point> ppp = p.farthestPoints({p0, p1}, 2);
  for(auto p : ppp) {
    cout << p.first << endl;
  }*/
  Grasp g;
  g.load("../data/experimento2.txt");
  Solution s = g.grasp_algorithm(4, 2);
  s.show_service_points();
  s.show_groupings();
  Solution s2 = g.generate_neighbor_solution(s);
  s2.show_service_points();
  s2.show_groupings();
  return 0;
}