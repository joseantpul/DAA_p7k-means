#ifndef KMEANS
#define KMEANS



// 
// Resolucion resolucion <-- Problema instancia El objeto Resolución tiene dentro un objeto problema de donde saca la matriz de distancia etc
/* seleccionar k puntos como centroides iniciales <vector<puntos> > centroidesActuales; 
*  resolucion.centroides = centroidesActuales; 
*/
// do
//   vector<cluster> cluster_antiguo = resolucion.matrizCluster;
/*   Construir k clusters asignando a cada punto el centroide mas cercano
*    resolucion.construirClusters()
*/
/*   Recalcular centroides de cada cluster
*    resolucion.recalcularCentroides()
*/
//  while(cluster_antiguo = resolucion.matrizCluster)
// return Solucion solucion(resolucion) // del objeto resolucion saca lo que le importa


/**
 * class Resolucion {
 *   map<int, vector<puntos> > clusters //Para cada cluster (0, 1, numClusters - 1) sus elementos
 *   Problema instancia // de aquí se sacan la matriz de distancia y todo eso 
 *   int numClusters // número de clusters
 *   map<int, puntos> centroides Centroide para cada cluster (cada cluster 0, 1 ... numClusters - 1)
 *   
 *   métodos
 *   construirClusters() usando los centroides construir los clusters asignando a cada punto el centroide más cercano
 *   recalcularCentroides() para cada (nuevo) cluster generar los centroides (de nuevo) 
 * }
*/




#endif