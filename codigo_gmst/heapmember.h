/***************************************************************************
 *   Generalized Minimum Spanning Tree                                     *
 *                                                                         *
 *   Copyright (C) 2006 by Cristiane Ferreira                              *
 *   Criado em 12 de Março de 2006                                         *
 *   cferreira@ic.uff.br                                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef HEAPMEMBER_H
#define HEAPMEMBER_H

using namespace std;

class HeapMember{
public:

    int id;
    int value;

    HeapMember(int id, int value);

    ~HeapMember();

};

#endif
