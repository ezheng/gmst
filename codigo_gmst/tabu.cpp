/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 03 de Junho de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "tabu.h"

/**
 * Construtor
 * seta os parametros do algoritmo
 * 
 */
Tabu::Tabu(){	
	
	//Número máximo de iterações da busca	
	maxIterations = min(75*(int)Graph::graph.numberOfVertices, 4000);
	
	//Número máximo de iterações do sorteio dos clusters de forma aleatoria
	maxIterationsForRandomSelection = 50;	

	//Fator que ajusta a intensidade da diversificação
	factor = 0.05;
	
	//Armazena quantas iterações sem melhora são permitidas para cada estrutura de vizinhança
  	noImprovementIterations.push_back(250);
  	noImprovementIterations.push_back(175);
  	noImprovementIterations.push_back(100);

	Graph::graph.sortEdges();
}

Tabu::~Tabu(){}

/**
* Metodo que sorteia um valor para o tabu tenure
*/ 
int Tabu::sortTabuTenure(){

	//Intervalo em que será sorteado valor do tabu tenure: [lowerbound, upperbound]	
	int value = (int) ceil(4*log10((double)Graph::graph.numberOfVertices));
	int lowerbound = value -2;
	int upperbound = value +2;

	//(Intervalo fechado em upperbound)
	upperbound ++;		
	int tabuTenure = rand()%(upperbound - lowerbound) + lowerbound;	

	return tabuTenure;
}

/**
 * Metodo que executa a busca tabu
 */
unsigned int Tabu::runTabuSearch(){
	
	//Inicializando as estruturas que serao utilizadas	
	initialize();	
		
	//Construindo uma solução inicial	
	solution = Heuristic5::calculateTree();
	unsigned int cost = Heuristic5::getMSTCost(solution);
	
	updateParameters(cost, solution);	
	
	for(unsigned int i=0; i < solution.size(); i++){
		//Atualizando o número de vezes em que o vértice corrente entrou na solução
		verticesFrequencies[solution[i]]++;
	}
	
	//Por um número 'maxIterations' de iteracoes...
	while(iteration <= maxIterations){

		if(Parameters::getInstance().VERBOSE)	
			cout << iteration << " (" << bestCost << ") " << cost << " - " << iterInNeighborhood << " its na vizinhanca " << neighborhood << endl;
		
		//Aplicando a busca local sobre a solução corrente		
		vector<int> oldSolution = solution;
 		cost = applySearch(cost);

		//Atualizando os parâmetros		
		updateParameters(cost, oldSolution);
		iteration++;
	}
	return bestCost;
}

/**
 * Metodo que atualiza os parametros do algoritmo
 * 
 * @param cost = custo da solução atual
 * @param oldSolution = solução anterior na busca local
 */
void Tabu::updateParameters(unsigned int cost, vector<int> &oldSolution){
	
	//Para cada cluster...
	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){		
		//Armazenando o vértice do cluster i na solução corrente e na anterior
		int oldVertex = oldSolution[i];
		int vertex = solution[i];		
		//Se o vertice do cluster i é diferente nas duas solucoes
		if(oldVertex != vertex){
						
			//Atualizando o status tabu do vértice da solução antiga			
			tabuTillIteration[oldVertex] = iteration + sortTabuTenure();

			//Atualizando o número de vezes em que o vértice corrente entrou na solução
			verticesFrequencies[vertex]++;			
		}
		//Atualizando o 'aspiration level' de cada vértice presente na solução corrente
		aspirationLevel[vertex] = min(cost, aspirationLevel[vertex]);
	}
	
	//Se encontrou uma solução melhor que a melhor solução encontrada até o momento...
	if(cost < bestCost){
		bestSolution = solution;
		bestCost = cost;
		bestSolutionTime = cpuTime() - startTime;
		bestIteration = iteration;
 		if(iterationsForRandomSelection != -1)
 			iterationsForRandomSelection = 0;
		iterInNeighborhood = 0;
	}

	//Alternando a estrutura de vizinhança, se for o caso
	if(iterInNeighborhood == noImprovementIterations[neighborhood-1]){
		neighborhood = neighborhood%3 + 1;
		iterInNeighborhood = 0;
	}
	iterInNeighborhood++;
}

/**
* Metodo que executa a inicialização das estruturas para a busca tabu
*/
void Tabu::initialize(){

	startTime = cpuTime();	
	
	iteration = 1;	
	iterInNeighborhood = 0;
	iterationsForRandomSelection = -1;	
	mostFrequentCluster = 0;
	neighborhood = 1;
	bestCost = INFINITE;

	verticesFrequencies.clear();
	verticesFrequencies.assign(Graph::graph.numberOfVertices,0);

	tabuTillIteration.clear();
	tabuTillIteration.assign(Graph::graph.numberOfVertices,0);

	aspirationLevel.clear();
	aspirationLevel.assign(Graph::graph.numberOfVertices,INFINITE);
	
	clustersFrequencies.clear();
	clustersFrequencies.assign(Graph::graph.numberOfClusters,1);
}

/**
 * Método que executa a busca local, considerando a estrutura de vizinhança c=1,2,3 conforme o caso
 * 
 * @param currCost custo da solução corrente
 * 
 * @return custo da solução retornada pela busca local
 */
int Tabu::applySearch(unsigned int currCost){

	//Armazena os clusters selecionados para serem visitados a cada movimento da busca	
	vector<int> clusters = selectClusters();
	
	switch(neighborhood){
		case 1: {
			return applySearch1(clusters, currCost);
		}
		case 2:{
			return applySearch2(clusters, currCost);			
		}
		case 3: {
			return applySearch3(clusters, currCost);			
		}
		default:{
			cout << "ERRO! VIZINHANCA DESCONHECIDA: " << neighborhood << endl;
 			return INFINITE;
		}
	}
}

/**
 * Método que implementa a busca local com vizinhança c = 1
 * 
 * @param clusters clusters selecionados para estabelecer a vizinhanca da solucao corrente
 * @param currCost custo da solucao corrente
 * @return custo da solucao vizinha, agora armazenada em 'solution'
 */
int Tabu::applySearch1(vector<int> &clusters, unsigned int currCost){
	
	int vertex = solution[clusters[0]];

	double bestCombinedCost = INFINITE;
	unsigned int bestCost = currCost;
	int bestVertex = vertex;
	
	//cout << "cluster: " << clusters[0] << " (" << Graph::graph.clusters[clusters[0]].size() << ")" << endl;
	//Para cada vértice do cluster
	for(unsigned int i=0; i < Graph::graph.clusters[clusters[0]].size(); i++){

		int currVertex = Graph::graph.clusters[clusters[0]][i];
		//Se o vértice corrente nao for o vértice que já estava na solução
  		if(currVertex != vertex){

			//Trocando o vertice
			solution[clusters[0]] = currVertex;						
			//recalculando a MST com os vertices passados
			unsigned int cost = Heuristic5::getMSTCost(solution);
			double combinedCost = computeCost(cost, currCost, clusters);
			
// 			cout << "		vertex: " << currVertex << endl;
// 			cout << "		cost: " << cost << endl;
// 			cout << "		combinedCost: " << combinedCost << endl;
// 			cout << "		tabuTillIteration: " << tabuTillIteration[currVertex] << endl;
// 			cout << "		aspirationLevel: " << aspirationLevel[currVertex] << endl;
// 			cout << "-------------------------------------------" << endl;

			//A solução corrente só é considerada se o vertice corrente nao é tabu ou o custo da solucao corrente é menor que o nível de aspiração							
			if(((tabuTillIteration[currVertex] < iteration)||(cost < aspirationLevel[currVertex])) && (combinedCost < bestCombinedCost)){
				bestCost = cost;
				bestCombinedCost = combinedCost;
				bestVertex = currVertex;
			}
 		}
	}	
	solution[clusters[0]] = bestVertex;	
	return bestCost;
}

/**
 * Método que implementa a busca local com vizinhança c = 2
 * 
 * @param clusters os dois clusters selecionados para estabelecer a vizinhanca da solucao corrente
 * @param currCost custo da solucao corrente
 * @return custo da solucao vizinha, agora armazenada em 'solution'
 */
int Tabu::applySearch2(vector<int> &clusters, unsigned int currCost){

	int vertex1 = solution[clusters[0]];
	int vertex2 = solution[clusters[1]];

	double bestCombinedCost = INFINITE;
	unsigned int bestCost = currCost;	
	int bestVertex1 = vertex1;
	int bestVertex2 = vertex2;

	//cout << "clusters: " << clusters[0] << " (" << Graph::graph.clusters[clusters[0]].size() << "), " << clusters[1] << " (" << Graph::graph.clusters[clusters[1]].size() << ")" << endl;
	//For each vertex in cluster1
	for(unsigned int i=0; i < Graph::graph.clusters[clusters[0]].size(); i++){
		
		int currVertex1 = Graph::graph.clusters[clusters[0]][i];
		//If current vertex is not the one already in solution
 		if(currVertex1 != vertex1){

			//For each vertex in cluster2
			for(unsigned int j=0; j < Graph::graph.clusters[clusters[1]].size(); j++){

				int currVertex2 = Graph::graph.clusters[clusters[1]][j];
				//If current vertex is not the one already in solution
				if(currVertex2 != vertex2){

					//Trocando os vertices					
					solution[clusters[0]] = currVertex1;
					solution[clusters[1]] = currVertex2;

					//recomputing MST with given vertices
					unsigned int cost = Heuristic5::getMSTCost(solution);
					double combinedCost = computeCost(cost, currCost, clusters);
					
// 					cout << "		vertices: " << currVertex1 << ", " << currVertex2 << endl;
// 					cout << "		cost: " << cost << endl;
// 					cout << "		combinedCost: " << combinedCost << endl;
// 					cout << "		tabuTillIteration: " << tabuTillIteration[currVertex1] << ", " << tabuTillIteration[currVertex2] << endl;
// 					cout << "		aspirationLevel: " << aspirationLevel[currVertex1] << ", " << aspirationLevel[currVertex2] << endl;
// 					cout << "-------------------------------------------" << endl;

					//A solução corrente só é considerada se algum dos dois vértices não é tabu ou se o custo da solucao corrente é menor que o nível de aspiração de algum dos vértices
					if(((tabuTillIteration[currVertex1] < iteration)||(cost < aspirationLevel[currVertex1]) || (tabuTillIteration[currVertex2] < iteration)||(cost < aspirationLevel[currVertex2])) && (combinedCost < bestCombinedCost)){
						bestCost = cost;
						bestCombinedCost = combinedCost;				
						bestVertex1 = currVertex1;
						bestVertex2 = currVertex2;					
					}
 				}
			}
 		}
	}
	solution[clusters[0]] = bestVertex1;
	solution[clusters[1]] = bestVertex2;
		
	return bestCost;
}

/**
 * Método que implementa a busca local com vizinhança c =3  
 * 
 * @param clusters clusters selecionados para estabelecer a vizinhanca da solucao corrente
 * @param currCost custo da solucao corrente
 * @return custo da solucao vizinha, agora armazenada em 'solution'
 */
int Tabu::applySearch3(vector<int> &clusters, unsigned int currCost){

	int vertex1 = solution[clusters[0]];
	int vertex2 = solution[clusters[1]];
	int vertex3 = solution[clusters[2]];

	double bestCombinedCost = INFINITE;
	unsigned int bestCost = currCost;
	int bestVertex1 = vertex1;
	int bestVertex2 = vertex2;
	int bestVertex3 = vertex3;
	
	//cout << "clusters: " << clusters[0] << " (" << Graph::graph.clusters[clusters[0]].size() << "), " << clusters[1] << " (" << Graph::graph.clusters[clusters[1]].size() << "), " << clusters[2] << " (" << Graph::graph.clusters[clusters[2]].size() << ")" << endl;
	
	//For each vertex in cluster1
	for(unsigned int i=0; i < Graph::graph.clusters[clusters[0]].size(); i++){
		
		int currVertex1 = Graph::graph.clusters[clusters[0]][i];
		//If current vertex is not the one already in solution
 		if(currVertex1 != vertex1){

			//For each vertex in cluster2
			for(unsigned int j=0; j < Graph::graph.clusters[clusters[1]].size(); j++){

				int currVertex2 = Graph::graph.clusters[clusters[1]][j];
				//If current vertex is not the one already in solution
  				if(currVertex2 != vertex2){

					//For each vertex in cluster3
					for(unsigned int k=0; k < Graph::graph.clusters[clusters[2]].size(); k++){
						
						int currVertex3 = Graph::graph.clusters[clusters[2]][k];
						//If current vertex is not the one already in solution
						if(currVertex3 != vertex3){

							//Changing vertices
							solution[clusters[0]] = currVertex1;
							solution[clusters[1]] = currVertex2;
							solution[clusters[2]] = currVertex3;
									
							//recomputing MST with given vertices
							unsigned int cost = Heuristic5::getMSTCost(solution);
							double combinedCost = computeCost(cost, currCost, clusters);
							
// 							cout << "		vertices: " << currVertex1 << ", " << currVertex2 << ", " << currVertex3 << endl;
// 							cout << "		cost: " << cost << endl;
// 							cout << "		combinedCost: " << combinedCost << endl;
// 							cout << "		tabuTillIteration: " << tabuTillIteration[currVertex1] << ", " << tabuTillIteration[currVertex2] << ", " << tabuTillIteration[currVertex3] << endl;
// 							cout << "		aspirationLevel: " << aspirationLevel[currVertex1] << ", " << aspirationLevel[currVertex2] << ", " << aspirationLevel[currVertex3] << endl;
// 							cout << "-------------------------------------------" << endl;
							
							//A solução corrente só é considerada se algum dos vértices não é tabu ou se o custo da solucao corrente é menor que o nível de aspiração de algum vértices
  							if(((tabuTillIteration[currVertex1] < iteration)||(cost < aspirationLevel[currVertex1]) || (tabuTillIteration[currVertex2] < iteration) ||(cost < aspirationLevel[currVertex2]) || (tabuTillIteration[currVertex3] < iteration) ||(cost < aspirationLevel[currVertex3])) && (combinedCost < bestCombinedCost)){
										
								bestCost = cost;
								bestCombinedCost = combinedCost;
								bestVertex1 = currVertex1;
								bestVertex2 = currVertex2;
								bestVertex3 = currVertex3;								
							}
  						}
					}
  				}
			}
  		}
	}
	solution[clusters[0]] = bestVertex1;
	solution[clusters[1]] = bestVertex2;
	solution[clusters[2]] = bestVertex3;	
	
	return bestCost;
}

/**
 * Metodo que seleciona os clusters que serão utilizados em um movimento da busca local
 * A seleção pode ser feita de duas formas: proporcional às frequencias com que os clusters já foram selecionados
 * anteriormente, ou completamente aleatória. 
 * 
 * @return clusters que foram selecionados
 */
vector<int> Tabu::selectClusters(){

	bool equalFrequences = false;
	vector<bool> selectedClusters(Graph::graph.numberOfClusters, false);

	//Definindo de que forma será feita a seleção dos clusters
	if(iterationsForRandomSelection == maxIterationsForRandomSelection)
		iterationsForRandomSelection = -1;
	else if(iterationsForRandomSelection != -1)
		iterationsForRandomSelection++;	

	vector<int> clusters;

	//Selecionando cada um dos clusters	
	for(unsigned int i=0; i < neighborhood; i++){
		
		int cluster;
		//Se a selecao deve ser proporcional às frequencias...
		if(iterationsForRandomSelection == -1){
			
			//Computando a probabilidade de cada cluster ser escolhido...
			vector<float> clustersRoulette;
			float probability = (float)clustersFrequencies[mostFrequentCluster]/(float)clustersFrequencies[0];
			clustersRoulette.push_back(probability);
			//flag é usado para saber se os clusters estao com frequencias iguais
			bool flag = true;
			for(unsigned int j=1; j < Graph::graph.numberOfClusters; j++){
				probability = (float)clustersFrequencies[mostFrequentCluster]/(float)clustersFrequencies[j];	
				//cout << j << " " << clustersFrequencies[j] << " " << probability << endl;
				clustersRoulette.push_back(probability + clustersRoulette[j-1]);
				if(clustersFrequencies[j] != clustersFrequencies[0])
					flag = false;				
			}						
			//Valor máximo até que a roleta irá girar...
			int maxValue = (int)(round(clustersRoulette.back()*10));

			//Seleciona um dos clusters enquanto o escolhido já tiver sido escolhido anteriormente
			do{
				float value = rand()%maxValue/10.0;
				
				cluster = 0;				
				while(clustersRoulette[cluster] < value)
					cluster++;								
			}while(selectedClusters[cluster]);

			if(flag)
				equalFrequences = true;
		//Se a selecao deve ser aleatória
		} else{
				
			do {
				cluster = rand()%Graph::graph.numberOfClusters;				 
			} while(selectedClusters[cluster]);
		}
		//Inserindo o cluster selecionado no vetor, e atualizando seus dados
		clusters.push_back(cluster);
		selectedClusters[cluster] = true;
		clustersFrequencies[cluster]++;
		if(clustersFrequencies[cluster] > clustersFrequencies[mostFrequentCluster]){
			mostFrequentCluster = cluster;
		}
	}
	
	if(equalFrequences)
		iterationsForRandomSelection = 0;

	return clusters;
}

/**
 * Metodo que computa o custo da solucao adicionando a penalizacao pela frequencia dos clusters
 * 
 * @param currCost custo da solucao
 * @param oldCost custo da solucao anterior da busca local
 * @param clusters array com os clusters que estão sendo visitados na iteração corrente da busca local
 * 
 * @return custo da solução com a penalização
 */
double Tabu::computeCost(unsigned int currCost, unsigned int oldCost, vector<int> clusters){

	//Se a solução corrente não for melhor que a solução antiga
	if(currCost >= oldCost){

		double root = sqrt((double)(Graph::graph.numberOfClusters*Graph::graph.numberOfVertices));
		double sum = 0;
		
		//Somando a frequencia de cada vertice da solucao corrente que nao aparece na solucao anterior
		for(unsigned int i=0; i < clusters.size(); i++){
			sum += verticesFrequencies[solution[clusters[i]]];
		}
		//retornando custo de 'solution' + penalização 
		return currCost + factor*(double)oldCost*root*sum/(double)iteration;
	} else
		return currCost;
}

/**
 * Metodo que imprime a solucao
 */
void Tabu::printSolution(){
	
	cout << "Solution: ";
	for(unsigned int i=0; i < solution.size(); i++){
		cout << " " << solution[i];
	}
	cout << endl;	
}
/**
* Metodo chamado para executar o algoritmo
* Executa o algoritmo de busca tabu por TIMES vezes, sendo TIMES um parametro do programa
*
* Retorna: uma string contendo o custo médio das soluções encontradas, o melhor custo, o tempo médio de execução e tempo
* médio em que o algoritmo encontrou a melhor solução. Todos os valores separados por espaço.
*/
string Tabu::execute(){

	//Numero de vezes que o algoritmo vai executar
	unsigned int times = Parameters::getInstance().TIMES;

	//Custo médio das solucoes encontradas
	double avgCost = 0;
	//Melhor custo dentre as solucoes encontradas
	unsigned int bestSolutionCost = INFINITE;	
	//Tempo médio de execucao dos algoritmos
	double avgTime = 0;	
	//Tempo médio em que o algoritmo encontra a melhor solução
	double avgBestSolutionTime = 0;

	//Para cada execução...
	for(unsigned int i=0; i < times; i++){

		double initialTime = cpuTime();
		unsigned int cost = runTabuSearch();

		if(cost < bestSolutionCost)
			bestSolutionCost = (cost < bestSolutionCost)? cost: bestSolutionCost;

		avgCost += cost;		
		avgTime += cpuTime() - initialTime;		
		avgBestSolutionTime += bestSolutionTime;
	}
	//Custo médio das soluções
	avgCost /= (double)times;
	//Tempo médio de execução
	avgTime /= (double)times;
	//Tempo médio em que o algoritmo encontrou a melhor solução		
	avgBestSolutionTime/= (double)times;
	
	ostringstream oss;
	oss << avgCost << " " << bestSolutionCost << " " << avgTime << " " << avgBestSolutionTime << " ";

	return oss.str();
}

	//Armazenando quais sao os vertices de cada cluster que será visitado
// 	vector< vector<int> > vertices;
// 	for(unsigned int i=0; i < clusters.size(); i++){
// 		vertices.push_back(Graph::graph.getVerticesFromCluster(clusters[i]+1));
// 	}
// 
// 	//Salvando os vértices da solução corrente para cada cluster visitado
// 	vector<int> oldVertices;
// 	for(unsigned int i=0; i < vertices.size(); i++){
// 		oldVertices.push_back(solution[clusters[i]]);
// 	}
// 
// 	//Computando o número de iterações da busca
// 	unsigned int iterations = 1;
// 	for(unsigned int i=0; i < clusters.size(); i++){
// 		iterations *= vertices[clusters[i]].size() - 1;
// 	}
// 
// 	for(unsigned int i=0; i < iterations; i++){
// 
// 		for(unsigned int j=0; j < clusters.size(); j++){
// 			
// 		}	
// 	}
// 	
// 	return bestCost;
