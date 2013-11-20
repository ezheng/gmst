/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *  Esta heuristica seleciona um vertice de cada cluster e cria uma MST    *
 *  com os vertices selecionados                                           *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC7_H
#define HEURISTIC7_H

#include <vector>

#include "graph.h"
#include "heap.h"
#include "parameters.h"

using namespace std;

class Heuristic7{

public:
    Heuristic7();

    ~Heuristic7();

	/**
	* Metodo que calcula a avore
	*/
	static vector<int> calculateTree();

	/**
	* Versao randomizada do metodo que calcula a arvore
	*/
	static vector<int> calculateRandomTree();

private:

	/**
	 * Define uma ordem em que os grupos serão visitados
	 */
	static vector<int> defineOrder();
};

#endif
