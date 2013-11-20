/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 27 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC9_H
#define HEURISTIC9_H

#include <vector>

#include "graph.h"
#include "heuristic6.h"
#include "heuristic7.h"
#include "parameters.h"

/**
@author Cristiane Maria Santos Ferreira
*/
class Heuristic9{
private:

	/**
	* Metodo que cria os grupos de clusters de forma gulosa
	*/
	static vector<int> createClusters(int numberOfSets);

		/**
	 * Metodo que cria os grupos de clusters de forma aleatoria
		 */
	static vector<int> createRandomClusters(int numberOfSets);
	
	static void printClusters(vector< vector<int> > &clusters);	
	
public:
    Heuristic9();

    ~Heuristic9();

	/**
	* Método que calcula a arvore de cobertura utilizando a heuristica 6 calculando os cluster de forma gulosa
	*/
	static vector<int> calculateTree();

	/**
	 * Método que calcula a arvore de cobertura utilizando a heuristica 6 calculando os clusters de forma gulosa aleatoria
	 */
	static vector<int> calculateRandomTree();
};

#endif
