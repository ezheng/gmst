/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heuristic6.h"

Heuristic6::Heuristic6(){}


Heuristic6::~Heuristic6(){}
/**
* Metodo que calcula a avore
*/
vector<int> Heuristic6::calculateTree(){

	//Inicializando as estruturas de dados
	vector<int> solution(Graph::graph.numberOfClusters, NONE);
	
	int numberOfComponents = Graph::graph.numberOfClusters;
	SetUnion set(numberOfComponents);

	//Como as arestas do grafo ja estao ordenadas pelos seus pesos = As the graph edges are already sorted by their weights
	//consider-sea cada aresta na ordem em que estao armazenadas = sea​​-consider each edge in the order they are stored
	//Enquanto ainda existem componentes desconexas no grafo = While there are still disconnected components in the graph
	unsigned int edge=0;
	while((edge < Graph::graph.edges.size())&&(numberOfComponents > 1)){

		//cout << "Verificando aresta " << edge << " (Peso " << Graph::graph.edges[edge][COST] << ")" << endl;
		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		unsigned int cluster1 = Graph::graph.clusterOf[vertex1];

		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		unsigned int cluster2 = Graph::graph.clusterOf[vertex2];
		//If the vertices of the edge are already in solution and clusters are in different components, is a union of components
		//Se os vertices da aresta ja estao na solucao e os clusters estao em componentes diferentes, faz a uniao dos componentes
		if(((solution[cluster1] == NONE)||(solution[cluster1] == vertex1)) && ((solution[cluster2] == NONE)||(solution[cluster2] == vertex2)) && (set.join(cluster1, cluster2))){
			
			solution[cluster1] = vertex1;
			solution[cluster2] = vertex2;
			numberOfComponents--;			
		}		
		edge++;
	}
	return solution;
}

vector<int> Heuristic6::calculateRandomTree(){

	//float alpha = Parameters::getInstance().ALPHA;
	float alpha = 0.001;
	
	//Inicializando as estruturas de dados
	vector<int> solution(Graph::graph.numberOfClusters, NONE);
	
	int numberOfComponents = Graph::graph.numberOfClusters;
	SetUnion set(numberOfComponents);
	
	//Iniciando vetor com as arestas que vao sendo escolhidas
	vector<bool> choosen(Graph::graph.edges.size(), false);
	//Vetor com as arestas que formarao a lista restrita de candidatos
	vector<int> candidates;

	int minCost = Graph::graph.edges.front()[COST];
	
	//Como as arestas do grafo já estão ordenadas pelos seus pesos
	//considera cada aresta na ordem em que estao armazenadas
	//Enquanto ainda existem componentes desconexas no grafo
	unsigned int i=0;
	bool redoRCL = true;
	while((i < Graph::graph.edges.size()) && (numberOfComponents > 1)){

//   		cout << "Numero de candidatos: " << candidates.size() << endl;
//   		cout << "Numero de componentes: " << numberOfComponents << endl;
//  		cout << "MinCost: " << minCost << endl;

		if(redoRCL){
			//Criando lista de candidatos restrita
			unsigned int j=0;
			int maxCost = minCost + (int)((Graph::graph.edges.back()[COST] - minCost)*alpha);
			candidates.clear();
			while((j < Graph::graph.edges.size()) && (Graph::graph.edges[j][COST] <= maxCost)){
				if(!choosen[j])
					candidates.push_back(j);
				j++;
			}
			redoRCL = false;
		}
		//Escolhendo a aresta da lista de candidatos
		int edgeIndex = rand()%candidates.size();
		while(choosen[candidates[edgeIndex]]){
			edgeIndex =(edgeIndex+1)%candidates.size();
		}
		int edge = candidates[edgeIndex];
		if(Graph::graph.edges[edge][COST] == minCost){
			int k=1;
			while(choosen[edge+k]){
				k++;
			}
			minCost = Graph::graph.edges[edge+k][COST];
			redoRCL = true;
		}

		choosen[edge] = true;

		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		int cluster1 = Graph::graph.clusterOf[vertex1];

		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		int cluster2 = Graph::graph.clusterOf[vertex2];

		if(((solution[cluster1] == NONE)||(solution[cluster1] == vertex1)) && ((solution[cluster2] == NONE)||(solution[cluster2] == vertex2)) && (set.join(cluster1, cluster2))){
			
			solution[cluster1] = vertex1;
			solution[cluster2] = vertex2;
			numberOfComponents--;			
		}
		i++;
	}
	return solution;
}

/**
* Metodo que calcula a arvore, recebendo os vertices que ja estao definidos para os clusters
*/
int Heuristic6::calculateTree(vector<int> &solution){
	
	//Inicializando as estruturas de dados		
	int numberOfComponents = Graph::graph.numberOfClusters;
	SetUnion set(numberOfComponents);	

	//Como as arestas do grafo ja estao ordenadas pelos seus pesos
	//consider-sea cada aresta na ordem em que estao armazenadas
	//Enquanto ainda existem componentes desconexas no grafo
	unsigned int edge=0;
	while((edge < Graph::graph.edges.size())&&(numberOfComponents > 1)){

		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		int cluster1 = Graph::graph.clusterOf[vertex1];

		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		int cluster2 = Graph::graph.clusterOf[vertex2];

		if(((solution[cluster1] == NONE)||(solution[cluster1] == vertex1)) && ((solution[cluster2] == NONE)||(solution[cluster2] == vertex2)) && (set.join(cluster1, cluster2))){
			
			solution[cluster1] = vertex1;
			solution[cluster2] = vertex2;
			numberOfComponents--;			
		}		
		edge++;
	}
	return Heuristic5::getMSTCost(solution);
}

/**
* Calcula uma arvore de cobertura minima com as arestas passadas, a partir de uma adaptacao da heuristica 6
* Adiciona as arestas referentes à arvore do grupo à arvore que é passada como parametro (tree)
*/
vector<int> Heuristic6::calculateTrees(vector<int> &indices, int numberOfSets){

	vector<int> solution(Graph::graph.numberOfClusters, NONE);
	
	//Armazena os vertices de saida de cada cluster		
	int numberOfComponents = Graph::graph.numberOfClusters;
	SetUnion set(numberOfComponents);
		
	//Como as arestas do grafo ja estao ordenadas pelos seus pesos
	//considera-se cada aresta na ordem em que estao armazenadas
	//Enquanto ainda existem componentes desconexas no grafo
	unsigned int edge=0;
	
	while((edge < Graph::graph.edges.size())&&(numberOfComponents > numberOfSets)){
		
		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		int cluster1 = Graph::graph.clusterOf[vertex1];
		
		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		int cluster2 = Graph::graph.clusterOf[vertex2];
		
		//Só considera arestas que unem vertices que estao no mesmo conjunto de clusters
		if((indices[cluster1] == indices[cluster2]) && ((solution[cluster1] == NONE)||(solution[cluster1] == vertex1)) && ((solution[cluster2] == NONE)||(solution[cluster2] == vertex2)) && (set.join(cluster1, cluster2))){
			
			solution[cluster1] = vertex1;
			solution[cluster2] = vertex2;
			numberOfComponents--;
		}
		edge++;
	}
	
	for(unsigned int cluster=0; cluster < solution.size(); cluster++){
		//O cluster ficou sozinho no agrupamento dos clusters maiores
		if(solution[cluster] == -1){
			//Selecionando um vértice aleatoriamente
			//(Isso pode ser mudado para selecionar o vertice mais proximo de algum dos outros vertices)
			solution[cluster] = Graph::graph.clusters[cluster][rand()%Graph::graph.clusters[cluster].size()];
		}
	}
	return solution;
}
