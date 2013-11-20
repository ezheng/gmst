/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 09 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "constructor.h"

/**
* Metodo que seleciona a heuristica de construcao que sera utilizada de acordo com o atributo heuristic do objeto
* 'graph' refere-se ao grafo em cima do qual sera criada a solucao
*/
vector<int> Constructor::constructSolution(int heuristic){

	switch(heuristic){
	
		case HEURISTIC1:
			return Heuristic1::calculateTree();
					
		case RANDOMIZED_HEURISTIC1:
			return Heuristic1::calculateRandomTree();
					
		case HEURISTIC2:
			return Heuristic2::calculateTree();
					
		case RANDOMIZED_HEURISTIC2:
			return Heuristic2::calculateRandomTree();
			
		case HEURISTIC5:
			return Heuristic5::calculateTree();
		
		case HEURISTIC6:
			return Heuristic6::calculateTree();
		
		case RANDOMIZED_HEURISTIC6:
			return Heuristic6::calculateRandomTree();
		
		case HEURISTIC7:			 
			return Heuristic7::calculateTree();	
		
		case RANDOMIZED_HEURISTIC7:		 			
			return Heuristic7::calculateRandomTree();
		
		case HEURISTIC9:
			return Heuristic9::calculateTree();
			
		case RANDOMIZED_HEURISTIC9:
			return Heuristic9::calculateRandomTree();
			
		default:{
			cout << "Erro! Opcao de heuristica construtiva invalida: " << heuristic << endl;
			exit(1);
		}
	}
}
