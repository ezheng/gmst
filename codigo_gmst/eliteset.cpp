/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "eliteset.h"

EliteSet::EliteSet(){
	bestIteration = 1;
	averageCost = 0;
}

EliteSet::~EliteSet(){}

/**
* Procura a solucao no conjunto elite, se nao encontrar insere no conjunto elite em ordem decrescente de custo
* Se o conjunto elite ainda nao estiver completo, insere a nova solucao, caso contrário, a solucao só é inserida
* se for melhor que alguma outra solucao elite. Nesse ultimo caso, a pior solucao é descartada.
* Retorna 'true' caso a atualizacao tenha sido feita
*/
bool EliteSet::update(vector<int> &solution, int cost, int iteration){

	bool update = false;
	bool found = false;	
	list< vector<int> >::iterator it1 = solutionSet.begin();
	list<int>::iterator it2 = costSet.begin();

	//Procurando a posicao onde inserir a nova solucao, já que o conjunto está ordenado	
	//Enquanto os elementos do conjunto elite forem piores que a solucao 'tree'
	while((it2 != costSet.end()) && (*it2 > cost)){
		it2++;
		it1++;
	}

	if((it2 != costSet.end()) && (*it2 == cost))
		found = true;

	//Só insere se a solucao nao foi encontrada no conjunto elite
	//Obs.: Só vale a pena inserir no inicio da lista se o conjunto elite ainda nao está completo
	if((!found) && (((it2 == costSet.begin()) && (costSet.size() < (unsigned)Parameters::getInstance().ELITE_SET_SIZE)) || (it2 != costSet.begin()))){
		
		//Insere a solucao na posicao indicada
		solutionSet.insert(it1, solution);
		costSet.insert(it2, cost);
		update = true;
		lastUpdate = 0;

		//Se o conjunto elite já estiver cheio, remove o pior (ou seja, o primeiro) elemento
		if(costSet.size() > (unsigned) Parameters::getInstance().ELITE_SET_SIZE){
			costSet.pop_front();
			solutionSet.pop_front();
		}

		//Se encontrou uma solucao melhor que todas
		if(it2 == costSet.end()){
			if(Parameters::getInstance().VERBOSE)
				cout<< iteration << " --> Found Best Cost: " << cost << endl;
			bestIteration = iteration;
		}
	}

	//Imprimindo conjunto elite
  	if(Parameters::getInstance().VERBOSE)
  		print();

	return update;
}

/**
* Imprime os custos das solucoes do conjunto elite
*/
void EliteSet::print(){

	list<int>::iterator it;	
	list< vector<int> >::iterator it2 = solutionSet.begin();
	
	cout << "Elite Set:";	
	for(it=costSet.begin(); it != costSet.end(); it++){
		cout << " " << *it;
// 		if(Heuristic5::getMSTCost(*it2) != *it){
// 			cout << " Conta errada! " << endl;
// 			exit(1);
// 		}
		it2++;
	}
 	cout << endl;
}

/**
 * Imprime as solucoes do conjunto elite
 */
void EliteSet::printSolutions(){

	list< vector<int> >::iterator it1;
	list<int>::iterator it2;
	cout << "Custo";
	for(unsigned int i=0; i < Graph::graph.numberOfClusters; i++){
		cout << " " << i;
	}
	cout << endl;
	
	for(it1=solutionSet.begin(),it2 = costSet.begin(); it1 != solutionSet.end(); it1++,it2++){
		cout << *it2;
		for(unsigned int i=0; i < (*it1).size(); i++){
			cout << " " << (*it1)[i];
		}
		cout << endl;
	}
	cout << endl;
}
/**
* Apaga todas as solucoes do conjunto elite
*/
void EliteSet::clear(){
	costSet.clear();
	solutionSet.clear();
	bestIteration = 1;
	lastUpdate = 1;
}

/**
* Retorna a pior - ou seja, a primeira - solucao do conjunto elite
*/
list< vector<int> >::iterator EliteSet::begin(){
	return solutionSet.begin();
}

/**
* Retorna a melhor - ou seja, a ultima - solucao do conjunto elite
*/
vector<int> EliteSet::bestSolution(){

	vector<int> solution;

	if(solutionSet.size() > 0)
		solution = solutionSet.back();

	return solution;
}

/**
 * Retorna o custo da melhor solucao
 */
int EliteSet::bestCost(){
	
	if(costSet.size() > 0)
		return costSet.back();
	else
		return -1;
}

/**
* Retorna a iteracao em que foi encontrada a melhor solucao do conjunto elite
*/
int EliteSet::getBestIteration(){
	return bestIteration;
}
/**
* Verifica se a solucao passada pode ser considerada uma 'boa solucao' 
* Para uma arvore ser uma 'boa solucao', seu custo nao pode muito maior que a melhor solucao do conjunto elite
* Para isso, utiliza-se o parametro ELITE_SOLUTION_LIMIT
*/
bool EliteSet::goodSolution(int cost){
	//float solutionLimit = Parameters::getInstance().ELITE_SOLUTION_LIMIT;
	
 	float solutionLimit = Parameters::getInstance().ELITE_SOLUTION_LIMIT + lastUpdate*0.001;

	return ((float)cost/(float)costSet.back() < solutionLimit);
	//return (cost < averageCost);
}

/**
* Metodo que seleciona uma das solucoes do conjunto elite para realizar o Path Relinking
* A solucao selecionada é sempre a mais distante (em termos de custo) da solucao passada como argumento
*/
int EliteSet::chooseASolution(const vector<int> &solution, vector<int> &choosen){

	int biggestDistance = 0;	
	int cost = -1;
	
	list< vector<int> >::iterator it1;
	list<int>::iterator it2;
	
	//A solucao escolhida eh aquela com maior diferenca (de custo) para a solucao passada	
	for(it1=solutionSet.begin(),it2=costSet.begin(); it1 != solutionSet.end(); it1++, it2++){
		
		int distance = computeDistance(*it1, solution);		 		
		if(distance > biggestDistance){
			biggestDistance = distance;
			choosen = *it1;
			cost = *it2;
		}
	}
	return cost;
}

/**
 * Calcula a distance entre duas solucoes
 * 
 * @param solution1 uma das solucoes de que se deseja obter a distancia
 * @param solution2 uma das solucoes de que se deseja obter a distancia
 * 
 * @return distancia entre as duas solucoes
 */
int EliteSet::computeDistance(const vector<int> &solution1, const vector<int> &solution2){
	
	int distance = 0;
	
	for(unsigned int i=0; i < solution1.size(); i++){
		if(solution1[i] != solution2[i])
			distance++;
	}
	return distance;
}

/**
 * Retorna os vertices de cada cluster que aparecem no conjunto elite
 */
vector< vector<int> > EliteSet::returnVertices(){

	vector< vector<int> > vertices(Graph::graph.numberOfClusters);
	vector<bool> flag(Graph::graph.numberOfVertices,false);

	//Para cada solucao do conjunto elite
	for(list< vector<int> >::iterator it=solutionSet.begin(); it != solutionSet.end(); it++){
		//Para cada cluster
		for(unsigned int cluster=0; cluster < (*it).size(); cluster++){
			int vertex = (*it)[cluster];
			//Se o vértice ainda não foi inserido
			if(!flag[vertex]){				
				vertices[cluster].push_back(vertex);
				flag[vertex] = true;
			}	
		}
	}
	return vertices;
}

/**
 * Remove todos os elementos do conjunto elite, exceto o último, ou seja, o melhor
 */
void EliteSet::reset(){

	//Salvando a melhor solução
	vector<int> solution = solutionSet.back();
	int cost = costSet.back();

	//Resetando as listas com as soluções do Conjunto Elite
	solutionSet.clear();
	costSet.clear();

	//Re-inserindo a melhor solução
	solutionSet.push_back(solution);
	costSet.push_back(cost);
	
	lastUpdate = 1;
}
