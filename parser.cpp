#include <iostream>
#include <string>
#include <cstdlib>
#include <utility>
#include <fstream>

#include "node.h"
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"

using namespace std;

Token token;
NFInfo result;
int a0;
int a1;
int depth;
ofstream assemblyFile;
//file.open("out.asm");

Node* parser(char* file, char* baseFile, int valC){
	//Token token;
	File info;
	string totalInstance = "";
	int line = 1;
	if(token.line == 0){
		token.line = line;
	}
	token = scanner(file, baseFile, totalInstance, token.line);
	totalInstance += token.instance;
	info.file = file;
	info.bFile = baseFile;
	info.ttlInst = totalInstance;
	info.ln = token.line;
	if(token.type == "ERR01" || token.type == "ERR02" || token.type == "NL") {
		cout << "ERROR: Line Number: " << token.line << " Token: " << token.instance << endl;
	}
	// Output for clarification:
	cout << "  --> Output Can Also Be Found in 'out.asm' <--  " << endl;
	// Call Nonterminals in Parser.cpp
	// Store ID for Node with Nonterminal or Terminal
	assemblyFile.open("out.asm");
	result = S(info);
	cout << "STOP" << endl;
	assemblyFile << "STOP" << endl;
	for(int i = 0; i < result.idCount; i++){
		cout << result.id[i] << " 0" << endl;
		string tempOutput = result.id[i] + " 0";
		assemblyFile << tempOutput << endl;
	}
	Node* root = result.node;
	assemblyFile.close();
	return root;
}

// Nonterminals in order
NFInfo S(File info){
	// Production Rule: CD
	Node* node = createNewNode(0);
	//cout << "S" << endl;
	int tempTkCount = result.tokenCount;
        result = C(info);
        result.tokenCount -= tempTkCount;
        for(int i = 0; i < result.tokenCount; i++){
                token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                info.ttlInst += token.instance;
        }
	node->ch1 = result.node;
	tempTkCount = result.tokenCount;
        result = D(info);
        result.tokenCount -= tempTkCount;
        for(int i = 0; i < result.tokenCount; i++){
                token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                info.ttlInst += token.instance;
        }
	node->ch2 = result.node;
	result.node = node;
	result.info = info;
	return result;
}
NFInfo A(File info){
	// Production Rule: F AP
	Node* node = createNewNode(1);
	//cout << "A" << endl;
	int tempTkCount = result.tokenCount;
        result = F(info);
        result.tokenCount -= tempTkCount;
        for(int i = 0; i < result.tokenCount; i++){
                token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                info.ttlInst += token.instance;
        }
	cout << "LOAD " << result.identifier << endl;
	assemblyFile << "LOAD " << result.identifier << endl;
	node->ch1 = result.node;
	tempTkCount = result.tokenCount;
        result = AP(info);
        result.tokenCount -= tempTkCount;
        for(int i = 0; i < result.tokenCount; i++){
                token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                info.ttlInst += token.instance;
        }
	node->ch2 = result.node;
	result.node = node;
	result.info;
	return result;
}
// AP = A' = A Prime
NFInfo AP(File info){
	// Production Rule: F ?$ | .
	Node* node = createNewNode(10);
	//cout << "AP" << endl;
	int found = 0;
	a0 = token.instance[0];
	if(a0 == 46){ // "."
		node->t1 = token;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		result.node = node;
		result.info = info;
		cout << "STORE tempA" << endl;
		assemblyFile << "STORE tempA" << endl;
		for(int i = 0; i < result.idCount; i++){
			if(result.id[i] == "tempA"){
                        	found = 1;
			}
                }
		if(found == 0){
                        result.id[result.idCount] = "tempA";
                        result.idCount += 1;
                }
		return result;
	}else{
		int tempTkCount = result.tokenCount;
                result = F(info);
                result.tokenCount -= 1;//tempTkCount;
                for(int i = 0; i < result.tokenCount; i++){
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
                }
		cout << "ADD " << result.identifier << endl;
		assemblyFile << "ADD " << result.identifier << endl;
		cout << "STORE tempA" << endl;
		assemblyFile << "STORE tempA" << endl;
		for(int i = 0; i < result.idCount; i++){
			if(result.id[i] == "tempA"){
				found = 1;
			}
		}
		if(found == 0){
			result.id[result.idCount] = "tempA";
			result.idCount += 1;
		}
		node->ch1 = result.node;
		int a01 = token.instance[0];
        	int a02 = token.instance[1];
		if(a01 == 63 && a02 == 36){//token.instance == "?$ "){
			node->t2 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			result.node = node;
			result.info = info;	
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <A>" << endl;
}
NFInfo B(File info){
	// Production Rule: .t2A!
	Node* node = createNewNode(2);
	//cout << "B" << endl;
	if(token.instance == ". "){
		node->t1 = token;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		if(token.type == "T2"){
			node->t2 = token;
			string id = token.instance;
			insert(id, 2);
			string tempIdentifier = "v" + id.substr(1);
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			int tempTkCount = result.tokenCount;
                	result = A(info);
                	result.tokenCount -= tempTkCount;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			node->ch3 = result.node;
			cout << "LOAD tempA" << endl;
			assemblyFile << "LOAD tempA" << endl;
			// Check for which ID is going to be used
			cout << "STORE " << tempIdentifier << endl;
			assemblyFile << "STORE " << tempIdentifier << endl;
			int a01 = token.instance[0];
			//cout << a01 << " / " << token.instance[0] << endl;
			if(a01 == 33){
				node->t4 = token;
				token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
				info.ttlInst += token.instance;
				result.tokenCount++;
				result.node = node;
				result.info = info;
				return result;
			}
		}
	}
	cout << "PARSER ERROR: Unexpected Token{" << token.instance << "} inside nonterminal <B>" << endl;
}
NFInfo C(File info) {
	// Production Rule: t2*
	Node* node = createNewNode(3);
	//cout << "C" << endl;
	if(token.type == "T2"){
		node->t1 = token;
		string id = token.instance;
		insert(id, 3);
		string tempIdentifier = "v" + id.substr(1);
		cout << "READ " << tempIdentifier << endl;
		assemblyFile << "READ " << tempIdentifier << endl;
		result.id[result.idCount] = tempIdentifier;
		result.idCount += 1;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		a0 = token.instance[0];
		if(token.type == "T3" && a0 == 42){
			node->t2 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			result.node = node;
			result.info = info;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <C>" << endl;
}
NFInfo D(File info){
	// Production Rule: DH? | empty
	Node* node = createNewNode(4);
	//cout << "D" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	if(token.type == "NLTk" || token.type == "EOFTk" || token.instance == " "){
		node->t1 = token;
		result.node = node;
		result.info = info;
		return result;
	}else if(token.instance == "?" || token.instance == ". " || a0 == 44 || (a0 == 44 && a1 == 59) || (a0 == 42 && a1 == 34) || token.type == "T2"){
		int tempTkCount = result.tokenCount;
                result = H(info);
                result.tokenCount++;
                for(int i = 0; i < result.tokenCount; i++){
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
		}
		node->ch1 = result.node;
		a0 = token.instance[0];
		if(a0 == 63){
			node->t2 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			int tempTkCount = result.tokenCount;
			result = D(info);
                	result.tokenCount -= tempTkCount;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			node->ch3 = result.node;
			result.node = node;
			result.info = info;
			return result;
		}
	}else{
		cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <D>" << endl;
	}
}
NFInfo E(File info){
	// Production Rule: ,AAH | ,;FH
	Node* node = createNewNode(5);
	//cout << "E" << endl;
	a0 = token.instance[0];
        a1 = token.instance[1];
	if(token.type == "T3"){
		if(a0 == 44 && a1 == 59){
			node->t1 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			int tempTkCount = result.tokenCount;
                	result = F(info);
                	result.tokenCount -= tempTkCount;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			node->ch2 = result.node;
			cout << "LOAD 0" << endl;
			assemblyFile << "LOAD 0" << endl;
			cout << "STORE tempE" << endl;
			assemblyFile << "STORE tempE" << endl;
			result.id[result.idCount] = "tempE";
			result.idCount += 1;
			cout << "Loop: NOOP" << endl;
			assemblyFile << "Loop: NOOP" << endl;
			cout << "LOAD " << result.identifier << endl;
			assemblyFile << "LOAD " << result.identifier << endl;
			cout << "SUB tempE" << endl;
			assemblyFile << "SUB tempE" << endl;
			cout << "BRZNEG End" << endl;
			assemblyFile << "BRZNEG End" << endl;
			tempTkCount = result.tokenCount;
                	result = H(info);
                	result.tokenCount -= tempTkCount;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			node->ch3 = result.node;
			cout << "LOAD tempE" << endl;
			assemblyFile << "LOAD tempE" << endl;
			cout << "ADD 1" << endl;
			assemblyFile << "ADD 1" << endl;
			cout << "STORE tempE" << endl;
			assemblyFile << "STORE tempE" << endl;
			cout << "BR Loop" << endl;
			assemblyFile << "BR Loop" << endl;
			cout << "End: NOOP" << endl;
			assemblyFile << "End: NOOP" << endl;
			result.node = node;
			result.info = info;
			return result;
		}else if(a0 == 44){
                        node->t1 = token;
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
                        result.tokenCount++;
                        int tempTkCount = result.tokenCount;
                	result = A(info);
                	result.tokenCount -= tempTkCount;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			cout << "LOAD tempA" << endl;
			assemblyFile << "LOAD tempA" << endl;
			cout << "STORE tempE" << endl;
			assemblyFile << "STORE tempE" << endl;
			result.id[result.idCount] = "tempE";
                        result.idCount += 1;
			node->ch2 = result.node;
                        tempTkCount = result.tokenCount;
                	result = A(info);
			result.tokenCount += 3;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
			}
			node->ch3 = result.node;
			cout << "LOAD tempE" << endl;
			assemblyFile << "LOAD tempE" << endl;
			cout << "SUB tempA" << endl;
			assemblyFile << "SUB tempA" << endl;
			cout << "BRZNEG Else" << endl;
			assemblyFile << "BRZNEG Else" << endl;
			tempTkCount = result.tokenCount;
                	result = H(info);
                	result.tokenCount += 6;
                	for(int i = 0; i < result.tokenCount; i++){
                        	token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        	info.ttlInst += token.instance;
                	}
			node->ch4 = result.node;
                        cout << "Else: NOOP" << endl;
			assemblyFile << "Else: NOOP" << endl;
			result.node = node;
                        result.info = info;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <E>" << endl;
}
NFInfo F(File info){
	// Production Rule: t1 | t2
	Node* node = createNewNode(6);
	//cout << "F" << endl;
	if(token.type == "T1"){
		node->t1 = token;
		string id = token.instance;
                insert(id, 6);
		// Determine value for F
		int ascii = int(token.instance[0]);
		if(ascii >= 65 && ascii <= 90){
			// Positive t1 value
			string tempInstance = token.instance;
			result.identifier = tempInstance.substr(1);
		}else if(ascii >= 97 && ascii <= 122){
			// Negative t1 value
			string tempInstance = token.instance;
			result.identifier = "-" + tempInstance.substr(1);
		}
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		result.node = node;
		result.info = info;
		return result;
	}else if(token.type == "T2"){
		node->t1 = token;
		string id = token.instance;
		insert(id, 6);
		string tempInstance = token.instance;
                result.identifier = "v" + tempInstance.substr(1);
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <F>" << endl;
}
NFInfo G(File info){
	// Production Rule: B | C | J
	Node* node = createNewNode(7);
	//cout << "G" << endl;
	a0 = token.instance[0];
        a1 = token.instance[1];
	if(token.instance == ". "){
		result = B(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;	
		return result;
	}else if(token.type == "T2"){
		result = C(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;
		return result;
	}else if(a0 == 42 && a1 == 34){
		result = J(info);
		node->ch1 = result.node;
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <G>" << endl;
}
NFInfo H(File info){
	// Production Rule: E? | G. | empty
	Node* node = createNewNode(8);
	//cout << "H" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	if(a0 == 44 || (a0 == 44 && a1 == 59)){
		int tempTkCount = result.tokenCount;
                result = E(info);
		// Fix Inconsistency Here:
                result.tokenCount += 4;
		result.tokenCount -= tempTkCount;
                for(int i = 0; i < result.tokenCount; i++){
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
                }
		node->ch1 = result.node;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		a0 = token.instance[0];
		if(a0 == 63){
			node->t2 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			result.node = node;
			result.info = info;
			return result;
		}
	}else if(token.instance == ". " || token.type == "T2" || a0 == 42 && a1 == 34){
		int tempTkCount = result.tokenCount;
                result = G(info);
		// Fix Inconsistency Here:
		result.tokenCount += 2;
                result.tokenCount -= tempTkCount;
                for(int i = 0; i < result.tokenCount; i++){
                        token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                        info.ttlInst += token.instance;
		}
		node->ch1 = result.node;
		int a01 = token.instance[0];
		if(a01 == 46){
			node->t2 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
		}
		result.node = node;
		result.info = info;	
		return result;
	}else{
		result.node = node;
		result.info = info;
		return result;
	}
	cout << "PARSER ERROR: Unexpected Token{ " << token.instance << "} inside nonterminal <H>" << endl;
}
NFInfo J(File info){
	// Production Rule: *" A.
	Node* node = createNewNode(9);
	//cout << "J" << endl;
	a0 = token.instance[0];
	a1 = token.instance[1];
	if(a0 == 42 && a1 == 34){
		node->t1 = token;
		token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
		info.ttlInst += token.instance;
		result.tokenCount++;
		int tempTkCount = result.tokenCount;
		result = A(info);
		result.tokenCount += 4;
		result.tokenCount -= tempTkCount;
		for(int i = 0; i < result.tokenCount; i++){
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
                	info.ttlInst += token.instance;
		}
		cout << "LOAD tempA" << endl;
		assemblyFile << "LOAD tempA" << endl;
		cout << "WRITE tempA" << endl;
		assemblyFile << "WRITE tempA" << endl;
		node->ch2 = result.node;
		if(token.instance == ". "){
			node->t3 = token;
			token = scanner(info.file, info.bFile, info.ttlInst, info.ln);
			info.ttlInst += token.instance;
			result.tokenCount++;
			result.node = node;
			result.info = info;
			return result;
		}
	}
	cout << "PARSER ERROR: Unexpected Token{" << token.instance << "} inside nonterminal <J>" << endl;
}
