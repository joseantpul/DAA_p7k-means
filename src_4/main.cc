#include "interfaz.h"

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
  return 0;*/
  Grasp p;
  p.load("../data/experimento2.txt");
  Solution sol = p.construction_phase(3, 2);
  sol = p.GVNS(sol);
  sol.show_service_points();
  sol.show_groupings();

  return 0;
}