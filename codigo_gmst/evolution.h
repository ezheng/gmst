/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <vector>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>

#include "constructor.h"
#include "eliteset.h"
#include "parameters.h"
#include "population.h"
#include "searcher.h"
#include "util.h"

/**
 * Classe que implementa o algoritmo genético
 * 
	@author Cristiane Maria Santos FerreirA <cferreira@ic.uff.br>
*/
class Evolution{

public:
    Evolution();

    ~Evolution();

	/**
	 * Armazena a população
	 */
	Population population;

	/**
	 * GRASP com os parametros default
	 */
	int calculateTree();
	
	/**
	 * Metodo usado para testar o algoritmo
	 */
	string execute();

private:

	/**
	 * Armazena a iteracao corrente do GRASP
	 */
	int generation;

	/**
	 * Armazena o numero maximo de iteracoes GRASP
	 */
	int maxGenerations;

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
	 * Armazena o indice das heuristicas que serao utilizadas pelo GRASP adaptativo
	 */
	vector<int> heuristics;

	/**
	 * Inicializacao de variaveis, definicao de parametros, etc.
	 */
	void initialize();

	/**
	 * Constroi a populacao
	 */	
	void constructPopulation();
	
	/**
	 * Método que realiza o cruzamento para a próxima geração
 	*/
	void crossover();

	/**
	 * Método que implementa o operador de mutação
	 */
	void mutation();

	/**
	 * Método que aplica a busca local sobre toda a população
	 */
	void localSearch();
			
	/**
	 * Verifica se ja foi atingido o criterio de parada
	 */
	bool stopCriterionReached();
};

#endif
