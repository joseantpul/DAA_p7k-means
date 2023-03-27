#ifndef KMEANS
#define KMEANS

#include "solution.h"
#include "matrixPoints.h"

class Kmeans {
 private:
  MatrixPoints matrix_points;
  std::vector<std::vector<point> >  clusters; // Para cada cluster (0, 1, numClusters - 1) sus elementos
  int numClusters; // número de clusters
  std::vector<point> centroids; // Centroide para cada cluster (cada cluster 0, 1 ... numClusters - 1)

 public:
  Kmeans() {}

  int number_of_points() {
    return this->matrix_points.number_of_points();
  }

  void load(string filename) {
    matrix_points.load(filename);
  }

  Solution kmeans_algorithm(int k) {
    this->numClusters = k;
    /*
    this->generarPrimerosCentroides();
    int iteracion = 1;
    std::vector<std::vector<punto> > clusters_antiguos;
    do {
      clusters_antiguos = this->clusters;
      this->construirClusters();
      //mostrar_clusters();
      this->recalcularCentroides();
      std::cout << "Iteración: " << iteracion << std::endl;
      iteracion++;
    } while(!this->esIgual(clusters_antiguos));*/
    this->clusters = {};
    this->centroids = {};
    Solution sol(this->clusters, this->centroids, 89.2, number_of_points());
    return sol;
  }

  void generarPrimerosCentroides() {
    // Limpiar centroides actuales
    centroides.clear();
    // Inicializar la generación de números aleatorios
    std::mt19937 generador(static_cast<unsigned long>(std::time(nullptr)));
    std::uniform_int_distribution<int> distribucion(0, instancia.matrizCoordenadas.size() - 1);
    // Seleccionar el primer centroide al azar
    int indiceAleatorio = distribucion(generador);
    centroides.push_back(instancia.matrizCoordenadas[indiceAleatorio]);
    // Iterar hasta obtener numClusters centroides
    for (int i = 1; i < numClusters; i++) {
      double maxDistancia = std::numeric_limits<double>::min();
      int indiceMaxDistancia = -1;
      // Para cada punto en la matriz de coordenadas
      for (int j = 0; j < instancia.matrizCoordenadas.size(); j++) {
        double distanciaTotal = 0;
        // Calcular la distancia acumulada a los centroides actuales
        for (const punto& centroide : centroides) {
          distanciaTotal += instancia.calcularDistanciaEntrePuntos(centroide, instancia.matrizCoordenadas[j]);
        }
        // Si la distancia acumulada es mayor que la máxima actual, actualizar la máxima y el índice
        if (distanciaTotal > maxDistancia) {
          maxDistancia = distanciaTotal;
          indiceMaxDistancia = j;
        }
      }
      // Añadir el punto más alejado de los centroides actuales como nuevo centroide
      if (indiceMaxDistancia != -1) {
        centroides.push_back(instancia.matrizCoordenadas[indiceMaxDistancia]);
      } else {
        std::cerr << "Error: No se encontró el punto más alejado de los centroides actuales" << std::endl;
      }
    }
  }

};





#endif