#ifndef GRASP
#define GRASP

#include "matrixPoints.h"
#include "solution.h"

class Grasp {
 public:
  Solution construction_phase(int k, int LRCsize);
  Solution GVNS(Solution current_solution);
  Solution VND(Solution service_points);
  Grasp() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
  }
  void load(string filename);
  Solution grasp_algorithm(int k, int LRCsize);
  int number_of_points() {
    return this->points.number_of_points();
  }
  Solution generate_neighbor_solution(Solution solution);
 private:
  Solution best_solution;
  int LRCsize;
  MatrixPoints points;
  vector<point> LRC;
  vector<point> service_points;
  point takeRandomFromLRC();
  void generateLRC(Solution current_solution);
  Solution local_search(Solution current_solution);
  void update_best_solution(Solution new_solution, int& iwc);
  double calculate_Pmedian(vector<point> solution_service_points);

  vector<point> delete_solution_space(vector<point> original_sp);
  vector<point> insert_solution_space(vector<point> original_sp);
  vector<point> exchange_solution_space(vector<point> original_sp);

  Solution shake(Solution original_sp, int number_exchanges);

};

Solution Grasp::VND(Solution current_solution) {
  int l = 1;
  while(l < 4) { // 3 estructuras, intercambio, inserción, elminación
    vector<point> new_solution_sp;
    vector<point> current_solution_sp = current_solution.get_service_points();
    double pmedian_new_sol;
    if (l == 1) {
      new_solution_sp = exchange_solution_space(current_solution_sp);
      pmedian_new_sol = calculate_Pmedian(new_solution_sp);
    } else if (l == 2) {
      new_solution_sp = insert_solution_space(current_solution_sp);
      double penalty_factor = calculate_Pmedian(new_solution_sp) * 0.3; 
      pmedian_new_sol = calculate_Pmedian(new_solution_sp) + penalty_factor;
    } else if (l == 3){
      if (current_solution_sp.size() == 1) {
        cout << "ALERTA: va a empezar a generar soluciones vacias DISMINUIR support_factor" << endl;
      }
      new_solution_sp = delete_solution_space(current_solution_sp);
      double support_factor = calculate_Pmedian(new_solution_sp) * 0.2;
      pmedian_new_sol = calculate_Pmedian(new_solution_sp) - support_factor;
    }
    Solution new_solution(new_solution_sp, this->points, pmedian_new_sol);
    if (new_solution.getP_median() < current_solution.getP_median()) {
      current_solution = new_solution;
      l = 1;
    } else {
      l++;
    }
  }
  //current_solution.show_service_points();
  return current_solution;
}

Solution Grasp::shake(Solution original_solution, int number_exchanges) {
  vector<point> original_sp = original_solution.get_service_points();
  if (original_sp.size() > 0) {
    int exchanges_completed = 0;
    vector<point> already_used;
    while(exchanges_completed < number_exchanges) {
      point candidate_point = points.takeRandomPoint();
      if (std::find(original_sp.begin(), original_sp.end(), candidate_point) != original_sp.end()
       || std::find(already_used.begin(), already_used.end(), candidate_point) != already_used.end()) {
        continue;
      }
      int random_index = std::rand() % original_sp.size();
      already_used.push_back(original_sp[random_index]);
      original_sp[random_index] = candidate_point;
      exchanges_completed++;
    }
    return Solution(original_sp, this->points, calculate_Pmedian(original_sp));
  } else {
    cout << "No se pudo hacer el agitamiento, número de puntos de servicio: " << original_sp.size() << endl;
    return original_solution;
  }
}

Solution Grasp::GVNS(Solution current_solution) {
  int max_iterations = 10;
  int kmax = 4; // Número máximo de intercambios en el shaking + 1
  vector<point> current_sp = current_solution.get_service_points();
  if (current_sp.size() <= 1) {
    cout << current_sp.size();
    throw std::runtime_error("No se puede hacer GVNS con un solo punto de servicio");
  }
  if (current_sp.size() == 2) {
    int kmax = 2; // numIntercambiosPosibles + 1;
  }
  for(int i = 0; i < max_iterations; i++) {
    int k = 1;
    while(k < kmax) {
      Solution shake_sp = shake(current_solution, k);
      Solution local_optimum = VND(shake_sp);
      //cout << "en gvns" << endl;
      //local_optimum.show_service_points();
      //cout << "curretn sol" << endl;
      //current_solution.show_service_points();
      if (local_optimum.getP_median() < current_solution.getP_median()) {
        k = 1;
        current_solution = local_optimum;        
      } else {
        k += 1;
      }
    }
  }
  return current_solution;
}

void Grasp::update_best_solution(Solution new_solution, int& iwc) {
  if(this->best_solution.isEmpty()) {
    best_solution = new_solution;
    return;
  }
  if(new_solution.getP_median() < best_solution.getP_median()) {
    iwc = 0;
    best_solution = new_solution;
    return;
  }
  iwc += 1;
}

vector<point> Grasp::exchange_solution_space(vector<point> original_sp) {
  vector<point> best_exchanged_sp = original_sp;
  double min_p_median = calculate_Pmedian(original_sp);
  for (size_t i = 0; i < original_sp.size(); ++i) {
    for (const point& candidate_point : points.pointsMatrix) {
      // Comprueba si candidate_point ya está en original_sp
      if (std::find(original_sp.begin(), original_sp.end(), candidate_point) != original_sp.end()) {
        continue;
      }
      vector<point> exchanged_sp = original_sp;
      exchanged_sp[i] = candidate_point;
      double current_p_median = calculate_Pmedian(exchanged_sp); 
      /* cout << "Mirando punto con pmedia: " << current_p_median << ": ";
      for(const point& p: exchanged_sp) {
        cout << p.first << " ";
      }
      cout << endl; */
      if (current_p_median < min_p_median) {
        min_p_median = current_p_median;
        best_exchanged_sp = exchanged_sp;
      }
    }
  }
  return best_exchanged_sp;
}

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

vector<point> Grasp::insert_solution_space(vector<point> original_sp) {
  
  vector<point> best_inserted_sp;
  double min_p_median = std::numeric_limits<double>::max();
  for (const point& candidate_point : points.pointsMatrix) {
    // Comprueba si candidate_point ya está en original_sp
    if (std::find(original_sp.begin(), original_sp.end(), candidate_point) != original_sp.end()) {
      continue;
    }
    vector<point> inserted_sp = original_sp;
    inserted_sp.push_back(candidate_point);
    double current_p_median = calculate_Pmedian(inserted_sp);
    if (current_p_median < min_p_median) {
      min_p_median = current_p_median;
      best_inserted_sp = inserted_sp;
    }
  }
  return best_inserted_sp;
}

// Genera la (mejor/primera mejor)? solucion vecina, si no hay ninguna mejor devuelve la misma 
Solution Grasp::generate_neighbor_solution(Solution solution) {
  // Movimientos de eliminación, inserción e intercambio, cada uno genera una nueva lista de service_points
  // Para cada una comprobar su Pmediana y quedarnos con la mejor, (PARA HAYAR LA PMEDIANA DE LOS VECINOS HAY Q HACER ESO)
  vector<point> current_service_points = solution.get_service_points();
  //--------delete---------------------------------------------------------------
  vector<point> best_sp_delete = this->delete_solution_space(current_service_points);
  double p_median_delete = calculate_Pmedian(best_sp_delete);
  double support_factor = p_median_delete * 0.075;
  p_median_delete -= (support_factor * best_sp_delete.size()); // Apoyando eliminación
  //--------insert---------------------------------------------------------------
  vector<point> best_sp_insert = this->insert_solution_space(current_service_points);
  double p_median_insert = calculate_Pmedian(best_sp_insert);
  double penalty_factor = p_median_insert * 0.1; 
  p_median_insert += (penalty_factor * best_sp_insert.size()); // Penalizando inserción 
  //--------intercambio-----------------------------------------------------------
  vector<point> best_sp_exchange = this->exchange_solution_space(current_service_points);
  double p_median_exchange = calculate_Pmedian(best_sp_exchange);
  double min_p_median_neighbor = std::min({p_median_delete, p_median_insert, p_median_exchange});
  vector<point> best_sp_neighbor;
  if(min_p_median_neighbor == p_median_delete) {
    best_sp_neighbor = best_sp_delete;
  } else if(min_p_median_neighbor == p_median_insert) {
    best_sp_neighbor = best_sp_insert;
  } else {
    best_sp_neighbor = best_sp_exchange;
  }
  //cout << "Pm original: " << solution.getP_median() << " Pm menor: " << min_p_median_neighbor << endl;
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
  //cout << LRC.size();
}

Solution Grasp::construction_phase(int k, int LRCsize) {
  this->LRCsize = LRCsize;
  Solution sol;
  point inicial_point = points.takeRandomPoint();
  sol.add_service_point(inicial_point);
  for(int i = 0; i < k - 1; i++) { // Ya tenemos un punto de servicio, por lo que recorremos k - 1
    this->generateLRC(sol);
    sol.add_service_point(this->takeRandomFromLRC());
  }
  sol.update_data(this->points, true); // No generamos los agrupamientos, solo p mediana
  return sol;
}

// Para hacer la fase constructiva descomentar todo menos sol = this->busqueda_local(sol)
Solution Grasp::grasp_algorithm(int k, int LRCsize) {
  const int max_iterations_without_change = 10;
  const int max_iterations = 50;
  if (this->points.pointsMatrix.empty()) {
    throw std::runtime_error("No se puede hacer el algoritmo sin antes cargar un fichero de datos");
  }
  this->best_solution.clear();
  int iterations_without_change = 0;
  this->LRCsize = LRCsize;
  for(int i = 0; i < max_iterations; i++) {
    Solution sol = this->construction_phase(k, LRCsize);
    sol = this->GVNS(sol);
    this->update_best_solution(sol, iterations_without_change);
    if (iterations_without_change > max_iterations_without_change) {
      break;
    }
  }
  best_solution.update_data(points);
  this->service_points.clear(); // Limpiamos por si queremos usar el algoritmo varias veces
  this->LRC.clear();
  return this->best_solution;
}

void Grasp::load(string filename) {
  points.load(filename);
}

double Grasp::calculate_Pmedian(vector<point> solution_service_points) {
  return points.calculate_Pmedian(solution_service_points);
}


#endif