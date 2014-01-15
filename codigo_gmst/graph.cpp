/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 06 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "graph.h"

Graph Graph::graph;

Graph::Graph(){}

void Graph::createInstance(Instance& instance){

	Graph g(instance, 0, 0);
	graph = g;
}

void Graph::createInstance(Instance& instance, int _target, double _timeLimit){
	
	Graph g(instance, _target, _timeLimit);
	graph = g;
}

Graph::Graph(Instance& instance, int _target, double _timeLimit){
 		
	timeLimit = _timeLimit;
	target = _target;
	bestMSTAlgorithm = PRIM;
	instanceName = instance.name;
	
	initializeStructures(instance);
}

Graph::~Graph(){}

/**
* Metodo que inicializa as estruturas de dados do grafo
*/
void Graph::initializeStructures(Instance& instance){

	edges.resize(instance.getNumberOfEdges());
	for(int i=0; i < instance.getNumberOfEdges(); i++)
		edges[i].resize(3);

	adjacency.resize(instance.getNumberOfVertices());
	exists.resize(instance.getNumberOfVertices());
	//adjVertices.resize(instance.getNumberOfVertices());
	adjClusters.resize(instance.getNumberOfVertices());
	for(int i=0; i < instance.getNumberOfVertices(); i++){
		adjacency[i].assign(instance.getNumberOfVertices(), INFINITE);
		exists[i].assign(instance.getNumberOfVertices(), false);
	}
		

	numberOfVertices = instance.getNumberOfVertices();
	numberOfClusters = instance.getNumberOfClusters();
	createClusters(instance.getVertices());
	 	
	minDistances.resize(numberOfClusters);
	maxDistances.resize(numberOfClusters);	
	neighbors.resize(numberOfClusters);
	for(unsigned int i=0; i < numberOfClusters; i++){
		minDistances[i].assign(numberOfClusters, INFINITE);
		maxDistances[i].assign(numberOfClusters, 0);		
	}		
	
    createEdgesArray(instance.getVertices1(), instance.getVertices2(), instance.getEdgesCost());

	for(unsigned int i=0;i < numberOfVertices; i++){
		for(unsigned int j=0; j < numberOfClusters; j++){
			if(j != (unsigned)clusterOf[i]){
				adjClusters[i].push_back(j);
			}
		}
	}
}

/**
* Metodo que inclui as arestas do grafo
*/
void Graph::createEdgesArray(vector<int> vertices1, vector<int> vertices2, vector<int> edgesCost){		
		
	int totalCost = 0;
	for(unsigned int i=0; i < vertices1.size(); i++){
		
		edges[i][VERTEX1] = vertices1[i];
		edges[i][VERTEX2] = vertices2[i];
		edges[i][COST] = edgesCost[i];

		totalCost += edgesCost[i];
		
		//Armazenando a menor distancia entre cada par de clusters
		if(minDistances[clusterOf[vertices1[i]]][clusterOf[vertices2[i]]] > edgesCost[i]){

			minDistances[clusterOf[vertices1[i]]][clusterOf[vertices2[i]]] = edgesCost[i];
			minDistances[clusterOf[vertices2[i]]][clusterOf[vertices1[i]]] = edgesCost[i];
		}

		if(maxDistances[clusterOf[vertices1[i]]][clusterOf[vertices2[i]]] < edgesCost[i]){

			maxDistances[clusterOf[vertices1[i]]][clusterOf[vertices2[i]]] = edgesCost[i];
			maxDistances[clusterOf[vertices2[i]]][clusterOf[vertices1[i]]] = edgesCost[i];
		}

		//Armazenando o índice de cada aresta em forma de matriz de adjacências
		adjacency[vertices1[i]][vertices2[i]] = edgesCost[i];
		adjacency[vertices2[i]][vertices1[i]] = edgesCost[i];
		exists[vertices1[i]][vertices2[i]] = true;
		exists[vertices2[i]][vertices1[i]] = true;
// 		adjVertices[vertices1[i]].push_back(vertices2[i]);
// 		adjVertices[vertices2[i]].push_back(vertices1[i]);
	}

	int ray = totalCost/((int)Parameters::getInstance().NEIGHBOR_DISTANCE*edges.size());

	//Preenchendo as listas de clusters vizinhos
	for(unsigned int cluster1=0; cluster1 < numberOfClusters; cluster1++){
		for(unsigned int cluster2=cluster1+1; cluster2 < numberOfClusters; cluster2++){
			if(minDistances[cluster1][cluster2] < ray){
				neighbors[cluster1].push_back(cluster2);
				neighbors[cluster2].push_back(cluster1);
			}
		}
	}
}

/**
* Metodo que ordena as arestas do grafo
*/
void Graph::sortEdges(){

	quicksort(0, edges.size());
}

/**
* Implementa o método de ordenacao quicksort sobre as arestas do grafo
*/
void Graph::quicksort(int begin, int end){
    if (end - begin > 1) {
       int pivot = edges[begin][COST];
       int l = begin + 1;
       int r = end;
       while(l < r) {
          if (edges[l][COST] <= pivot) {
             l++;
          } else {
             r--;
             changeEdges(l, r); 
          }
       }
       l--;
       changeEdges(begin, l);
       quicksort(begin, l);
       quicksort(r, end);
    }
}

/**
* Metodo que troca as arestas 'edge1' e 'edge2'
*/
void Graph::changeEdges(int edge1, int edge2){
	
	int vertex1 = edges[edge1][VERTEX1];
	int vertex2 = edges[edge1][VERTEX2];
	int cost = edges[edge1][COST];

	setEdge(edge1, edges[edge2][VERTEX1], edges[edge2][VERTEX2], edges[edge2][COST]);
	setEdge(edge2, vertex1, vertex2, cost);
}

/**
* Seta uma aresta com indice 'index'
*/
void Graph::setEdge(int index, int vertex1, int vertex2, int cost){
	
	edges[index][VERTEX1] = vertex1;
	edges[index][VERTEX2] = vertex2;
	edges[index][COST] = cost;
}

/**
* Metodo que seta as informações sobre os clusters
*/
void Graph::createClusters(vector<int> _clusterOf){

	clusters.resize(numberOfClusters);
	clusterOf = _clusterOf;

	for(unsigned int i=0; i < clusterOf.size(); i++){
		clusters[clusterOf[i]].push_back(i);
	}	
}

/**
* Imprime o grafo
*/
void Graph::print(){

	cout << "Number Of Clusters: " << numberOfClusters << endl ;
	printClusters();
	
	cout << endl << "Arestas do Grafo (Peso V1 V2): " << endl;

	for(unsigned int i=0; i < edges.size(); i++){
		cout << edges[i][COST] << " " << edges[i][VERTEX1] << " " << edges[i][VERTEX2] << endl;
	}

	cout << endl;
}

/**
* Retorna todos as arestas saintes ou entrantes em 'vertex'
*/
vector<int> Graph::edgesInVertex(int vertex){

	vector<int> edgesInVertex;

	for(unsigned int i=0; i < edges.size(); i++){
		if((edges[i][VERTEX1] == vertex) || (edges[i][VERTEX2] == vertex))
			edgesInVertex.push_back(i);
	}

	return edgesInVertex;
}

/**
* Retorna o vértice da aresta diferente do vértice passado
* Ex.: aresta 1:1->2  getNode(1,1) = 2, getNode(1,2) = 1
*/
int Graph::getOpposite(int edge, int vertex){

	if(edges[edge][VERTEX1] == vertex)
		return edges[edge][VERTEX2];
	else
		return edges[edge][VERTEX1];
}

/**
* Retorna todos os vertices pertencentes ao cluster passado
*/
vector<int> Graph::getVerticesFromCluster(int cluster){
	return clusters[cluster];
}

/**
* Seleciona aleatoriamente um vertice do cluster
*/
int Graph::chooseAVertexFromCluster(int cluster){

	return clusters[cluster][rand()%clusters[cluster].size()];
}

/**
* Seleciona aleatoriamente um vertice do cluster diferente do vertice passado
*/
int Graph::chooseAVertexFromCluster(int cluster, int vertex){

	int index = rand()%clusters[cluster].size();
	//Se o vertice escolhido for igual a 'vertex', seleciona o seu sucessor
	while(clusters[cluster][index] == vertex)
		index = rand()%clusters[cluster].size();
	
	return clusters[cluster][index];
}

/**
* Metodo que imprime os vertices de cada cluster
*/
void Graph::printClusters(){
	
	for(unsigned int i=0; i < numberOfClusters; i++){
		cout << (i+1) << ":";
		for(unsigned int j=0; j < clusters[i].size(); j++)
			cout << " " << clusters[i][j];
		cout << endl;
	}	
}

/**
 * Retorna o vértice do cluster passado que possui a menor distancia para algum outro vértice do grafo
 */
int Graph::getBestVertex(int cluster){

	int bestVertex = -1;
	int minDistance = INFINITE;

	//Para cada vértice do cluster passado
	for(unsigned int i=0; i < clusters[cluster].size(); i++){

		int vertex = clusters[cluster][i];
		//Para cada vértice do grafo que não pertence ao cluster passado
		for(unsigned int j=0; j < numberOfVertices; j++){

			//Se a distancia do vertice j a 'vertex' é menor que a minima
			if((exists[vertex][j]) && (adjacency[vertex][j] < minDistance)){
				minDistance = adjacency[vertex][j];
				bestVertex = vertex;
			}
		}
	}
	return bestVertex;
}

/**
 * Retorna o vértice do cluster passado com menor distancia para o vértice 'vertex'
 */
int Graph::getClosestVertex(int cluster, int vertex){

	int closestVertex = NONE;
	int minDistance = INFINITE;
	
	for(unsigned int i=0; i < clusters[cluster].size(); i++){

		int currVertex = clusters[cluster][i];
		if((exists[vertex][currVertex]) && (adjacency[vertex][currVertex] < minDistance)){
			closestVertex = currVertex;
			minDistance = adjacency[vertex][currVertex];
		}
	}
	return closestVertex;
}

/**
 *
 * Aplica os testes de redução sobre a instância   Applies the reduction tests on body
 */
void Graph::reductionTests(){

	//Criando uma lista auxiliar para percorrer e retirar as arestas === Creating a list helper to go and remove the edges
	list< vector<int> > edgesAux(edges.begin(), edges.end());

	int count=0;
	list< vector<int> > ::iterator it = edgesAux.begin();
	// Verificando se a aresta *it pode ser retirada  ===  Checking if the edge * it can be removed
	while(it!= edgesAux.end()){
		int vertex1 = (*it)[VERTEX1];
		int vertex2 = (*it)[VERTEX2];
		int cost = (*it)[COST];
		bool removeEdge = false;
		int cluster = 0;
 		//Para cada cluster == For each cluster
		while(((unsigned)cluster < numberOfClusters) && (!removeEdge)){

			if((cluster != clusterOf[vertex1]) && (cluster != clusterOf[vertex2])){
				removeEdge = true;
				unsigned int i=0;
				//Passando por todos os vertices do cluster == Going through all the vertices of the cluster
				while((i < clusters[cluster].size()) && (removeEdge)){
					int vertex = clusters[cluster][i];
					if((!exists[vertex1][vertex])||(!exists[vertex2][vertex])){
						removeEdge = false;
					} else{
						int cost1 = adjacency[vertex1][vertex];
						int cost2 = adjacency[vertex2][vertex];
						if((cost < cost1) || (cost < cost2))
						removeEdge = false;
					}
					i++;
				}
				if(removeEdge){
					std::cout << "edges removed" << std::endl;
					it = edgesAux.erase(it);
					exists[vertex1][vertex2] = false;
					exists[vertex2][vertex1] = false;
// 					adjVertices[vertex1].remove(vertex2);
// 					adjVertices[vertex2].remove(vertex1);
					count++;
				}
			}
			cluster++;
		}
		if(!removeEdge)
			it++;
	}
	
	/*it = edgesAux.begin();
	//Para cada aresta
	while(it!= edgesAux.end()){

		int v1 = (*it)[VERTEX1];
		int v2 = (*it)[VERTEX2];
		int cost = (*it)[COST];

		//removeEdge = true se a aresta DEVE ser removida, false caso contrário
		bool removeEdge = false;
		int cluster1 = 0;
 		//Procurando 'cluster1'
		while(((unsigned)cluster1 < numberOfClusters) && (!removeEdge)){

			if((cluster1 != clusterOf[v1]) && (cluster1 != clusterOf[v2])){

				//Passo1: Analisar todas as arestas do 'cluster1' ao vértice 'v1'
				removeEdge = true;
				unsigned int i=0;
				while((removeEdge) && (i < clusters[cluster1].size())){
					int v = clusters[cluster1][i];
					if(!exists[v][v1]){
						removeEdge = false;
					}else{
						int cost1 = adjacency[v][v1];
						if(cost1 > cost)
							removeEdge = false;
					}
					i++;
				}

				//Se a aresta ainda pode ser removida...
				if(removeEdge){
					removeEdge = false;
					//Procurando 'cluster2'
					int cluster2 = 0;
					while(((unsigned)cluster2 < numberOfClusters) && (!removeEdge)){

						if((cluster2 != clusterOf[v1]) && (cluster2 != clusterOf[v2])){
							removeEdge = true;
							//Passo2: Analisar todas as arestas do 'cluster2' ao vértice 'v2'
							i=0;
							while((i < clusters[cluster2].size())&&(removeEdge)){
								int v = clusters[cluster2][i];
								if(!exists[v][v2]){
									removeEdge = false;
								}else{
									int cost2 = adjacency[v][v2];
									if(cost2 > cost){
										removeEdge = false;
									}
								}
								i++;
							}

							if(removeEdge){
								//Passo3: Analisar todas as arestas do 'cluster1' ao 'cluster2'
								for(i=0; i < clusters[cluster1].size();i++){
									for(unsigned int j=0; j < clusters[cluster2].size();j++){
										//vértice do cluster1
										int k1 = clusters[cluster1][i];
										//vértice do cluster2
										int k2 = clusters[cluster2][j];
										if(!exists[k1][k2]){
											removeEdge = false;
										}else{
											int costAux = adjacency[k1][k2];
											if(costAux > cost){
												removeEdge = false;
											}
										}
									}
								}
							}
							//Agora a aresta pode ser removida
							if(removeEdge){
								it = edgesAux.erase(it);
								exists[v1][v2] = false;
								exists[v2][v1] = false;
// 								adjVertices[v1].remove(v2);
// 								adjVertices[v2].remove(v1);
							}
						}
						cluster2++;
					}
				}
			}
			cluster1++;
		}
		if(!removeEdge)
			it++;
	}*/

	cout << "** " << 100*edgesAux.size()/edges.size() << "% de arestas restantes" << endl;
	//Inserindo todas as arestas restantes novamente no grafo  Inserting all the remaining edges in the graph again
	edges.clear();
	edges.insert(edges.begin(), edgesAux.begin(), edgesAux.end());

	adjClusters.clear();
	adjClusters.resize(numberOfVertices);
	
	vector< vector<bool> > flags(numberOfVertices);
	for(unsigned int i=0; i < numberOfVertices; i++){
		flags[i].assign(numberOfClusters, false);
	}

	//Inserindo no array de nós adjacentes os nós de cada aresta que nao foi retirada
	for(unsigned int edge=0; edge < edges.size(); edge++){
		int cluster1 = clusterOf[edges[edge][VERTEX1]];
		int cluster2 = clusterOf[edges[edge][VERTEX2]];

		//marcando que existe ao menos uma aresta entre o vértice e o cluster
		if(!flags[edges[edge][VERTEX1]][cluster2]){
			adjClusters[edges[edge][VERTEX1]].push_back(cluster2);
			flags[edges[edge][VERTEX1]][cluster2] = true;
		}
		if(!flags[edges[edge][VERTEX2]][cluster1]){
			adjClusters[edges[edge][VERTEX2]].push_back(cluster1);
			flags[edges[edge][VERTEX2]][cluster1] = true;
		}
	}
}
