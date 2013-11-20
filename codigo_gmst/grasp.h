/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 14 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>

#include "constructor.h"
#include "eliteset.h"
#include "parameters.h"
#include "searcher.h"
#include "util.h"

using namespace std;

class GRASP{
public:
    GRASP();

    ~GRASP();

	/**
	* Armazena o conjunto de solucoes elite
	*/
	EliteSet eliteSet;

	/**
	* GRASP com os parametros default
	*/
	int calculateTree();
	
	/**
	* Metodo usado para testar o algoritmo
	*/
	string execute(string instanceName, int timeLimit);

private:

	/**
	 * Armazena a informacao se ja foi aplicado o ils
	 */
	int ilsApplication;
	
	/**
	* Armazena a iteracao corrente do GRASP
	*/	
	int iteration; 

	/**
	* Armazena o numero maximo de iteracoes GRASP
	*/
	int maxIter;

	/**
	* Define a heuristica construtiva a ser utilizada
	*/
	int constructiveHeuristic;

	/**
	* Tempo em que o algoritmo comecou a executar
	*/
	double startTime;

	/**
	 * Tempo em que o algoritmo encontrou o alvo fácil
	 */
	double easyTargetTime;
	
	/**
	* Metodo que implementa o path relinking entre as solucoes 'solution1' e 'solution2'
	* A solucao base será a de menor custo, e a outra, obviamente sera a solucao alvo
	* Retorna true caso o metodo tenha obtido soluções que entraram no conjunto elite
	*/
	bool pathRelinking(vector<int> base, int baseCost, vector<int> guide, int guideCost);	
			
	/**
	* Armazena o indice das heuristicas que serao utilizadas pelo GRASP adaptativo
	*/
	vector<int> heuristics;

	vector<int> iterations;

	vector<int> totalIterations;
	/**
	* Armazena o indice das heuristicas gulosas que serao utilizadas pelo GRASP adaptativo
	*/
	vector<int> greedHeuristics;

	/**
	* Metodo que faz um movimento do path relinking no sentido da solucao 'base' para a solucao 'target'
	* O movimento é feito trocando-se um vertice da solucao base por um vertice da solucao alvo
	* Retorna o custo da solucao gerada desse movimento
	*/
	int makeVertexMovement(vector<int> &base, vector<int> &guide);

	/**
	 * Metodo que faz um movimento do path relinking no sentido da solucao 'base' para a solucao 'target'
	 * O movimento é feito trocando-se uma aresta da solucao base por uma aresta da solucao alvo
	 * Retorna o custo da solucao gerada desse movimento
		 */
	int makeEdgeMovement(vector<int> &base, vector<int> &guide);
	
	/**
	* Metodo que faz o path relinking da solucao passada como parametro para cada um dos membros do conjunto elite
	*/
	bool completePathRelinking(vector<int> solution, int cost);

	/**
	* Faz o pre-processamento necessario para a execucao de um teste GRASP
	* (Inicializacao de variaveis, definicao de parametros, etc.)
	*/
	void initialize();

	/**
	* Metodo que executa o teste adaptativo, ou seja, com uma fase de treinamento para escolher a melhor configuracao do GRASP
	*/
	int adaptiveCalculateTree();

	/**
	 * Aplica uma busca local ILS em cada membro do conjunto elite
	 * @return true caso alguma atualização tenha sido feito, false caso contrário
	 */
	bool eliteSetILS();

	/**
	 * Aplica todas as atualizações ao conjunto elite
	 */
	bool updateParameters(vector<int> &solution, int cost);

	/**
	 * Verifica se ja foi atingido o criterio de parada
	 */
	bool stopCriterionReached();
};

#endif
