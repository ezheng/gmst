/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC1_H
#define HEURISTIC1_H

#include <vector>

#include "graph.h"
#include "heap.h"
#include "heuristic5.h"
#include "parameters.h"

using namespace std;

class Heuristic1{
public:
    Heuristic1();

    ~Heuristic1();

	/**
	 * Adaptacao de Prim gulosa 
	 */	
	static vector<int> calculateTree();

	/**
	 * Adaptacao de Prim gulosa aleatória
	 */
	static vector<int> calculateRandomTree();
			
};

#endif
