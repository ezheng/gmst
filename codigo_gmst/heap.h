/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <vector>

#include "heapmember.h"

using namespace std;

class Heap{

private:
    vector<HeapMember> members;

    /**
     * Metodo para descer o elemento 'member' no heap, sabendo que ele se encontra na posicao 'pos'
     */
    void down(int pos, HeapMember &member);

    /**
     * Metodo para subir o elemento 'member' no heap, sabendo que ele se encontra na posicao 'pos'
     */
    void up(int pos, HeapMember &member);

public:
    Heap();

    ~Heap();

    /**
     * Metodo que busca o elemento com chave 'u' no heap. Caso encontre, retorna sua posicao +1.
     * Caso contrario retorna 0
     */
    int member(int id);

	/**
	* Retorna o numero de elementos do Heap
	*/
    int size();

	/**
 	* Retorna se o heap esta vazio
	*/
	bool empty();

    /**
     * Metodo que remove e retorna o elemento com menor valor do heap, ou seja, o primeiro
     * Se o Heap estiver vazio, retorna -1
     */
    int findAndDeleteMin();

    int getMin();

    /**
     * Metodo que insere a entrada no heap, na posicao correta
     */
    void insert(int id, int value);

    /**
     * Metodo que verifica se existe a entrada 'u' no heap. Caso exista, substitui seu valor por 'd'.
     * Caso contrario, simplesmente insere a entrada {u, d} no heap
     */
    void adjust(int id, int value);

    /**
     * Metodo que imprime o status do heap na tela
     */
    void print();
    
};

#endif
