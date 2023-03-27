// Resolucion.h
#ifndef RESOLUCION
#define RESOLUCION

#include <map>
#include "solucion.h"
#include <random>
#include <limits>
#include <ctime>
#include <cmath>


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
    // Calcular k mediante heurística 
    this->numClusters = this->metaheuristica();
    //std::cout << "Número de clusters: " << numClusters << std::endl;
    // Elegir primeros k centroides
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
    } while(!this->esIgual(clusters_antiguos));

    Solucion sol(this->clusters, this->centroides);
    return sol;
  }

  /**
   * Vacio los cluster que ya había y creo unos nuevos, asignando
   * a cada punto su cluster más cercano usando la distancia euclidea
  */
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
        double distancia = instancia.calcularDistanciaEntrePuntos(this->centroides[i], puntoActual);
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

  /**
   * Esto se ejecuta después de formar nuevos clusters con construirClusters,
   * una vez formados se vuelven a recalcular los centroides de cada cluster
  */
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

  /**
   * Se generan los primeros k centroides generando un punto al azar y añadiendo el 
   * resto de centroides como los puntos más alejados de este
  */
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

  /**
   * Comprueba si dos agrupamientos son iguales
  */
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


  int metaheuristica() {
    int n = instancia.matrizCoordenadas.size();
    int minClusters = 2; // Mínimo número de clusters
    // ESTA MEDIDA DEL 10% PARA PROBLEMAS CON POCOS PUNTOS TE CAGA
    int maxClusters = std::max(minClusters, static_cast<int>(n * 0.3)); // Máximo número de clusters, 10% de los puntos
    std::cout << "Máximo número de clusters: " << maxClusters << std::endl;
    double menorSSE = std::numeric_limits<double>::max();
    int optimoNumClusters = 0;
    for (int k = minClusters; k <= maxClusters; k++) {
      this->numClusters = k;
      // Inicializar los primeros centroides
      this->generarPrimerosCentroides();
      std::vector<std::vector<punto>> clusters_antiguos;
      do {
        clusters_antiguos = this->clusters;
        this->construirClusters();
        this->recalcularCentroides();
      } while(!this->esIgual(clusters_antiguos));

      double sseActual = calcularSSE(this->centroides, this->clusters);
      std::cout << "SSE para " << k << " clusters: " << sseActual << std::endl;

      if (sseActual < menorSSE) {
        menorSSE = sseActual;
        optimoNumClusters = k;
      }
    }
    this->clusters.clear();
    this->centroides.clear();
    return optimoNumClusters;
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
 
  // Calcula la suma de errores al cuadrado para un único cluster
  double calcularSSE(const punto& centroide, const std::vector<punto>& cluster) {
    double sse = 0;
    for (const punto& puntoActual : cluster) {
      double distancia = instancia.calcularDistanciaEntrePuntos(centroide, puntoActual);
      sse += distancia * distancia;
    }
    return sse;
  }

  // Calcula la suma de errores al cuadrado para varios clusters
  double calcularSSE(const std::vector<punto>& nuevosCentroides, const std::vector<std::vector<punto>>& clustersNuevos) {
    double sse = 0;
    for (int i = 0; i < nuevosCentroides.size(); i++) {
      const punto& centroideActual = nuevosCentroides[i];
      const std::vector<punto>& clusterActual = clustersNuevos[i];
      for (const punto& puntoActual : clusterActual) {
        double distancia = instancia.calcularDistanciaEntrePuntos(centroideActual, puntoActual);
        sse += distancia * distancia;
      }
    }
    return sse;
  }

  // Calcula la suma de errores al cuadrado usando los atributos clusters y centroides de la clase Kmeans
  double calcularSSE() {
    return calcularSSE(this->centroides, this->clusters);
  }

  private:
   

};

#endif


/**
Ahora quiero que hagas el método int metaheuristica() que devuelve el número óptimo de 
clusters para una determinada nube de puntos, es decir nuestro atributo instancia, 
puedes usar el 10% de los puntos y tiene que dar un mínimo de dos clusters
*/