//**************************************************************************
 
// Replace with appropriate header comment......

//**************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
 
using namespace std;

int nextToken = 0;            // hold nextToken returned by lex

// Which tree level are we currently in?  
static int level = 0;

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse
// Symbol Table
//*****************************************************************************
// Symbol table
symbolTableT symbolTable;
// Determine if a symbol is in the symbol table
bool inSymbolTable(string idName) {
  symbolTableT::iterator it;
  it = symbolTable.find( idName );
  // If idName is missing, will be set to the end
  return !(it == symbolTable.end());
}


int beginNUM = 0;
//*****************************************************************************
// Symbol table


//*****************************************************************************
// Indent to reveal tree structure
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}

int lex() 
{
  char* token_str;  // point to the logical name of the token

  nextToken = yylex();
  if( nextToken == TOK_EOF ) {
    yytext[0] = 'E'; yytext[1] = 'O'; yytext[2] = 'F'; yytext[3] = 0;   
  }

  // Determine the logical name of the lexeme
  switch (nextToken) 
    {
       // Keywords
        case TOK_BEGIN: token_str = (char*)"TOK_BEGIN"; break;
        case TOK_BREAK: token_str = (char*)"TOK_BREAK"; break;
        case TOK_CONTINUE: token_str = (char*)"TOK_CONTINUE"; break;
        case TOK_DOWNTO: token_str = (char*)"TOK_DOWNTO"; break;
        case TOK_ELSE: token_str = (char*)"TOK_ELSE"; break;
        case TOK_END: token_str = (char*)"TOK_END"; break;
        case TOK_FOR: token_str = (char*)"TOK_FOR"; break;
        case TOK_IF: token_str = (char*)"TOK_IF"; break;
        case TOK_LET: token_str = (char*)"TOK_LET"; break;
        case TOK_PROGRAM: token_str = (char*)"TOK_PROGRAM"; break;
        case TOK_READ: token_str = (char*)"TOK_READ"; break;
        case TOK_THEN: token_str = (char*)"TOK_THEN"; break;
        case TOK_TO: token_str = (char*)"TOK_TO"; break;
        case TOK_VAR: token_str = (char*)"TOK_VAR"; break;
        case TOK_WHILE: token_str = (char*)"TOK_WHILE"; break;
        case TOK_WRITE: token_str = (char*)"TOK_WRITE"; break;

        // DataTypes
        case TOK_INTEGER: token_str = (char*)"TOK_INTEGER"; break;
        case TOK_REAL: token_str = (char*)"TOK_REAL"; break;

        // Punctuation
        case TOK_SEMICOLON: token_str = (char*)";"; break;
        case TOK_COLON: token_str = (char*)":"; break;
        case TOK_OPENPAREN: token_str = (char*)"("; break;
        case TOK_CLOSEPAREN: token_str = (char*)")"; break;

        // Operators
        case TOK_PLUS: token_str = (char*)"+"; break;
        case TOK_MINUS: token_str = (char*)"-"; break;
        case TOK_MULTIPLY: token_str = (char*)"*"; break;
        case TOK_DIVIDE: token_str = (char*)"/"; break;
        case TOK_ASSIGN: token_str = (char*)":="; break;
        case TOK_EQUALTO: token_str = (char*)"="; break;
        case TOK_LESSTHAN: token_str = (char*)"<"; break;
        case TOK_GREATERTHAN: token_str = (char*)">"; break;
        case TOK_NOTEQUALTO: token_str = (char*)"<>"; break;
        case TOK_MOD: token_str = (char*)"MOD"; break;
        case TOK_NOT: token_str = (char*)"NOT"; break;
        case TOK_OR: token_str = (char*)"OR"; break;
        case TOK_AND: token_str = (char*)"AND"; break;

        // Identifiers
        case TOK_IDENT: token_str = (char*)"TOK_IDENT"; break;

        // Literals
        case TOK_INTLIT: token_str = (char*)"TOK_INTLIT"; break;
        case TOK_FLOATLIT: token_str = (char*)"TOK_FLOATLIT"; break;
        case TOK_STRINGLIT: token_str = (char*)"TOK_STRINGLIT"; break;

        // Unknown token
        case TOK_UNKNOWN:     token_str = (char*)"TOK_UNKNOWN"; break;

        default:              token_str = (char*)"=== unmapped token name ===";
    }

  return nextToken;
}
//*****************************************************************************
// Report what we found
void output(string what) {
  cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.
bool first_of_program(void);
//*****************************************************************************
// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
bool first_of_program(void) 
{
    return nextToken == TOK_PROGRAM;
}
bool first_of_statement()
{
    // This is a simplified check; you might want to expand this 
    // depending on the rest of your grammar.
    return nextToken == TOK_IDENT || 
           nextToken == TOK_BEGIN || 
           nextToken == TOK_IF || 
           nextToken == TOK_WHILE || 
           nextToken == TOK_READ || 
           nextToken == TOK_WRITE || nextToken == TOK_THEN;
}

bool isfactor()
{
    return nextToken ==TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_NOT || nextToken == TOK_MINUS || nextToken == TOK_OPENPAREN || nextToken == TOK_CLOSEPAREN;
}
ProgramNode* program() {
    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
    else {
        auto text = yytext;
        if(printParse)
        cout << "found |" << text << "| PROGRAM" << endl;
    }
    auto text = yytext;
    string programName = text;
    if(printParse)
    cout << psp() << "enter <program>" << endl;
    ++level;

    // Consume PROGRAM token
    lex();

    // Get identifier
    if (nextToken != TOK_IDENT)
        throw "'Identifier' expected";
    else {
        auto text = yytext;
        if(printParse)
        cout << psp() << "found |" << text << "| IDENTIFIER" << endl;
        programName = yytext;
        lex(); // consume IDENT
    }

    // Get semicolon
    if (nextToken != TOK_SEMICOLON)
        throw "';' expected";
    else {
        auto text = yytext;
        if(printParse)
        cout << psp() << "found |" << text << "| SEMICOLON" << endl;
        lex(); // consume SEMICOLON
    }

    // Go to block
    //cout << psp() << "enter <block>" << endl;
    auto text1 = yytext;
    if(printParse)
    cout << psp() <<"found |" << text1 << "| BLOCK" << endl;
    ProgramNode* newProgramNode = new ProgramNode(block());

    --level;
    if(printParse)
    cout << psp() << "exit <program>" << endl;
    return newProgramNode;
}

BlockNode* block() {
    if(printParse)
    cout << psp() << "enter <block>" << endl;
    level++;
    BlockNode* newblockNode = new BlockNode(level, compoundstmnt());

    lex(); // Advance to the next token

    level--; // Exit block level
    if(printParse)
    cout << psp() << "exit <block>" << endl;
    return newblockNode;
}

CompoundStatementNode* compoundstmnt() {
    CompoundStatementNode* newcompoundStmt = new CompoundStatementNode(level, statement());
    return newcompoundStmt;
}
StatementNode* statement() 
{
     if (nextToken == TOK_VAR)
    {
  
        lex();  // consume VAR
        bool looper = true; 
        
        string type, id1;
        while(looper)
        {
           id1 = yytext;
            // Check if identifier is already in the symbol table
        
            if(printParse)
            cout << psp() << "found |" << id1 << "| IDENTIFIER" << endl;
            lex();

            if (nextToken != TOK_COLON)
                throw "':' expected";
            else 
            {
                auto text = yytext;
                if(printParse)
                cout << psp() << "found |" << text << "| COLON" << endl;
            }
            lex();

            // get type (assuming only two types: INTEGER and REAL)
            if (nextToken != TOK_INTEGER && nextToken != TOK_REAL)
                throw "'INTEGER' or 'REAL' expected";
            else   
            {
                type = yytext;
                if(printParse)
                cout << psp() << "found |" << type << "| TYPE" << endl;  
            }
            lex();

            if (nextToken != TOK_SEMICOLON)
                throw "';' expected";
            else   
            {
                auto text = yytext;
                if(printParse)
                cout << psp() << "found |" << text << "| SEMICOLON" << endl;
            }
            if(printParse)
            cout << psp()  << "-- idName: |" << id1 << "| idType: |" << type << "| --" <<  endl;
            if (symbolTable.find(id1) != symbolTable.end()) 
            {
                throw "101: identifier declared twice";
            }
            else symbolTable.insert({id1, 0});
            {
                lex();
            }
            auto text = yytext;

            if (nextToken == TOK_BEGIN && nextToken != TOK_END)
            {
                looper = false;
            }
        }
    }

    while (first_of_statement()) {
        StatementNode* stmt = nullptr;

        if (nextToken == TOK_IDENT) {
            stmt = assignment();
        }
        else if (nextToken == TOK_BEGIN) {
            stmt = begin();
            return stmt;
        }
        else if (nextToken == TOK_IF) {
            stmt = ifstatement();
        }
        else if (nextToken == TOK_WHILE) {
            stmt = whilestatement();
        }
        else if (nextToken == TOK_READ) {
            stmt = readstatement();
        }
        else if (nextToken == TOK_WRITE) {
            stmt = writestatement();
            return stmt;
        }
        else {
            throw "Syntax error: unexpected token in <statement>";
        }

        if (nextToken == TOK_SEMICOLON) {
            if(printParse)
            cout << psp() << "found |" << yytext << "| SEMICOLON" << endl;
            lex();
        }

        return stmt;
    }
    return nullptr;
}
AssignmentNode* assignment()
{   
    if(printParse)
    cout << psp() << "found |" << yytext << "| STATEMENT" << endl;
    if(printParse)
    cout << psp() <<  "enter <assignment>" << endl;
    level++;
    auto text = yytext;
    if(printParse)
    cout << psp() << "found |" << text << "| IDENTIFIER" << endl;
    if(printParse)
    cout << psp() << text << endl;
    string identifier = yytext;
    lex(); // consume identifier
        
    text = yytext; 
    if (nextToken != TOK_ASSIGN)
        throw "':=' expected";
    else 
    {
        if(printParse)
        cout << psp() << "found |" << text << "| ASSIGN" << endl;
        lex(); //consume Assign
    }
        //lex();
    text = yytext; 
    //cout  << text <<<< endl;

    ExpressionNode* expr = expression();
    level--;
    if(printParse)
    cout << psp() <<  "exit <assignment>" << endl;
    return new AssignmentNode(level, identifier, expr);
}
BeginNode* begin()
{
        auto text = yytext;
        if(beginNUM >= 1) 
        {
            if(printParse)
            cout << psp() << "found |" << text << "| STATEMENT" << endl;
        }
        if(printParse)
        cout << psp() << "found |" << text << "| BEGIN" << endl;
        beginNUM++;
        if(printParse)
        cout << psp() <<  "enter <compound_stmt>" << endl;
        level++;
        if (nextToken == TOK_BEGIN) {lex();} // consume BEGIN
        vector<StatementNode*> statements;
        //text = yytext; 
        
        while (nextToken != TOK_END && nextToken != TOK_EOF) 
        {
            StatementNode* stmt = statement();
            if (stmt) {
            statements.push_back(stmt); // Add each statement to the vector
        }
            if (nextToken == TOK_SEMICOLON)
            {
                auto text = yytext;
                if(printParse)
                 cout << psp() << "found |" << text << "| SEMICOLON" << endl;
                lex();
            }
        }
        text = yytext;
        //cout << psp() <<text << endl;
        if(nextToken == TOK_EOF || nextToken == TOK_END)
        {
            level--;
            if(printParse)
            cout << psp() << "found |END| END" << endl;
            lex();
        }
        else 
        {
            level--;
        }
        if(printParse)
        cout << psp() << "exit <compound_stmt>" << endl;
    return new BeginNode(level, statements);
}
IfNode* ifstatement()
{
        auto text = yytext; 
        if(printParse)
        cout << psp() << "found |" << text << "| STATEMENT" << endl;
        if(printParse)
        cout << psp() <<  "enter <if>" << endl;
        level++;

        lex(); // consume IF
        text = yytext; 

       // cout << psp() << "found |"  << text << "| EXPRESSION" << endl;
       ExpressionNode* condition = expression();
        
        if (nextToken != TOK_THEN)
            throw "'THEN' expected";
        level--;
        text = yytext;
        if(printParse)
        cout << psp() << "found |"  << text << "| THEN" << endl;
        if(printParse)
        cout << psp() << "enter <then>" << endl;
        level++;
        lex(); // consume THEN

        StatementNode* thenStmt = compoundstmnt();
        level--;
        if(printParse)
        cout << psp() << "exit <then>" << endl;

        StatementNode* elseStmt = nullptr;
        if (nextToken == TOK_ELSE) 
        {
            text = yytext;
            if(printParse)
            cout << psp() << "found |"  << text << "| ELSE" << endl;
            if(printParse)
            cout << psp() << "enter <else>" << endl; 
            level++;
            lex(); // consume ELSE
            
            elseStmt = compoundstmnt();
            level--;
            if(printParse)
            cout << psp() << "exit <else>" << endl; 
        }
        if(printParse)
        cout << psp() <<  "exit <if>" << endl;
    return new IfNode(level, condition, thenStmt, elseStmt);
}
WhileNode* whilestatement()
{
        auto text = yytext; 
        if(printParse)
        cout << psp() << "found |" << text << "| STATEMENT" << endl;
        if(printParse)
        cout << psp() <<  "enter <while>" << endl;
        level++;
        lex(); // consume WHILE
        
        text = yytext;
       // cout << psp() << "found |" << text << "| EXPRESSION" << endl;
        ExpressionNode* condition = expression();
        StatementNode* body = statement();
        //lex();
        text = yytext;
        //cout << "while" << text << endl;
        level--;
        if(printParse)
        cout << psp() <<  "exit <while>" << endl;
    return new WhileNode(level, condition, body);
}
ReadNode* readstatement()
{
        auto text = yytext; 
        if(printParse)
        cout << psp() << "found |" << text << "| STATEMENT" << endl;
        if(printParse)
        cout << psp() <<  "enter <read>" << endl;
        level++;
        lex(); // consume READ
        if (nextToken != TOK_OPENPAREN)
            throw "'(' expected";
        
        text = yytext;
        if(printParse)
        cout << psp() << "found |" << text << "| OPENPAREN" << endl;
        lex(); // consuem parnt=

        if (nextToken != TOK_IDENT)
            throw "Identifier expected";
        text = yytext;
        if(printParse)
        cout << psp() << "found |" << text << "| IDENTIFIER" << endl;
        if(printParse)
        cout << psp() << text << endl;
        string identifier = text;
        lex();

        if (nextToken != TOK_CLOSEPAREN)
            throw "')' expected";


        text = yytext;
        if(printParse)
        cout << psp() << "found |" << text << "| CLOSEPAREN" << endl;
        lex();
        level--;
        if(printParse)
        cout << psp() <<  "exit <read>" << endl;
    return new ReadNode(level, identifier);
}
WriteNode* writestatement()
{
        auto text = yytext; 
        if(printParse)
        cout << psp() << "found |" << text << "| STATEMENT" << endl;
        if(printParse)
        cout << psp() <<  "enter <write>" << endl;
        lex(); // consume WRITE
        level++;
        string content;
        int type;

        if (nextToken != TOK_OPENPAREN)
            throw "'(' expected";
        
        else 
        {
          auto text = yytext; 
          if(printParse)
          cout << psp() << "found |" << text << "| OPENPAREN" << endl;
          lex();
        }
        //lex();

        if (nextToken == TOK_STRINGLIT || nextToken == TOK_IDENT)
        {
            type = nextToken; 
            if(printParse)
            cout << psp() << "found |" << yytext << "| WRITE" << endl;
            if(printParse)
            cout << psp() << yytext << endl;
            content = yytext;
            lex();
        }
        else
        {
            throw "Write statement expects a string literal or identifier";
        }

        if (nextToken != TOK_CLOSEPAREN)
            throw "')' expected";
        else 
        {
          auto text = yytext; 
          if(printParse)
          cout << psp() << "found |" << text << "| CLOSEPAREN" << endl;

        }
        lex();
        level--;
        if(printParse)
        cout << psp() << "exit <write>" << endl;
    return new WriteNode(level, content, type);
}
ExpressionNode* expression()
{
    auto text1 = yytext;
    if(printParse)
    cout << psp() <<"found |" << text1 << "| EXPRESSION" << endl;
    if(printParse)
    cout << psp() << "enter <expression>" << endl;
    ++level;

    //auto text1 = yytext;
    //cout << psp() <<"found |" << text1 << "| SIMPLE_EXP" << endl;
    SimpleExpNode* firstTerm = simple_Expression();
    ExpressionNode* newExpressionNode= new  ExpressionNode(level, firstTerm);
    //simple_Expression();

    if (nextToken == TOK_GREATERTHAN || nextToken == TOK_LESSTHAN || nextToken == TOK_EQUALTO || TOK_NOTEQUALTO) // unary plus & minus
    {
        if( nextToken == TOK_GREATERTHAN)
        {      

            text1 = yytext;
            if(printParse)
            cout << psp() <<"found |" << text1 << "| GREATERTHAN" << endl;
            if(printParse)
            cout << psp() << text1 << endl;
            text1 = yytext;
            //cout << psp() <<"found |" << text1 << "| SIMPLE_EXP" << endl;

            newExpressionNode->restTermOps.push_back(TOK_GREATERTHAN);  // Store the PLUS operator
            lex();  // Advance to the next token
            newExpressionNode->restTerms.push_back(simple_Expression()); 
        }
        if( nextToken ==  TOK_LESSTHAN)
        {      

            text1 = yytext;
            if(printParse)
            cout << psp() <<"found |" << text1 << "| LESSTHAN" << endl;
            if(printParse)
            cout << psp() << text1 << endl;
            
            text1 = yytext;
            //cout << psp() <<"found |" << text1 << "| SIMPLE_EXP" << endl;
            newExpressionNode->restTermOps.push_back(TOK_LESSTHAN);  // Store the PLUS operator
            lex();  // Advance to the next token
            newExpressionNode->restTerms.push_back(simple_Expression()); 
        }
        if( nextToken ==  TOK_EQUALTO)
        {      

            text1 = yytext;
            if(printParse)
            cout << psp() <<"found |" << text1 << "| EQUALTO" << endl;
            if(printParse)
            cout << psp() << text1 << endl;
            text1 = yytext;
            //cout << psp() <<"found |" << text1 << "| SIMPLE_EXP" << endl;
            newExpressionNode->restTermOps.push_back(TOK_EQUALTO);  // Store the PLUS operator
            lex();  // Advance to the next token
            newExpressionNode->restTerms.push_back(simple_Expression()); 
        }
         if( nextToken == TOK_NOTEQUALTO)
        {      

            text1 = yytext;
            if(printParse)
            cout << psp() <<"found |" << text1 << "| NOTEQUALTO" << endl;
            if(printParse)
            cout << psp() << text1 << endl;
        
            text1 = yytext;
            //cout << psp() <<"found |" << text1 << "| SIMPLE_EXP" << endl;
            newExpressionNode->restTermOps.push_back(TOK_NOTEQUALTO);  // Store the PLUS operator
            lex();  // Advance to the next token
            newExpressionNode->restTerms.push_back(simple_Expression()); 
        }
    }

    --level;
    if(printParse)
    cout << psp() << "exit <expression>" << endl;
    return newExpressionNode;
}

SimpleExpNode* simple_Expression()
{
    auto text = yytext;
    if(printParse)
    cout << psp() <<"found |" << text << "| SIMPLE_EXP" << endl;
    if(printParse)
    cout << psp() << "enter <simple_exp>" << endl;

    ++level;

    text = yytext;
    if(printParse)
    cout << psp() <<"found |" << text << "| TERM" << endl;

    TermNode* firstTerm = term();
    SimpleExpNode* newSimpleExpNode = new SimpleExpNode(level, firstTerm);

    // Loop to handle additional operators and terms
    while (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) 
    {
        if (nextToken == TOK_PLUS) {
            text = yytext;
            if(printParse)
            cout << psp() << "found |" << text << "| PLUS" << endl;
            if(printParse)
            cout << psp() << text << endl;
            newSimpleExpNode->restTermOps.push_back(TOK_PLUS);  // Store the PLUS operator
            lex();  // Advance to the next token
            text = yytext;
            if(printParse)
            cout << psp() <<"found |" << text << "| TERM" << endl;
            newSimpleExpNode->restTerms.push_back(term());  // Parse and store the next term
        }
        else if (nextToken == TOK_MINUS) {
            text = yytext;
            if(printParse)
            cout << psp() << "found |" << text << "| MINUS" << endl;
            if(printParse)
            cout << psp() << text << endl;
            newSimpleExpNode->restTermOps.push_back(TOK_MINUS);  // Store the MINUS operator
            lex();  // Advance to the next token
            text = yytext;
            if(printParse)
            cout << psp() <<"found |" << text << "| TERM" << endl;
            newSimpleExpNode->restTerms.push_back(term());  // Parse and store the next term
        }
        else if (nextToken == TOK_OR) {
            text = yytext;
            if(printParse)
            cout << psp() << "found |" << text << "| OR" << endl;
            if(printParse)
            cout << psp() << text << endl;
            newSimpleExpNode->restTermOps.push_back(TOK_OR);  // Store the OR operator
            lex();  // Advance to the next token
            text = yytext;
            if(printParse)
            cout << psp() <<"found |" << text << "| TERM" << endl;
            newSimpleExpNode->restTerms.push_back(term());  // Parse and store the next term
        }
    }
    --level;
    if(printParse)
    cout << psp() << "exit <simple_exp>" << endl;
    return newSimpleExpNode;
}

TermNode* term()
{
    if(printParse)
    cout << psp() << "enter <term>" << endl;
    ++level;
    auto text1 = yytext;

    FactorNode* firstFactor = factor();
    TermNode* newTermNode = new TermNode(level, firstFactor);
        while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND)
        {
            if(nextToken == TOK_MULTIPLY)
            {
                text1 = yytext;
                if(printParse)
                cout << psp() << "found |*| MULTIPLY" << endl; 
                if(printParse)
                cout << psp() << text1 << endl;
                newTermNode->restFactorOps.push_back(TOK_MULTIPLY);
                lex();
                newTermNode->restFactors.push_back(factor());
            }
            if(nextToken == TOK_DIVIDE)
            {
                text1 = yytext;
                if(printParse)
                cout << psp() << "found |/| DIVIDE" << endl; 
                if(printParse)
                cout << psp() << text1 << endl;
                newTermNode->restFactorOps.push_back(TOK_DIVIDE);
                lex();
                newTermNode->restFactors.push_back(factor());
            }
            if(nextToken == TOK_AND)
            {
                text1 = yytext;
                if(printParse)
                cout << psp() << "found |*| AND" << endl; 
                if(printParse)
                cout << psp() << text1 << endl;
                newTermNode->restFactorOps.push_back(TOK_AND); 
                lex();
                newTermNode->restFactors.push_back(factor());  
            }

    }
    --level;
    if(printParse)
    cout << psp() << "exit <term>" << endl;
    return newTermNode;
}

FactorNode* factor()
{
    if(printParse)
    cout << psp() <<"found |" << yytext << "| FACTOR" << endl;
    if(printParse)
    cout << psp() << "enter <factor>" << endl;
    ++level;
    FactorNode* newFactorNode = nullptr;
    if (nextToken == TOK_INTLIT)
    { 
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| INTLIT" << endl;
        if(printParse)
        cout << psp() << text1 << endl;
        newFactorNode = new IntLitNode(level, atoi(yytext));
        lex();
    }
    else if (nextToken == TOK_FLOATLIT)
    {
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| FLOATLIT" << endl;
        if(printParse)
        cout << psp() << text1 << endl;
        newFactorNode = new FloatNode(level, atof(yytext));
        lex();
    }
    else if (nextToken == TOK_IDENT)
    {   
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| IDENTIFIER" << endl;
        if(printParse)
        cout << psp() << text1 << endl;

        if (symbolTable.find(text1) == symbolTable.end()) 
        {
           throw "104: identifier not declared";
        }
        newFactorNode = new IdNode(level, string(yytext));
        lex();
    }
    else if (nextToken == TOK_NOT)
    {   
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| NOT" << endl;
        if(printParse)
        cout << psp() << text1 << endl;
        lex();
        newFactorNode = new NotNode(level, factor());
        //factor();
    }
    else if (nextToken == TOK_MINUS)
    {   
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| MINUS" << endl;
        if(printParse)
        cout << psp() <<  yytext << endl;
        lex();
        newFactorNode = new MinusNode(level, factor());
        //factor();
    }
    else if (nextToken == TOK_OPENPAREN)
    { 
        auto text1 = yytext;
        if(printParse)
        cout << psp() <<"found |" << text1 << "| OPENPAREN" << endl;
        if(printParse)
        cout << psp() << text1 << endl;
        lex();
        newFactorNode = new NestedExprNode(level, expression());
        //expression();

        if (nextToken != TOK_CLOSEPAREN)
            throw "')' expected";
        else{
          auto text1 = yytext;
          if(printParse)
          cout << psp() <<"found |" << text1 << "| CLOSEPAREN" << endl;
        }
        lex();
    }
    else
    {
        throw "Syntax error: factor expected";
    }

    --level;
    if(printParse)
    cout << psp() << "exit <factor>" << endl;
    return newFactorNode;
}