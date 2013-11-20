/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 06 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef STDGRAPH_H
#define STDGRAPH_H

#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

#include "instance.h"
#include "limits.h"
#include "parameters.h"

#define PRIM true
#define KRUSKAL false

using namespace std;

/**
@author Cristiane Maria Santos Ferreira
* 
*
* Classe que representa um grafo
*/
class Graph{

public:

	Graph();

    ~Graph();

	/**
	 * Nome da instancia
	 */
	string instanceName;
	
	/**
	* Define o tempo limite para o GRASP executar
	*/
	double timeLimit;
		
	/**
	* Define a solucao alvo a ser encontrada pelo GRASP
	*/
	unsigned int target;

    /**
    * Armazena as arestas do grafo
    * edges[i][0] armazena o vertice de saida da aresta
    * edges[i][1] armazena o vertice de entrada da aresta
    * edges[i][2] armazena o peso da aresta
    */
    vector< vector<int> > edges;

	/**
	 * Armazena, para cada par de vértices, a informação se existe ou não uma aresta entre eles
	 */
	vector< vector<bool> > exists;

	/**
	 * Armazena, para cada vértice, a lista dos vértices adjacentes a ele
	 */
	//vector< list<int> > adjVertices;

	/**
	 * Armazena, para cada vértice, a lista dos clusters adjacentes a ele
	 */
	vector< list<int> > adjClusters;
	
    /**
    * Armazena as arestas do grafo, de forma que se possa procurar pelos vertices
    * vertexEdges[i][j] armazena o custo da aresta i-j. Se ela não existir, o valor será NONE
    */
    vector< vector<int> > adjacency;

    /**
    * Array com todos os vertices do grafo. O valor clusterOf[i] eh o indice do cluster que contem o vertice i
    */
    vector<int> clusterOf;

	/**
	* Seta a distancia entre cada par de clusters
	* minDistance[0][1] armazena a menor distância entre os clusters 1 e 2
	* maxDistance[0][1] armazena a maior distância entre os clusters 1 e 2
	*/
	vector< vector<int> > minDistances;
	vector< vector<int> > maxDistances;

    /**
    * Armazena o numero de clusters do grafo
    */
    unsigned int numberOfClusters;
	unsigned int numberOfVertices;

	bool bestMSTAlgorithm;
	
	/**
	* Armazena os vertices de cada cluster
	*/
    vector< vector<int> > clusters;

	/**
	 * Armazena, para cada cluster, os clusters que estão a uma distãncia mínima "ray"
	 */
	vector< vector<int> > neighbors;

	/**
	* Metodo que inclui as arestas do grafo
	*/
    void createEdgesArray(vector<int> edgesOut, vector<int> edgesIn, vector<int> edgesCost);

    /**
    * Metodo que ordena as arestas do grafo
    */
    void sortEdges();

    /**
    * Metodo que troca as arestas 'edge1' e 'edge2'
    */
    void changeEdges(int edge1, int edge2);
    
    /**
    * Seta uma aresta com indice 'index'
    */
    void setEdge(int index, int vertex1, int vertex2, int cost);
    
    /**
    * Metodo que seta as informações sobre os clusters
    */
	void createClusters(vector<int> _clusterOf);

	/**
	* Imprime o grafo
	*/
    void print();

    /**
     * Retorna todos as arestas saintes ou entrantes em 'vertex'
     */
    vector<int> edgesInVertex(int vertex);

    /**
     * Retorna o vértice da aresta diferente do vértice passado
     * Ex.: aresta 1:1->2  getNode(1,1) = 2, getNode(1,2) = 1
     */
    int getOpposite(int edge, int vertex);

    /**
    * Retorna todos os vertices pertencentes ao cluster passado
    */
    vector<int> getVerticesFromCluster(int cluster);

    /**
    * Seleciona aleatoriamente um vertice do cluster diferente do vertice passado
    */
    int chooseAVertexFromCluster(int cluster);

	/**
	* Seleciona aleatoriamente um vertice do cluster diferente do vertice passado
	*/
	int chooseAVertexFromCluster(int cluster, int vertex);

	/**
	* Metodo que imprime os vertices de cada cluster
	*/
	void printClusters();

	/**
	 * Retorna o vértice do cluster passado que possui a menor distancia para algum outro vértice do grafo
	 */
	int getBestVertex(int cluster);

	/**
	 * Retorna o vértice do cluster passado com menor distancia para o vértice 'vertex'
	 */
	int getClosestVertex(int cluster, int vertex);
		
	/**
	 * Verifica quais clusters sao rejeitados por quais
	 */
	void reductionTests();
	
	static Graph graph;

	static void createInstance(Instance& instance);
	static void createInstance(Instance& instance, int _target, double _timeLimit);

private:

	Graph(Instance& instance, int target, double timeLimit);
	
	/**
	* Implementa o método de ordenacao quick sort
	*/
	void quicksort(int begin, int end);

	/**
	* Metodo que inicializa as estruturas de dados do grafo
	*/
	void initializeStructures(Instance& instance);
};

#endif
