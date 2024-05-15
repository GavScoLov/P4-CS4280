#include <iostream>
#include <string>
#include <cstdlib>

#include "node.h"
#include "token.h"
#include "scanner.h"
#include "testTree.h"
#include "parser.h"

using namespace std;

// Declare Symbol Variables
string symbolTable[MAX];
int symbolCount = 0;

// Function for Creating each new node
Node * createNewNode(int type){
	// Define size of newNode
	Node *newNode = new Node();
	// Set to null -or- zero
	newNode->type = type;
	Token nullT1;
	Token nullT2;
	nullT1.type = "";
	nullT2.type = "";
	nullT1.instance = "";
	nullT2.instance = "";
	nullT1.line = 0;
	nullT2.line = 0;

	newNode->t1 = nullT1;
	newNode->t2 = nullT2;
	newNode->ch1 = NULL;
	newNode->ch2 = NULL;
	newNode->ch3 = NULL;
	newNode->ch4 = NULL;
	return newNode;
}

void freeNode(Node* node) {
	if (node == NULL) {
		return;
	}
	// Recursively free child nodes
	freeNode(node->ch1);
	freeNode(node->ch2);
	freeNode(node->ch3);
	freeNode(node->ch4);
	// Free the current node
	free(node);
}
	
// Insert ID into Symbol Table
void insert(string identifier, int nonterminal){
	string TypeDB[11] = { "S", "A", "B", "C", "D", "E", "F", "G", "H", "J", "AP" };
	if(symbolCount < MAX){
		if(verify(identifier) == false){
			if(nonterminal == 3){
				symbolTable[symbolCount++] = identifier;
				//cout << "ID: " << identifier << endl;
			}
		}else{
			if(nonterminal != 3){
				//cout << "<" << TypeDB[nonterminal] << "> USING ID: " << identifier << endl;
				//exit(0);
			}else{
				cout << "RECLARATION ERROR: " << identifier << " already declared." << endl;
				exit(0);
			}
		}
	} else {
		cout << "INSERT ERROR: Table Overflow, Maximum Symbols Reached" << endl;
		// Exit Program
		exit(0);;
	}
}

// Verify ID in the Symbol Table
bool verify(string identifier){
	for(int i = 0; i < symbolCount; ++i){
		if(symbolTable[i] == identifier){
			return true;
		}
	}
	return false;
	//iinsert(identifier);
}

void preorderTraversal(Node *node, int depth){
	int count = 0;
	if(node != NULL) {
		// Create DB for Types on Output
		string TypeDB[11] = { "S", "A", "B", "C", "D", "E", "F", "G", "H", "J", "AP" };
		// Print the current node
		for (int i = 0; i < depth * 4; i++) {
			cout << " ";
		}
		cout << TypeDB[node->type] << endl;
		// Print the Current Node
		if(node->t1.instance == " "){
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			cout << "empty" << endl;
		}else if(!node->t1.type.empty()) {
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			if(node->t1.type == "T2" || node->t1.type == "T1"){
				cout << node->t1.type << " " << node->t1.instance << endl;
			}else{
				cout << node->t1.instance << endl;
			}
		}else{
			preorderTraversal(node->ch1, depth+1);
		}
		if(node->t2.instance == " "){
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			cout << "empty" << endl;
		}else if(!node->t2.type.empty()) {
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			if(node->t2.type == "T2" || node->t2.type == "T1"){
				cout << node->t2.type << " " << node->t2.instance << endl;
			}else{
				cout << node->t2.instance << endl;
			}
		}else{
			preorderTraversal(node->ch2, depth+1);
		}
		if(node->t3.instance == " "){
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			cout << "empty" << endl;
		}else if(!node->t3.type.empty()) {
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			if(node->t3.type == "T2" || node->t3.type == "T1"){
				cout << node->t3.type << " " << node->t3.instance << endl;
			}else{
				cout << node->t3.instance << endl;
			}
		}else{
			preorderTraversal(node->ch3, depth+1);
		}
		if(node->t4.instance == "empty"){
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			cout << "empty" << endl;
		}else if(!node->t4.type.empty()) {
			for (int i = 0; i < (depth + 1) * 4; i++) {
				cout << " ";
			}
			if(node->t4.type == "T2" || node->t4.type == "T1"){
				cout << node->t4.type << " " << node->t4.instance << endl;
			}else{
				cout << node->t4.instance << endl;
			}
		}else{
			preorderTraversal(node->ch4, depth+1);
		}
	}
}

