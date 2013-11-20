/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 30 de Maio de 2006                                          *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 * Classe que gera instâncias completas e euclideanas                      *
 *                                                                         *
 ***************************************************************************/
#ifndef INSTANCEGENERATOR_H
#define INSTANCEGENERATOR_H

#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits.h>
#include <cstdlib>

#define MAX_X 1000
#define MAX_Y 1000

#define INFINITE INT_MAX
using namespace std;

class InstanceGenerator{

public:

	/**
	* Metodo que gera uma instancia com as características passadas
	*/
	static void generateInstance(unsigned int numVertices, unsigned int numClusters);

	/**
	* Metodo que gera todas as instancias
	*/
	static void generateAll();

	/**
	* Metodo que gera um arquivo para cada cluster da instancia passada
	*/
	static void generateClusterFiles(string instance, string cooFile, unsigned int numVertices, unsigned int numClusters);

private:
	
	/**
	* Imprime a instancia nos arquivos de saida
	*/
	static void printToFiles(vector<unsigned int> posX, vector<unsigned int> posY, vector<unsigned int> vertices, vector<unsigned int> edges1, vector<unsigned int> edges2, vector<unsigned int> edgesCosts, unsigned int numVertices, unsigned int numClusters);

	/**
	* Imprime a instancia em um arquivo com suas coordenadas
	*/
	static string printToCooFile(vector<unsigned int> posX, vector<unsigned int> posY, unsigned int numVertices, unsigned int numClusters);

	/**
	* Imprime a instancia em um arquivo no formato  que sera utilizado pelo programa
	*/
	static string printToCluFile(vector<unsigned int> vertices, vector<unsigned int> edges1, vector<unsigned int> edges2, vector<unsigned int> edgesCosts, unsigned int numVertices, unsigned int numClusters);

	/**
	* Metodo que faz o agrupamento dos vertices, criando os clusters
	*/
	static vector<unsigned int> generateClusters(vector<unsigned int> posX, vector<unsigned int> posY, unsigned int numberOfClusters);

	/**
	* Escolhe o centro mais proximo do vertice passado
	*/
	static unsigned int closestCenter(unsigned int vertex, vector<unsigned int> posX, vector<unsigned int> posY, vector<unsigned int> centers);

	/**
	* Procura o vertice mais distance das coordenadas passadas
	*/
	static unsigned int farthestVertex(float positionX, float positionY, vector<unsigned int> posX, vector<unsigned int> posY, vector<bool> flag);
};

#endif
