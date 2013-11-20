/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 09 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H

#include "graph.h"
#include "heuristic1.h"
#include "heuristic2.h"
#include "heuristic5.h"
#include "heuristic6.h"
#include "heuristic7.h"
#include "heuristic9.h"
#include "parameters.h"

using namespace std;

/**
@author Cristiane Maria Santos Ferreira
*/
class Constructor{

public:    

	static vector<int> constructSolution(int heuristic);
};

#endif
