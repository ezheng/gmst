/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC2_H
#define HEURISTIC2_H

#include <vector>

#include "graph.h"
#include "setunion.h"
#include "heuristic5.h"
#include "parameters.h"

/**
@author Cristiane Maria Santos FerreirA
*/
class Heuristic2{
public:
    Heuristic2();

    ~Heuristic2();

	/**
	 * Constrói solução de maneira gulosa
	 */
	static vector<int> calculateTree();

	/**
	 * Constrói solução de maneira aleatória
	 */
	static vector<int> calculateRandomTree();
};

#endif
