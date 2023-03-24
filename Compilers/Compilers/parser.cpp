#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>

// Define the different types of tokens
enum TokenType {
    program,
    block,
    printStatement

   
};

// Define a struct to represent a token
struct ParseToken {
    TokenType type;
    std::string value;
};

// Define a function to tokenize a string of C++ code
/*std::vector<Token> tokenize(std::string code) {
    // ... implementation of tokenizing function ...
    // Returns a vector of Token objects
}*/

// Define a function to parse a stream of tokens
// Returns a pointer to the root node of the syntax tree
//TreeNode* parse(std::vector<Token>& tokens) {
    // ... implementation of parsing function ...
    // Returns a pointer to the root node of the syntax tree
//}

// Define a struct to represent a node in the syntax tree
struct TreeNode {
    std::string type;
    std::vector<TreeNode*> children;
};

using namespace std;

class Parser {

public:
	~Parser();
	void parse(list<Token>);

private:

};

Parser::~Parser()
= default;

/*
void parseProgram() {
//Block $
addNode(root, program);
parseBlock();
match("$");
moveUp();
}

void parseBlock() {
// { StatementList }
addNode(branch, block);
match("{");
parseStatementList();
match("}");

moveUp();
}

void parseStatement() {
addNode(branch, statement);
if (token = printStatement) 
    parsePrint();
else if (token = assignmentStatement)
    parseAssignmentStatement();
else if (token = varDecl) 
    parseVarDecl();
else if (token = while)
    parseWhile();
else if (token = if)
    parseIf();
else
    parseBlock();

moveUp();
}

void parseStatementList() {
addNode(branch, statementList);
if (token = statement) {
    parseStatement();
    parseStatementList();
 }
 else
    //continue

moveUp();
}

void parsePrintStatement() {
addNode(branch, printStatement);
match("print");
match("(");
parseStatement();
match(")");
moveUp();
}

void parseAssignmentStatement() {
addNode(branch, assignmentStatement);
ParseId();
match("=");
parseExpression();
moveUp();
}

void parseVarDecl() {
addNode(branch, varDecl);
parseType();
parseId();
moveUp();
}

void parseWhileStatement() {
addNode(branch, while);
match("while");
parseBooleanExpr();
parseBlock();
moveUp();
}

void parseIfStatement() {
addNode(branch, if);
match("if");
parseBooleanExpr();
parseBlock();
moveUp();
}

void parseExpr() {
addNode(branch, expr);
if (token = 

moveUp();
}

parseIntExpr() {
addNode(branch, intExpr);

moveUp();
}

parseStringExpr() {
addNode(branch, stringExpr);

moveUp();
}

parseBooleanExpr() {
addNode(branch, boolExpr);

moveUp();
}

parseId() {
addNode(branch, id);

moveUp();
}

parseCharList() {
addNode(branch, charList);

moveUp();
}

parseType() {
addNode(branch, type);

moveUp();
}

parseChar() {
addNode(branch, char);

moveUp();
}

parseSpace() {
addNode(branch, space);

moveUp();
}

parseDigit() {
addNode(branch, digit);

moveUp();
}

parseBoolOp() {
addNode(branch, boolop);

moveUp();
}

parseBoolVal() {
addNode(branch, boolVal);

moveUp();
}

parseIntOp() {
addNode(branch, intOp);
match(+);
moveUp();
}

parseComment() {
addNode(branch, comment);

moveUp();
}





void match(expected) {
	x = checkExpected
	if x addNode(leaf, x)
		else error
}
*/
void Parser::parse(list<Token> masterTokenStreamLexed) {
    
    for (auto const& i : masterTokenStreamLexed) {
        tokenType = i.tokenType;
        value = i.value;
        parse
        //std::cout << i.value << "hit" << endl;
    }
    //std::string code = "int main() { return 0; }";
    //std::vector<Token> tokens = tokenize(code);
    //TreeNode* root;
    //parse(tokens)
    // ... do something with the syntax tree ...
    //return 0;
}
