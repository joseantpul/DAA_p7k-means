#include "resolucion.h"

int main() {
    Kmeans problema("../data/experimento.txt");
    Solucion solucion = problema.kmeans_algoritmo();
    solucion.mostrar_centroides();
    solucion.mostrar_clusters();
    /*
    problema.centroides.push_back({problema.instancia.matrizCoordenadas[1]});
    problema.centroides.push_back({problema.instancia.matrizCoordenadas[5]});
    problema.centroides.push_back({problema.instancia.matrizCoordenadas[9]});
    problema.numClusters = 3;

    problema.mostrar_centroides();
    problema.construirClusters();
    problema.mostrar_clusters();

    problema.recalcularCentroides();
    problema.mostrar_centroides();
    problema.construirClusters();
    problema.mostrar_clusters();*/

    return 0;
}
