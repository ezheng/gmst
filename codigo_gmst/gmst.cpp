/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 06 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *																		   *
 * Programa que implementa um algoritmo GRASP e uma busca tabu para o      *
 * problema GMST                                                           *
 * As instancias devem estar no diretório especificado no arquivo		   *
 * instance.h															   *
 *                                                                         *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

#include "evolution.h"
#include "graph.h"
#include "grasp.h"
#include "heuristic5.h"
#include "instance.h"
#include "instancegenerator.h"
#include "parameters.h"
#include "tabu.h"

#define INSTANCES_FILE "instances.dat"
//#define PLOT_DIR "plot/"
std::string INSTANCES_DIR;

using namespace std;

//Indica se serão aplicados os testes de redução sobre a instância
bool reductionTests = true;
//Indica se o algoritmo a ser executado será o grasp
bool executeGrasp = true;
//Indica se deve-se criar os arquivos para plotar a melhor solução == Indicates whether to create the files to plot the best solution
bool plotSolution = true;
//Indica se deve-se imprimir a solução no formato de uma solução do problema de steiner == Indicates whether to print the solution in the form of a solution to the problem of Steiner
bool printSteiner = false;

//Armzena o nome de cada instância
vector<string> instances;
//Armazena o tempo limite de execução dos algoritmos para cada instância
vector<double> timeLimits;
//Armazena o custo alvo de cada instância
vector<int> targets;

/**
 * Cria um arquivo com a solução no formato de uma solução do problema de steiner
 */
void printSTPSolution(string instanceName, vector<int> solution){

	stringstream oss;
	oss << "solutions/" << instanceName << ".stp.sol";
	ofstream fout(oss.str().c_str(), ios::out);

	if(!fout.fail()){

		//Imprimindo informações iniciais sobre a solucao
		fout << "STEINER PROBLEM SOLUTION" << endl << endl;
		fout << "Instance " << instanceName << ".stp" << endl;

		int cost = Heuristic5::getMSTCost(solution);
		fout << "Objective Cost " << cost << endl;
	
		//O cluster da raiz é o cluster com menos vértices e com menor identificador
		int rootCluster = 0;
		for(unsigned int i=1; i < Graph::graph.numberOfClusters; i++){
			if(Graph::graph.clusters[i].size() < Graph::graph.clusters[rootCluster].size()){
				rootCluster = i;
			}
		}

		//Imprimindo os arcos da solução e os arcos adicionais (para os terminais)
		fout << endl << "Arcs" << endl;
		fout << Heuristic5::printSteinerMST(solution, rootCluster);
	
		fout << "EOF" << endl;
		fout.close();
	} else {
		cout << "Erro ao criar arquivo " << oss.str() << endl;
	}
}

/**
 * Cria arquivos no formato do gnuplot para a visualização da melhor solução encontrada
 */
void createPlotFiles(string instanceName, vector<int> solution, int timeLimit){
	
	int pos1=0;
	//while((instanceName[pos1] >= '0') && ((int)instanceName[pos1] <= '9')){
	//	pos1++;
	//}
	int pos2 = 0;
	while(instanceName[pos2+pos1] != '.'){
		pos2++;
	}
	ostringstream oss_enliang;
	oss_enliang << INSTANCES_DIR << "_time" << timeLimit << "/";
	//string plotFile = INSTANCES_DIR + instanceName.substr(pos1, pos2) + ".coo";
	string plotFile = oss_enliang.str() + instanceName.substr(pos1, pos2) + ".coo";

	vector<int> tree = Heuristic5::calculateMST(solution);
	vector<float> posX(Graph::graph.numberOfVertices);
	vector<float> posY(Graph::graph.numberOfVertices);

	ostringstream oss1;
	//oss1 << INSTANCES_DIR << instanceName << ".coo";
	ifstream fin(plotFile.c_str(), ios::in);
	//Lendo a posição de cada vertice no plano
	if(fin.fail()){
		cout << "Erro ao abrir arquivo " << plotFile << "!" << endl;
		exit(1);
	} else {		
		for(unsigned int i=0; i < Graph::graph.numberOfVertices; i++){
			fin >> posX[i] >> posY[i];
		}
		fin.close();	

		ofstream fout_Enliang( (oss_enliang.str() + instanceName + ".vertices").c_str(), ios::out);

		//Criando o arquivo com as informações sobre as arestas = Creating the file with information about the edges
	//	ofstream fout1( (oss_enliang.str() + instanceName + ".gnudat1").c_str() , ios::out);

		//Para cada aresta da solução, imprimindo o vertice 1
		for(unsigned int vertex1=0; vertex1 < tree.size(); vertex1++){
			if(tree[vertex1] != -1){
	//			fout1 << posX[vertex1] << " " << posY[vertex1];
				fout_Enliang << vertex1 << " ";
	//			if(vertex1 < tree.size()-1)
	//				fout1 << " ";
			}
		}
	//	fout1 << endl;
		fout_Enliang << endl;
		//Para cada aresta da solução, imprimindo o vertice 2 =For each edge of the solution, printing the vertex 2
		for(unsigned int vertex1=0; vertex1 < tree.size(); vertex1++){
			if(tree[vertex1] != -1){
				int vertex2 = tree[vertex1];
	//			fout1 << posX[vertex2] << " " << posY[vertex2];
				fout_Enliang << vertex2 << " ";
	//			if(vertex1 < tree.size()-1)
	//				fout1 << " ";
			}
		}
	//	fout1.close();
		fout_Enliang.close();

		//Criando arquivo que conterá as posições dos vértices de cada cluster =Creating file containing the positions of the vertices of each cluster
	/*	ofstream cooFile((oss_enliang.str() + instanceName + ".gnudat2").c_str(), ios::out);
		
		//Inserindo os dados por linha do arquivo = Entering the data file line
		unsigned int line = 0;
		while(line < Graph::graph.numberOfVertices){
	
			//Inserindo a posição de um vértice de cada cluster
			for(unsigned int cluster=0; cluster < Graph::graph.numberOfClusters; cluster++){
				//Se o cluster ainda tiver algum vértice
				if(Graph::graph.clusters[cluster].size() > line){
					cooFile << posX[Graph::graph.clusters[cluster][line]] << " " << posY[Graph::graph.clusters[cluster][line]] << " ";
				} else {
					cooFile << "-" << " " << "-" << " ";
				}
			}
			cooFile << endl;
			line++;
		}
		cooFile.close(); */
	
	/*	//Agora criando arquivo com os comandos do gnuplot
		ofstream gnuFile( (oss_enliang.str() + instanceName + ".gnu").c_str(), ios::out);
		gnuFile << "set output \""<< instanceName << ".eps\"" << endl;
		gnuFile << "set terminal postscript eps color" << endl;
		gnuFile << "plot ";
	
		//Comandos para plotar as arestas do grafo
		for(unsigned int i=0; i < Graph::graph.numberOfClusters-1; i++){
			//gnuFile << " \"" << instanceName << ".gnudat1" << "\" using " << i*2+1 << ":" << (i+1)*2 << " title \"\" with lines 0";
			gnuFile << " \"" << instanceName << ".gnudat1" << "\" using " << i*2+1 << ":" << (i+1)*2 << " title \"\" with lines";
			if(i < Graph::graph.numberOfClusters-2){
				gnuFile << ",";
			}
		}
		//Comandos para plotar os vértices do grafo
		gnuFile << ",";
		for(unsigned int i=1; i <= Graph::graph.numberOfClusters; i++){
			gnuFile << " \"" << instanceName << ".gnudat2" << "\" using " << i*2-1 << ":" << i*2 << " title \"\" with points";
			if(i < Graph::graph.numberOfClusters){
				gnuFile << ",";
			}
		}
		gnuFile << endl;
		gnuFile.close(); */
	
		if(Parameters::getInstance().PRINT_SOLUTION){
			cout << "Solucao:" << endl;
			for(unsigned int i=0; i < solution.size(); i++){
				cout << i+1 << ": " << solution[i] << endl;
			}
		}
	}
}

/**
* Imprime o modo como o programa deve ser utilizado e quais são seus parametros
*/
void help(){

	cout << "|| USO:" << endl;
	// use
	cout << "|| gmst <instancia> <parametro1=valor1> ... <parametroN=valorN>" << endl;
	//
	cout << "|| ou" << endl;
	// or
	cout << "|| gmst instances.dat <parametro1=valor1> ... <parametroN=valorN>" << endl << endl;

	cout << "LISTA DE PARAMETROS:" << endl << endl;
	// list of parameters
	cout << "|| ALPHA - Valor do parametro alpha do algoritmo construtivo" << endl;
	// alpha - value of the parameter alpha  constructive algorithm
	cout << "|| MAX_ITER_GRASP - Numero de iteracoes GRASP" << endl;

	cout << "|| MAX_ITER_LOCAL_SEARCH - Numero maximo de iteracoes da busca local" << endl;
	cout << "|| MINIMUM_SUPORT - Suporte mínimo do algoritmo de mineração apriori" << endl;
	cout << "|| HEURISTIC - Heuristica de construtcao a ser utilizada (1 a 12)" << endl;
	cout << "|| SEARCH_METHOD - Metodo de busca local (1 a 5)" << endl;
	cout << "|| ELITE_SET_SIZE - Numero de elementos no conjunto elite" << endl;
	cout << "|| PATH_RELINKING_ITER - Iteracao que ativa o path relinking (-1 para nao ativar)" << endl;
	cout << "|| TIMES - Numero de vezes que o algoritmo ira executar para cada instancia" << endl;
	cout << "|| SEED - Semente a ser utilizada para o calculo dos numeros aleatorios" << endl;
	cout << "|| ELITE_SOLUTION_LIMIT - Razao maxima entre um certa solucao e a melhor solucao do conjunto elite para que a primeira seja considerada boa" << endl;
	cout << "|| -VERBOSE - Define se deve ser impresso o resultado de cada iteracao GRASP (sem valor)" << endl;
	cout << "|| -PRINT_PARAMETERS - Define se os parametros devem ser impressos (sem valor)" << endl;
	cout << "|| -PRINT_SOLUTION - Define se a solucao final deve ser impressa (sem valor)" << endl;
	cout << "|| TABU_TENURE - Define o tamanho da lista tabu" << endl;

	cout << endl << "=================================================================================================" << endl;
}

/**
* Função que testa todas as instancias
* Function that tests all instances
* Executa o algoritmo especificado sobre cada instância e imprime os resultados
* Executes the specified algorithm on each instance and prints the results
* Os nomes das instâncias, os tempos de execução e os alvos são retirados dos arrays 'instances', 'timeLimits' e 'targets',
* respectivamente
* The names of the instances, the execution times and the targets are removed from the arrays 'instances',
* 'timeLimits' and 'targets', respectively
*/
void execute(){

	//Setando a semente que sera usada para calcular os numeros aleatorios
	if(Parameters::getInstance().SEED == -1){
		int seed = time(NULL);
		Parameters::setSeed(seed);
	}
	srand(Parameters::getInstance().SEED);
	cout << "Semente: " << Parameters::getInstance().SEED << endl;
	
	//Para cada instância
	for(unsigned int i=0; i < instances.size() ;i++){
		//Cria a instância e o grafo
		Instance instance(instances[i], timeLimits[i]);
		
		if(Parameters::getInstance().STOP_CRITERION == FIND_TARGET)
			Graph::createInstance(instance, targets[i], timeLimits[i]);
		else
			Graph::createInstance(instance);

		if(reductionTests)
			Graph::graph.reductionTests();

 		if(executeGrasp){
 			GRASP grasp;
 			cout << instances[i] << " " << grasp.execute(instances[i], timeLimits[i]) << endl;
 			//Se deve imprimir a melhor solução em arquivo gnuplot
 			if(plotSolution){
 				createPlotFiles(instances[i], grasp.eliteSet.bestSolution(), timeLimits[i]);
 			}
			//Se deve gerar um arquivo com a solução no formato de uma solução de steiner
			if(printSteiner){
				printSTPSolution(instances[i], grasp.eliteSet.bestSolution());
			}
 		}else{
			Evolution evolution;

			cout << instances[i] << " " << evolution.execute() << endl;
			if(plotSolution){
				createPlotFiles(instances[i], evolution.population.bestSolution(), timeLimits[i]);
			}
//  			Tabu tabu;
//      			cout << instances[i] << " " << tabu.execute() << endl;
//  			//Se deve imprimir a melhor solução em arquivo gnuplot
 		}
	}
}

/**
 * Funcao para testar o valor de alpha das heuristicas construtivas
 */
void testAlpha(){
	
	unsigned int times = Parameters::getInstance().TIMES;
	
	vector<float> alphaValues;
	alphaValues.push_back(0.1);
	alphaValues.push_back(0.15);
	alphaValues.push_back(0.2);
	alphaValues.push_back(0.25);
	alphaValues.push_back(0.3);
	alphaValues.push_back(0.35);
	alphaValues.push_back(0.4);
	alphaValues.push_back(0.5);
	alphaValues.push_back(0.6);
	alphaValues.push_back(0.7);
	alphaValues.push_back(0.8);
	alphaValues.push_back(0.9);
	alphaValues.push_back(1.0);

	vector< vector<float> > averageCosts(alphaValues.size());
	vector< vector<double> > stdDeviations(alphaValues.size());
	vector< vector<int> > bestCosts(alphaValues.size());
	for(unsigned int i=0; i < alphaValues.size(); i++){
		averageCosts[i].resize(instances.size());
		stdDeviations[i].resize(instances.size());
		bestCosts[i].assign(instances.size(),INFINITE);
	}

	//Para cada instância
	for(unsigned int j=0; j < instances.size(); j++){		

		cout << "Instancia: " << instances[j] << endl;
		Instance instance = Instance(instances[j]);
		Graph::createInstance(instance);
		Graph::graph.reductionTests();
		Graph::graph.sortEdges();

		//Para cada valor de alpha
		for(unsigned int i=0; i < alphaValues.size(); i++){

			cout << "Alpha: " << alphaValues[i] << endl;
			Parameters::setAlpha(alphaValues[i]);

			vector<int> costs(times, 0);
			int totalCost = 0;
			
			//Para cada execução da heurística
			for(unsigned int k=0; k < times; k++){
				vector<int> solution = Constructor::constructSolution(Parameters::getInstance().HEURISTIC);
				int cost = Heuristic5::getMSTCost(solution);

				totalCost += cost;
				costs[k] = cost;
				bestCosts[i][j]= min(cost, bestCosts[i][j]);
			}
			averageCosts[i][j] = totalCost/times;
		
			for(unsigned int k=0; k < times; k++){
				stdDeviations[i][j] += pow((costs[k]-averageCosts[i][j]),2);
			}
			stdDeviations[i][j] = sqrt(stdDeviations[i][j]);
		}
	}

	vector<int> maxBestCosts(instances.size(),0);
	vector<int> minBestCosts(instances.size(),INFINITE);
	vector<float> maxAverageCosts(instances.size(),0);
	vector<float> minAverageCosts(instances.size(), INFINITE);
	vector<double> maxStdDeviations(instances.size(),0);
	vector<double> minStdDeviations(instances.size(), INFINITE);
	//Calculando valores minimos e maximos
	for(unsigned int j=0; j < instances.size(); j++){

		for(unsigned int i=0; i < alphaValues.size();i++){
			maxBestCosts[j] = max(maxBestCosts[j],bestCosts[i][j]);
			minBestCosts[j] = min(minBestCosts[j],bestCosts[i][j]);
			maxAverageCosts[j] = max(maxAverageCosts[j], averageCosts[i][j]);
			minAverageCosts[j] = min(minAverageCosts[j], averageCosts[i][j]);
			maxStdDeviations[j] = max(maxStdDeviations[j], stdDeviations[i][j]);
			minStdDeviations[j] = min(minStdDeviations[j], stdDeviations[i][j]);
		}
	}

	//Normalizando e plotando os valores
	for(unsigned int i=0; i < alphaValues.size(); i++){

		cout << alphaValues[i];
		for(unsigned int j=0; j < instances.size(); j++){
			float bestCost = 100.0*(float)(bestCosts[i][j]-minBestCosts[j])/(float)(maxBestCosts[j]-minBestCosts[j]);
			float averageCost = 100.0*(averageCosts[i][j]-minAverageCosts[j])/(maxAverageCosts[j]-minAverageCosts[j]);
			double stdDeviation = 100.0*(stdDeviations[i][j]-minStdDeviations[j])/(maxStdDeviations[j]-minStdDeviations[j]);

			cout << " " << bestCost << " " << bestCosts[i][j] << " " << averageCost << " " << averageCosts[i][j] << " " << stdDeviation << " " << stdDeviations[i][j];
		}
		cout << endl;
	}
}

/**
* Metodo que extrai os nomes dos arquivos com as instancias que devem ser testadas
*/
void extractInstancesNames(char* fileName){
				
	ifstream fin(fileName, ios::in);	

	if(fin.fail()){
		cout << "Erro ao abrir aquivo " << fileName << "!" << endl;

	} else {
	
		while(!fin.eof()){
			string instance;
			int timeLimit;
			int target;
	
			fin >> instance >> target >> timeLimit >> INSTANCES_DIR;
			instances.push_back(instance);
			targets.push_back(target);
			timeLimits.push_back(timeLimit);
		}
		fin.close();
	}
}

/**
* Metodo que seta os parametros da aplicacao
*
* Retorna: string com mensagem de erro, se houver ou "OK", caso contrário
*/
string setApplicationParameters(int argc, char *argv[]){

	for(int i=2; i < argc; i++){

		string parameter(argv[i]);		
		string parameterName("");

		//buscando o nome do parametro		
		int j=0;
		int stringSize = parameter.size();

		if(parameter.at(j) == '-'){
			parameterName = parameter.substr(1);
			j = stringSize;
		} else {			
			while((j < stringSize) && (parameter.at(j) != '=')){
				parameterName += parameter.at(j);
				j++;
			}		
		}

		if(parameterName.empty()){
			return "ERRO DE SINTAXE NA ESPECIFICACAO DOS PARAMETROS!";
		} else{

			j++;
			string parameterValue("");

			while(j < stringSize){
				parameterValue += parameter.at(j);
				j++;       
			}
								
			if(parameterName == "ALPHA")				
				Parameters::setAlpha(atof(parameterValue.c_str()));

			else if (parameterName == "MAX_ITER_GRASP")
				Parameters::setMaxIterationsGRASP(atoi(parameterValue.c_str()));

			else if (parameterName == "HEURISTIC")
				Parameters::setHeuristic(atoi(parameterValue.c_str()));

			else if(parameterName == "NUMBER_OF_CENTERS")
				Parameters::setNumberOfCenters(atoi(parameterValue.c_str()));

			else if (parameterName == "ELITE_SET_SIZE")
				Parameters::setEliteSetSize(atoi(parameterValue.c_str()));
			
			else if(parameterName == "PATH_RELINKING_ITER")
				Parameters::setPathRelinkingIter(atoi(parameterValue.c_str()));

			else if(parameterName == "TIMES")
				Parameters::setTimes(atoi(parameterValue.c_str()));

			else if(parameterName == "SEED")
				Parameters::setSeed(atoi(parameterValue.c_str()));

			else if(parameterName == "ELITE_SOLUTION_LIMIT")
				Parameters::setEliteSolutionLimit(atof(parameterValue.c_str()));
	
			else if(parameterName == "VERBOSE")
				Parameters::setVerbose();

			else if(parameterName == "PRINT_PARAMETERS")
				Parameters::setPrintParameters();

			else if(parameterName == "PRINT_SOLUTION")
				Parameters::setPrintSolution();			

			else if(parameterName == "STOP_CRITERION")
				Parameters::setStopCriterion(atoi(parameterValue.c_str()));

			else if(parameterName == "TABU")
				executeGrasp = false;

			else if(parameterName == "EVOLUTION")
				executeGrasp = false;			

			else if(parameterName == "GRASP")
				executeGrasp = true;

			else if(parameterName == "PLOT")
				plotSolution = true;

			else if(parameterName == "NO_REDUCTION")
				reductionTests = false;

			else if(parameterName == "PRINT_STP_SOLUTION")
				printSteiner = true;

			else if(parameterName == "ILS")
				Parameters::setIls(true);

			else if(parameterName == "VNS")
				Parameters::setVns(true);
			
			else if(parameterName == "NO_ILS")
				Parameters::setIls(false);

			else if(parameterName == "NO_VNS")
				Parameters::setVns(false);
						
			else if(parameterName == "NEIGHBOR_DISTANCE")
				Parameters::setNeighborDistance(atof(parameterValue.c_str()));

			else if(parameterName == "MAX_NO_UPDATE_ITER")
				Parameters::setMaxNoUpdateIter(atoi(parameterValue.c_str()));

			else if(parameterName == "EASY_TARGET")
				Parameters::setEasyTarget(atoi(parameterValue.c_str()));
						
			//Parametro desconhecido
			else if(i > 3)
				return "================== PARAMETRO DESCONHECIDO: "+parameterName+" ==================";
		}
	}
	return "OK";
}

/**
 * Função para testar os construtivos sobre as instâncias
 */
void testConstructives(){


	for(unsigned int i=0; i < instances.size(); i++){
		
 		Instance instance = Instance(instances[i]);
 		Graph::createInstance(instance);
 		Graph::graph.reductionTests();

		double avgCost = 0.0;
		for(unsigned int j=0; j < Graph::graph.edges.size(); j++){
			avgCost += Graph::graph.edges[j][COST];
		}
		avgCost /= Graph::graph.edges.size();

		double stdDeviation = 0.0;
		for(unsigned int j=0; j < Graph::graph.edges.size(); j++){
			stdDeviation += pow((Graph::graph.edges[j][COST] - avgCost), 2);
		}
		stdDeviation = sqrt(stdDeviation);
		
		cout << instances[i] << " " << avgCost << " " << stdDeviation << endl;
	}
		
// 	vector<int> heuristics;
//  	heuristics.push_back(HEURISTIC5);
//  	heuristics.push_back(RANDOMIZED_HEURISTIC6);
//  	heuristics.push_back(RANDOMIZED_HEURISTIC1);
//  	heuristics.push_back(RANDOMIZED_HEURISTIC7);
//  	heuristics.push_back(RANDOMIZED_HEURISTIC9);
// 
// 	vector<int> greedHeuristics;
// 	greedHeuristics.push_back(HEURISTIC1);
// 	greedHeuristics.push_back(HEURISTIC6);
// 	greedHeuristics.push_back(HEURISTIC7);
// 	greedHeuristics.push_back(HEURISTIC9);
// 
// 	vector<int> bests(heuristics.size(),0);
// 	//Para cada instância
// 	for(unsigned int i=0; i < instances.size(); i++){
// 
// 		Instance instance = Instance(instances[i]);
// 		Graph::createInstance(instance);
// 		Graph::graph.reductionTests();
// 		Graph::graph.sortEdges();
// 
// 		cout << instances[i];
// 
// 		//Para cada heurística gulosa
// // 		for(unsigned int j=0; j < greedHeuristics.size(); j++){
// // 
// // 			double startTime = cpuTime();
// // 			vector<int> solution = Constructor::constructSolution(greedHeuristics[j]);
// // 			int cost = Heuristic5::getMSTCost(solution);
// // 			cout << cost << " " << (cpuTime() - startTime) << " ";
// // 		}
// 
// 		int bestCost = INFINITE;
// 		vector<int> totalCosts(heuristics.size(),0);
// 		//Para cada heurística não-gulosa
// 		for(unsigned int j=0; j < heuristics.size(); j++){
// 
// 			double startTime = cpuTime();
// 			for(int k=0; k < 100; k++){
// 				
// 				vector<int> solution = Constructor::constructSolution(heuristics[j]);
// 				totalCosts[j] += Searcher::localSearch(solution);
// //				totalCosts[j] += Heuristic5::getMSTCost(solution);
// 			}
// 			double totalTime = (cpuTime() - startTime);
// 			cout << " " << totalCosts[j]/100.0 << " " << totalTime/100.0;
// 			bestCost = min(totalCosts[j], bestCost);
// 		}
// 		for(unsigned int j=0; j < heuristics.size(); j++){
// 			if(totalCosts[j] == bestCost){
// 				bests[j]++;
// 			}
// 		}
// 		cout << endl;
// 	}
// 
// 	cout << "Melhores";
// 	for(unsigned int i=0; i < heuristics.size();i++){
// 		cout << " " << bests[i];
// 	}
// 	cout << endl;
}

/**
* Metodo que executa a aplicacao
*/
void runApplication(int argc, char *argv[]){

	//Se o numero de parametros for menor que 1, a sintaxe de execucao do programa esta errada
	if(argc < 2){

		help();		
	}else{

		//Seta os parametros da aplicacao segundo os argumentos que o usuario passou
		string mensagem = setApplicationParameters(argc, argv);
		
		//Se nao houver erro de sintaxe na passagem de parametros para o programa
		if(mensagem == "OK"){

			if(Parameters::getInstance().PRINT_PARAMETERS)
				Parameters::getInstance().printAll();

			//Se o primeiro parametro for um arquivo com o nome de todas as instancias
			// if the first parameter is a file with the name of all instances
			if(!strcmp(argv[1], "-all")){
				extractInstancesNames(argv[2]);
			//Senao testa-se apenas a instancia que foi passada como parametro
			} else{
				string instanceName(argv[1]);				
				instances.push_back(instanceName);

				if(Parameters::getInstance().STOP_CRITERION == FIND_TARGET){
					int target = atoi(argv[2]);							
					int timeLimit = atoi(argv[3]);
					targets.push_back(target);
					timeLimits.push_back(timeLimit);
				}				
			}
			execute();
//			testAlpha();
//			testConstructives();
		} else{
			cout << mensagem << endl;
			help();
		}
	}
}

/**
 * Compara os tempos dos algoritmos de Kruskal e Prim
 */
void compareKruskalPrim(int argc, char *argv[]){
	extractInstancesNames(argv[1]);

	for(unsigned int inst=0; inst < instances.size(); inst++){
		Instance instance(instances[inst]);
		Graph::createInstance(instance);
		Graph::graph.reductionTests();
		Graph::graph.sortEdges();

		vector<int> vertices(Graph::graph.numberOfClusters);
		for(unsigned int cluster = 0; cluster < Graph::graph.numberOfClusters; cluster++){
			int numVertices = Graph::graph.clusters[cluster].size();
			vertices[cluster] = Graph::graph.clusters[cluster][random()%numVertices];
		}

		double startTime1 = cpuTime();
		//Calculando 100 MSTs com Prim
		for(int i=0; i < 100; i++){
			Heuristic5::getMSTCost(vertices);
		}
		double finalTime1 = cpuTime() - startTime1;

		double startTime2 = cpuTime();
		//Calculando 100 MSTs com Kruskal
		for(int i=0; i < 100; i++){
			Heuristic5::getMSTCostByKruskal(vertices);
		}
		double finalTime2 = cpuTime() - startTime2;

		cout << instances[inst] << " " << Graph::graph.numberOfVertices<< " " << Graph::graph.edges.size() << " " << Graph::graph.edges.size()/Graph::graph.numberOfVertices << " " << finalTime1 << " " << finalTime2 << endl;
	}
}

/**
 * Verifica quais instancias sao euclideanas
 */
void verifyEuclideanInstances(int argc, char *argv[]){

	extractInstancesNames(argv[1]);
	
	for(unsigned int inst=0; inst < instances.size(); inst++){
		Instance instance(instances[inst]);
		Graph::createInstance(instance);
	
		bool euclidean = true;
		unsigned int i=0;
		while((i < Graph::graph.numberOfVertices) && (euclidean)){
			unsigned int j=i+1;
			while((j < Graph::graph.numberOfVertices) && (euclidean)){
	
				if(Graph::graph.exists[i][j]){
					int cost1 = Graph::graph.adjacency[i][j];
					//Verifica a desigualdade triangular para cada trio de vértices
					unsigned int k=j+1;
					while((k < Graph::graph.numberOfVertices) && (euclidean)){
	
						if((Graph::graph.exists[i][k]) && (Graph::graph.exists[j][k])){
							int cost2 = Graph::graph.adjacency[j][k];
							int cost3 = Graph::graph.adjacency[i][k];
							if((cost1-1 > cost2 + cost3) || (cost2-1 > cost1 + cost3) || (cost3-1 > cost2 + cost1)){
								euclidean = false;
								//cout << "Nao eh euclideana por causa dos vertices " << i << ", " << j <<" e " <<  k << endl;
							}
						}
						k++;
					}
				}
				j++;
			}
			i++;
		}
	
		if(euclidean)
			cout << instances[inst] << endl;
	}
}
/**
 * Calcula e imprime o custo da solução passada como parametro
 */
void printSolutionCost(int argc, char *argv[]){

	string instanceName(argv[1]);
	Instance instance(instanceName);
	Graph::createInstance(instance);
	Graph::graph.reductionTests();
	Graph::graph.sortEdges();

	cout << "Solucao:" << endl;
	vector<int> solution(Graph::graph.numberOfClusters);
	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){
		solution[i] = atoi(argv[i+2]);
		cout << solution[i] << " ";
	}

	int cost = Heuristic5::getMSTCost(solution);
	cout << endl << "Custo: " << cost << endl;
}

/**
* Tudo comeca aqui!
*/
int main(int argc, char *argv[]){	
	
	runApplication(argc, argv);
	
	//printSolutionCost(argc, argv);

	return EXIT_SUCCESS;
}
