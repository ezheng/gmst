/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 07 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "instance.h"
#include <sstream>

std::string INSTANCES_DIR;

Instance::Instance(string fileName){

	name = fileName;
	numberOfClusters = 0;
	//extractData(fileName);
	std::cout<< "error" << std::endl;
	//exit(0);
}

Instance::Instance(string fileName, int timeLimit){
	name = fileName;
	numberOfClusters = 0;
	extractData(fileName, timeLimit);

}

Instance::Instance(int *verticesData, size_t numberOfVertices, int *edgeData, size_t numberOfEdges)
{
	numberOfClusters = 0;
	vertices.resize(numberOfVertices);

	for(int i = 0; i < 3; i++)
		edges[i].resize(numberOfEdges);

	for(size_t i = 0; i < numberOfVertices; i++)
	{
		vertices[i] = verticesData[i] - 1;
		if(verticesData[i] > numberOfClusters)
			numberOfClusters = verticesData[i];
	}

	for(size_t i = 0; i < numberOfEdges; i++)
	{
		edges[VERTEX1][i] = edgeData[i];
		edges[VERTEX2][i] = edgeData[i + numberOfEdges];
		edges[COST][i] = edgeData[i + 2 * numberOfEdges];
	}

}


Instance::~Instance(){}

/**
* Extrai os dados do arquivo 'fileName' e constroi os atributos da instância 'instance'
*/

void Instance::extractData(string fileName, int timeLimit){

	ostringstream oss_enliang;
	oss_enliang << INSTANCES_DIR << "_time"<<timeLimit<<"/";
	//string path(INSTANCES_DIR + fileName);
	string path(oss_enliang.str() + fileName);
	FILE* f = fopen(path.c_str(), "r");		

	if(!f){

		//cout << "Erro na leitura do arquivo \"" << fileName << "\"" << endl;
		cout << "Error reading file \"" << fileName << "\"" << endl;
	} else {

		int value;
		fscanf (f,"%d",&value);
	
		//Lendo cada cluster referente a cada vertice
		// reading each cluster for each vertex
		while((!feof(f)) && (value != 0)){
			
			if(value > numberOfClusters)
				numberOfClusters = value;
			vertices.push_back(value-1);					
			fscanf (f,"%d",&value);
		}		
		
		//Se o arquivo ja acabou, o formato estava incorreto!
		//If the file already over, the format was incorrect!
		if(feof(f)){
			//cout << "Arquivo \"" << fileName << "\" em formato invalido!" << endl;
			cout << "file \"" << fileName << "\" invalid format! " << endl;
		} else {

			//Lendo os dados sobre as arestas do grafo
			//Reading the data on the graph edges
			while(!feof(f)){
	
				edges[VERTEX1].push_back(value);
	
				fscanf(f,"%d",&value);
				edges[VERTEX2].push_back(value);
	
				fscanf (f,"%d",&value);
				edges[COST].push_back(value);
	
				fscanf (f,"%d",&value);	
			}
		}
	}
	fclose(f);
}

/**
* Retorna o conjunto de vertices da instancia, com seus respectivos clusters
*/
vector<int> Instance::getVertices(){
	return vertices;
}

/**
* Retorna as arestas do grafo (Somente a informacao dos vertices de saida)
*/
vector<int> Instance::getVertices1(){
	return edges[VERTEX1];
}

/**
* Retorna as arestas do grafo (Somente a informacao dos vertices de entrada)
*/
vector<int> Instance::getVertices2(){
	return edges[VERTEX2];
}

/**
* Retorna as arestas do grafo (Somente a informacao dos pesos dos vertices)
*/
vector<int> Instance::getEdgesCost(){
	return edges[COST];
}

/**
* Retorna o numero de arestas do grafo
*/
int Instance::getNumberOfEdges(){
	return edges[VERTEX1].size();
}

/**
* Retorna o numero de vertices do grafo
*/
int Instance::getNumberOfVertices(){
	return vertices.size();
}

/**
* Retorna o numero de clusters
*/
int Instance::getNumberOfClusters(){
	return numberOfClusters;
}
