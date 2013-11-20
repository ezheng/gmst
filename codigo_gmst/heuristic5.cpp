/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 03 de Abril de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heuristic5.h"

/** 
* Esta heuristica constroi uma solucao montando uma arvore que expanda todos os clusters
* do grafo, selecionando cada nó de forma aleatoria
*/
Heuristic5::Heuristic5(){}

Heuristic5::~Heuristic5(){}

vector<int> Heuristic5::calculateTree(){

	//Inicializando as estruturas de dados
	vector<int> solution(Graph::graph.numberOfClusters);	

	//Escolhendo um vertice de saida para cada cluster do grafo
	for(unsigned int cluster=0; cluster < Graph::graph.numberOfClusters; cluster++){
		solution[cluster] = Graph::graph.clusters[cluster][rand()%Graph::graph.clusters[cluster].size()];
	}
	return solution;
}

/**
 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
 * Retorna um vetor contendo o nó pai de cada nó na MST, para a raiz da árvore o valor será -1
 * Utiliza o algoritmo de PRIM
 */
vector<int> Heuristic5::calculateMST(vector<int> &vertices){

	//Armazena as arestas de cada vértice na arvore resultante
	vector<int> tree(Graph::graph.numberOfVertices, NONE);
	//Armazena a distâcia de cada vértice à raiz
	vector<int> distances(Graph::graph.numberOfVertices, INFINITE);
	//Marca os vertices que já estão definitivamente na arvore
	vector<bool> mark(Graph::graph.numberOfVertices, false);

	//Heap utilizado como estrutura auxiliar
	Heap heap;

	// Raiz da arvore
	int node = vertices[0];
	distances[node] = 0;
	heap.insert(node,distances[node]);

	//Inicializando o algoritmo
	for(unsigned int i=1; i < vertices.size(); i++)
		heap.insert(vertices[i],distances[vertices[i]]);

	while(!heap.empty()){

		node = heap.findAndDeleteMin();
		mark[node] = true;

		//Para cada no i adjacente ao no node
		for(unsigned int i=0; i < vertices.size(); i++){
			int adjNode = vertices[i];

			//Se o peso da aresta que une "node" a "adjNode" eh menor a distancia atual do no "adjNode"
			if((!mark[adjNode]) && (Graph::graph.exists[adjNode][node]) && (Graph::graph.adjacency[adjNode][node] < distances[adjNode])){
				distances[adjNode] = Graph::graph.adjacency[adjNode][node];
				heap.adjust(adjNode,distances[adjNode]);
				tree[adjNode] = node;
			}
		}
	}
	return tree;
}

/**
 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
 * Utiliza o algoritmo de PRIM
 * Retorna o custo da árvore gerada
 */
int Heuristic5::getMSTCost(vector<int> &vertices){

	if(Graph::graph.bestMSTAlgorithm == KRUSKAL){
		return getMSTCostByKruskal(vertices);
	}
	
	int	cost = 0;
	//Armazena a distâcia de cada vértice à raiz
	vector<int> distances(Graph::graph.numberOfVertices, INFINITE);
	//Marca os vertices que já está definitivamente na arvore
	vector<bool> mark(Graph::graph.numberOfVertices, false);

	//Heap utilizado como estrutura auxiliar
	Heap heap;

	// Raiz da arvore
	int node = vertices[0];
	distances[node] = 0;
	heap.insert(node,distances[node]);

	//Inicializando o algoritmo
	for(unsigned int i=1; i < vertices.size(); i++){
		heap.insert(vertices[i],distances[vertices[i]]);
	}

	while(!heap.empty()){

		node = heap.findAndDeleteMin();
		mark[node] = true;

		for(list<int> ::iterator it=Graph::graph.adjClusters[node].begin();it != Graph::graph.adjClusters[node].end(); it++){
			int adjNode = vertices[*it];

			//Se o peso da aresta que une "node" a "adjNode" eh menor a distancia atual do no "adjNode"
			if((!mark[adjNode]) && (Graph::graph.exists[node][adjNode]) && (Graph::graph.adjacency[adjNode][node] < distances[adjNode])){

				if(distances[adjNode] != INFINITE)
					cost -= distances[adjNode];
				cost += Graph::graph.adjacency[adjNode][node];
				distances[adjNode] = Graph::graph.adjacency[adjNode][node];
				heap.adjust(adjNode,distances[adjNode]);
			}
		}
	}
	return cost;
}

/**
 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
 * Retorna um vetor contendo o índice de cada aresta que faz parte da MST
 * Utiliza o algoritmo de KRUSKAL
 */
vector<int> Heuristic5::calculateMSTByKruskal(vector<int> &vertices){

	//Inicializando as estruturas de dados
	vector<int> tree;
	int numberOfComponents = vertices.size();
	SetUnion set(numberOfComponents);

	//Como as arestas do grafo ja estao ordenadas pelos seus pesos
	//considera-se cada aresta na ordem em que estao armazenadas
	//Enquanto ainda existem componentes desconexas no grafo
	unsigned int edge=0;
	while((edge < Graph::graph.edges.size())&&(numberOfComponents > 1)){

		//cout << "Verificando aresta " << edge << " (Peso " << Graph::graph.edges[edge][COST] << ")" << endl;
		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		unsigned int cluster1 = Graph::graph.clusterOf[vertex1];

		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		unsigned int cluster2 = Graph::graph.clusterOf[vertex2];

		//Se os vertices da aresta ja estao na solucao e os clusters estao em componentes diferentes, faz a uniao dos componentes
		if((vertices[cluster1] == vertex1)&&(vertices[cluster2] == vertex2) && (set.join(cluster1, cluster2))){
			tree.push_back(edge);
			numberOfComponents--;
		}
		edge++;
	}
	return tree;
}

/**
 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
 * Retorna um vetor contendo o índice de cada aresta que faz parte da MST
 * Utiliza o algoritmo de KRUSKAL
 */
int Heuristic5::getMSTCostByKruskal(vector<int> &vertices){
	
	int numberOfComponents = vertices.size();
	SetUnion set(numberOfComponents);
	int cost = 0;

	//Como as arestas do grafo ja estao ordenadas pelos seus pesos
	//considera-se cada aresta na ordem em que estao armazenadas
	//Enquanto ainda existem componentes desconexas no grafo
	unsigned int edge=0;
	while((edge < Graph::graph.edges.size())&&(numberOfComponents > 1)){

		int vertex1 = Graph::graph.edges[edge][VERTEX1];
		unsigned int cluster1 = Graph::graph.clusterOf[vertex1];

		int vertex2 = Graph::graph.edges[edge][VERTEX2];
		unsigned int cluster2 = Graph::graph.clusterOf[vertex2];

		//Se os nós da aresta são os que ja estao na solucao e estao em componentes diferentes, faz a uniao dos componentes
		if((vertices[cluster1] == vertex1)&&(vertices[cluster2] == vertex2) && (set.join(cluster1, cluster2))){
			//cout << "Inserindo aresta [" << vertex1 << "][" << vertex2 << "] -> custo " << Graph::graph.edges[edge][COST] << endl;
			cost += Graph::graph.edges[edge][COST];
			numberOfComponents--;
		}
		edge++;
	}

// 	vector<int> tree = calculateMST(vertices);
// 	if(!checkMST(tree, vertices)){
// 		exit(1);
// 	}
	return cost;
}

/**
* Metodo que verifica se o array passado é uma arvore
*/
bool Heuristic5::checkMST(vector<int> &tree, vector<int> &solution){

	bool ok = true;	
	vector<int> components(Graph::graph.numberOfVertices, -1);
	int numberOfComponents = Graph::graph.numberOfClusters;
		
	for(unsigned int i=0; i < solution.size(); i++){			
		components[solution[i]] = i;
	}
	
	unsigned int vertex1 = 0;
	while((ok) && (vertex1 < tree.size())){

		if(tree[vertex1] != NONE){
			int vertex2 = tree[vertex1];
			//cout << "Aresta " << tree[vertex] << " possui os vertices " << vertex << " e " << vertex2 << endl;
			if(components[vertex1] != components[vertex2]){
				numberOfComponents--;
				for(unsigned int i=0; i < Graph::graph.numberOfVertices; i++){
					int component = components[vertex1];
					if(components[i] == component){
						components[i] = components[vertex2];
					}
				}
			} else {
				ok = false;
				cout << "Solucao com ciclos!" << endl;
			}
		}
		vertex1++;
	}
	if((ok) && (numberOfComponents > 1)){
		ok = false;
		cout << "Solucao desconexa!" << endl;
	}else if(ok) {
		for(unsigned int i=0; i < solution.size(); i++){
			if((unsigned)Graph::graph.clusterOf[solution[i]] != i){
				ok = false;
				cout << "Cluster do vertice " << solution[i] << " nao e " << i << " e sim " << Graph::graph.clusterOf[solution[i]] << endl;
			}
		}
	}
	return ok;
}

/**
 * Checa se os vertices passados realmente formam uma MST com o custo passado
 */
void Heuristic5::checkMST(vector<int> &solution, int cost){
	if(getMSTCost(solution) != cost){
		cout << "VERTICES NAO CORRESPONDEM A UMA SOLUCAO DE CUSTO " << cost << endl;
	}
}

/**
 * Imprime os arcos da MST gerada com os vertices presentes em 'solucao' a partir do cluster 'rootCluster' como raiz no formato de uma solucao do problema de steiner
 * Retorna um stream para as arestas que foram impressas
 */
string Heuristic5::printSteinerMST(vector<int> solution, int rootCluster){

	ostringstream oss;
	//Imprimindo a aresta do terminal raiz até o não-terminal raiz
	//Os vertices estão numerados no intervalo [1:n]
	oss << "A " << Graph::graph.numberOfVertices+1 << " " << solution[rootCluster]+1 << endl;
			
	//Armazena as arestas de cada vértice na arvore resultante
	vector<int> tree(Graph::graph.numberOfVertices, NONE);
	//Armazena a distâcia de cada vértice à raiz
	vector<int> distances(Graph::graph.numberOfVertices, INFINITE);
	//Marca os vertices que já estão definitivamente na arvore
	vector<bool> mark(Graph::graph.numberOfVertices, false);

	//Heap utilizado como estrutura auxiliar
	Heap heap;

	// Raiz da arvore
	int node = solution[rootCluster];
	distances[node] = 0;
	heap.insert(node,distances[node]);

	//Inicializando o algoritmo
	for(unsigned int i=1; i < solution.size(); i++)
		heap.insert(solution[i],distances[solution[i]]);

	while(!heap.empty()){

		node = heap.findAndDeleteMin();
		mark[node] = true;
		if(node != solution[rootCluster]){
			//Imprimindo aresta do pai do nó até ele
			oss << "A " << tree[node]+1 << " " << node+1 << endl;
			int terminal = (Graph::graph.clusterOf[node] > rootCluster)? Graph::graph.numberOfVertices + Graph::graph.clusterOf[node]: Graph::graph.numberOfVertices + Graph::graph.clusterOf[node]+1;
			//Imprimindo aresta do nó até o terminal correspondente
			oss << "A " << node+1 << " " << terminal+1 << endl;
		}

		//Para cada no i adjacente ao no node
		for(unsigned int i=0; i < solution.size(); i++){
			int adjNode = solution[i];

			//Se o peso da aresta que une "node" a "adjNode" eh menor a distancia atual do no "adjNode"
			if((!mark[adjNode]) && (Graph::graph.exists[adjNode][node]) && (Graph::graph.adjacency[adjNode][node] < distances[adjNode])){
				distances[adjNode] = Graph::graph.adjacency[adjNode][node];
				heap.adjust(adjNode,distances[adjNode]);
				tree[adjNode] = node;
			}
		}
	}
	return oss.str();
}
