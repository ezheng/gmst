/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#include "heap.h"

Heap::Heap(){}


Heap::~Heap(){}


/**
* Metodo que busca o elemento com chave 'u' no heap. Caso encontre, retorna sua posicao.
* Caso contrario retorna -1
*/
int Heap::member(int id){
	
	int pos=0;

	while(((unsigned)pos < members.size()) && (members[pos].id != id)) pos++;
	
	return ((unsigned)pos == members.size())? -1: pos;
}

/**
* Retorna o numero de elementos do Heap
*/
int Heap::size(){
	return members.size();
}

/**
* Retorna se o heap esta vazio
*/
bool Heap::empty(){
	return (size() == 0);
}

/**
* Metodo que remove e retorna o elemento com menor valor do heap, ou seja, o primeiro
* Se o Heap estiver vazio, retorna -1
*/
int Heap::findAndDeleteMin(){

	if(!empty()){
			int size = members.size();
			HeapMember member = members[0];
			HeapMember member2 = members[size-1];
	
			members[0] = member2;			
			members.erase(members.end());
	
			down(1,member2);
	
			return member.id;
	
	}else {
			return -1;
	}
}

int Heap::getMin(){

	return members[0].id;
}

/**
* Metodo que insere a entrada no heap, na posicao correta
*/
void Heap::insert(int id, int value){
	
	HeapMember member(id, value);
	members.push_back(member);
	up(members.size(), member);
}

/**
* Metodo para subir o elemento 'member' no heap, sabendo que ele se encontra na posicao 'pos'
*/
void Heap::up(int pos, HeapMember &member){
	
	members[pos-1] = member;

	int pos2 = pos/2;
	while(pos2 > 0){

		HeapMember member2 = members[pos2-1];
		if(member2.value > member.value){
			members[pos-1] = member2;			
			members[pos2-1] = member;			
		}else
			pos2 = 0;
		pos = pos2;
		pos2 /= 2;
	}
}

/**
* Metodo para descer o elemento 'member' no heap, sabendo que ele se encontra na posicao 'pos'
*/
void Heap::down(int pos, HeapMember &member){
	
	members[pos-1] = member;
	int size = members.size();

	int pos2 = pos*2;
	while(pos2 <= size){

		if(pos2+1 <= size){
			//Verificando qual elemento é menor, pos2 ou pos2+1
			if((members[pos2]).value < (members[pos2-1]).value)
				pos2 = pos2+1;
		}
		HeapMember member2 = members[pos2-1];
		if(member2.value < member.value){
			//Trocando as posições
			members[pos-1] = member2;
			members[pos2-1] = member;
		}else
			pos2 = size;

		pos = pos2;
		pos2 *= 2;
	}
}

/**
* Metodo que verifica se existe a entrada 'u' no heap. Caso exista, substitui seu valor por 'd'.
* Caso contrario, simplesmente insere a entrada {u, d} no heap
*/
void Heap::adjust(int id, int value){

	int pos = member(id);

	if(pos != -1){
		HeapMember member = members[pos];

		if(member.value < value){
			member.value = value;
			down(pos+1, member);
		}else {
			member.value = value;
			up(pos+1, member);
		}
	}else
		insert(id, value);
}

/**
* Metodo que imprime o status do heap na tela
*/
void Heap::print(){
	
	cout << "H = {";
	for(unsigned int i=0; i < members.size(); i++){
		HeapMember member = members[i];
		cout << "[" << member.id << ", " << member.value << "] ";
	}
	cout << "} " << members.size() << endl;	
}
