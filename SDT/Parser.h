#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"

typedef struct nodeType {
	struct tokenType token;            	// 토큰 종류
	enum { terminal, nonterm } noderep; 	// 노드의 종류
	struct nodeType* son;             	// 왼쪽 링크
	struct nodeType* brother;         	// 오른쪽 링크
	struct nodeType* father;	   	// 상위 링크
} Node;

void semantic(int);
void printToken(struct tokenType token); // -> MiniC Scanner
void dumpStack();
void errorRecovery();

Node* parser();
void printTree(Node* pt, int indent);


#endif // PARSER_H