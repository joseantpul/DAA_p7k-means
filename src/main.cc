#include "resolucion.h"

int main() {
    Kmeans problema("../data/experimento2.txt");
    Solucion solucion = problema.kmeans_algoritmo();
    solucion.mostrar_centroides();
    solucion.mostrar_clusters();

    return 0;
}
