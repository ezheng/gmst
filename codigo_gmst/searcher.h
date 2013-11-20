/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 14 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SEARCHER_H
#define SEARCHER_H

#include "graph.h"
#include "heuristic5.h"
#include "heuristic6.h"
#include "parameters.h"

#include <vector>
#include <iostream>

using namespace std;

/**
@author Cristiane Maria Santos Ferreira
*/
class Searcher{
public:
	/**
	* Metodo que implementa a busca local de Golden
	*/
	static int localSearch(vector<int> &solution);

	/**
	 * Metodo que implementa a busca local de Golden com VNS
	 */
	static int localSearchVNS(vector<int> &solution);
	
	/**
	 * Metodo que implementa a busca local ILS
	 */
	static int ils(vector<int> &solution, int cost);
private:

	/**
 	* Método que seleciona 'c' clusters para a busca local
 	* A selecao é feita com base na MST gerada a partir da solucao passada
	 */
	static vector<int> selectClusters(vector<int> &solution, int c, int cluster);
			
	/**
 	* Metodo que troca o vertice de saida dos clusters passado
	*/
	static int chooseBestVertex(vector<int> &solution, int cost, vector<int> &clusters);
			
	/**
	 * Metodo que troca o vertice de saida do cluster passado
	 */
	static int chooseBestVertex(vector<int> &solution, int cost, int cluster);

	/**
	 * Metodo que troca o vertice de saida dos dois clusters passados
	 */
	static int chooseBestVertex(vector<int> &solution, int cost, int cluster1, int cluster2);
};

#endif
