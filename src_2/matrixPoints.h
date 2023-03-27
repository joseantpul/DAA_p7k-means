#ifndef MATRIXPOINTS
#define MATRIXPOINTS

#include "utils.h"


class MatrixPoints {
 private:
  int num_points;
  int dimension;
 public:

  std::vector<point> pointsMatrix;

  int gdimension() {
    return this->dimension;
  }

  void load(string filename) {
    std::ifstream ifile(filename);
    if (!ifile) {
      std::cerr << "Error, could not open the file with the instance" << std::endl;
      return;
    }

    int m, n;
    ifile >> m >> n;
    this->num_points = m;
    this->dimension = n;

    std::string line;
    std::getline(ifile, line); // Leer el salto de línea después de n

    for (int i = 0; i < m; ++i) {
      std::getline(ifile, line);
      std::replace(line.begin(), line.end(), ',', '.');
      std::istringstream iss(line);
      point_values valores(n);
      for (int j = 0; j < n; ++j) {
        iss >> valores[j];
      }
      this->pointsMatrix.push_back(std::make_pair(i, valores));
    }
  }

  int number_of_points() {
    return this->num_points;
  }

  double distanceBetweenPoints(point otherp, point otherp2) {
    double distance = 0;
    for (int k = 0; k < otherp.second.size(); k++) {
      distance += pow(otherp.second[k] - otherp2.second[k], 2);
    }
    return sqrt(distance);
  }
};


#endif