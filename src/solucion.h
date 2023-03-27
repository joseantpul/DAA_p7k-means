// Clase solución

#include "problema.h"

class Solucion {
 public:
  std::vector<std::vector<punto> > clusters;
  std::vector<punto> centroides;

  Solucion(const std::vector<std::vector<punto> >& input_clusters, const std::vector<punto>& input_centroides) {
    clusters = input_clusters;
    centroides = input_centroides;
  }

  void mostrar_centroides() {
    int contador = 0;
    for (const punto& centroide : centroides) {
      std::cout << "Centroide " << contador << ": (";
      for (size_t i = 0; i < centroide.second.size(); ++i) {
        std::cout << centroide.second[i];
        if (i < centroide.second.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << ")\n";
      contador++;
    }
  }

  void mostrar_clusters() {
    int contador = 0;
    for (const std::vector<punto>& cluster : clusters) {
      std::cout << "Cluster " << contador << ": [";
      for (size_t i = 0; i < cluster.size(); ++i) {
        std::cout << cluster[i].first;
        if (i < cluster.size() - 1) {
          std::cout << ", ";
        }
      }
      std::cout << "]\n";
      contador++;
    }
  }
};