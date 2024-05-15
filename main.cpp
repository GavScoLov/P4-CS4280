#include <iostream>
#include <fstream>
#include <cstring>
//#include <filesystem>

#include "node.h"
#include "testTree.h"
#include "parser.h"

using namespace std;
//namespace fs = filesystem;

int main(int argc, char* argv[]){
	int val = 0;
	char* originFile = NULL;
	//const char* fileType = ".txt";
	const char* keyType = "out";
	char* file;
	
	if(argv[1]){
		file = (char*)malloc(sizeof(argv[1]));
		originFile = (char*)malloc(sizeof(argv[1]));
		file = argv[1];
		strcpy(originFile, file);
		//strcat(file, fileType);
		val = 2;
	}else{
		file = (char*)malloc(sizeof("out"));
		strcpy(file, keyType);
		originFile = strdup(keyType);
	}

	if(argc > 2){
		// Too many arguments
		cout << "Fatal Error: Too many arguments" << endl;
		cout << "Proper Usage: statSem [filename]" << endl;
		return 0;
	}else if(argc < 2){
		val = 1;
	}
	// Parser
	Node* root = parser(file, originFile, val);
	
	// Successful Parser Execution
	//cout << "OK" << endl;
	
	// Print Symbol Table
	//cout << "Symbol Table: " << endl;
	//int i = 0;
	//while(symbolTable[i] != ""){
	//	cout << symbolTable[i] << endl;
	//	i++;	
	//}
	// Print Parse Tree
	//preorderTraversal(root, 0);
	return 0;
}
