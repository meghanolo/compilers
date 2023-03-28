#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

int current = 0;

struct Node {
    string name;
    vector<Node*> children;
};

class Parser {

public:
	~Parser();                                                           
	void parse(vector<Token>);

private:

};

Parser::~Parser()
= default;

void match(string expected, vector<Token> lexerList) {

    auto actual = lexerList.at(current).tokenType;
    if (actual == expected) {
        current++;
        return;
    }
    else
        cout << "ERROR! Expected token " << expected << ". Received " << actual << "." << endl;
}

void printCST(Node* node, int indent = 0) {
    for (int i = 0; i < indent; i++) {
        cout << "  ";
    }
    cout << node->name << endl;

    for (int i = 0; i < node->children.size(); i++) {
        printCST(node->children[i], indent + 1);
    }
}

Node* parseChar(vector<Token> lexerList) {
    Node* Char = new Node();
    Char->name = "Char";

    match("charToken", lexerList);

    return Char;
}

Node* parseSpace(vector<Token> lexerList) {
    Node* space = new Node();
    space->name = "space";

    match("spaceToken", lexerList);

    return space;
}


Node* parseCharList(vector<Token> lexerList) {
    Node* charList = new Node();
    charList->name = "charList";

    string token = lexerList.at(current).tokenType;

    if (token == "charToken") {
        charList->children.push_back(parseChar(lexerList));
        charList->children.push_back(parseCharList(lexerList));
    }
    else if (token == "spaceToken") {
        charList->children.push_back(parseSpace(lexerList));
        charList->children.push_back(parseCharList(lexerList));
    }
    else {
        //continue
    }
    return charList;
}

Node* parseType(vector<Token> lexerList) {
    Node* type = new Node();
    type->name = "type";

    string token = lexerList.at(current).tokenType;

    if (token == "intToken")
        match("intToken", lexerList);
    else if (token == "boolToken")
        match("boolToken", lexerList);
    else if (token == "stringToken")
        match("stringToken", lexerList);

    return type;
}

Node* parseDigit(vector<Token> lexerList) {
    Node* digit = new Node();
    digit->name = "digit";

    match("digitToken", lexerList);

    return digit;
}

Node* parseBoolOp(vector<Token> lexerList) {
    Node* boolOp = new Node();
    boolOp->name = "boolOp";

    string token = lexerList.at(current).tokenType;

    if (token == "boolEqualToken")
        match("boolEqualToken", lexerList);
    else if (token == "boolNotToken")
        match("boolNotToken", lexerList);

    return boolOp;
}

Node* parseBoolVal(vector<Token> lexerList) {
    Node* boolVal = new Node();
    boolVal->name = "boolVal";

    return boolVal;
}

Node* parseIntOp(vector<Token> lexerList) {
    Node* intOp = new Node();
    intOp->name = "intOp";

    match("addToken", lexerList);

    return intOp;
}

Node* parseComment(vector<Token> lexerList) {
    Node* comment = new Node();
    comment->name = "comment";

    return comment;
}

Node* parseBlock(vector<Token> lexerList) {
// { StatementList }

    Node* block = new Node();
    block->name = "Block";

    match("openBracToken", lexerList);
    block->children.push_back(parseStatementList(lexerList));
    match("closeBracToken", lexerList);

    return block;

}
Node* parseExpr(vector<Token> lexerList) {

    Node* expression = new Node();
    expression->name = "Expression";

    string token = lexerList.at(current).tokenType;

    if (token == "charToken")
        expression->children.push_back(parseChar(lexerList));
    else if (token == "intToken")
        expression->children.push_back(parseVarDecl(lexerList));
    else if (token == "boolToken")
        expression->children.push_back(parseVarDecl(lexerList));
    else if (token == "stringToken")
        expression->children.push_back(parseVarDecl(lexerList));

    return expression;
}

Node* parseId(vector<Token> lexerList) {
    Node* id = new Node();
    id->name = "id";

    id->children.push_back(parseChar(lexerList));

    return id;
}

Node* parseAssignmentStatement(vector<Token> lexerList) {

    Node* assignmentStatement = new Node();
    assignmentStatement->name = "assignmentStatement";

    assignmentStatement->children.push_back(parseId(lexerList));
    match("assignmentToken", lexerList);
    assignmentStatement->children.push_back(parseExpr(lexerList));

    return assignmentStatement;
}

Node* parseBooleanExpr(vector<Token> lexerList) {

    Node* boolExpr = new Node();
    boolExpr->name = "boolExpr";

    string token = lexerList.at(current).tokenType;

    if (token == "true" || token == "false") {
        boolExpr->children.push_back(parseBoolVal(lexerList));
    }
    else if (token == "openParenToken") {
        match("openParenToken", lexerList);
        boolExpr->children.push_back(parseExpr(lexerList));
        boolExpr->children.push_back(parseBoolOp(lexerList));
        boolExpr->children.push_back(parseExpr(lexerList));
        match("closeParenToken", lexerList);
    }

    return boolExpr;
}


Node* parseIfStatement(vector<Token> lexerList) {

    Node* ifStatement = new Node();
    ifStatement->name = "ifStatement";

    match("ifToken", lexerList);
    ifStatement->children.push_back(parseBooleanExpr(lexerList));
    ifStatement->children.push_back(parseBlock(lexerList));

    return ifStatement;
}

Node* parseStatement(vector<Token> lexerList) {

    Node* statement = new Node();
    statement->name = "Statement";


    string token = lexerList.at(current).tokenType;

    if (token == "printToken")
        statement->children.push_back(parsePrintStatement(lexerList));
    else if (token == "assignmentToken")
        statement->children.push_back(parseAssignmentStatement(lexerList));
    else if (token == "intToken")
        statement->children.push_back(parseVarDecl(lexerList));
    else if (token == "boolToken")
        statement->children.push_back(parseVarDecl(lexerList));
    else if (token == "stringToken")
        statement->children.push_back(parseVarDecl(lexerList));
    else if (token == "whileToken")
        statement->children.push_back(parseWhileStatement(lexerList));
    else if (token == "ifToken")
        statement->children.push_back(parseIfStatement(lexerList));
    else
        statement->children.push_back(parseBlock(lexerList));

    return statement;

}
Node* parsePrintStatement(vector<Token> lexerList) {

    Node* printStatement = new Node();
    printStatement->name = "printStatement";

    match("printToken", lexerList);
    match("openParenToken", lexerList);
    printStatement->children.push_back(parseStatement(lexerList));
    match("closeParenToken", lexerList);

    return printStatement;
}

Node* parseStatementList(vector<Token> lexerList) {

    Node* statementList = new Node();
    statementList->name = "statementList";

    string token = lexerList.at(current).tokenType;

    if (token == "printToken") {
        statementList->children.push_back(parsePrintStatement(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "assignmentToken") {
        statementList->children.push_back(parseAssignmentStatement(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "intToken") {
        statementList->children.push_back(parseVarDecl(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "boolToken") {
        statementList->children.push_back(parseVarDecl(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "stringToken") {
        statementList->children.push_back(parseVarDecl(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "whileToken") {
        statementList->children.push_back(parseWhileStatement(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "ifToken") {
        statementList->children.push_back(parseIfStatement(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else if (token == "openBracToken") {
        statementList->children.push_back(parseBlock(lexerList));
        statementList->children.push_back(parseStatementList(lexerList));
    }
    else {}
        //continue

    return statementList;

}

Node* parseVarDecl(vector<Token> lexerList) {

    Node* varDecl = new Node();
    varDecl->name = "varDecl";

    varDecl->children.push_back(parseType(lexerList));
    varDecl->children.push_back(parseId(lexerList));

    return varDecl;
}


Node* parseWhileStatement(vector<Token> lexerList) {

    Node* whileStatement = new Node();
    whileStatement->name = "whileStatement";

    match("while", lexerList);
    whileStatement->children.push_back(parseBooleanExpr(lexerList));
    whileStatement->children.push_back(parseBlock(lexerList));

    return whileStatement;
}


Node* parseIntExpr(vector<Token> lexerList) {

    Node* intExpr = new Node();
    intExpr->name = "intExpr";

    string token = lexerList.at(current).tokenType;

    if (token == "digitToken") {
        if (lexerList[++current].tokenType == "addToken") {
            intExpr->children.push_back(parseIntOp(lexerList));
        }
        else
        {
            //continue
        }
    }

    return intExpr;
}


Node* parseStringExpr(vector<Token> lexerList) {

    Node* stringExpr = new Node();
    stringExpr->name = "stringExpr";

    match("quoteToken", lexerList);
    stringExpr->children.push_back(parseCharList(lexerList));
    match("quoteToken", lexerList);


    return stringExpr;
}






Node* parseList(vector<Token> masterTokenStreamLexed) {
    Node* root = new Node();
    root->name = "Program";

    root->children.push_back(parseBlock(masterTokenStreamLexed));
    match("endProgram", masterTokenStreamLexed);

    return root;
}

void Parser::parse(vector<Token> masterTokenStreamLexed) {
    
    // Get a pointer to the third element in the list
    auto it = masterTokenStreamLexed.begin(); // use std::next to advance the iterator

    // Create a pointer to the third element in the list
    Token* ptr = &(*it);

    /*string tokenType;
    string value;
    for (auto const& i : masterTokenStreamLexed) {
        tokenType = i.tokenType;
        value = i.value;
        
        //std::cout << i.value << "hit" << endl;
    }*/

    Node* root = parseList(masterTokenStreamLexed);
    printCST(root);
}
