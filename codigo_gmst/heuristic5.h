/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 03 de Abril de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEURISTIC5_H
#define HEURISTIC5_H

#include <sstream>
#include <vector>
#include <list>

#include "graph.h"
#include "heap.h"
#include "setunion.h"

/** 
* 
* Esta heuristica constroi uma solucao montando uma arvore que expanda todos os clusters
* do grafo de forma aleatoria
*
*/
class Heuristic5{
public:
    Heuristic5();

    ~Heuristic5();

	/**
	* Metodo que calcula a arvore 
	*/
	static vector<int> calculateTree();

	/**
	* Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
	* Retorna um vetor contendo o nó pai de cada nó na MST, para a raiz da árvore o valor será -1
	* Utiliza o algoritmo de PRIM
	*/
	static vector<int> calculateMST(vector<int> &vertices);

	/**
	* Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
	* Utiliza o algoritmo de PRIM
	* Retorna o custo da árvore gerada
	*/
	static int getMSTCost(vector<int> &vertices);

	/**
	 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
	 * Retorna um vetor contendo o índice de cada aresta que faz parte da MST
	 * Utiliza o algoritmo de KRUSKAL
	 */
	static vector<int> calculateMSTByKruskal(vector<int> &vertices);

	/**
	 * Metodo que calcula uma MST com o grafo passado, utilizando apenas os vertices do vetor 'vertices'
	 * Utiliza o algoritmo de KRUSKAL
	 * Retorna o custo da árvore gerada
	 */
	static int getMSTCostByKruskal(vector<int> &vertices);
	/**
	* Método que verifica se o array passado é uma arvore
	*/
	static bool checkMST(vector<int> &tree, vector<int> &solution);

	/**
	 * Checa se os vertices passados realmente formam uma MST com o custo passado
	 */
	static void checkMST(vector<int> &solution, int cost);

	/**
	 * Imprime os arcos da MST gerada com os vertices presentes em 'solucao' a partir do cluster 'rootCluster' como raiz no formato de uma solucao do problema de steiner
	 * Retorna um stream para as arestas que foram impressas
	 */
	static string printSteinerMST(vector<int> solution, int rootCluster);
};

#endif
