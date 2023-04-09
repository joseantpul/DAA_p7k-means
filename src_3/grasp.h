#ifndef GRASP
#define GRASP

#include "matrixPoints.h"
#include "solution.h"

class Grasp {
 public:
  Grasp() {
    max_iterations = 1000;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
  }
  void load(string filename);
  Solution grasp_algorithm(int k, int LRCsize);
  int number_of_points() {
    return this->points.number_of_points();
  }
  Solution generate_neighbor_solution(Solution solution);
 private:
  int max_iterations;
  int LRCsize;
  MatrixPoints points;
  vector<point> LRC;
  vector<point> service_points;
  Solution construction_phase(int k, int LRCsize);
  point takeRandomFromLRC();
  void generateLRC(Solution current_solution);
  Solution local_search(Solution current_solution);
  double calculate_Pmedian(vector<point> solution_service_points);
  //Solution generate_neighbor_solution(Solution solution);
  // Devuelve los mejores puntos de servicio de cada espacio de soluciones
  vector<point> delete_solution_space(vector<point> original_sp);
  vector<point> insert_solution_space(vector<point> original_sp);
  vector<point> exchange_solution_space(vector<point> original_sp);
};

vector<point> Grasp::delete_solution_space(vector<point> original_sp) {
  vector<point> best_deleted_sp;
  double min_p_median = std::numeric_limits<double>::max();
  for (int i = 0; i < original_sp.size(); i++) {
      vector<point> deleted_sp = original_sp;
      deleted_sp.erase(deleted_sp.begin() + i);
      double current_p_median = calculate_Pmedian(deleted_sp); // Esto cambiarlo por la función esa
    if (current_p_median < min_p_median) {
      min_p_median = current_p_median;
      best_deleted_sp = deleted_sp;
    }
  }
  return best_deleted_sp;
}

// Genera la (mejor/primera mejor)? solucion vecina, si no hay ninguna mejor devuelve la misma 
Solution Grasp::generate_neighbor_solution(Solution solution) {
  // Movimientos de eliminación, inserción e intercambio, cada uno genera una nueva lista de service_points
  // Para cada una comprobar su Pmediana y quedarnos con la mejor, (PARA HAYAR LA PMEDIANA DE LOS VECINOS HAY Q HACER ESO)
  vector<point> current_service_points = solution.get_service_points();
  vector<point> best_sp_delete = this->delete_solution_space(current_service_points);
  double p_median_delete = calculate_Pmedian(best_sp_delete);
  /*vector<point> best_sp_insert = this->insert_solution_space(current_service_points);
  double p_median_insert = calculate_Pmedian(best_sp_insert);
  vector<point> best_sp_exchange = this->exchange_solution_space(current_service_points);
  double p_median_exchange = calculate_Pmedian(best_sp_exchange);
  double min_p_median_neighbor = std::min(p_median_delete, p_median_insert, p_median_exchange);*/
  double min_p_median_neighbor = p_median_delete;
  vector<point> best_sp_neighbor;
  if(min_p_median_neighbor == p_median_delete) {
    best_sp_neighbor = best_sp_delete;
  }/* else if(min_p_median_neighbor == p_median_insert) {
    best_sp_neighbor = best_sp_insert;
  } else {
    best_sp_neighbor = best_sp_exchange;
  }*/
  cout << "Pm original: " << solution.getP_median() << " Pm delete: " << min_p_median_neighbor << endl;
  if (min_p_median_neighbor < solution.getP_median()) {
    return Solution(best_sp_neighbor, this->points, min_p_median_neighbor);
  }
  return solution; // No hay ningún vecino mejor
}


Solution Grasp::local_search(Solution solution) {
  Solution current_solution = solution;
  while(true) {
    Solution best_neighbor = generate_neighbor_solution(current_solution); 
    if (best_neighbor.getP_median() < current_solution.getP_median()) {
      current_solution = best_neighbor;
    } else {
      break;
    }
  }
  return current_solution;
}

point Grasp::takeRandomFromLRC() {
  if (LRC.size() > 0) {
    int random_index = std::rand() % this->LRC.size();
    return LRC[random_index];
  } else {
    throw std::runtime_error("La LRC está vacía");
  }
}

// LRC con los puntos más alejados de la solución
// Distancia de un punto al conjunto DE SOLUCIONES distancia mínima a uno de los puntos
void Grasp::generateLRC(Solution current_solution) {
  vector<point> current_service_points = current_solution.get_service_points();
  this->LRC.clear();
  this->LRC = points.farthestPoints(current_service_points, this->LRCsize);
}

// FASE CONSTRUCTIVA TERMINADA
Solution Grasp::construction_phase(int k, int LRCsize) {
  Solution sol;
  point inicial_point = points.takeRandomPoint();
  sol.add_service_point(inicial_point);
  for(int i = 0; i < k - 1; i++) { // Ya tenemos un punto de servicio, por lo que recorremos k - 1
    this->generateLRC(sol);
    sol.add_service_point(this->takeRandomFromLRC());
  }
  return sol;
}

  //Para hacer la fase constructiva descomentar todo menos sol = this->busqueda_local(sol)
Solution Grasp::grasp_algorithm(int k, int LRCsize) {
  Solution mejor_solucion;
  this->LRCsize = LRCsize;
  //for(int i = 0; i < max_iterations; i++) {
    Solution sol = this->construction_phase(k, LRCsize);
    //sol = this->local_search(sol);
    //this->actualizar(sol, mejor_solucion);
  //}
  //return mejor_solucion;
  sol.generate_groupings(points);
  sol.setP_median(calculate_Pmedian(sol.get_service_points()));
  return sol;
}

void Grasp::load(string filename) {
  points.load(filename);
}

double Grasp::calculate_Pmedian(vector<point> solution_service_points) {
  return points.calculate_Pmedian(solution_service_points);
}


#endif