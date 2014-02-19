/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 27 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heuristic9.h"

Heuristic9::Heuristic9(){}

Heuristic9::~Heuristic9(){}

vector<int> Heuristic9::calculateTree(){

 	int factor = 2;
	int min = max((int) Graph::graph.numberOfClusters/Parameters::getInstance().NUMBER_OF_CENTERS - factor, 2);
	int numberOfSets = min + rand()%(2*factor+1);

	vector<int> clusters = createClusters(numberOfSets);
	return Heuristic6::calculateTrees(clusters, numberOfSets);
}

vector<int> Heuristic9::calculateRandomTree(){

 	int factor = 2;
	int min = max((int) Graph::graph.numberOfClusters/Parameters::getInstance().NUMBER_OF_CENTERS - factor, 2);
	int numberOfSets = min + rand()%(2*factor+1);

	vector<int> clusters = createRandomClusters(numberOfSets);
	return Heuristic6::calculateTrees(clusters, numberOfSets);
}

/**
* Metodo que cria os grupos de clusters de forma gulosa = Method that creates the cluster groups of greedy fashion
*/
vector<int> Heuristic9::createClusters(int numberOfSets){

	vector<int> sets(Graph::graph.numberOfClusters);
	vector<bool> choosenCenters(Graph::graph.numberOfClusters, false);

	//********* Primeira fase do K-Means ********* First phase of the K-Mean
	//Escolhendo o primeiro centro = Choosing the first center
	int center = rand()%Graph::graph.numberOfClusters;
	vector< list<int> > setsOfClusters(1);
	setsOfClusters[0].push_back(center);
	choosenCenters[center] = true;
	sets[center] = 0;
	for(int i=1; i < numberOfSets; i++){

		int maxDistance = 0;
		int farthestCluster = -1;
		//Calculando a distancia de todos os clusters aos centros ja escolhidos = Calculating the distance of all the clusters centers already chosen
		for(unsigned int cluster=0; cluster < Graph::graph.numberOfClusters; cluster++){

			//Se o cluster ja nao for um centro
			if(!choosenCenters[cluster]){
				int distance = 0;
				//Para cada centro ja escolhido, soma a distancia do cluster corrente
				for(unsigned int k=0; k < setsOfClusters.size(); k++){
					int currCenter = setsOfClusters[k].front();
					distance += Graph::graph.minDistances[currCenter][cluster];
				}
				//Se esse cluster for o mais distante de todos
				if(distance > maxDistance){
					maxDistance = distance;
					farthestCluster = cluster;
				}
			}
		}
		//O novo centro será o cluster mais distante dos outros = The new center will be the most distant cluster of other
		list<int> cluster;
		cluster.push_back(farthestCluster);
		choosenCenters[farthestCluster] = true;
		setsOfClusters.push_back(cluster);
		sets[farthestCluster] = i;
	}
	
	//Associando cada cluster a um dos centros
	for(unsigned int i=0; i< Graph::graph.numberOfClusters; i++){

		//Se o cluster nao eh um centro		
		if(!choosenCenters[i]){

			int minDistance = INFINITE;
			int closestCluster = -1;
	
			//Procura o centro mais proximo do cluster
			for(unsigned int j=0; j < setsOfClusters.size(); j++){
//				std::cout<< "sets of clusters front\t" << setsOfClusters[j].front() << std::endl;
				if(i == 26){
					std::cout << setsOfClusters[j].front() << std::endl;
					std::cout << "clusterId  " << i << std::endl;
					std::cout<< Graph::graph.minDistances[i][setsOfClusters[j].front()] << std::endl;
				}
				if(Graph::graph.minDistances[i][setsOfClusters[j].front()] < minDistance){
					closestCluster = j;
					minDistance = Graph::graph.minDistances[i][setsOfClusters[j].front()];
				}
			}
			std::cout << "set of cluster size" << setsOfClusters.size() << " ";
			std::cout << "closestCluster" << closestCluster << std::endl;
			setsOfClusters[closestCluster].push_back(i);
			sets[i] = closestCluster;
		}
	}
	
	//printClusters(setsOfClusters);
	return sets;
}

/**
 * Metodo que cria os grupos de clusters de forma gulosa aleatoria
 */
vector<int> Heuristic9::createRandomClusters(int numberOfSets){

	vector<int> sets(Graph::graph.numberOfClusters);
	vector<bool> choosenCenters(Graph::graph.numberOfClusters, false);
	vector< list<int> > setsOfClusters(numberOfSets);
		
	//********* Primeira fase do K-Means *********
	//Escolhendo os centros
	for(int i=0; i < numberOfSets; i++){
		int center;
		do{
			center = rand()%Graph::graph.numberOfClusters;
		}while(choosenCenters[center]);
		setsOfClusters[i].push_back(center);
		choosenCenters[center] = true;
		sets[center] = i;
	}

	//Associando cada cluster a um dos centros
	for(unsigned int cluster=0; cluster< Graph::graph.numberOfClusters; cluster++){

		//Se o cluster nao eh um centro
		if(!choosenCenters[cluster]){

			int closestCenter = rand()%setsOfClusters.size();
			int minDistance = Graph::graph.minDistances[cluster][setsOfClusters[closestCenter].front()];

			//Procura o centro mais proximo do cluster
			for(unsigned int center=0; center < setsOfClusters.size(); center++){

				if(Graph::graph.minDistances[cluster][setsOfClusters[center].front()] < minDistance){
					closestCenter = center;
					minDistance = Graph::graph.minDistances[cluster][setsOfClusters[center].front()];
				}
			}
			setsOfClusters[closestCenter].push_back(cluster);
			sets[cluster] = closestCenter;
		}
	}
	
	//printClusters(setsOfClusters);
	return sets;
}

void Heuristic9::printClusters(vector< vector<int> > &setsOfClusters){

	cout << endl;
	cout << "Imprimindo grupos de clusters" << endl;
	for(unsigned int i=0; i < setsOfClusters.size(); i++){
		cout << "Grupo " << i << ":";
		for(unsigned int j=0; j < setsOfClusters[i].size(); j++){
			cout << " " << setsOfClusters[i][j]+1;
		}
		cout << endl;
	}	
}
