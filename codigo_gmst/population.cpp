/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "population.h"

Population::Population(){
	bestGeneration = 1;
}

Population::~Population(){}

/**
 * Procura a solucao na população, se nao encontrar a insere em ordem decrescente de custo
 * Se a população ainda nao estiver completa, insere a nova solucao, caso contrário, a solucao só é inserida
 * se for melhor que alguma outra solucao. Nesse ultimo caso, a pior solucao é descartada.
 * Retorna 'true' caso a atualizacao tenha sido feita
 */
bool Population::update(vector<int> &solution, int cost, int generation){

	bool update = false;
	bool found = false;
	list< vector<int> >::iterator it1 = solutionSet.begin();
	list<int>::iterator it2 = costSet.begin();

	//Procurando a posicao onde inserir a nova solucao, jÃ¡ que o conjunto estÃ¡ ordenado	
	//Enquanto os elementos do conjunto elite forem piores que a solucao 'tree'
	while((it2 != costSet.end()) && (*it2 > cost)){
		it2++;
		it1++;
	}

	if((it2 != costSet.end()) && (*it2 == cost))
		found = true;

	//Só insere se a solucao nao foi encontrada na população
	//Obs.: Só vale a pena inserir no inicio da lista se a população ainda nao está completa
	if((!found) && (((it2 == costSet.begin()) && (costSet.size() < (unsigned)Parameters::getInstance().ELITE_SET_SIZE)) || (it2 != costSet.begin()))){
		//Verificando a distância para todos os elementos da posição corrente até o fim do conjunto elite
		//Se 'solution' não estiver a MIN_ELITE_DISTANCE unidades de distância de todas as soluções melhores que ela, então não é inserida
		list< vector<int> >::iterator it3 = it1;
		list<int>::iterator it4 = it2;
		int distance = Parameters::getInstance().MIN_ELITE_DISTANCE;

		while((it3 != solutionSet.end()) && (distance >= Parameters::getInstance().MIN_ELITE_DISTANCE)){
			distance = computeDistance(solution, *it3);
			//cout << "Distancia da solucao melhor: " << distance << " (" << *it4 << ")" << endl;
			it3++;it4++;
		}

		//Se a distâcia atÃ© todos os elementos foi maior ou igual a trÃªs, o elemento pode ser inserido
		if(distance >= Parameters::getInstance().MIN_ELITE_DISTANCE){

			//cout << "Solucao sera inserida!" << endl;
			it3 = it1;
			it4 = it2;
			if(it3 != solutionSet.begin()){
				it3--; it4--;
			}
			//Verificando cada elemento com custo menor que 'cost'
			while(it3 != solutionSet.begin()){
				distance = computeDistance(solution, *it3);
				//cout << "Distancia da solucao pior: " << distance << " (" << *it4 << ")" << endl;
				//Se a distancia nÃ£o estive Ã  distancia minima, ela Ã© excluida do conjunto elite
				if(distance < Parameters::getInstance().MIN_ELITE_DISTANCE){

					it3 = solutionSet.erase(it3);
					it4 = costSet.erase(it4);
					//cout << "Solucao apagada!" << endl;
				}
				it3--;it4--;
			}
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
					cout<< generation << " --> Found Best Cost: " << cost << endl;
				bestGeneration = generation;
			}
		}
	}

	//Imprimindo população
	if(Parameters::getInstance().VERBOSE)
		print();

	return update;
}

/**
 * Imprime os custos das solucoes do conjunto elite
 */
void Population::print(){

	list<int>::iterator it;
	list< vector<int> >::iterator it2 = solutionSet.begin();
	
	cout << "Population:";
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
void Population::printSolutions(){

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
void Population::clear(){
	costSet.clear();
	solutionSet.clear();
	bestGeneration = 1;
	lastUpdate = 1;
}

/**
 * Retorna a pior - ou seja, a primeira - solucao do conjunto elite
 */
list< vector<int> >::iterator Population::begin(){
	return solutionSet.begin();
}

/**
 * Retorna a melhor - ou seja, a ultima - solucao do conjunto elite
 */
vector<int> Population::bestSolution(){

	vector<int> solution;

	if(solutionSet.size() > 0)
		solution = solutionSet.back();

	return solution;
}

/**
 * Retorna o custo da melhor solucao
 */
int Population::bestCost(){
	
	if(costSet.size() > 0)
		return costSet.back();
	else
		return -1;
}

/**
 * Retorna a iteracao em que foi encontrada a melhor solucao do conjunto elite
 */
int Population::getBestGeneration(){
	return bestGeneration;
}

/**
 * Calcula a distance entre duas solucoes
 *
 * @param solution1 uma das solucoes de que se deseja obter a distancia
 * @param solution2 uma das solucoes de que se deseja obter a distancia
 *
 * @return distancia entre as duas solucoes
 */
int Population::computeDistance(const vector<int> &solution1, const vector<int> &solution2){
	
	int distance = 0;
	
	for(unsigned int i=0; i < solution1.size(); i++){
		if(solution1[i] != solution2[i])
			distance++;
	}
	return distance;
}

/**
 * Retorna o tamanho da população
 */
int Population::size(){
	return solutionSet.size();
}

/**
 * Metodo que retorna a solução mais distante (em termos de custo) de 'solution'
 */
int Population::chooseASolution(const vector<int> &solution, vector<int> &choosen){

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
 * Método que recalcula o custo de todas as soluções
 */
void Population::recomputeCosts(){

	list< vector<int> >::iterator it = solutionSet.begin();
	costSet.clear();
	
	for(; it != solutionSet.end(); it++){
		int cost = Heuristic5::getMSTCost(*it);
		costSet.push_back(cost);
	}
}

/**
 * Método que reconstrói a população, construindo novas soluções no lugar daquelas que foram removidas
 */
void Population::rebuild(){

	while((int)solutionSet.size() < Parameters::getInstance().ELITE_SET_SIZE){
		vector<int> solution = Heuristic5::calculateTree();
		int cost = Heuristic5::getMSTCost(solution);
		update(solution, cost, 0);
	}
}
