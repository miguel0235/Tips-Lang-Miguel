


#include <cmath>
#include "node.h"
#define EPSILON 0.001
static bool truth(float F) {
return !((EPSILON > F) && (F > -EPSILON));
}



//

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("|  ");
}



ProgramNode::ProgramNode(BlockNode* blockNode) 
{
    this->block = blockNode;
}


ProgramNode::~ProgramNode() {
    if (printDelete) {
        cout << "Deleting ProgramNode" << endl;
    }
    delete block;
}
// Print function
void ProgramNode::printTo(ostream& os) {
    os << "(program";
    
    if (block) 
    {
        os << endl;
        os << *block;
        os << endl;
    }
    
    os << "program)";
}


ostream& operator<<(ostream& os, ProgramNode& node) {
    node.printTo(os);
    return os;
}

BlockNode::BlockNode(int level, CompoundStatementNode* compoundStmt) 
{
    this->level = level;
    this->compoundStatement = compoundStmt;
}


BlockNode::~BlockNode() 
{
    if (printDelete) {
        cout << "Deleting BlockNode" << endl;
    }
    delete compoundStatement;
}


void BlockNode::printTo(ostream& os) {
    indent(level-1);
    os << "(block" ;

    os << *compoundStatement;
    
    os << endl;indent(level-1);
    os << "block)";
}


ostream& operator<<(ostream& os, BlockNode& node) {
    node.printTo(os);
    return os;
}
CompoundStatementNode::CompoundStatementNode(int level, StatementNode* stmt) 
{
    this->level = level;
    this->statement = stmt;
}

// Destructor
CompoundStatementNode::~CompoundStatementNode() {
    if (printDelete) {
        //cout << "Deleting StatementNode:CompoundStmtNode" << endl;
    }
    delete statement;
}


void CompoundStatementNode::printTo(ostream& os) {
    //indent(level);
    //os << "(compound_stmt"<< endl;
    

    statement->printTo(os);
    
    
    //indent(level);
    //os << "compound_stmt)" << endl;
}

ostream& operator<<(ostream& os, CompoundStatementNode& node) {
    node.printTo(os);
    return os;
}


StatementNode::~StatementNode() {}

ostream& operator<<(ostream& os, StatementNode& fn) {
  os << endl; indent(fn.level-1); os << "(statement";
  fn.printTo(os);
  os << endl; indent(fn.level-1); os << "statement)";
	return os;
}


AssignmentNode::AssignmentNode(int level, string id, ExpressionNode* expr)
{
    this->level = level;
    identifier = id;
    this->expression = expr;
}
AssignmentNode::~AssignmentNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:AssignmentStmtNode" << endl;
    }
    delete expression;
}
void AssignmentNode::printTo(ostream& os) {
    os << endl; indent(level); os << "(assignment_stmt ( " << identifier << " := )";
    os << *expression;
    os << endl; indent( level); os << "assignment_stmt)";
}

BeginNode::BeginNode(int level, const vector<StatementNode*>& stmts)
{

    this->level = level;
    this->statements = stmts;
}
BeginNode::~BeginNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:CompoundStmtNode" << endl;
    }
    for (auto stmt : statements) {
        delete stmt;
    }
    statements.clear();
}
void BeginNode::printTo(ostream& os) {
    os << endl; indent(level);os << "(compound_stmt";
    for (auto stmt : statements) 
    {
        stmt->printTo(os);
    }
    os << endl;indent(level);os << "compound_stmt)";
}

// IfNode implementation
IfNode::IfNode(int level, ExpressionNode* condStmt, StatementNode* thenStmt, StatementNode* elseStmt)
{
    this->level = level;
    this->condition = condStmt;
    this->thenStmt = thenStmt;
    this->elseStmt = elseStmt;
}
IfNode::~IfNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:IfStmtNode" << endl;
    }
    delete condition;
    delete thenStmt;
    delete elseStmt;
}

void IfNode::printTo(ostream& os) {
     os << endl;indent( level);
    os << "(if_stmt" << *condition;


    if (thenStmt) 
    {
        os << endl;indent(level);os << "(then";
        thenStmt->printTo(os);
         os << endl;indent(level);os << "then)";
    }
    if (elseStmt) {
        os << endl; indent(level);os << "(else";
        elseStmt->printTo(os);
        os << endl;indent(level);os << "else)";
    }

     os << endl;indent(level);os << "if_stmt)";
}

// WhileNode
WhileNode::WhileNode(int level, ExpressionNode* cond, StatementNode* body)
    : condition(cond), body(body) {
    this->level = level;
}
WhileNode::~WhileNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:WhileStmtNode" << endl;
    }
    delete condition;
    delete body;
}
void WhileNode::printTo(ostream& os) {
    os << endl;indent(level);os << "(while_stmt";
    
    os << *condition;
    body->printTo(os);

    os << endl;indent(level);os << "while_stmt)";
}

// ReadNode 
ReadNode::ReadNode(int level, string id)
 {
    this->level = level;
    identifier = id;
}
void ReadNode::printTo(ostream& os) {
    os << endl;indent(level);os << "(read_stmt ( " << identifier << " )" << endl;
    indent(level);os << "read_stmt)";
}
ReadNode::~ReadNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:ReadStmtNode" << endl;
    }
}
// WriteNode 
WriteNode::WriteNode(int level, string content, int type) : content(content), type(type)  {
    this->level = level;
}
void WriteNode::printTo(ostream& os) {
    os << endl;indent(level);
    os << "(write_stmt ( " << content << " )" << endl;
    indent(level);  os << "write_stmt)";
}
WriteNode::~WriteNode() {
    if (printDelete) {
        cout << "Deleting StatementNode:WriteStmtNode" << endl;
    }
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
    os << endl;
    indent(fn.level); os << "(factor";
  fn.printTo(os);
  os << endl; indent(fn.level); os << "factor)";
	return os;
}
// ---------------------------------------------------------------------
IdNode::IdNode(int l, string name) {
  level = l - 1;
  id = new string(name);
}
IdNode::~IdNode() {
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode" << endl;
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream& os) {
	 os << " ( IDENT: " << *id << " )";
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int l, int value) {
  level = l - 1;
  int_literal = value;
}
IntLitNode::~IntLitNode() {
    if (printDelete) {
        cout << "Deleting FactorNode:IntLitNode" << endl;
    }
}

void IntLitNode::printTo(ostream& os) {
	os << " ( INTLIT: " << int_literal << " )";
}

FloatNode::FloatNode(int l, float value) {
  level = l - 1;
  float_literal = value;
}
FloatNode::~FloatNode() {
    if (printDelete) {
        cout << "Deleting FactorNode:FloatLitNode" << endl;
    }
}
void FloatNode::printTo(ostream& os) {
	os << " (FloatLit: " << float_literal << " )";
}

//--------------------
//minus node
MinusNode::MinusNode(int level, FactorNode* mn) {
  level = level;
   minusPtr = mn;
}
void MinusNode::printTo(ostream& os) {
	indent( level);os << " (- " << *minusPtr << " )";
}
MinusNode::~MinusNode() {
    if (printDelete) {
        cout << "Deleting FactorNode:MinusNode" << endl;
    }
    delete minusPtr;
}

//---------------------

NotNode::NotNode(int level, FactorNode* nn) {
   this->level = level;
   notPtr = nn;
}
void NotNode::printTo(ostream& os) 
{
    // I was having a indention problem when running this 
    // and this was the only fix that worked. if i changed 
    // one indention for factor this one would break
    // so chat helped to change this. 
    os << " ma(NOT"; 
    os << endl; indent(level); os << "(factor";
    notPtr->printTo(os);
    
    os << endl; indent(level); os << "factor)";
}
NotNode::~NotNode() {
    if (printDelete) {
        cout << "Deleting FactorNode:NotNode" << endl;
    }
    delete notPtr;
}

NestedExprNode::NestedExprNode(int level, ExpressionNode* en) {
  level = level;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) 
{
	os << *exprPtr;
}
NestedExprNode::~NestedExprNode() {
    if (printDelete) {
        cout << "Deleting FactorNode:NestedExprNode" << endl;
    }
    delete exprPtr;
}

TermNode::TermNode(int level, FactorNode* firstFactor)  
{
    this->level = level;
    this->firstFactor = firstFactor;
}

// Destructor for TermNode
TermNode::~TermNode() {
    if (printDelete) {
        cout << "Deleting TermNode" << endl;
    }
    delete firstFactor;
    for (auto factor : restFactors) {
        delete factor;
    }
    restFactors.clear();
}


ostream& operator<<(ostream& os, TermNode& tn) {

     cout << endl; indent( tn.level -1);os << "(term";

        //indent( tn.level );
    os << endl;
    indent( tn.level );os << "(factor";
    tn.firstFactor->printTo(os);
    os<< endl;
    indent(tn.level);os << "factor)";
    

    // Print each operator and corresponding factor
    for (size_t i = 0; i < tn.restFactors.size(); ++i) {
        // Print operator indentation
        
        // Print operator
        if (tn.restFactorOps[i] == TOK_MULTIPLY) {
             cout << endl; indent( tn.level -1 );os << "*";
        } else if (tn.restFactorOps[i] == TOK_DIVIDE) {
             cout << endl; indent( tn.level-1 );os << "/";
        } else if (tn.restFactorOps[i] == TOK_AND) {
             cout << endl; indent( tn.level-1 );os << "AND";
        } else {
            os << "Unknown";
        }

        // Print the factor following the operator
        os<< endl;indent( tn.level);os << "(factor";;
        //os<< endl;os << "(test";os<< endl;
        tn.restFactors[i]->printTo(os);
        
        os<< endl;indent( tn.level);os << "factor)";
    }

    // Closing tag for term
    os<< endl;indent( tn.level-1);os << "term)";
    
    return os;
}

SimpleExpNode::SimpleExpNode(int level, TermNode* firstTerm) 
    : level(level), firstTerm(firstTerm) {}


SimpleExpNode::~SimpleExpNode() {
    if (printDelete) {
        cout << "Deleting SimpleExpNode" << endl;
    }
    delete firstTerm;
    for (auto term : restTerms) {
        delete term;
    }
    restTerms.clear();
}



ostream& operator<<(ostream& os, SimpleExpNode& se) 
{
    indent( se.level -1);os << "(simple_exp";

    // Print the first term
    if (se.firstTerm) {
        os << *se.firstTerm;
    }

    // Print each additional operator
    for (size_t i = 0; i < se.restTerms.size(); ++i) {
        int op = se.restTermOps[i];
        //indent( se.level - 1);
        // Print operator
        if (op == TOK_PLUS) 
        {
            os<< endl;indent( se.level - 1); os << "+";
        } else if (op == TOK_MINUS) {
             os<< endl;indent( se.level - 1);os << "-";
        } else if (op == TOK_OR) {
             os<< endl;indent( se.level -1);os << "or";
        } else {
             os<< endl;indent( se.level - 1);os << "unknown_op";
        }
        
        // Print the next term in restTerms
        if (se.restTerms[i]) {
            os << *se.restTerms[i];
        }
    }

    os << endl;indent( se.level- 1);os << "simple_exp)";
    return os;
}



ExpressionNode::ExpressionNode(int level, SimpleExpNode* firstTerm) 
{
    this-> level = level;
    this->firstTerm = firstTerm;
}

ExpressionNode::~ExpressionNode() {
    if (printDelete) {
        cout << "Deleting ExpressionNode" << endl;
    }
    delete firstTerm;
    for (auto term : restTerms) {
        delete term;
    }
    restTerms.clear();
}


ostream& operator<<(ostream& os, ExpressionNode& se)
{
    os << endl;indent( se.level-1);os << "(expression"  << endl;
    // Print the first term
    if (se.firstTerm) {
        os << *se.firstTerm;
    }

    // Print each  
    for (size_t i = 0; i < se.restTerms.size(); ++i) {
        int op = se.restTermOps[i];

        // Print operator
        if (op == TOK_GREATERTHAN) 
        {
            os<< endl;indent( se.level - 1); os << ">" << endl;
        } else if (op == TOK_LESSTHAN) {
             os<< endl;indent( se.level - 1);os << "<" << endl;
        } else if (op == TOK_EQUALTO) {
             os<< endl;indent( se.level -1);os << "=" << endl;
        } else if (op == TOK_NOTEQUALTO) {
             os<< endl;indent( se.level -1);os << "<>" << endl;
        } else {
             os<< endl;indent( se.level - 1);os << "unknown_op";
        }

        // Print 
        if (se.restTerms[i]) {
            os << *se.restTerms[i];
        }
    }
  os << endl;indent( se.level- 1);os << "expression)";
    return os;
}

void ProgramNode::interpret() {
    block->interpret();
}

void BlockNode::interpret() {
    compoundStatement->interpret();
}

void CompoundStatementNode::interpret() {
    statement->interpret();
}

void AssignmentNode::interpret() 
{
    // powerpoint
    auto variable = symbolTable.find(identifier);
    variable->second = expression->interpret();
}

void BeginNode::interpret() 
{
    int length = statements.size();
    for (int i = 0; i < length; ++i) {
        statements[i]->interpret();
    }
}

void IfNode::interpret() 
{
    if(condition->interpret())
    {
    if (thenStmt) {
        thenStmt->interpret();
        
    }} else if (elseStmt) {
        elseStmt->interpret();
    }
}

void WhileNode::interpret() 
{
    while (condition->interpret()) 
    {
        body->interpret();
    }
}

void ReadNode::interpret() 
{
    float value;
    cout << "Enter value" << identifier << ": ";
    cin >> value;

    auto variable = symbolTable.find(identifier);
    variable->second = value;
}

void WriteNode::interpret() 
{
    // from power point, had to add the type to node and parse
    switch(type) {
        case TOK_STRINGLIT:
        // Strip leading and trailing quote characters
            cout << content.substr(1, content.length() - 2) << endl;
            break;
        case TOK_IDENT:
        // Lookup variable and print its value
            auto variable = symbolTable.find(content);
            cout << variable->second << endl;
            break;
    }
}


float IdNode::interpret() 
{
    auto variable = symbolTable.find(*id);
    return variable->second;
}

float IntLitNode::interpret() 
{
    return int_literal;
}

float FloatNode::interpret() {
    return float_literal;
}

float MinusNode::interpret() 
{
    return -minusPtr->interpret();
}

float NotNode::interpret() 
{
    return !truth(notPtr->interpret());
}

float NestedExprNode::interpret() 
{
    return exprPtr->interpret();
}

float TermNode::interpret() 
{
    float result = firstFactor->interpret();
    int length = restFactorOps.size();
    for (int i = 0; i < length; ++i) {
        float nextValue = restFactors[i]->interpret();
        switch (restFactorOps[i]) {
            case TOK_MULTIPLY:
                result *= nextValue;
                break;
            case TOK_DIVIDE:
                result /= nextValue;
                break;
            case TOK_AND:
                result = result && nextValue;
                break;
            default:
                throw "error";
        }
    }

    return result;
}

float SimpleExpNode::interpret() {
    float result = firstTerm->interpret();
    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        float nextValue = restTerms[i]->interpret();
        switch (restTermOps[i]) {
            case TOK_PLUS:
                result += nextValue;
                break;
            case TOK_MINUS:
                result -= nextValue;
                break;
            case TOK_OR:
                result = result || nextValue;
                break;
            default:
                throw "error";
        }
    }

    return result;
}

float ExpressionNode::interpret() {
    float result = firstTerm->interpret();
    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        float nextValue = restTerms[i]->interpret();
        switch (restTermOps[i]) {
            case TOK_GREATERTHAN:
                result = result > nextValue;
                break;
            case TOK_LESSTHAN:
                result = result < nextValue;
                break;
            case TOK_EQUALTO:
                result = result - nextValue < EPSILON;
                break;
            case TOK_NOTEQUALTO:
                result = result - nextValue >= EPSILON;
                break;
            default:
                throw "error";
        }
    }

    return result;
}