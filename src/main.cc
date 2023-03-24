#include "problema.h"

int main() {
    Problema problema("../data/data1.txt");

    // Imprimir las coordenadas y las distancias para verificar la implementación
    std::cout << "Matriz de Coordenadas:" << std::endl;
    for (const punto& p : problema.matrizCoordenadas) {
        std::cout << "ID: " << p.first << " Coordenadas: ";
        for (double coord : p.second) {
            std::cout << coord << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Matriz de Distancias:" << std::endl;
    for (const auto& fila : problema.matrizDistancias) {
        for (double dist : fila) {
            std::cout << dist << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
