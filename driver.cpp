//*****************************************************************************
// purpose: parser for TIPS
// version: Spring 2024
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "lexer.h"
#include "parser.h"
#include "node.h"
#include <string.h>
using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
}

// Needed global variables
extern int nextToken; // token returned from yylex

//*****************************************************************************
// The main processing loop
//
bool printDelete = false;
bool printParse = false;

int main(int argc, char* argv[]) {

    // Default print flags
    bool printTree = false;
    bool printSymbolTable = false;
    // helped to get the file name 
    //https://stackoverflow.com/questions/58520915/opening-a-file-with-a-user-given-name-fopen-in-c
    const char* inputFileName = "";
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            printParse = true; // Enable print while parsing
        }
        if (strcmp(argv[i], "-t") == 0) {
            printTree = true; // Enable print parse tree
        }
        if (strcmp(argv[i], "-s") == 0) {
            printSymbolTable = true; // Enable print symbol table
        }
        if (strcmp(argv[i], "-d") == 0) {
            printDelete = true; // Enable print while deleting parse tree
            cout << "delete tree show" << endl;
        }else {
            // Assume this is the input file name
            inputFileName = argv[i];
        }
    
    }

    // Open the input file
    yyin = fopen(inputFileName, "r");
    if (!yyin) {
        cerr << "ERROR: Could not open file '" << inputFileName << "'" << endl;
        return EXIT_FAILURE;
    }

    // O
    // Set the output stream
    yyout = stdout;
    // Get the first token
    nextToken = yylex();

    // Fire up the parser!
    try {
        ProgramNode* root = nullptr;
        root = program();

        if (nextToken != TOK_EOF) {
            throw "end of file expected, but there is more here!";
        }
        if (printTree)
        cout << endl << "=== GO BULLDOGS! Your parse was successful! ===" << endl;

        // Print the parse tree if requested
        if (printTree) {
            cout << endl << "*** Print the Tree ***" << endl << endl;
            cout << *root << endl;
        }

        // Print the symbol table if requested
        if (printSymbolTable) {
            cout << endl << "*** User Defined Symbols ***" << endl;
            cout << "*** Print the Symbol Table ***" << endl;
            for (auto it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                cout << setw(8) << it->first << ": " << it->second << endl;
            }
        }

        // Interpret the tree
        cout << endl << "*** Interpret the Tree ***" << endl;
        if (root) {
            root->interpret();
        }

        // Delete the tree if requested
        if (printDelete) {
            cout << endl << "*** Delete the Tree ***" << endl;
        }
        delete root;
        root = nullptr;

    } catch (const char* errmsg) {
        cout << endl << "***ERROR:" << endl;
        cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
        cout << errmsg << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}