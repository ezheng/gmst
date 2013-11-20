/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ELITESET_H
#define ELITESET_H

#include "graph.h"
#include "parameters.h"
#include "searcher.h"

#include <list>
/**
@author Cristiane Maria Santos Ferreira
*/
class EliteSet{
public:
    EliteSet();

    ~EliteSet();

	/**
	* Procura a solucao no conjunto elite, se nao encontrar insere no conjunto elite em ordem decrescente de custo
	* Se o conjunto elite ainda nao estiver completo, insere a nova solucao, caso contrário, a solucao só é inserida
	* se for melhor que alguma outra solucao elite. Nesse ultimo caso, a pior solucao é descartada.
	* Retorna 'true' caso a atualizacao tenha sido feita
	*/
	bool update(vector<int> &solution, int cost, int iteration);

	/**
	* Apaga todas as solucoes do conjunto elite
	*/
	void clear();

	list< vector<int> >::iterator begin();

	/**
	* Retorna a melhor solucao do conjunto elite
	*/
	vector<int> bestSolution();	
	
	/**
	 * Retorna o custo da melhor solucao
	 */
	int bestCost();

	/**
	 * Custo médio das soluções que foram avaliadas pelo conjunto elite
	 */
	int averageCost;
	
	/**
	* Verifica se a solucao passada pode ser considerada uma 'boa solucao' 
	* Para uma arvore ser uma 'boa solucao', seu custo nao pode muito maior que a melhor solucao do conjunto elite
	* Para isso, utiliza-se o parametro ELITE_SOLUTION_LIMIT
	*/
	bool goodSolution(int cost);

	/**
	* Metodo que seleciona uma das solucoes do conjunto elite para realizar o Path Relinking
	* Escolhe a solucao mais distante de 'tree'
	*/
	int chooseASolution(const vector<int> &solution, vector<int> &choosen);

	/**
	* Metodo que imprime os custos das solucoes do conjunto elite
	*/
	void print();

	/**
	 * Metodo que imprime as solucoes do conjunto elite
	 */
	void printSolutions();
			
	/**
	* Retorna a iteracao em que foi encontrada a melhor solucao
	*/
	int getBestIteration();

	/**
	 * Armazena a ultima iteracao em que o conjunto elite foi atualizado
	 */
	int lastUpdate;
	/**
	* Armazena as solucoes do conjunto elite em ordem descrescente de custo (ou seja, as melhores solucoes estao no final da lista)
	*/
	list< vector<int> > solutionSet;
	list< int > costSet;

	/**
	 * Calcula a distancia entre duas solucoes
	 * 
	 * @param solution1 uma das solucoes de que se deseja obter a distancia
	 * @param solution2 uma das solucoes de que se deseja obter a distancia
	 * 
	 * @return distancia entre as duas solucoes
	*/
	static int computeDistance(const vector<int> &solution1, const vector<int> &solution2);

	/**
	 * @return vertices de cada cluster que aparecem no conjunto elite
	 */
	vector< vector<int> > returnVertices();

	/**
	 * Remove todos os elementos do conjunto elite, exceto o último (ou seja, o melhor)
	 */
	void reset();
private:

	/**
	* Armazena a iteracao em que a melhor solucao foi encontrada
	*/
	int bestIteration;
};

#endif
