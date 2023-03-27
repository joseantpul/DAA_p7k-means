#ifndef INTERFAZ
#define INTERFAZ

#include "kmeans.h"

using namespace std;

class interfaz_algoritmos {
 public:
  interfaz_algoritmos() {}
  void show_kmeans_table(std::string filename);
  //void show_grasp_table(std::string filename);

 private:
  Kmeans kmeans;
  //GRASP grasp();
};


void interfaz_algoritmos::show_kmeans_table(std::string filename) {
  this->kmeans.load(filename);
  int min_clusters = 2;
  int max_clusters = kmeans.number_of_points() * 0.1; 
  if (max_clusters < min_clusters) {
    max_clusters = 2;
  } 
  vector<int> times;
  vector<double> SSEs;
  vector<vector<int> > points_per_cluster; // for each group of clusters how many points are in that group of clusters for each cluster 
  vector<int> numberK;
  int number_of_points;
  for(int i = min_clusters; i <= max_clusters; i++) {
    numberK.push_back(i);
    auto startTime = high_resolution_clock::now();
    Solution sol = kmeans.kmeans_algorithm(i);
    auto finalTime = high_resolution_clock::now();
    times.push_back(duration_cast<microseconds>(finalTime - startTime).count());
    vector<int> points_per_c = {};
    number_of_points = sol.number_of_points();
    SSEs.push_back(sol.getSSE());
  }
  cout << "Instance | " << "Points | " << "K | " << "SSE | " << "time" << endl; 
  for(int i = 0; i < numberK.size(); i++) {
    cout << filename << " | " << number_of_points << " | " 
    << numberK[i] << " | " << SSEs[i] << " | " << times[i] << endl; 
  }
}

/*
void interfaz_algoritmos::show_grasp_table(std::string filename) {

}
*/

#endif