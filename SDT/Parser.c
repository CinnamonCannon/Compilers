#include "MiniC.tbl"    
#include "Scanner.h"             /* Mini C table for appendix A */
//#define  NO_RULES    97            /* number of rules */
//#define  GOAL_RULE  (NO_RULES+1)   /* accept rule */
//#define  NO_SYMBOLS  85            /* number of grammar symbols */
//#define  NO_STATES  153            /* number of states */
#define  PS_SIZE    100              /* size of parsing stack */

typedef struct nodeType {
	struct tokenType token;            	// 토큰 종류
	enum { terminal, nonterm } noderep; 	// 노드의 종류
	struct nodeType* son;             	// 왼쪽 링크
	struct nodeType* brother;         	// 오른쪽 링크
	struct nodeType* father;	   	// 상위 링크
} Node;

void semantic(int);
void printToken(struct tokenType token);
void dumpStack();
void errorRecovery();

Node* buildNode(struct tokenType token);
Node* buildTree(int nodeNumber, int rhsLength);
int meaningfulToken(struct tokenType token);


/***************************************************************************
 *  문법과 tbl이 확장된 경우, PGS의 출력을 확인하며 변경 사항을 적용해줘야 함.
 ***************************************************************************/ 
enum nodeNumber {
	ERROR_NODE,
	PROGRAM,
	FUNC_DEF,
	FUNC_HEAD,
	DCL_SPEC,
	CONST_TYPE,
	INT_TYPE,
	VOID_TYPE,
	FORMAL_PARA,
	PARAM_DCL,
	COMPOUND_ST,
	DCL_LIST,
	DCL,
	DCL_ITEM,
	SIMPLE_VAR,
	ARRAY_VAR,
	STAT_LIST,
	EXP_ST,
	CASE_ST,
	DEFAULT_ST,
	CONTINUE_ST,
	BREAK_ST,
	IF_ST,
	IF_ELSE_ST,
	WHILE_ST,
	DO_WHILE_ST,
	SWITCH_ST,
	FOR_ST,
	INIT_PART,
	CONDITION_PART,
	POST_PART,
	RETURN_ST,
	ASSIGN_OP,
	ADD_ASSIGN,
	SUB_ASSIGN,
	MUL_ASSIGN,
	DIV_ASSIGN,
	MOD_ASSIGN,
	LOGICAL_OR,
	LOGICAL_AND,
	EQ,
	NE,
	GT,
	LT,
	GE,
	LE,
	ADD,
	SUB,
	MUL,
	DIV,
	REMAINDER,
	UNARY_MINUS,
	LOGICAL_NOT,
	PRE_INC,
	PRE_DEC,
	INDEX,
	CALL,
	POST_INC,
	POST_DEC,
	ACTUAL_PARAM,
    IDENT,
    NUMBER
};

char* nodeName[] = {
    "ERROR_NODE",
    "PROGRAM", "FUNC_DEF", "FUNC_HEAD", "DCL_SPEC", "CONST_TYPE", "INT_TYPE", "VOID_TYPE",
    "FORMAL_PARA", "PARAM_DCL", "COMPOUND_ST", "DCL_LIST", "DCL", "DCL_ITEM", "SIMPLE_VAR", "ARRAY_VAR",
    "STAT_LIST", "EXP_ST", "CASE_ST", "DEFAULT_ST", "CONTINUE_ST", "BREAK_ST", "IF_ST", "IF_ELSE_ST",
    "WHILE_ST", "DO_WHILE_ST", "SWITCH_ST", "FOR_ST", "INIT_PART", "CONDITION_PART", "POST_PART",
    "RETURN_ST", "ASSIGN_OP", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
    "LOGICAL_OR", "LOGICAL_AND", "EQ", "NE", "GT", "LT", "GE", "LE",
    "ADD", "SUB", "MUL", "DIV", "REMAINDER",
    "UNARY_MINUS", "LOGICAL_NOT", "PRE_INC", "PRE_DEC",
    "INDEX", "CALL", "POST_INC", "POST_DEC",
    "ACTUAL_PARAM",
    "IDENT", "NUMBER"
};

int ruleName[] = {
    0,           /* 0 */
    PROGRAM,     /* 1 */
    0,           /* 2 */
    0,           /* 3 */
    0,           /* 4 */
    0,           /* 5 */
    FUNC_DEF,    /* 6 */
    FUNC_HEAD,   /* 7 */
    DCL_SPEC,    /* 8 */
    0,           /* 9 */
    0,           /* 10 */
    0,           /* 11 */
    0,           /* 12 */
    CONST_TYPE,  /* 13 */
    INT_TYPE,    /* 14 */
    VOID_TYPE,   /* 15 */
    0,           /* 16 */
    FORMAL_PARA, /* 17 */
    0,           /* 18 */
    0,           /* 19 */
    0,           /* 20 */
    0,           /* 21 */
    PARAM_DCL,   /* 22 */
    COMPOUND_ST, /* 23 */
    DCL_LIST,    /* 24 */
    DCL_LIST,    /* 25 */
    0,           /* 26 */
    0,           /* 27 */
    DCL,         /* 28 */
    0,           /* 29 */
    0,           /* 30 */
    DCL_ITEM,    /* 31 */
    DCL_ITEM,    /* 32 */
    SIMPLE_VAR,  /* 33 */
    ARRAY_VAR,   /* 34 */
    0,           /* 35 */
    0,           /* 36 */
    STAT_LIST,   /* 37 */
    0,           /* 38 */
    0,           /* 39 */
    0,           /* 40 */
    0,           /* 41 */
    0,           /* 42 */
    0,           /* 43 */
    0,           /* 44 */
    0,           /* 45 */
    0,           /* 46 */
    0,           /* 47 */
    0,           /* 48 */
    0,           /* 49 */
    0,           /* 50 */
    EXP_ST,      /* 51 */
    0,           /* 52 */
    0,           /* 53 */
    CASE_ST,     /* 54 */
    DEFAULT_ST,  /* 55 */
    CONTINUE_ST, /* 56 */
    BREAK_ST,    /* 57 */
    IF_ST,       /* 58 */
    IF_ELSE_ST,  /* 59 */
    WHILE_ST,    /* 60 */
    DO_WHILE_ST, /* 61 */
    SWITCH_ST,   /* 62 */
    FOR_ST,      /* 63 */
    INIT_PART,   /* 64 */
    CONDITION_PART, /* 65 */
    POST_PART,   /* 66 */
    RETURN_ST,   /* 67 */
    0,           /* 68 */
    0,           /* 69 */
    ASSIGN_OP,   /* 70 */
    ADD_ASSIGN,  /* 71 */
    SUB_ASSIGN,  /* 72 */
    MUL_ASSIGN,  /* 73 */
    DIV_ASSIGN,  /* 74 */
    MOD_ASSIGN,  /* 75 */
    0,           /* 76 */
    LOGICAL_OR,  /* 77 */
    0,           /* 78 */
    LOGICAL_AND, /* 79 */
    0,           /* 80 */
    EQ,          /* 81 */
    NE,          /* 82 */
    0,           /* 83 */
    GT,          /* 84 */
    LT,          /* 85 */
    GE,          /* 86 */
    LE,          /* 87 */
    0,           /* 88 */
    ADD,         /* 89 */
    SUB,         /* 90 */
    0,           /* 91 */
    MUL,         /* 92 */
    DIV,         /* 93 */
    REMAINDER,   /* 94 */
    0,           /* 95 */
    UNARY_MINUS, /* 96 */
    LOGICAL_NOT, /* 97 */
    PRE_INC,     /* 98 */
    PRE_DEC,     /* 99 */
    0,           /* 100 */
    INDEX,       /* 101 */
    CALL,        /* 102 */
    POST_INC,    /* 103 */
    POST_DEC,    /* 104 */
    0,           /* 105 */
    0,           /* 106 */
    ACTUAL_PARAM, /* 107 */
    0,           /* 108 */
    0,           /* 109 */
    0,           /* 110 */
    0,           /* 111 */
    0            /* 112 */
};

int sp;                               // stack pointer
int stateStack[PS_SIZE];              // state stack
int symbolStack[PS_SIZE];             // symbol stack
Node* valueStack[PS_SIZE];            // value stack

Node* parser()
{
	extern int parsingTable[NO_STATES][NO_SYMBOLS + 1];
	extern int leftSymbol[NO_RULES + 1], rightLength[NO_RULES + 1];
	int entry, ruleNumber, lhs;
	int currentState;
	struct tokenType token;
	Node* ptr;

	sp = 0; stateStack[sp] = 0;  // initial state
	token = scanner();
	while (1) {
		currentState = stateStack[sp];
		entry = parsingTable[currentState][token.number];
		if (entry > 0)                          /* shift action */
		{
			sp++;
			if (sp > PS_SIZE) {
				printf("critical compiler error: parsing stack overflow");
				exit(1);
			}
			symbolStack[sp] = token.number;
			stateStack[sp] = entry;
			valueStack[sp] = meaningfulToken(token) ? buildNode(token) : NULL;
			token = scanner();
		}
		else if (entry < 0)                   /* reduce action */
		{
			ruleNumber = -entry;
			if (ruleNumber == GOAL_RULE) /* accept action */
			{
				//                      printf(" *** valid source ***\n");
				return valueStack[sp - 1];
			}
			//                 semantic(ruleNumber);
			ptr = buildTree(ruleName[ruleNumber], rightLength[ruleNumber]);
			sp = sp - rightLength[ruleNumber];
			lhs = leftSymbol[ruleNumber];
			currentState = parsingTable[stateStack[sp]][lhs];
			sp++;
			symbolStack[sp] = lhs;
			stateStack[sp] = currentState;
			valueStack[sp] = ptr;
		}
		else                               /* error action */
		{
			printf(" === error in source ===\n");
			printf("Current Token : ");
			printToken(token);
			dumpStack();
			errorRecovery();
			token = scanner();
		}
	} /* while (1) */
} /* parser */

void semantic(int n)
{
	printf("reduced rule number = %d\n", n);
}

void dumpStack()
{
	int i, start;

	if (sp > 10) start = sp - 10;
	else start = 0;

	printf("\n *** dump state stack :");
	for (i = start; i <= sp; ++i)
		printf(" %d", stateStack[i]);

	printf("\n *** dump symbol stack :");
	for (i = start; i <= sp; ++i)
		printf(" %d", symbolStack[i]);
	printf("\n");
}

void printToken(struct tokenType token)
{
	if (token.number == tident)
		printf("%s", token.value.id);
	else if (token.number == tnumber)
		printf("%d", token.value.num);
	else
		printf("%s", tokenName[token.number]);

}

void errorRecovery()
{
	struct tokenType tok;
	int parenthesisCount, braceCount;
	int i;

	// step 1: skip to the semicolon
	parenthesisCount = braceCount = 0;
	while (1) {
		tok = scanner();
		if (tok.number == teof) exit(1);
		if (tok.number == tlparen) parenthesisCount++;
		else if (tok.number == trparen) parenthesisCount--;
		if (tok.number == tlbrace) braceCount++;
		else if (tok.number == trbrace) braceCount--;
		if ((tok.number == tsemicolon) && (parenthesisCount <= 0) && (braceCount <= 0))
			break;
	}

	// step 2: adjust state stack
	for (i = sp; i >= 0; i--) {
		// statement_list ->  statement_list .  statement
		if (stateStack[i] == 36) break;	 // second statement part

		// statement_list ->  .  statement
		// statement_list ->  .  statement_list statement
		if (stateStack[i] == 24) break;	 // first statement part

		// declaration_list ->  declaration_list .  declaration
		if (stateStack[i] == 25) break;  // second internal dcl

		// declaration_list ->  .  declaration
		// declaration_list ->  .  declaration_list declaration
		if (stateStack[i] == 17) break;  // internal declaration

		// external declaration
		// external_dcl ->  .  declaration
		if (stateStack[i] == 2) break;	// after first external dcl
		if (stateStack[i] == 0) break;	// first external declaration
	}
	sp = i;
}

int meaningfulToken(struct tokenType token)
{
	if ((token.number == tident) || (token.number == tnumber))
		return 1;
	else return 0;
}

Node* buildNode(struct tokenType token)
{
	Node* ptr;
	ptr = (Node*)malloc(sizeof(Node));
	if (!ptr) {
		printf("malloc error in buildNode()\n");
		exit(1);
	}
	ptr->token = token;
	ptr->noderep = terminal;
	ptr->son = ptr->brother = NULL;
	return ptr;
}

Node* buildTree(int nodeNumber, int rhsLength)
{
	int i, j, start;
	Node* first, * ptr;

	i = sp - rhsLength + 1;
	/* step 1: find a first index with node in value stack */
	while (i <= sp && valueStack[i] == NULL) i++;
	if (!nodeNumber && i > sp) return NULL;
	start = i;
	/* step 2: linking brothers */
	while (i <= sp - 1) {
		j = i + 1;
		while (j <= sp && valueStack[j] == NULL) j++;
		if (j <= sp) {
			ptr = valueStack[i];
			while (ptr->brother) ptr = ptr->brother;
			ptr->brother = valueStack[j];
		}
		i = j;
	}
	first = (start > sp) ? NULL : valueStack[start];
	/* step 3: making subtree root and linking son */
	if (nodeNumber) {
		ptr = (Node*)malloc(sizeof(Node));
		if (!ptr) {
			printf("malloc error in buildTree()\n");
			exit(1);
		}
		ptr->token.number = nodeNumber;
		ptr->noderep = nonterm;
		ptr->son = first;
		ptr->brother = NULL;
		return ptr;
	}
	else return first;
}