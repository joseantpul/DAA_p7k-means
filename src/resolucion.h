// Resolucion.h
#ifndef RESOLUCION
#define RESOLUCION

#include <iostream>
#include <vector>
#include <map>
#include "problema.h"

// Clase solución

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


class Kmeans {
 public:
  std::vector<std::vector<punto> >  clusters; // Para cada cluster (0, 1, numClusters - 1) sus elementos
  Problema instancia; // de aquí se sacan la matriz de distancia y todo eso
  int numClusters; // número de clusters
  std::vector<punto> centroides; // Centroide para cada cluster (cada cluster 0, 1 ... numClusters - 1)

  Kmeans(const std::string& nombreArchivo) : instancia(nombreArchivo) {}

  //hacer un método para comprobar que el número de cluster es igual al tamaño del vector de cluster y centroides
  //y comprobar quel os centroides tienen el tamaño instancia.dimension

  Solucion kmeans_algoritmo() {
    centroides.push_back({instancia.matrizCoordenadas[1]});
    centroides.push_back({instancia.matrizCoordenadas[5]});
    centroides.push_back({instancia.matrizCoordenadas[9]});
    numClusters = 3;

    // Calcular k mediante heurística
    // Elegir primeros k centroides
    int iteracion = 1;
    std::vector<std::vector<punto> > clusters_antiguos;
    do {
      clusters_antiguos = this->clusters;
      this->construirClusters();
      this->recalcularCentroides();
      std::cout << "Iteración: " << iteracion << std::endl;
      iteracion++;
    } while(!this->esIgual(clusters_antiguos));

    Solucion sol(this->clusters, this->centroides);
    return sol;
  }

  void construirClusters() {
    // Vaciar atributo clusters
    clusters.clear();
    clusters.resize(this->numClusters, {});
    // Para cada punto de la matriz de coordenadas de instancia
    for (const punto& puntoActual : instancia.matrizCoordenadas) {
      int indiceCentroideMasCercano = -1;
      double menorDistancia = std::numeric_limits<double>::max();

      // Para cada centroide i
      for (int i = 0; i < numClusters; i++) {
        double distancia = instancia.calcularDistanciaEntrePuntos(this->centroides[i], puntoActual.first);
        // Si la distancia calculada es menor que la menorDistancia actual
        if (distancia < menorDistancia) {
          indiceCentroideMasCercano = i;
          menorDistancia = distancia;
        }
      }
    // Suponemos que los atributos clusters y centroides están en concordancia
      int indiceClusterAlquepertenece = indiceCentroideMasCercano;
      if (indiceClusterAlquepertenece != -1) {
        this->clusters[indiceClusterAlquepertenece].push_back(puntoActual);
      } else {
        std::cerr << "Error: No se encontró el centroide más cercano" << std::endl;
      }
    }
  }

  void recalcularCentroides() {
    this->centroides.clear();
    for(int indiceCluster = 0; indiceCluster < numClusters; indiceCluster++) {
      std::vector<punto> clusterActual = this->clusters[indiceCluster];
      punto centroideActual;
      centroideActual.first = -1; // no tiene índice por que no pertenece realmente al conjunto de puntos original
      centroideActual.second.resize(instancia.dimension, 0);
      for(const punto& puntoActual: clusterActual) {
        for(int i = 0; i < instancia.dimension; i++) { //sumar cada punto
          centroideActual.second[i] += puntoActual.second[i];
        }
      }
      for(int i = 0; i < instancia.dimension; i++) { //dividir por el número de puntos en el cluster
        centroideActual.second[i] /= clusterActual.size();
      }
      this->centroides.push_back(centroideActual);
    } 
  }

  void generarPrimerosClusters() {
        // Implementa la lógica para generar los primeros clusters usando la heurística y el 10% como se menciona en el pseudocódigo
  }

  bool esIgual(const std::vector<std::vector<punto> >& cluster_antiguo) {
    if (clusters.size() != cluster_antiguo.size()) { // No debería de suceder nunca en kmeans
      return false;
    }
    for (int indiceCluster = 0; indiceCluster < this->clusters.size(); indiceCluster++) {
      const std::vector<punto>& puntosClusterActual = clusters[indiceCluster];
      const std::vector<punto>& puntos_antiguos = cluster_antiguo[indiceCluster];
      if (puntosClusterActual.size() != puntos_antiguos.size()) { // si tienen distinto tamaño ya no son iguales
        return false;
      }
      std::vector<punto> copia_puntos(puntosClusterActual);
      std::vector<punto> copia_puntos_antiguos(puntos_antiguos);
      std::sort(copia_puntos.begin(), copia_puntos.end());
      std::sort(copia_puntos_antiguos.begin(), copia_puntos_antiguos.end());
      for (size_t i = 0; i < copia_puntos.size(); ++i) {
        if (copia_puntos[i].first != copia_puntos_antiguos[i].first) {
          return false;
        }
      }
    }
    return true;
  }

  //estos métodos quitarlos de aquí cuando termine
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




#endif



/**
 * construir clusters: 
 *     // Implementa la lógica para construir los clusters asignando a cada punto el centroide más cercano
    /
     * vaciar atributo clusters
     * Para cada punto de la matriz de coordenadas de instancia {
     *   int indiceCentroideMasCercano
     *   para cada centroide i {
     *     si(instancia.calcularDistanciaEntrePuntos(centroide, puntoActual) < menorDistancia )
     *       indiceCentroideMasCercano = i;
     *       menorDistancia = instancia.calcularDistanciaEntrePuntos(centroide, puntoActual);
     *   }
     *   int indiceClusterAlquepertenece = indiceCentroideMasCercano //suponemos que los atributos clusters y centroides están en corcondancia, es decir para el this->clusters[i] tiene centroide this->centroides[i]
     *   this->clusters[i].push_back(punto)
     * }
    */