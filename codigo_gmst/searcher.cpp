/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 14 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "searcher.h"

/**
* Metodo que troca o vertice de saida do cluster passado
*/
int Searcher::chooseBestVertex(vector<int> &solution, int cost, int cluster){
		
	int vertex = solution[cluster];
	unsigned int bestCost = cost;
	int bestVertex = vertex;
	
	//cout << "cluster: " << cluster << " (" << Graph::graph.clusters[cluster].size() << ")" << endl;
	//Para cada vértice do cluster
	for(unsigned int i=0; i < Graph::graph.clusters[cluster].size(); i++){

		int currVertex = Graph::graph.clusters[cluster][i];
		//Se o vértice corrente nao for o vértice que já estava na solução
		if(currVertex != vertex){

			//Trocando o vertice			
			solution[cluster] = currVertex;
			//recalculando a MST com os vertices passados			
			unsigned int cost = Heuristic5::getMSTCost(solution);			
			//A solução corrente só é considerada se o vertice corrente nao é tabu ou o custo da solucao corrente é menor que o nível de aspiração
			if(cost < bestCost){
				bestCost = cost;
				bestVertex = currVertex;
			}
		}
	}
	solution[cluster] = bestVertex;	
	
	return bestCost;
}//End Method changeVertex(Tree tree, int cluster)

/**
 * Metodo que troca o vertice de saida dos dois clusters passados
 */
int Searcher::chooseBestVertex(vector<int> &solution, int cost, int cluster1, int cluster2){

	int vertex1 = solution[cluster1];
	int vertex2 = solution[cluster2];
	unsigned int bestCost = cost;
	int bestVertex1 = vertex1;
	int bestVertex2 = vertex2;
	
// 	cout << "clusters: " << cluster1 << " (" << Graph::graph.clusters[cluster1].size() << ") e " << cluster2 << " (" << Graph::graph.clusters[cluster2].size() << ")"<< endl;
	//Para cada vértice do cluster
	for(unsigned int i=0; i < Graph::graph.clusters[cluster1].size(); i++){

		int currVertex1 = Graph::graph.clusters[cluster1][i];
		solution[cluster1] = currVertex1;
		
		for(unsigned int j=0; j < Graph::graph.clusters[cluster2].size(); j++){

			int currVertex2 = Graph::graph.clusters[cluster2][j];
			//Se o vértice corrente nao for o vértice que já estava na solução
			if((currVertex1 != vertex1) || (currVertex2 != vertex2)){
	
				//Trocando os vertices					
				solution[cluster2] = currVertex2;
				//recalculando a MST com os vertices passados			
				unsigned int cost = Heuristic5::getMSTCost(solution);
				//A solução corrente só é considerada se o vertice corrente nao é tabu ou o custo da solucao corrente é menor que o nível de aspiração
				if(cost < bestCost){
					bestCost = cost;
					bestVertex1 = currVertex1;
					bestVertex2 = currVertex2;
				}
			}
		}
	}
	solution[cluster1] = bestVertex1;
	solution[cluster2] = bestVertex2;
	
	return bestCost;
}//End Method changeVertex(Tree tree, int cluster)

/**
 * Metodo que troca o vertice de saida dos clusters passado
 */
int Searcher::chooseBestVertex(vector<int> &solution, int cost, vector<int> &clusters){

	if(clusters.size() == 1){
		return chooseBestVertex(solution, cost, clusters[0]);
	} else if(clusters.size() == 2){
		return chooseBestVertex(solution, cost, clusters[0], clusters[1]);
	}

	int cluster1 = clusters[0];
	int cluster2 = clusters[1];
	int cluster3 = clusters[2];
	
	int vertex1 = solution[cluster1];
	int vertex2 = solution[cluster2];
	int vertex3 = solution[cluster3];
	int bestVertex1 = vertex1;
	int bestVertex2 = vertex2;
	int bestVertex3 = vertex3;
	unsigned int bestCost = cost;
	
	//cout << "cluster: " << cluster << " (" << Graph::graph.clusters[cluster].size() << ")" << endl;
	//Para cada vértice do cluster
	for(unsigned int i=0; i < Graph::graph.clusters[cluster1].size(); i++){

		int currVertex1 = Graph::graph.clusters[cluster1][i];
		solution[cluster1] = currVertex1;
		
		for(unsigned int j=0; j < Graph::graph.clusters[cluster2].size(); j++){

			int currVertex2 = Graph::graph.clusters[cluster2][j];
			solution[cluster2] = currVertex2;
			
			for(unsigned int k=0; k < Graph::graph.clusters[cluster3].size(); k++){

				int currVertex3 = Graph::graph.clusters[cluster3][k];
				//Se o vértice corrente nao for o vértice que já estava na solução
				if((currVertex1 != vertex1) || (currVertex2 != vertex2) || (currVertex3 != vertex3)){
		
					//Trocando os vertices											
					solution[cluster3] = currVertex3;
					//recalculando a MST com os vertices passados			
					unsigned int cost = Heuristic5::getMSTCost(solution);
					//A solução corrente só é considerada se o vertice corrente nao é tabu ou o custo da solucao corrente é menor que o nível de aspiração
					if(cost < bestCost){
						bestCost = cost;
						bestVertex1 = currVertex1;
						bestVertex2 = currVertex2;
						bestVertex3 = currVertex3;
					}
				}	
			}
		}
	}
	solution[cluster1] = bestVertex1;
	solution[cluster2] = bestVertex2;
	solution[cluster3] = bestVertex3;
	
	//return bestCost;
	return cost;
}//End Method changeVertex(Tree tree, vector cluster)

vector<int> defineOrder(){

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
* Metodo que implementa a busca local de Golden
*/
int Searcher::localSearch(vector<int> &solution){

	int	cost = Heuristic5::getMSTCost(solution);
	//Definindo a ordem que os clusters serão visitados
	vector<int> order = defineOrder();

	unsigned int iterations = 0;
	int round = 0;

	//Enquanto nao houverem K iteracoes sem melhora
	while(iterations < Graph::graph.numberOfClusters){

		//A cada iteracao um cluster é selecionado seguindo a ordem pré-definida
		int tempCost = chooseBestVertex(solution, cost, order[round]);

		round = (round+1)%Graph::graph.numberOfClusters;
		iterations++;
		//cout << iterations << " " << neighbor.getCost() << endl;

		//Se encontrou uma solucao melhor
		if(tempCost < cost){
			iterations = 0;
			cost = tempCost;
		}
	}
	return cost;
}

/**
 * Método que seleciona 'c' clusters para a busca local
 * A selecao é feita com base na MST gerada a partir da solucao passada
 */
vector<int> Searcher::selectClusters(vector<int> &solution, int c, int cluster){

	vector<int> tree = Heuristic5::calculateMST(solution);
	vector<int> clusters;

	clusters.push_back(cluster);
	int parent = tree[cluster];
	while((clusters.size() < (unsigned) c) && (parent != NONE)){
		clusters.push_back(Graph::graph.clusterOf[parent]);
		parent = tree[parent];
	}
	return clusters;
}

/**
 * Metodo que implementa a busca local de Golden com VNS
 */
int Searcher::localSearchVNS(vector<int> &solution){

	int	cost = Heuristic5::getMSTCost(solution);
	//Definindo a ordem que os clusters serão visitados
	vector<int> order = defineOrder();

	unsigned int c = 1;
	unsigned int iterations = 0;
	int round = 0;

	//Enquanto nao houverem K iteracoes sem melhora
	while((iterations < Graph::graph.numberOfClusters) || (c <= 3)){

		vector<int> clusters = selectClusters(solution, c, order[round]);
		//A cada iteracao um cluster é selecionado seguindo a ordem pré-definida
		int tempCost = chooseBestVertex(solution, cost, clusters);

		round = (round+1)%Graph::graph.numberOfClusters;
		iterations++;		

		//Se encontrou uma solucao melhor
		if(tempCost < cost){
			//cout << iterations << " c: " << c << " custo: " << cost << endl;
			iterations = 0;
			cost = tempCost;
			c = 1;			
		} else if(iterations%Graph::graph.numberOfClusters == 0)
			c++;
	}
	return cost;
}
/**
 * Método que implementa a busca local ils
 */
int Searcher::ils(vector<int> &solution, int cost){

	//Definindo a ordem que os clusters serão visitados
	vector<int> order = defineOrder();

	unsigned int iterations = 0;
	int round = 0;
	
	//Enquanto nao houverem K iteracoes sem melhora ou não atingiu o critério de parada
	while((iterations < Graph::graph.numberOfClusters) && (!((Parameters::getInstance().STOP_CRITERION == FIND_TARGET)&&((unsigned)cost <= Graph::graph.target)))){

		int cluster = order[round];
		//Salvando a solucao corrente
		vector<int> currSolution = solution;
		currSolution[cluster] = Graph::graph.clusters[cluster][random()%(Graph::graph.clusters[cluster].size())];
		for(unsigned int i=0; i < Graph::graph.neighbors[cluster].size(); i++){
			int c = Graph::graph.neighbors[cluster][i];
			currSolution[c] = Graph::graph.clusters[c][random()%(Graph::graph.clusters[c].size())];
		}
		//Construindo uma nova solucao a partir de "auxSolution"
		int currCost = localSearch(currSolution);

		if(Parameters::getInstance().VERBOSE)
			cout << iterations << " Custo: " << cost << ", numClusters: " << Graph::graph.neighbors[cluster].size() << ", NovoCusto: " << currCost << endl;
		
		if(currCost < cost){
			solution = currSolution;
			cost = currCost;
			iterations = 0;
		}
		
		round = (round+1)%Graph::graph.numberOfClusters;
		iterations++;
	}
	return cost;
}
