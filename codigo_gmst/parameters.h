/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>

#define HEURISTIC1 0
#define HEURISTIC2 1
#define HEURISTIC3 2
#define HEURISTIC4 3
#define RANDOMIZED_HEURISTIC1 4
#define RANDOMIZED_HEURISTIC2 5
#define HEURISTIC5 6
#define HEURISTIC6 7
#define RANDOMIZED_HEURISTIC6 8
#define HEURISTIC7 9
#define RANDOMIZED_HEURISTIC7 10
#define HEURISTIC8 11
#define HEURISTIC9 12
#define RANDOMIZED_HEURISTIC9 13

#define METHOD1 1
#define METHOD2 2
#define METHOD3 3
#define METHOD4 4
#define METHOD5 5
#define METHOD6 6
#define METHOD7 7

#define FIND_TARGET 0
#define ITERATIONS 1
#define ILS_APPLICATION 2

#define INFINITE INT_MAX
#define NONE -1

using namespace std;

/**
* Classe que define os parametros da aplicação 
*/
class Parameters{
public:
	
	/**
	* Parametro utilizado pelas heuristicas construtivas GRASP
	*/
	float ALPHA;
	static void setAlpha(float alpha);

	/**
	* Heuristica construtiva default
	*/
	int HEURISTIC;
	static void setHeuristic(int heuristic);

	/**
	* Numero maximo de iteracoes do GRASP
	*/
	int MAX_ITERATIONS_GRASP;
	static void setMaxIterationsGRASP(int maxIterations);

	/**
	* Define o numero de grupos em que os clusters do grafo serao divididos na heuristica 9
	*/
	int NUMBER_OF_CENTERS;	
	static void setNumberOfCenters(int numberOfCenters);

	/**
	* Define o tamanho do conjunto elite
	*/
	int ELITE_SET_SIZE;
	static void setEliteSetSize(int eliteSetSize);
	static void incrementEliteSetSize();
	static void decrementEliteSetSize();

	/**
	* Define a iteracao em que o Path Relinking vai comecar a ser feito
	*/
	int PATH_RELINKING_ITER;
	static void setPathRelinkingIter(int pathRelinkingIter);

	/**
	* Define o numero de vezes o algoritmo vai executar para cada instancia
	*/
	int TIMES;
	static void setTimes(int times);

	/**
	* Define a semente que sera usada para calcular os numeros aleatorios
	*/
	int SEED;
	static void setSeed(int seed);

	/**
	* Define o quao pior uma solucao pode ser da melhor solucao encontrada, para que seja feito um path relinking com ela
	*/
	float ELITE_SOLUTION_LIMIT;
	static void setEliteSolutionLimit(float limit);

	/**
	* Define se devem as informacoes auxiliares devem ser impressas
	*/
	bool VERBOSE;
	static void setVerbose();

	/**
	* Define se os parametros devem ser impressos
	*/
	bool PRINT_PARAMETERS;
	static void setPrintParameters();

	/**
	* Define se a solucao final deve ser impressa
	*/
	bool PRINT_SOLUTION;
	static void setPrintSolution();

	/**
	* Define o criterio de parada do algoritmo, se por numero de iteracoes ou por alvo
	*/
	int STOP_CRITERION;
	static void setStopCriterion(int stopCriterion);

	/**
	 * Define a distância mínima entre dois clusters para que estes sejam considerados vizihos
	 */
	float NEIGHBOR_DISTANCE;
	static void setNeighborDistance(float neighborDistance);

	/**
	 * Define o número máximo permitido de iterações sem atualização do conjunto elite. Caso esse número seja atingido, o conjunto elite é resetado.
	 */
	int MAX_NO_UPDATE_ITER;
	static void setMaxNoUpdateIter(int maxNoUpdateIter);
	
	/**
	 * Define se será utilizado o ILS
	 */
	bool ILS;
	static void setIls(bool ils);

	/**
	 * Define se será utilizado o VNS
	*/
	bool VNS;
	static void setVns(bool vns);

	/**
	 * Define a distancia minima entre os elementos do conjunto elite
	 */
	int MIN_ELITE_DISTANCE;
	static void setMinEliteDistance(int minEliteDistance);

	/**
	 * Define o alvo fácil
	 */
	int EASY_TARGET;
	static void setEasyTarget(int easyTarget);
	
	static Parameters getInstance();

    Parameters();

    ~Parameters();

	/**
	* Imprime todos os parametros
	*/
	void printAll();

	/**
	* Imprime apenas os parametros mais importantes
	*/
	void print();
private:
	static Parameters parameters;

};

#endif
