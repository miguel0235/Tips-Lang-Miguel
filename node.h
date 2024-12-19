#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include <map>

extern bool printDelete; 
using namespace std;
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;
   // shall we print deleting the tree?

class FactorNode;
class StatementNode;
class ExpressionNode;
class BlockNode;
class CompoundStatementNode;

// ProgramNode class
class ProgramNode {
public:
    BlockNode* block; 


    ProgramNode(BlockNode* blockNode);
    ~ProgramNode();
    void printTo(ostream& os);
    void interpret();
};

// Overload operator<< for ProgramNode
ostream& operator<<(ostream& os, ProgramNode& node);
class BlockNode {
public:
    int level = 0;
    CompoundStatementNode* compoundStatement;

    BlockNode(int level, CompoundStatementNode* compoundStmt);
    ~BlockNode();
    void printTo(ostream& os);
    void interpret();
};
ostream& operator<<(ostream& os, BlockNode& node);



class StatementNode {
public:
    int level = 0;  // Recursion level of this node

    virtual void printTo(ostream& os) = 0; // Pure virtual function
    virtual ~StatementNode();
    virtual void interpret() = 0;
};
ostream& operator<<(ostream&, StatementNode&); // Node print operator
// Derived classes for each type of statement

class CompoundStatementNode : public StatementNode {
public:
    int level = 0;
    StatementNode* statement; // Holds a single statement

    CompoundStatementNode(int level, StatementNode* stmt);
    ~CompoundStatementNode();
    void printTo(ostream& os);
    void interpret();
};

// Overload the operator<< to print CompoundStatementNode
ostream& operator<<(ostream& os, CompoundStatementNode& node);
// Assignment Node
class AssignmentNode : public StatementNode {
public:
    int level = 0;  
    string identifier;
    ExpressionNode* expression;

    AssignmentNode(int level, string id, ExpressionNode* expr);
    ~AssignmentNode();
    void printTo(ostream& os) override;
    void interpret();
};

// Begin Node
class BeginNode : public StatementNode {
public:
    int level = 0;  
    vector<StatementNode*> statements;

    BeginNode(int level, const vector<StatementNode*>& stmts);
    ~BeginNode();
    void printTo(ostream& os) override;
    void interpret();
};

// If Node
class IfNode : public StatementNode {
public:
    int level = 0;  
    ExpressionNode* condition;
    StatementNode* thenStmt;
    StatementNode* elseStmt;

    IfNode(int level, ExpressionNode* cond, StatementNode* thenStmt, StatementNode* elseStmt = nullptr);
    ~IfNode();
    void printTo(ostream& os) override;
    void interpret();
};

// While Node
class WhileNode : public StatementNode {
public:
    int level = 0;  
    ExpressionNode* condition;
    StatementNode* body;

    WhileNode(int level, ExpressionNode* cond, StatementNode* body);
    ~WhileNode();
    void printTo(ostream& os) override;
    void interpret();
};

// Read Node
class ReadNode : public StatementNode {
public:
    int level = 0;  
    string identifier;
    ~ReadNode();
    ReadNode(int level, string id);
    void printTo(ostream& os) override;
    void interpret();
};

// Write Node
class WriteNode : public StatementNode {
public:
    int level = 0;    
    string content;
    int type;
    ~WriteNode();
    WriteNode(int level, string content, int type);
    void printTo(ostream& os) override;
    void interpret();
};

class FactorNode {
public:
  int level = 0;                        // recursion level of this node

  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
	                                       // the subclass destructors to be called
  virtual float interpret() = 0;
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator

// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);
    float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
    float interpret();
};

class FloatNode : public FactorNode {
public:
    float float_literal = 0;

    FloatNode(int level, float value);
    ~FloatNode();
    void printTo(ostream & os);
    float interpret();
};
// ---
// ---------------------------------------------------------------------
class NotNode : public FactorNode {
public:
    FactorNode* notPtr = nullptr;

    NotNode(int level, FactorNode* nn);
    void printTo(ostream & os);
    ~NotNode();
    float interpret();
};

class MinusNode : public FactorNode {
public:
    FactorNode* minusPtr = nullptr;

    MinusNode(int level, FactorNode* mn);
    void printTo(ostream & os);
    ~MinusNode();
    float interpret();
};

class NestedExprNode : public FactorNode {
public:
    ExpressionNode* exprPtr = nullptr;

    NestedExprNode(int level, ExpressionNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
    float interpret();
};
ostream& operator<<(ostream&, NestedExprNode&); 

class TermNode {
public: 
    int level = 0; 
    FactorNode* firstFactor = nullptr;
    vector<int> restFactorOps;
    vector<FactorNode*> restFactors;

    TermNode(int level, FactorNode* firstFactor);
    ~TermNode();
    float interpret();
};
ostream& operator<<(ostream&, TermNode&);


class SimpleExpNode {
public:
    int level = 0;                      // Recursion level of this node
    TermNode* firstTerm = nullptr;      // Pointer to the first term in the expression
    vector<int> restTermOps;            // Operators for each additional term (e.g., +, -, or)
    vector<TermNode*> restTerms;        // Additional terms

    SimpleExpNode(int level, TermNode* firstTerm);
    ~SimpleExpNode();
    float interpret();
  
};

// Overloaded << operator for Expression
ostream& operator<<(ostream& , SimpleExpNode&);

class ExpressionNode {
public:
    int level = 0;                      // Recursion level of this node
    SimpleExpNode* firstTerm = nullptr;      // Pointer to the first term in the expression
    vector<int> restTermOps;            // Operators for each additional term (e.g., +, -, or)
    vector<SimpleExpNode*> restTerms;        // Additional terms

    ExpressionNode(int level, SimpleExpNode* firstTerm);
    ~ExpressionNode();
    float interpret();
};

// Overloaded << operator for SimpleExpNode
ostream& operator<<(ostream& , ExpressionNode&);


#endif