#ifndef PROBLEMA
#define PROBLEMA

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm> // replace
#include <utility> // pair

typedef std::vector<double> valores_punto;
typedef std::pair<int, valores_punto> punto;

class Problema {
public:
  std::vector<punto> matrizCoordenadas;
  std::vector<std::vector<double>> matrizDistancias;
  int num_puntos;
  int dimension;

  Problema(const std::string& nombreArchivo) {
    cargarCoordenadas(nombreArchivo);
    calcularDistancias();
  }

  void cargarCoordenadas(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
      std::cerr << "Error al abrir el archivo" << std::endl;
      return;
    }

    int m, n;
    archivo >> m >> n;
    num_puntos = m;
    dimension = n;

    std::string linea;
    std::getline(archivo, linea); // Leer el salto de línea después de n

    for (int i = 0; i < m; ++i) {
      std::getline(archivo, linea);
      std::replace(linea.begin(), linea.end(), ',', '.');
      std::istringstream iss(linea);
      valores_punto valores(n);
      for (int j = 0; j < n; ++j) {
        iss >> valores[j];
      }
      matrizCoordenadas.push_back(std::make_pair(i, valores));
    }
  }

  void calcularDistancias() {
    matrizDistancias.resize(num_puntos, std::vector<double>(num_puntos));
    for (int i = 0; i < num_puntos; ++i) {
      for (int j = 0; j < num_puntos; ++j) {
        if (i == j) {
          matrizDistancias[i][j] = 0;
        } else {
          double distancia = 0;
          for (size_t k = 0; k < matrizCoordenadas[i].second.size(); ++k) {
            distancia += pow(matrizCoordenadas[i].second[k] - matrizCoordenadas[j].second[k], 2);
          }
          matrizDistancias[i][j] = sqrt(distancia);
        }
      }
    }
  }

  double calcularDistanciaEntrePuntos(punto otroPunto, int index) {
    double distancia = 0;
    for (size_t k = 0; k < otroPunto.second.size(); ++k) {
      distancia += pow(otroPunto.second[k] - matrizCoordenadas[index].second[k], 2);
    }
    return sqrt(distancia);
  }

  double calcularDistanciaEntrePuntos(punto otroPunto, punto otroPunto2) {
    double distancia = 0;
    for (size_t k = 0; k < otroPunto.second.size(); ++k) {
      distancia += pow(otroPunto.second[k] - otroPunto2.second[k], 2);
    }
    return sqrt(distancia);
  }
};

//-------------------------------------------------------------------------------s



#endif


/**Ahora crea una nueva clase que se llame Solucion esta clase va a representar la solución de pasarle el algoritmo k-means para crear clusters a una instancia del objeto Problema, es decir representa la solución del algoritmo k-means que es un conjunto de clusters cada uno con su centroide. Quiero que tenga los siguientes atributos: */