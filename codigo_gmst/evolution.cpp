/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "evolution.h"

Evolution::Evolution(){

	//Setando os parametros do algoritmo
//	Parameters::setMaxIterationsGRASP(10);
	Parameters::setEliteSetSize(30);
//	Parameters::setStopCriterion(ITERATIONS);
	Parameters::setMinEliteDistance(Graph::graph.numberOfClusters/10);
	
	maxGenerations = Parameters::getInstance().MAX_ITERATIONS_GRASP;

	heuristics.push_back(RANDOMIZED_HEURISTIC1);
	heuristics.push_back(HEURISTIC5);
	heuristics.push_back(RANDOMIZED_HEURISTIC6);
	heuristics.push_back(RANDOMIZED_HEURISTIC7);
	heuristics.push_back(RANDOMIZED_HEURISTIC9);

}

Evolution::~Evolution(){}

/**
 * Verifica se ja foi atingido o criterio de parada
 */
bool Evolution::stopCriterionReached(){

	if((Parameters::getInstance().EASY_TARGET != NONE) && (easyTargetTime == 0) && (population.bestCost() > 0) && (population.bestCost() <= Parameters::getInstance().EASY_TARGET)){
		easyTargetTime = cpuTime() - startTime;
	}
	
	bool criterion1 = ((Parameters::getInstance().STOP_CRITERION == FIND_TARGET) && (((unsigned)population.bestCost() <= Graph::graph.target) || ((cpuTime() - startTime) > Graph::graph.timeLimit)));
	bool criterion2 = ((Parameters::getInstance().STOP_CRITERION == ITERATIONS) && (generation > maxGenerations));

	//O algoritmo deve parar se:
	//O criterio de parada é um alvo e esse ja foi encontrado ou o tempo limite jah foi atingido
	//Ou
	//O criterio de parada é o numero de iteracoes, e esse jah foi atingido

	return ((criterion1) || (criterion2));
}

/**
 * Faz o pre-processamento necessario para a execucao de um teste Evolution
 * (Inicializacao de variaveis, definicao de parametros, etc.)
 */
void Evolution::initialize(){

	startTime = cpuTime();
	easyTargetTime = 0;
	generation = 1;
	constructiveHeuristic = Parameters::getInstance().HEURISTIC;

	//Ordenando arestas
	Graph::graph.sortEdges();

	vector<int> vertices(Graph::graph.numberOfClusters);
	for(unsigned int cluster = 0; cluster < Graph::graph.numberOfClusters; cluster++){
		int numVertices = Graph::graph.clusters[cluster].size();
		vertices[cluster] = Graph::graph.clusters[cluster][random()%numVertices];
	}

	double startTime1 = cpuTime();
	//Calculando 10 MSTs com Prim
	for(int i=0; i < 100; i++){
		Heuristic5::getMSTCost(vertices);
	}
	double finalTime1 = cpuTime() - startTime1;

	double startTime2 = cpuTime();
	//Calculando 10 MSTs com Kruskal
	for(int i=0; i < 100; i++){
		Heuristic5::getMSTCostByKruskal(vertices);
	}
	double finalTime2 = cpuTime() - startTime2;
	if(finalTime2 < finalTime1){
		Graph::graph.bestMSTAlgorithm = KRUSKAL;
	}	
}

/**
 * Constroi a populacao
 */
void Evolution::constructPopulation(){

	population.clear();

	//Enquanto a população estiver menor que o tamanho definido
	while(population.size() < Parameters::getInstance().ELITE_SET_SIZE){

		//Escolhe uma heuristica
		int heuristic = heuristics[rand()%heuristics.size()];
		//Cria a solução
		vector<int> solution = Constructor::constructSolution(heuristic);
		//Calcula o custo da solução
		//int cost = Searcher::localSearch(solution);
		int cost = Heuristic5::getMSTCost(solution);
		//Atualiza a população
		population.update(solution, cost, generation);
	}
}

/**
 * Método que realiza o cruzamento para a próxima geração
 */
void Evolution::crossover(){

	vector< vector<int> > offsprings;
	
	list< vector<int> >::iterator it = population.solutionSet.end();
	int numParents = 0;
	while((it!=population.solutionSet.begin()) && (numParents < population.size()/3)){

		it--;
		vector<int> solution1 = *it;
		vector<int> solution2;
		//Seleciona a solução mais distante de 'solution1'
		population.chooseASolution(solution1, solution2);

		//Enquanto as duas soluções são diferentes
			while(Population::computeDistance(solution1, solution2) != 0){

			int bestCost = INFINITE;
			int bestCluster = NONE;

			//Consulta cada cluster das duas solucoes
			for(unsigned int cluster=0; cluster < Graph::graph.numberOfClusters; cluster++){
				
 				//Se os vertices sao diferentes nas duas solucoes, gera uma nova solucao
				if(solution1[cluster] != solution2[cluster]){
					int vertex = solution1[cluster];
					//Troca o vertice de saida do cluster e refaz a MST				
					solution1[cluster] = solution2[cluster];
					int cost = Heuristic5::getMSTCost(solution1);

					//Se encontrou um solucao melhor...
					if(cost < bestCost){
						bestCost = cost;
						bestCluster = cluster;
					}
					solution1[cluster] = vertex;
				}
			}
			//Copiando para 'solution1' um vértice de 'solution2'
			solution1[bestCluster] = solution2[bestCluster];

			//Salvando a solução intermediária como filho gerado
			vector<int> offspring = solution1;
			offsprings.push_back(offspring);
		}
		numParents++;
	}
	
	for(unsigned int i=0; i < offsprings.size(); i++){
		//int cost = Searcher::localSearch(offsprings[i]);
		int cost = Heuristic5::getMSTCost(offsprings[i]);
		population.update(offsprings[i], cost, generation);
	}
}

/**
 * Método que implementa o operador de mutação
 */
void Evolution::mutation(){

}

/**
 * Método que aplica a busca local sobre toda a população
 */
void Evolution::localSearch(){

	list< vector<int> >::iterator it = population.solutionSet.begin();

	for(;it != population.solutionSet.end(); it++){
		vector<int> solution = *it;
		int cost = Searcher::localSearch(solution);
		population.update(solution, cost, generation);
	}
}

/**
 * Metodo que executa o algoritmo evolutivo
 */
int Evolution::calculateTree(){
	
	initialize();

	//Construindo a população inicial
	constructPopulation();

	//A cada geração
	while(!stopCriterionReached()){

		//Aplicando operador de cross-over sobre a população
		crossover();
		//Aplicando mutação
		//mutation();
		//Aplica a busca local sobre toda a população
		localSearch();
		//Refaz a população
		population.rebuild();
		if(Parameters::getInstance().VERBOSE){
			cout << "******** Generation " << generation << ", bestCost: " << population.bestCost() << endl;
		}

		generation++;
	}

	//Retorna o custo da melhor solucao do conjunto elite
	return population.bestCost();
}


/**
 * Metodo usado para testar o algoritmo
 */
string Evolution::execute(){

	double totalTime = 0;
	double bestTime = INFINITE;
	unsigned int times = Parameters::getInstance().TIMES;

	ostringstream oss;
	ostringstream ossFileName;
	ossFileName << "execucao_" <<  Parameters::getInstance().SEED << ".txt";
	ofstream fout(ossFileName.str().c_str(), ios::app);
	fout << Graph::graph.instanceName << endl;
	
	unsigned int totalCost = 0;
	int bestCost = INFINITE;
	int avgBestGeneration = 0;

	//Para cada execução do genético
	for(unsigned int i=0; i < times; i++){

		int cost = calculateTree();
		
		totalCost += cost;
		avgBestGeneration += population.getBestGeneration();
		bestCost = min(cost, bestCost);

		//Extraindo tempo de execução
		double finalTime = cpuTime() - startTime;
		totalTime += finalTime;
		if(finalTime < bestTime)
			bestTime = finalTime;

		//Plotando em um arquivo auxiliar as informacoes sobre a iteracao corrente
		fout << i << " " << cost << " " << easyTargetTime << " " << finalTime << " " << population.getBestGeneration() << " " << generation << endl;
	}
	
	oss << (float)totalCost/(float)times << " " << bestCost << " " << (totalTime)/times << " " << bestTime << " " << (float)avgBestGeneration/(float)times << " ";
	fout << oss.str() << endl;
	fout << "-----------------------------------------------------------------" << endl;
	fout.close();
	
	return oss.str();
}

