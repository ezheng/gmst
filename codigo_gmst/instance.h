/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 07 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <fstream>

#define VERTEX1  0
#define VERTEX2  1
#define COST  2

//#define INSTANCES_DIR "/home/posgrad/cferreira/gmstInstances/"
//#define INSTANCES_DIR "../brook3"
//std::string INSTANCES_DIR "../brook3"
using namespace std;

/**
@author Cristiane Maria Santos Ferreira
*
* Esta classe eh uma instancia do problema
*
* FORMATO DO ARQUIVO:
*
* (GRUPO AO QUAL PERTENCE O VERTICE 1)
* (GRUPO AO QUAL PERTENCE O VERTICE 2)
* (GRUPO AO QUAL PERTENCE O VERTICE 3)
* ...
* (GRUPO AO QUAL PERTENCE O VERTICE N)
*
* (LINHA EM BRANCO)
*
* (VERTICE1 DA ARESTA 1) (VERTICE2 DA ARESTA 1) (PESO DA ARESTA 1)
* (VERTICE1 DA ARESTA 2) (VERTICE2 DA ARESTA 2) (PESO DA ARESTA 2)
* (VERTICE1 DA ARESTA 3) (VERTICE2 DA ARESTA 3) (PESO DA ARESTA 3)
* ...
* (VERTICE1 DA ARESTA M) (VERTICE2 DA ARESTA M) (PESO DA ARESTA M)
*/

class Instance{

public:
    Instance(string fileName);
    Instance(string fileName, int timeLimit);
//    Instance (const Instance & obj){
//       *this = obj;
//     }

//	Instance inst2 (inst1);

//    Instance (const Instance & obj)
//     : vertices (obj.vertices),
       
//    {}
    
    ~Instance();

	/**
	 * Nome da instancia
	 */
	string name;
	
	/**
	* Armazena o cluster em que cada vertice esta inserido
	*/
    vector<int> vertices;
	
    vector<int> edges[3];

	/**
	* Armazena o numero de clusters da instancia
	*/
    int numberOfClusters;

    /**
     * Extrai os dados do arquivo 'fileName' e constroi os atributos da instância 'instance'
     */
    void extractData(string fileName, int timeLimit);

    /**
     * Retorna o conjunto de vertices da instancia, com seus respectivos clusters
     */
    vector<int> getVertices();

    /**
     * Retorna as arestas do grafo (Somente a informacao dos vertices de saida)
     */
    vector<int> getVertices1();

    /**
     * Retorna as arestas do grafo (Somente a informacao dos vertices de entrada)
     */
    vector<int> getVertices2();

    /**
     * Retorna as arestas do grafo (Somente a informacao dos pesos dos vertices)
     */
    vector<int> getEdgesCost();

    /**
     * Retorna o numero de arestas do grafo
     */
    int getNumberOfEdges();

    /**
     * Retorna o numero de vertices do grafo
     */
    int getNumberOfVertices();

    /**
     * Retorna o numero de clusters
     */
    int getNumberOfClusters();

	/**
	* Imprime a instancia
	*/
	void print();

};
#endif
