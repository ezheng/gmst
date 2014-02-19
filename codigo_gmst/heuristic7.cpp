/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heuristic7.h"

Heuristic7::Heuristic7(){}


Heuristic7::~Heuristic7(){}

/**
 * Define uma ordem em que os grupos serão visitados
*/
vector<int> Heuristic7::defineOrder(){

	//Definindo a ordem em que os clusters serao visitados
	vector<int> order;
	vector<bool> selectedClusters(Graph::graph.numberOfClusters, false);

	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){
		int cluster = rand()%Graph::graph.numberOfClusters;

		while(selectedClusters[cluster]){
			cluster = (cluster+1)%Graph::graph.numberOfClusters;
		}
		selectedClusters[cluster] = true;
		order.push_back(cluster);
	}
	
	return order;
}

/**
* Metodo que calcula a avore = Method that calculates the tree
*/
vector<int> Heuristic7::calculateTree(){

	//cout << "Calculando arvore com heuristica 7 (gulosa)." << endl;
	vector<int> solution(Graph::graph.numberOfClusters);

	vector<int> order = defineOrder();
	//Para cada cluster, procura o vertice mais proximo dos outros clusters = For each cluster, looking for the nearest vertex of the other clusters
	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){

		unsigned int cluster = order[i];
		vector<int> vertices = Graph::graph.clusters[cluster];
		int bestVertex = NONE;
		long minDistance = INFINITE;

		//Para cada vertice do cluster, soma sua distancia aos vertices dos outros clusters = For each vertex of the cluster, its distance sum to the vertices of the other clusters
		for(unsigned int j=0; j < vertices.size();j++){
			int vertex = vertices[j];
			long totalDistance = 0;
			int num=0;

			for(unsigned int l=0; l < Graph::graph.neighbors[cluster].size(); l++){
				unsigned int c = Graph::graph.neighbors[cluster][l];
//			for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
				if(solution[c] == NONE){
					for(unsigned int k=0; k < Graph::graph.clusters[c].size();k++){
						if(Graph::graph.exists[Graph::graph.clusters[c][k]][vertex]){
							totalDistance += Graph::graph.adjacency[Graph::graph.clusters[c][k]][vertex];
							num++;
						}
					}
				} else if(Graph::graph.exists[solution[c]][vertex]){
					totalDistance += Graph::graph.adjacency[solution[c]][vertex];
					num++;
				}
			}

			if(num != 0)
				totalDistance /= num;
			else
				totalDistance = INFINITE;
			if(totalDistance < minDistance){
				bestVertex = vertex;
				minDistance = totalDistance;
			}
		}
		//Seta o melhor vertice do cluster atual
		if(bestVertex == NONE){
			solution[cluster] = Graph::graph.clusters[cluster][rand()%Graph::graph.clusters[cluster].size()];
		} else {			
			solution[cluster] = bestVertex;		
		}
	}
		
	return solution;
}

vector<int> Heuristic7::calculateRandomTree(){

	//cout << "Calculando arvore com heuristica 7 (aleatoria)." << endl;

	vector<int> solution(Graph::graph.numberOfClusters, NONE);
	vector<int> order = defineOrder();
	//Para cada cluster, procura o vertice mais proximo dos outros clusters
	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){
				
		unsigned int cluster = order[i];
		
		vector<int> vertices = Graph::graph.clusters[cluster];
		vector<int> distances(vertices.size(),0);
		
		int minDistance = INFINITE;
		int maxDistance = 0;		

		//Para cada vertice do cluster, soma sua distancia aos vertices dos outros clusters
		for(unsigned int j=0; j < vertices.size();j++){
			int vertex = vertices[j];
			int num=0;

			for(unsigned int l=0; l < Graph::graph.neighbors[cluster].size(); l++){
				unsigned int c = Graph::graph.neighbors[cluster][l];
			//for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
				if(solution[c] == NONE){
					for(unsigned int k=0; k < Graph::graph.clusters[c].size();k++){
						if(Graph::graph.exists[Graph::graph.clusters[c][k]][vertex]){
							distances[j] += Graph::graph.adjacency[Graph::graph.clusters[c][k]][vertex];
							num++;
						}
					}
				} else if(Graph::graph.exists[solution[c]][vertex]){
					distances[j] += Graph::graph.adjacency[solution[c]][vertex];
					num++;
				}
			}

			if(num != 0)
				distances[j] /= num;
			else
				distances[j] = INFINITE;
			minDistance = min(minDistance, distances[j]);
			maxDistance = max(maxDistance, distances[j]);
		}
		int dMax = (int)(minDistance + (float)(maxDistance - minDistance)*Parameters::getInstance().ALPHA);
		
		//Criando a lista de candidatos restrita
		vector<int> candidates;

		for(unsigned int j=0; j < distances.size(); j++){
			if(distances[j] <= dMax){
				candidates.push_back(vertices[j]);
			}
		}

		if(candidates.size() > 0){
			//Escolhendo um dos vertices
			int index = rand()%candidates.size();
			int choosenVertex = candidates[index];
		
			//Seta o melhor vertice do cluster atual
			solution[cluster] = choosenVertex;
		}else{
			solution[cluster] = Graph::graph.clusters[cluster][rand()%Graph::graph.clusters[cluster].size()];
		}
	}
	return solution;
}
