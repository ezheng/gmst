/*
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 14 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "grasp.h"
extern std::string INSTANCES_DIR;

GRASP::GRASP(){
	
	maxIter = Parameters::getInstance().MAX_ITERATIONS_GRASP;
	constructiveHeuristic = Parameters::getInstance().HEURISTIC;

	heuristics.push_back(RANDOMIZED_HEURISTIC1);
	heuristics.push_back(HEURISTIC5);
	heuristics.push_back(RANDOMIZED_HEURISTIC6);
	heuristics.push_back(RANDOMIZED_HEURISTIC7);
	heuristics.push_back(RANDOMIZED_HEURISTIC9);

	iterations.assign(heuristics.size(),1);
	
	greedHeuristics.push_back(HEURISTIC1);
	greedHeuristics.push_back(HEURISTIC6);
	greedHeuristics.push_back(HEURISTIC7);
	greedHeuristics.push_back(HEURISTIC9);
}

GRASP::~GRASP(){}

/**
* Metodo que executa o teste adaptativo, ou seja, com uma fase de treinamento para escolher a melhor configuracao do GRASP
* Method that performs adaptive testing, ie, a training phase to choose the best configuration of the GRASP
*/
int GRASP::adaptiveCalculateTree(){

	initialize();

	//Execução das heurísticas completamente gulosas = Execution of completely greedy heuristics
	int maxNoUpdateIterations = std::max(Graph::graph.numberOfClusters/10, 1u) ;
	//int maxNoUpdateIterations = std::min(Graph::graph.numberOfClusters/10, 1u) ;

	unsigned int heuristic = 0;
	while((heuristic < greedHeuristics.size()) && (!stopCriterionReached())){

		constructiveHeuristic = greedHeuristics[heuristic];		
		vector<int> solution = Constructor::constructSolution(constructiveHeuristic);		
		
		//Cada heuristica executa por um numero 'maxNoUpdateIterations' de iteracoes
		for(int i=0; i < maxNoUpdateIterations; i++){
			vector<int> auxSolution = solution;
			int	cost = Heuristic5::getMSTCost(auxSolution);
			cost = Searcher::localSearch(auxSolution);

			if(Parameters::getInstance().VERBOSE)
				cout << "HEURISTICA " << greedHeuristics[heuristic] << " ----> " << iteration << " Solucao: " << cost << endl;

			//Atualiza a solucao no conjunto elite, se for o caso
			eliteSet.update(auxSolution, cost, iteration);
		}
		heuristic++;
	}
	
	//GRASP agora irá considerar as heuristicas que possuem um certo grau de aleatoriedade
	//GRASP will now consider the heuristics that have a certain degree of randomness
	heuristic = 0;

	while(!stopCriterionReached()){
		
		constructiveHeuristic = heuristics[heuristic];
		int heuristicIter=0;
		int updates = 0;
		//Cada heuristica executa por um numero 'maxNoUpdateIterations' de iteracoes
		while((heuristicIter < iterations[heuristic]) &&  (!stopCriterionReached())){

			Parameters::setAlpha((1+rand()%6)*0.05);
			vector<int> solution = Constructor::constructSolution(constructiveHeuristic);
			totalIterations[heuristic]++;
			int cost = Searcher::localSearch(solution);

			if(Parameters::getInstance().VERBOSE)
				cout << "HEURISTICA " << constructiveHeuristic << " ----> " << iteration << " Solucao: " << cost << endl;

			//Faz o path relinking se a solucao corrente for suficientemente boa
			if((!stopCriterionReached()) && (Parameters::getInstance().PATH_RELINKING_ITER != NONE) && (eliteSet.goodSolution(cost))){
				if(completePathRelinking(solution, cost))
					updates++;
			}

			if(updateParameters(solution, cost))
				updates++;
			heuristicIter++;
		}
		//Se a heuristica atualizou o conjunto elite, recebe um pouco mais de iterações
		if(updates > 0)
			iterations[heuristic]++;
//			iterations[heuristic]*= 1.2;

		//Passando para a próxima heurística construtiva
		heuristic = (heuristic == heuristics.size()-1)? 0: heuristic+1;
	}

	//Retorna a melhor (ou seja, a ultima) solucao do conjunto elite
	return eliteSet.bestCost();
}

/**
 * Aplica uma busca local ILS em cada membro do conjunto elite
 * Retorna true caso alguma atualização tenha sido feito, false caso contrário
 */
bool GRASP::eliteSetILS(){

	bool update = false;

	vector< vector<int> > solutions;
	vector<int> costs;
		
	//Criando um "saco" com as soluções do conjunto elite
	list< vector<int> >::iterator it1=eliteSet.solutionSet.begin();
	list<int>::iterator it2=eliteSet.costSet.begin();

	for(; it1 != eliteSet.solutionSet.end(); it1++, it2++){
		solutions.push_back(*it1);
		costs.push_back(*it2);
	}

	unsigned int i=0;
	//Aplicando o ils em cada solução do "saco"
	while((i < solutions.size()) && (!stopCriterionReached())){
		//Aplicando a busca ils sobre a solucao
 		int cost = Searcher::ils(solutions[i], costs[i]);
		//Verificando se a solucao gerada vai passar a fazer parte do conjunto elite
 		if(eliteSet.update(solutions[i], cost, iteration)){
			update = true;
			solutions.push_back(solutions[i]);
			costs.push_back(cost);
		}
		i++;
	}
	
	return update;
}

/**
 * Aplica todas as atualizações ao conjunto elite
 */
bool GRASP::updateParameters(vector<int> &solution, int cost){
	 	 
	 //eliteSet.averageCost = (eliteSet.averageCost*(iteration-1) + cost)/iteration;
	 //Atualiza a solucao no conjunto elite, se for o caso
	 bool update = eliteSet.update(solution, cost, iteration);

	//Se já tiverem se passado MAX_NO_UPDATE_ITER iterações sem atualização do conjunto elite e se o ILS não produzir nenhuma modificação, o conjunto elite é resetado
	 if(eliteSet.lastUpdate >= Parameters::getInstance().MAX_NO_UPDATE_ITER){
		 if(Parameters::getInstance().ILS){
			 update = eliteSetILS();
		 }
		 //Setando a flag que indica que o ils já foi aplicado
		 iterations.clear();
		 iterations.assign(heuristics.size(), 1);
		 ilsApplication++;
		 if(!update){
			 eliteSet.reset();
		 }
	 }
	 iteration++;
	 eliteSet.lastUpdate++;

//	cout << iteration << " " << (cpuTime()-startTime) << " " << cost << " " << eliteSet.bestCost() << endl;
	 return update;
 }
/**
 * Verifica se ja foi atingido o criterio de parada
 */
bool GRASP::stopCriterionReached(){

	if((Parameters::getInstance().EASY_TARGET != NONE) && (easyTargetTime == 0) && (eliteSet.bestCost() > 0) && (eliteSet.bestCost() <= Parameters::getInstance().EASY_TARGET)){
		easyTargetTime = cpuTime() - startTime;
	}
	
	bool criterion1 = ((Parameters::getInstance().STOP_CRITERION == FIND_TARGET) && (((unsigned)eliteSet.bestCost() <= Graph::graph.target) || ((cpuTime() - startTime) > Graph::graph.timeLimit)));
	bool criterion2 = ((Parameters::getInstance().STOP_CRITERION == ITERATIONS) && (iteration > maxIter));
	bool criterion3 = ((Parameters::getInstance().STOP_CRITERION == ILS_APPLICATION) && (ilsApplication == 2));
	
	//O algoritmo deve parar se:
	//O criterio de parada é um alvo e esse ja foi encontrado ou o tempo limite jah foi atingido
	//Ou
	//O criterio de parada é o numero de iteracoes, e esse jah foi atingido
	//Ou
	//O criterio de parada é a aplicacao do ils e jah se aplicou o numero suficiente
		
	return ((criterion1) || (criterion2) || (criterion3));	
}

/**
* Faz o pre-processamento necessario para a execucao de um teste GRASP
* (Inicializacao de variaveis, definicao de parametros, etc.)
*/
void GRASP::initialize(){

	ilsApplication = 0;
	startTime = cpuTime();
	easyTargetTime = 0;
	eliteSet.clear();
	iteration = 1;
	constructiveHeuristic = Parameters::getInstance().HEURISTIC;

	//Todas as heuristicas comecam com um máximo de 1 iteracao
	iterations.clear();
	iterations.assign(heuristics.size(), 1);
	totalIterations.clear();
	totalIterations.assign(heuristics.size(), 0);
				
	//Ordenando arestas
	Graph::graph.sortEdges();

	vector<int> vertices(Graph::graph.numberOfClusters);
	for(unsigned int cluster = 0; cluster < Graph::graph.numberOfClusters; cluster++){
		int numVertices = Graph::graph.clusters[cluster].size();
		//vertices[cluster] = Graph::graph.clusters[cluster][random()%numVertices];
		vertices[cluster] = Graph::graph.clusters[cluster][rand()%numVertices];
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
	//Se nao houver path relinking, o elite set so tera um elemento	
	if(Parameters::getInstance().PATH_RELINKING_ITER == NONE)
		Parameters::setEliteSetSize(1);
}

/**
* Metodo que executa o algoritmo GRASP
*/
int GRASP::calculateTree(){
	
	initialize();
	//A cada iteracao GRASP...
 	while(!stopCriterionReached()){

		Parameters::setAlpha((1+rand()%6)*0.05);
		//constructiveHeuristic = heuristics[random()%heuristics.size()];		
		vector<int> solution = Constructor::constructSolution(constructiveHeuristic);
		//int cost = (Parameters::getInstance().VNS)? Searcher::localSearchVNS(solution) : Searcher::localSearch(solution);
		int cost = Searcher::localSearch(solution);
		if(Parameters::getInstance().VERBOSE)
			cout << iteration << " noUpdates: " << eliteSet.lastUpdate << ", Solucao: " << cost << endl;

		//A partir da iteracao PATH_RELINKING_ITER faz o path relinking se a solucao corrente for suficientemente boa
		if((!stopCriterionReached()) && (Parameters::getInstance().PATH_RELINKING_ITER != NONE) && (iteration >= Parameters::getInstance().PATH_RELINKING_ITER) && (eliteSet.goodSolution(cost))){
			completePathRelinking(solution, cost);
		}
		updateParameters(solution, cost);
	}

	//Retorna o custo da melhor solucao do conjunto elite
	return eliteSet.bestCost();
}

/**
 * Metodo que faz o path relinking da solucao passada como parametro para cada um dos membros do conjunto elite
 */
bool GRASP::completePathRelinking(vector<int> solution, int cost){
	
	vector<int> solution2;
	//Procura a solução do conjunto elite mais distante de 'solution'
	int cost2 = eliteSet.chooseASolution(solution, solution2);
	//Se encontrou uma solução diferente de 'solution' no conjunto elite
	if(solution2.size() > 0){
		//O Path Relinking será no sentido da solução de menor custo para a de maior
		if(cost < cost2)
			return pathRelinking(solution, cost, solution2, cost2);
		else
			return pathRelinking(solution2, cost2, solution, cost);
	//Se não encontrou uma solução diferente, retorna 'false', não houve atualização do CE
	}else 
		return false;
}

/**
* Metodo que implementa o path relinking entre as solucoes 'solution1' e 'solution2'
* A solucao base será a de menor custo, e a outra, obviamente sera a solucao alvo
* Retorna true caso o metodo tenha obtido melhoras
*/
bool GRASP::pathRelinking(vector<int> base, int baseCost, vector<int> guide, int guideCost){
	
	bool update = false;	
	
	if(Parameters::getInstance().VERBOSE){
		cout << "Path Relinking de " << baseCost << " a " << guideCost << endl;
	}
	//Enquanto as solucoes forem diferentes
	while(EliteSet::computeDistance(base, guide) != 0){
		
		//Realiza o movimento		
		int cost = makeVertexMovement(base, guide);		
		if(Parameters::getInstance().VERBOSE)
			cout << cost << ": ";
		//Se for uma solucao melhor que as do conjunto elite
		if(eliteSet.update(base, cost, iteration)){
			update = true;
			//Salva a solucao encontrada e faz uma busca local
			vector<int> temp = base;
			int tempCost = Searcher::localSearch(temp);
			eliteSet.update(temp, tempCost, iteration);
		}
	}
	return update;
}

/**
* Metodo que faz um movimento do path relinking no sentido da solucao 'base' para a solucao 'target'
* O movimento é feito trocando um vertice da solucao base por um vertice usado na solucao alvo e recalculando a MST
* Retorna a solucao gerada desse movimento
*/
int GRASP::makeVertexMovement(vector<int> &base, vector<int> &guide){

	int bestCost = INFINITE;
	int bestCluster = NONE;

	//Consulta cada cluster das duas solucoes
	for(unsigned int cluster=0; cluster < Graph::graph.numberOfClusters; cluster++){
				
 		//Se os vertices sao diferentes nas duas solucoes, gera uma nova solucao
		if(base[cluster] != guide[cluster]){
			int currVertex = base[cluster];
			//Troca o vertice de saida do cluster e refaz a MST				
			base[cluster] = guide[cluster];
			int cost = Heuristic5::getMSTCost(base);

 			//Faz uma busca local em cada candidato a solução intermediária
			if(rand()%1000 < 10*((float)eliteSet.lastUpdate/(float)Parameters::getInstance().MAX_NO_UPDATE_ITER)){
				//Salva a solucao encontrada e faz uma busca local
				vector<int> temp = base;
				int tempCost = Searcher::localSearch(temp);
				eliteSet.update(temp, tempCost, iteration);
			}
			//Se encontrou um solucao melhor...
 			if(cost < bestCost){
				bestCost = cost;
				bestCluster = cluster;
 			}
			base[cluster] = currVertex;
		}
	}
	base[bestCluster] = guide[bestCluster];
	return bestCost;
}

/**
* Metodo usado para testar o algoritmo
*/
string GRASP::execute(string instanceName, int timeLimit){

	double totalTime = 0;
	double bestTime = INFINITE;
	unsigned int times = Parameters::getInstance().TIMES;

	ostringstream oss;
	ostringstream ossFileName;
	ossFileName << INSTANCES_DIR << "_time" << timeLimit <<"/" << instanceName << "_result_" <<  timeLimit << ".txt";
	ofstream fout(ossFileName.str().c_str(), ios::app);
	fout << Graph::graph.instanceName << endl;
	
	unsigned int totalCost = 0;
	int bestCost = INFINITE;
	int avgBestIteration = 0;
	vector<float> avgIterations(heuristics.size(),0);
	
	//Para cada execução do GRASP
	for(unsigned int i=0; i < times; i++){

		double initialTime = cpuTime();
		
		int cost = (Parameters::getInstance().HEURISTIC == NONE)? adaptiveCalculateTree(): calculateTree();
		
		totalCost += cost;
		avgBestIteration += eliteSet.getBestIteration();
		bestCost = min(cost, bestCost);

		//Extraindo tempo de execução
		double finalTime = cpuTime() - initialTime;
		totalTime += finalTime;

		if(finalTime < bestTime)
			bestTime = finalTime;

		for(unsigned int j=0; j < heuristics.size(); j++){
			avgIterations[j] += totalIterations[j];
		}
		//Plotando em um arquivo auxiliar as informacoes sobre a iteracao corrente
		fout << i << " " << cost << " " << easyTargetTime << " " << finalTime << " " << eliteSet.getBestIteration() << " " << iteration << endl;
		//oss << i << " " << finalTime << " " << tree.getCost() << endl;
	}
	//Plotando o numero de iteracoes medio que foi dado para cada heuristica
	fout << "Iteracoes: ";
	for(unsigned int i=0; i < heuristics.size(); i++){
		fout << avgIterations[i]/(float)times << " ";
	}
	fout << endl;
	
 	oss << (float)totalCost/(float)times << " " << bestCost << " " << (totalTime)/times << " " << bestTime << " " << (float)avgBestIteration/(float)times << " ";
	fout << oss.str() << endl;
	fout << "-----------------------------------------------------------------" << endl;
	fout.close();
	
	return oss.str();
}
