/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heuristic1.h"

Heuristic1::Heuristic1(){}

Heuristic1::~Heuristic1(){}

/**
 * Adaptacao de Prim gulosa
 */
vector<int> Heuristic1::calculateTree(){

	vector<int> bestTree;
	int bestCost = INFINITE;

	//Heap utilizado como estrutura auxiliar
	Heap heap;
	
	//Para cada cluster como raiz
	for(unsigned int root = 0; root < Graph::graph.numberOfClusters; root++){
	
		//Armazena o vertice de cada cluster = Stores the vertex of each cluster
		vector<int> tree(Graph::graph.numberOfClusters,NONE);
		//Armazena a distâcia de cada cluster ao cluster raiz
		vector<int> distances(Graph::graph.numberOfClusters, INFINITE);
		//Marca os clusters que já estão definitivamente na arvore
		vector<bool> mark(Graph::graph.numberOfClusters, false);
	
		// Raiz da arvore é o vértice do cluster 'root' que possui a menor aresta
		tree[root] =  Graph::graph.getBestVertex(root);
		distances[root] = 0;
	
		//Inicializando o algoritmo
		for (unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
			heap.insert(c,distances[c]);
		}
		//Enquanto a heap nao estiver vazia
		while(!heap.empty()){
	
			int cluster = heap.findAndDeleteMin();
			int vertex = tree[cluster];
			mark[cluster] = true;
			
			//Para cada cluster do grafo
			for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
				
				//Se o cluster ainda não foi visitado
				if(!mark[c]){
					//currVertex é o vertice do cluster 'c' mais proximo de 'vertex'
					int currVertex = Graph::graph.getClosestVertex(c, vertex);
					//Se o custo da aresta 'vertex'-'currVertex' é menor que a distancia atual do no cluster 'c'
					if((currVertex != NONE) && (Graph::graph.adjacency[vertex][currVertex] < distances[c])){
						distances[c] = Graph::graph.adjacency[vertex][currVertex];
						heap.adjust(c,Graph::graph.adjacency[vertex][currVertex]);
						tree[c] = currVertex;
					}
				}
			}
		}		
		if(Heuristic5::getMSTCost(tree) < bestCost){
			bestTree = tree;
		}		
	}
	return bestTree;
}

/**
 * Adaptacao de Prim gulosa aleatória
 */
vector<int> Heuristic1::calculateRandomTree(){

	vector<int> bestTree;
	int bestCost = INFINITE;

	//Para cada cluster como raiz
	for(unsigned int root = 0; root < Graph::graph.numberOfClusters; root++){
		
		//Armazena o vertice de cada cluster
		vector<int> tree(Graph::graph.numberOfClusters,NONE);
		//Armazena a distâcia de cada cluster ao cluster raiz
		vector<int> distances(Graph::graph.numberOfClusters, INFINITE);
		//Marca os clusters que já estão definitivamente na arvore
		vector<bool> mark(Graph::graph.numberOfClusters, false);
	
		// Raiz da arvore é o vértice do cluster 'root' que possui a menor aresta		
		tree[root] =  Graph::graph.getBestVertex(root);
		distances[root] = 0;

		int cluster = root;
		unsigned int selectedClusters=1;
		
		while(selectedClusters < Graph::graph.numberOfClusters){
				
			int vertex = tree[cluster];
			mark[cluster] = true;
			
			//Para cada cluster do grafo
			for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
				
				//Se o cluster ainda não foi visitado
				if(!mark[c]){
					//currVertex é o vertice do cluster 'c' mais proximo de 'vertex'		
					int currVertex = Graph::graph.getClosestVertex(c, vertex);	
					//Se o custo da aresta 'vertex'-'currVertex' é menor que a distancia atual do no cluster 'c'
					if((currVertex != NONE) && (Graph::graph.adjacency[vertex][currVertex] < distances[c])){
						distances[c] = Graph::graph.adjacency[vertex][currVertex];
						tree[c] = currVertex;
					}
				}
			}
			//Criando lista de candidatos para selecionar o proximo cluster
			if(selectedClusters < Graph::graph.numberOfClusters){
				int minDistance = INFINITE;
				int maxDistance = 0;
				for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
					if(!mark[c]){
						minDistance = min(minDistance, distances[c]);
						maxDistance = max(maxDistance, distances[c]);
					}
				}
				//Criando lista de candidatos
				vector<int> candidates;
				for(unsigned int c=0; c < Graph::graph.numberOfClusters; c++){
					float distance = minDistance + Parameters::getInstance().ALPHA*(maxDistance - minDistance);
					if((!mark[c]) && (distances[c] <= distance) && (distances[c] < INFINITE)){
						candidates.push_back(c);
					}
				}
				//Selecionando proximo cluster a ser considerado
				cluster = candidates[rand()%candidates.size()];
			}
			selectedClusters++;
		}
		if(Heuristic5::getMSTCost(tree) < bestCost){
			bestTree = tree;
		}
	}
	return bestTree;
}
