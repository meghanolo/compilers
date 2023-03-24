#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

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

struct Node {
    string name;
    vector<Node*> children;
};


class Parser {

public:
	~Parser();
	void parse(list<Token>);

private:

};

Parser::~Parser()
= default;

void printCST(Node* node, int indent = 0) {
    for (int i = 0; i < indent; i++) {
        cout << "  ";
    }
    cout << node->name << endl;

    for (int i = 0; i < node->children.size(); i++) {
        printCST(node->children[i], indent + 1);
    }
}

void parseProgram(list<Token> lexerList) {
//Block $
    Node* program = new Node();
    program->name = "Program";

    lexerList.pop_front();

    parseBlock(lexerList);
    match("$");
//moveUp();
}

void parseBlock(list<Token> lexerList) {
// { StatementList }
//addNode(branch, block);
//match("{");
parseStatementList(lexerList);
match("}");

//moveUp();
}

void parseStatement(list<Token> lexerList) {
addNode(branch, statement);
if (token = printStatement) 
    parsePrint();
else if (token = assignmentStatement)
    parseAssignmentStatement(lexerList);
else if (token = varDecl) 
    parseVarDecl(lexerList);
else if (token = while)
    parseWhile(lexerList);
else if (token = if)
    parseIf();
else
    parseBlock(lexerList);

moveUp();
}

void parseStatementList(list<Token> lexerList) {
addNode(branch, statementList);
if (token = statement) {
    parseStatement(lexerList);
    parseStatementList(lexerList);
 }
 else
    //continue

moveUp();
}

void parsePrintStatement(list<Token> lexerList) {
addNode(branch, printStatement);
match("print");
match("(");
parseStatement(lexerList);
match(")");
moveUp();
}

void parseAssignmentStatement(list<Token> lexerList) {
addNode(branch, assignmentStatement);
ParseId(lexerList);
match("=");
parseExpression(lexerList);
moveUp();
}

void parseVarDecl(list<Token> lexerList) {
addNode(branch, varDecl);
parseType(lexerList);
parseId(lexerList);
moveUp();
}

void parseWhileStatement(list<Token> lexerList) {
addNode(branch, while);
match("while");
parseBooleanExpr(lexerList);
parseBlock(lexerList);
moveUp();
}

void parseIfStatement(list<Token> lexerList) {
addNode(branch, if);
match("if");
parseBooleanExpr(lexerList);
parseBlock(lexerList);
moveUp();
}

void parseExpr(list<Token> lexerList) {
addNode(branch, expr);
if (token = 

moveUp();
}

parseIntExpr(list<Token> lexerList) {
addNode(branch, intExpr);

moveUp();
}

void parseStringExpr(list<Token> lexerList) {
addNode(branch, stringExpr);

moveUp();
}

void parseBooleanExpr(list<Token> lexerList) {
addNode(branch, boolExpr);

moveUp();
}

void parseId(list<Token> lexerList) {
addNode(branch, id);

moveUp();
}

void parseCharList(list<Token> lexerList) {
addNode(branch, charList);

moveUp();
}

void parseType(list<Token> lexerList) {
addNode(branch, type);

moveUp();
}

void parseChar(list<Token> lexerList) {
addNode(branch, char);

moveUp();
}

void parseSpace(list<Token> lexerList) {
addNode(branch, space);

moveUp();
}

void parseDigit(list<Token> lexerList) {
addNode(branch, digit);

moveUp();
}

void parseBoolOp(list<Token> lexerList) {
addNode(branch, boolop);

moveUp();
}

void parseBoolVal(list<Token> lexerList) {
addNode(branch, boolVal);

moveUp();
}

void parseIntOp(list<Token> lexerList) {
addNode(branch, intOp);
match("+");
moveUp();
}

void parseComment(list<Token> lexerList) {
addNode(branch, comment);

moveUp();
}


void match(string expected) {
	//x = checkExpected
	//if x addNode(leaf, x)
		//else error
}
Node* parseList(list<Token> masterTokenStreamLexed) {
    Node* root = new Node();
    root->name = "Program";

    parseProgram(masterTokenStreamLexed);

    // Implement the parsing logic here

    return root;
}

void Parser::parse(list<Token> masterTokenStreamLexed) {
    
    string tokenType;
    string value;
    for (auto const& i : masterTokenStreamLexed) {
        tokenType = i.tokenType;
        value = i.value;
        
        //std::cout << i.value << "hit" << endl;
    }

    Node* root = parseList(masterTokenStreamLexed);
    printCST(root);
}
