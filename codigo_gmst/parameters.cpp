/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "parameters.h"

Parameters Parameters::parameters;

Parameters::Parameters(){

	/**
	* Parametro utilizado pelas heuristicas construtivas GRASP
	*/
	ALPHA = 0.25;
	
	/**
	* Heuristica construtiva default
	*/
	HEURISTIC = NONE;
	
	/**
	* Numero maximo de iteracoes do GRASP
	*/
	MAX_ITERATIONS_GRASP = 50;	
	
	/**
	* Define o numero de grupos em que os clusters do grafo serao divididos na heuristica 9
	*/
	NUMBER_OF_CENTERS = 7;

	/**
	* Define o tamanho do conjunto elite
	*/
	ELITE_SET_SIZE = 4;

	/**
	* Define a iteracao em que o Path Relinking vai comecar a ser feito
	*/
	PATH_RELINKING_ITER =20;

	/**
	* Define o numero de vezes o algoritmo vai executar para cada instancia
	*/
	TIMES = 1;	

	/**
	* Define a semente que sera usada para calcular os numeros aleatorios
	*/
	SEED = -1;

	/**
	* Define o quao pior uma solucao pode ser da melhor solucao encontrada, para que seja feito um path relinking com ela
	*/
	ELITE_SOLUTION_LIMIT = 1.01;

	/**
	* Define se devem as informacoes auxiliares devem ser impressas
	*/
	VERBOSE = false;

	/**
	* Define se os parametros devem ser impressos
	*/
	PRINT_PARAMETERS = false;

	/**
	* Define se a solucao final deve ser impressa
	*/
	PRINT_SOLUTION = false;	

	/**
	* Define o criterio de parada do algoritmo, se por numero de iteracoes ou por alvo
	*/
	STOP_CRITERION = FIND_TARGET;

	/**
	 * Define se será utilizado o ils
	 */
	ILS = true;

	/**
	 * Define se será utilizada a busca local VNS
	 */
	VNS = false;
	
	/**
	 * Define a distância mínima entre dois clusters para que estes sejam considerados vizihos
	 */
	NEIGHBOR_DISTANCE = 3.0;

	/**
	 * Define o número máximo permitido de iterações sem atualização do conjunto elite. Caso esse número seja atingido, o conjunto elite é resetado.
	 */
	MAX_NO_UPDATE_ITER = 50;

	/**
	 * Define a distancia minima entre os elementos do conjunto elite
	*/
	MIN_ELITE_DISTANCE = 10;

	/**
	 * Define o alvo fácil
	 */
	int EASY_TARGET = -1;
}

Parameters::~Parameters(){}

void Parameters::setAlpha(float alpha){
	parameters.ALPHA = alpha;
}

void Parameters::setHeuristic(int heuristic){
	parameters.HEURISTIC = heuristic;
}

void Parameters::setMaxIterationsGRASP(int maxIterations){
	parameters.MAX_ITERATIONS_GRASP = maxIterations;
}

	
void Parameters::setNumberOfCenters(int numberOfCenters){
	parameters.NUMBER_OF_CENTERS = numberOfCenters;
}

void Parameters::setEliteSetSize(int eliteSetSize){
	parameters.ELITE_SET_SIZE = eliteSetSize;
}
void Parameters::incrementEliteSetSize(){
	parameters.ELITE_SET_SIZE = min(parameters.ELITE_SET_SIZE+1, 10);
}
void Parameters::decrementEliteSetSize(){
	parameters.ELITE_SET_SIZE--;
}

void Parameters::setPathRelinkingIter(int pathRelinkingIter){
	parameters.PATH_RELINKING_ITER = pathRelinkingIter;
}

void Parameters::setTimes(int times){
	parameters.TIMES = times;
}

void Parameters::setSeed(int seed){
	parameters.SEED = seed;
}

void Parameters::setEliteSolutionLimit(float limit){
	parameters.ELITE_SOLUTION_LIMIT = limit;
}

void Parameters::setVerbose(){
	parameters.VERBOSE = true;
}

void Parameters::setPrintParameters(){
	parameters.PRINT_PARAMETERS = true;
}

void Parameters::setPrintSolution(){
	parameters.PRINT_SOLUTION = true;
}

void Parameters::setStopCriterion(int stopCriterion){
	parameters.STOP_CRITERION = stopCriterion;
}

void Parameters::setIls(bool ils){
	parameters.ILS = ils;
}

void Parameters::setVns(bool vns){
	parameters.VNS = vns;
}

void Parameters::setNeighborDistance(float neighborDistance){
	parameters.NEIGHBOR_DISTANCE = neighborDistance;
}

void Parameters::setMaxNoUpdateIter(int maxNoUpdateIter){
	parameters.MAX_NO_UPDATE_ITER = maxNoUpdateIter;
}

void Parameters::setMinEliteDistance(int minEliteDistance){
	parameters.MIN_ELITE_DISTANCE = minEliteDistance;
}

void Parameters::setEasyTarget(int easyTarget){
	parameters.EASY_TARGET = easyTarget;
}

Parameters Parameters::getInstance(){
	
	return parameters;
}

/**
* Imprime apenas os parametros mais importantes
*/
void Parameters::print(){
	
	cout << "-------------------------------" << endl;	
	cout << "|ALPHA = " << ALPHA << endl;
	cout << "|HEURISTIC = " << HEURISTIC << endl;
	cout << "|PATH_RELINKING_ITER = " << PATH_RELINKING_ITER << endl;
	cout << "-------------------------------" << endl << endl;
}

/**
* Imprime todos os parametros
*/
void Parameters::printAll(){
	
	cout << "-------------------------------" << endl;	
	cout << "|ALPHA = " << ALPHA << endl;	
	cout << "|ELITE_SET_SIZE = " << ELITE_SET_SIZE << endl;
	cout << "|ELITE_SOLUTION_LIMIT = " << ELITE_SOLUTION_LIMIT << endl;
	cout << "|HEURISTIC = " << HEURISTIC << endl;
	cout << "|MAX_ITERATIONS_GRASP = " << MAX_ITERATIONS_GRASP << endl;
	cout << "|NUMBER_OF_CENTERS = " << NUMBER_OF_CENTERS << endl;
	cout << "|PATH_RELINKING_ITER = " << PATH_RELINKING_ITER << endl;
	cout << "|SEED = " << SEED << endl;
	cout << "|STOP_CRITERION = " << STOP_CRITERION << endl;
	cout << "|TIMES = " << TIMES << endl;
	cout << "-------------------------------" << endl << endl;
}
