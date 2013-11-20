/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 03 de Junho de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TABU_H
#define TABU_H

#include <string>
#include <ctime>
#include <sstream>
#include <vector>
#include <cmath>

#include "graph.h"
#include "heuristic5.h"
#include "parameters.h"
#include "util.h"

using namespace std;

class Tabu{
private:

	/**
	* Tempo de execução em foi encontrada a melhor solucao
	*/
	double bestSolutionTime;

	/**
	* Tempo em que o algoritmo comecou a executar
	*/
	double startTime;

	/**
	 * Armazena a solucao corrente
	 */
	vector<int> solution;

	/**
	* Armazena o custo da melhor solução
	*/
	unsigned int bestCost;

	/**
	* Armazena a iteracao corrente da busca tabu (lambda)
	*/
	unsigned int iteration;

	/**
	* Armazena o numero maximo de iteracoes do algoritmo (eta)
	*/
	unsigned int maxIterations;

	/**
	* Armazena o número de iteracoes sem melhora em que o algoritmo está na vizinhaca corrente
	*/
	unsigned int iterInNeighborhood;

	/**
	* Armazena o numero de iteracoes em que um vertice ficará como tabu (teta)
	*/
	int tabuTenure;

	/**
	* Armazena o numero de vezes que cada vertice entrou na solução (alpha)
	*/
	vector<unsigned int> verticesFrequencies;

	/**
	* Armazena até que iteração cada vértice será considerado tabu (beta)
	*/
	vector<unsigned int> tabuTillIteration;

	/**
	* Armazena o 'aspiration level' de cada vértice (gamma)
	*/
	vector<unsigned int> aspirationLevel;

	/**
	* Armazena o número de vezes em que cada cluster foi selecionado durante a busca local (delta)
	*/
	vector<int> clustersFrequencies;

	/**
	* Armazena o número máximo de iterações em que os clusters serão selecionados de maneira aleatória durante a busca local (phi2)
	*/
	int maxIterationsForRandomSelection;

	/**
	* Armazena o numero de iteracoes em que os clustes vêm sendo selecionados de maneira aleatoria
	*/
	int iterationsForRandomSelection;

	/**
	* Fator utilizado na função que computa o custo de cada solução vizinha, para ajustar a intensidade da diversificação (phi)
	*/
	float factor;	

	/**
	* Armazena o cluster mais selecionado durante a busca local
	*/
	int mostFrequentCluster;

	/**
	* Armazena o cluster menos selecionado durante a busca local
	*/
	int leastFrequentCluster;

	/**
	* Armazena a iteração em que foi encontrada a melhor solução
	*/
	unsigned int bestIteration;

	/**
	* Armazena o tamanho da estrutura de vizinhança utilizada na iteração corrente
	*/
	unsigned int neighborhood;

	/**
	* Armazena quantas iterações são permitidas para cada estrutura de vizinhança da busca local.
	* Ex.: Para a vizinhança c = 1, são permitidas 50 iterações sem melhora. Logo, noImprovementIterations[c-1] = 50.
	*/
	vector<unsigned int> noImprovementIterations;

	/**
	* Método que executa a busca tabu
	*/
	unsigned int runTabuSearch();

	/**
	* Método que inicializa as estruturas de dados para a busca tabu
	*/
	void initialize();

	/**
	* Método que atualiza os parametros do algoritmo
	*/
	void updateParameters(unsigned int cost, vector<int> &oldSolution);

	/**
	* Método que executa a busca local, considerando a estrutura de vizinhança c=1,2,3 conforme o caso
	 * 
	 * @param currCost custo da solução corrente
	 * 
	 * @return custo da solução retornada pela busca local
	*/
	int applySearch(unsigned int currCost);

	/**
	 * Método que implementa a busca local com vizinhança c = 1
	 * 
	 * @param clusters clusters selecionados para estabelecer a vizinhanca da solucao corrente
	 * @param currCost custo da solucao corrente
	 * @return custo da solucao vizinha, agora armazenada em 'solution'
	 */
	int applySearch1(vector<int> &clusters, unsigned int currCost);

	/**
	 * Método que implementa a busca local com vizinhança c = 2
	 * 
	 * @param clusters os dois clusters selecionados para estabelecer a vizinhanca da solucao corrente
	 * @param currCost custo da solucao corrente
	 * @return custo da solucao vizinha, agora armazenada em 'solution'
	 */
	int applySearch2(vector<int> &clusters, unsigned int currCost);

	/**
	 * Método que implementa a busca local com vizinhança c =3  
	 * 
	 * @param clusters clusters selecionados para estabelecer a vizinhanca da solucao corrente
	 * @param currCost custo da solucao corrente
	 * @return custo da solucao vizinha, agora armazenada em 'solution'
	 */
	int applySearch3(vector<int> &clusters, unsigned int currCost);

	/**
	 * Metodo que seleciona os clusters que serão utilizados em um movimento da busca local
	 * A seleção pode ser feita de duas formas: proporcional às frequencias com que os clusters já foram selecionados
	 * anteriormente, ou completamente aleatória. 
	 * 
	 * @return clusters que foram selecionados
	 */
	vector<int> selectClusters();

	/**
	* Método que sorteia um valor para o tabu tenure
	*/ 
	int sortTabuTenure();

	/**
	 * Metodo que computa o custo da solucao adicionando a penalizacao pela frequencia dos clusters
	 * 
	 * @param currCost custo da solucao
	 * @param oldCost custo da solucao anterior da busca local
	 * @param clusters array com os clusters que estão sendo visitados na iteração corrente da busca local
	 * 
	 * @return custo da solução com a penalização
 	*/
	double computeCost(unsigned int currCost, unsigned int oldCost, vector<int> clusters);

	/**
	 * Metodo que imprime a solucao
	 */
	void printSolution();
public:

	/**
	* Construtor
	*/
    Tabu();

    ~Tabu();

	/**
	* Metodo chamado para executar o algoritmo
	* Executa o algoritmo de busca tabu por TIMES vezes, sendo TIMES um parametro do programa
	*
	* Retorna: uma string contendo o custo médio das soluções encontradas, o melhor custo, o tempo médio de execução e tempo
	* médio em que o algoritmo encontrou a melhor solução. Todos os valores separados por espaço.
	*/
	string execute();

	/**
	 * Armazena a melhor solucao
	*/
	vector<int> bestSolution;
	

};

#endif
