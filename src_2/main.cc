#include "interfaz.h"


int main() {
  bool leave = false;
  interfaz_algoritmos interfaz; // clase que contiene un atributo para cada algoritmo y genera su tabla
  while(!leave) {
    string filename;
    cout << "Introduce el fichero que quieres usar" << endl;
    cin >> filename;
    interfaz.show_kmeans_table(filename);
    //interfaz.show_kmeans_table(file_name);
    int op = 0;
    cout << "¿Quieres salir del programa? (Introduce 1 en caso afirmativo)" << endl;
    cin >> op;
    if (op == 1) {
      leave = true;
    }
  }
  return 0;
}