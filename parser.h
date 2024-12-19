//**************************************************************************
 
// Replace with appropriate header comment......

//**************************************************************************

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <iostream>
#include "node.h"
#include <map>


using namespace std;

// Holds the symbols in the interpreted program

extern int nextToken;        // next token returned by lexer
extern bool printParse;

typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;



extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

// Function declarations
// Function declarations
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
ExpressionNode* expression();
SimpleExpNode* simple_Expression();
TermNode* term();
FactorNode* factor();

bool isfactor();

AssignmentNode* assignment();
BeginNode* begin();
IfNode* ifstatement();
WhileNode* whilestatement();
ReadNode* readstatement();
WriteNode* writestatement();
CompoundStatementNode* compoundstmnt();

#endif /* PARSER_H */
