/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef POPULATION_H
#define POPULATION_H

#include "graph.h"
#include "parameters.h"
#include "searcher.h"

#include <list>

/**
	@author Cristiane Maria Santos Ferreira <cferreira@ic.uff.br>
*/
class Population{

public:
    Population();

    ~Population();

/**
* Procura a solucao no conjunto elite, se nao encontrar insere no conjunto elite em ordem decrescente de custo
* Se o conjunto elite ainda nao estiver completo, insere a nova solucao, caso contrário, a solucao só é inserida
* se for melhor que alguma outra solucao elite. Nesse ultimo caso, a pior solucao é descartada.
 * Retorna 'true' caso a atualizacao tenha sido feita
	 */
bool update(vector<int> &solution, int cost, int generation);

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
int getBestGeneration();

/**
 * Retorna o tamanho da população
 */
int size();

/**
 * Metodo que retorna a solução mais distante (em termos de custo) de 'solution'
 */
int chooseASolution(const vector<int> &solution, vector<int> &choosen);

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
 * Método que recalcula o custo de todas as soluções
 */
void recomputeCosts();

/**
 * Método que reconstrói a população, construindo novas soluções no lugar daquelas que foram removidas
 */
void rebuild();
private:

	/**
	 * Armazena a iteracao em que a melhor solucao foi encontrada
	 */
	int bestGeneration;
};

#endif
