/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC6_H
#define HEURISTIC6_H

#include <vector>

#include "heuristic5.h"
#include "parameters.h"
#include "graph.h"
#include "setunion.h"

/**
@author Cristiane Maria Santos Ferreira
*/
class Heuristic6{
public:
    Heuristic6();

    ~Heuristic6();

	/**
	* Metodo que calcula a avore fazendo escolhas gulosas
	*/
	static vector<int> calculateTree();

	/**
	 * Metodo que gera um conjunto de arvores
	 */
	static vector<int> calculateTrees(vector<int> &indices, int numberOfSets);

	/**
	 * Calcula a arvore dado que alguns vertices ja estao fixos
	 */
	static int calculateTree(vector<int> &solution);

	/**
	 * Calcula a arvore fazendo escolhas "parcialmente" gulosas
	 */
	static vector<int> calculateRandomTree();
};

#endif
