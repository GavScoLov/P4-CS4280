#ifndef TESTTREE_H
#define TESTTREE_H

#include <iostream>
#include <string>

#include "node.h"

using namespace std;

const int MAX = 100;

// Variables
extern int symbolCount;
extern string symbolTable[];

void testTree(char*, char*, int);

Node * createNewNode(int);

void freeNode(Node* );

void insert(string, int);

bool verify(string);

void preorderTraversal(Node *, int);

#endif
