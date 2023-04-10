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

//Define functions
Node* parseChar(vector<Token> lexerList); 
Node* parseSpace(vector<Token> lexerList);
Node* parseCharList(vector<Token> lexerList);
Node* parseType(vector<Token> lexerList);
Node* parseDigit(vector<Token> lexerList);
Node* parseBoolOp(vector<Token> lexerList);
Node* parseBoolVal(vector<Token> lexerList);
Node* parseIntOp(vector<Token> lexerList);
Node* parseBlock(vector<Token> lexerList);
Node* parseExpr(vector<Token> lexerList);
Node* parseId(vector<Token> lexerList);
Node* parseAssignmentStatement(vector<Token> lexerList);
Node* parseBoolExpr(vector<Token> lexerList);
Node* parseIfStatement(vector<Token> lexerList);
Node* parseStatement(vector<Token> lexerList);
Node* parsePrintStatement(vector<Token> lexerList);
Node* parseStatementList(vector<Token> lexerList);
Node* parseVarDecl(vector<Token> lexerList);
Node* parseWhileStatement(vector<Token> lexerList);
Node* parseIntExpr(vector<Token> lexerList);
Node* parseStringExpr(vector<Token> lexerList);
Node* parseProgram(vector<Token> lexerList);

class Parser {

public:
	~Parser();                                                           
	void parse(vector<Token>);

private:

};

Parser::~Parser()
= default;

//Check if the token at the current position matches the expected token
bool match(string expected, vector<Token> lexerList) {

    auto actual = lexerList.at(current).tokenType;

    //If they match, move the current token
    if (actual == expected) {
        cout << "PARSER --> |VALID! Expected token " << expected << ". Received " << actual << " at position " << lexerList.at(current).linePosition << endl;
        current++;
        return true;
    }
    //Otherwise return an error
    else {
        cout << "PARSER --> |ERROR! Expected token " << expected << ". Received " << actual << " at position " << lexerList.at(current).linePosition << endl;
    }

    return false;
}

//Print the CST with - to separate layers
void printCST(Node* node, int indent = 0) {
    for (int i = 0; i < indent; i++) {
        cout << "-";
    }
    cout << node->name << endl;

    for (int i = 0; i < node->children.size(); i++) {
        printCST(node->children[i], indent + 1);
    }
}

//Create functions to match the grammar for each type of rule. In each function, create a new
    //node. If the conditions for said rule are met, return that node and add it to the
    //tree that is being recursively created, otherwise return null.

Node* parseChar(vector<Token> lexerList) {
    Node* Char = new Node();
    Char->name = "Char";

    if (match("charToken", lexerList)) 
        return Char;

    return NULL;
}

Node* parseSpace(vector<Token> lexerList) {
    Node* space = new Node();
    space->name = "space";

    if (match("spaceToken", lexerList))
        return space;

    return NULL;
}


Node* parseCharList(vector<Token> lexerList) {
    Node* charList = new Node();
    charList->name = "charList";

    string token = lexerList.at(current).tokenType;

    if (token == "charToken") {
            auto x = parseChar(lexerList);
            if (x != NULL) {
                charList->children.push_back(x);
            }
            auto y = parseCharList(lexerList);
            if (y != NULL) {
                charList->children.push_back(y);
            }
        }
    else if (token == "spaceToken") {
        auto x = parseSpace(lexerList);
        if (x != NULL) {
            charList->children.push_back(x);
        }
        auto y = parseCharList(lexerList);
        if (y != NULL) {
            charList->children.push_back(y);
        }
    }

    return charList;
}

Node* parseType(vector<Token> lexerList) {
    Node* type = new Node();
    type->name = "type";

    string token = lexerList.at(current).tokenType;

    if (token == "intToken") {
        if (match("intToken", lexerList))
            return type;
    }
    else if (token == "boolToken") {
        if (match("boolToken", lexerList))
            return type;
    }
    else if (token == "stringToken") {
        if (match("stringToken", lexerList))
            return type;
    }

    return NULL;
}

Node* parseDigit(vector<Token> lexerList) {
    Node* digit = new Node();
    digit->name = "digit";

    if (match("digitToken", lexerList))
        return digit;
    return NULL;

}

Node* parseBoolOp(vector<Token> lexerList) {
    Node* boolOp = new Node();
    boolOp->name = "boolOp";

    string token = lexerList.at(current).tokenType;

    if (token == "boolEqualToken") {
        if (match("boolEqualToken", lexerList))
            return boolOp;
    }
    else if (token == "boolNotToken") {
        if (match("boolNotToken", lexerList))
            return boolOp;
    }

    return NULL;
}

Node* parseBoolVal(vector<Token> lexerList) {
    Node* boolVal = new Node();
    boolVal->name = "boolVal";
    string token = lexerList.at(current).tokenType;

    if (token == "falseToken") {
        if (match("falseToken", lexerList))
            return boolVal;
    }
    else if (token == "trueToken") {
        if (match("trueToken", lexerList))
            return boolVal;
    }

    return NULL;
}

Node* parseIntOp(vector<Token> lexerList) {
    Node* intOp = new Node();
    intOp->name = "intOp";

    if (match("addToken", lexerList))
        return intOp;

    return NULL;
}

Node* parseBlock(vector<Token> lexerList) {
    // { StatementList }

    Node* block = new Node();
    block->name = "Block";

    if (match("openBracToken", lexerList)) {
        auto x = parseStatementList(lexerList);
        if (x != NULL) {
            block->children.push_back(x);
            if (match("closeBracToken", lexerList))
                return block;
        }
        if (match("closeBracToken", lexerList))
            return block;
    }

    return NULL;
}

Node* parseExpr(vector<Token> lexerList) {

    Node* expression = new Node();
    expression->name = "Expression";

    string token = lexerList.at(current).tokenType;

    if (token == "charToken") {
        auto x = parseId(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }       
    else if (token == "digitToken") {
        auto x = parseIntExpr(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }
    else if (token == "quoteToken") {
        auto x = parseStringExpr(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }
    else if (token == "openParenToken") {
        auto x = parseBoolExpr(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }
    else if (token == "falseToken") {
        auto x = parseBoolExpr(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }
    else if (token == "trueToken") {
        auto x = parseBoolExpr(lexerList);
        if (x != NULL) {
            expression->children.push_back(x);
        }
        return expression;
    }
       
       

    return NULL;
   
}

Node* parseId(vector<Token> lexerList) {
    Node* id = new Node();
    id->name = "id";

    auto x = parseChar(lexerList);
    if (x != NULL) {
        id->children.push_back(x);

        return id;
    }
    return NULL;
}

Node* parseAssignmentStatement(vector<Token> lexerList) {

    Node* assignmentStatement = new Node();
    assignmentStatement->name = "assignmentStatement";

    auto x = parseId(lexerList);
    if ((x != NULL) && (match("assignmentToken", lexerList))) {
        assignmentStatement->children.push_back(x);
        auto y = parseExpr(lexerList);
            if (y != NULL) {
                assignmentStatement->children.push_back(y);

                return assignmentStatement;
            }
    }
    

    return NULL;
}

Node* parseBoolExpr(vector<Token> lexerList) {

    Node* boolExpr = new Node();
    boolExpr->name = "boolExpr";

    string token = lexerList.at(current).tokenType;

    if (token == "trueToken" || token == "falseToken") {
        auto x = parseBoolVal(lexerList);
        if (x != NULL) {
            boolExpr->children.push_back(x);
            return boolExpr;
        }
    }
    else if (token == "openParenToken") {
        if (match("openParenToken", lexerList)) {
            auto x = parseExpr(lexerList);
            if (x != NULL) {
                boolExpr->children.push_back(x);
            }
            auto y = parseBoolOp(lexerList);
            if (y != NULL) {
                boolExpr->children.push_back(y);
            }
            auto z = parseExpr(lexerList);
            if ((z != NULL) && (match("closeParenToken", lexerList))) {
                boolExpr->children.push_back(z);

                return boolExpr;
                   
            }
        }
      
    }

    return NULL;
}


Node* parseIfStatement(vector<Token> lexerList) {

    Node* ifStatement = new Node();
    ifStatement->name = "ifStatement";

    if (match("ifToken", lexerList)) {
        auto x = parseBoolExpr(lexerList);
        if (x != NULL) {
            ifStatement->children.push_back(x);
        }
        auto y = parseBlock(lexerList);
        if (y != NULL) {
            ifStatement->children.push_back(y);

            return ifStatement;
        }
    }
   
    return NULL;
}

Node* parseStatement(vector<Token> lexerList) {

    Node* statement = new Node();
    statement->name = "Statement";

    string token = lexerList.at(current).tokenType;

    if (token == "printToken") {
        auto x = parsePrintStatement(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);

            return statement;
        }
    }
    else if (token == "charToken") {
        auto x = parseAssignmentStatement(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);

            return statement;
        }
    }
    else if ((token == "intToken") || (token == "boolToken") || (token == "stringToken")) {
        auto x = parseVarDecl(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);
            return statement;
        }
        
    }
    else if (token == "whileToken") {
        auto x = parseWhileStatement(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);
            return statement;
        }
    }
    else if (token == "ifToken") {
        auto x = parseIfStatement(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);
            return statement;
        }
    }
    else if (token == "openBracToken") {
        auto x = parseBlock(lexerList);
        if (x != NULL) {
            statement->children.push_back(x);
            return statement;
        }
    }
    return NULL;
}

Node* parsePrintStatement(vector<Token> lexerList) {

    Node* printStatement = new Node();
    printStatement->name = "printStatement";

    if (match("printToken", lexerList)) {
        if (match("openParenToken", lexerList)) {
            auto x = parseExpr(lexerList);
            if ((x != NULL) && (match("closeParenToken", lexerList))) {
                printStatement->children.push_back(x);
                return printStatement;
            }
        }
    }
    return NULL;
}

Node* parseStatementList(vector<Token> lexerList) {

    Node* statementList = new Node();
    statementList->name = "statementList";

    string token = lexerList.at(current).tokenType;

    if ((token == "printToken") || (token == "charToken") || (token == "intToken") || (token == "boolToken") || (token == "stringToken") || (token == "whileToken")
        || (token == "ifToken") || (token == "openBracToken") ) {
        auto x = parseStatement(lexerList);
        if (x != NULL) {
            statementList->children.push_back(x);
        }
        auto y = parseStatementList(lexerList);
        if (y != NULL) {
            statementList->children.push_back(y);
        }
        return statementList;
    }


    return NULL;

}

Node* parseVarDecl(vector<Token> lexerList) {

    Node* varDecl = new Node();
    varDecl->name = "varDecl";

    auto x = parseType(lexerList);
    if (x != NULL) 
        varDecl->children.push_back(x);
    auto y = parseId(lexerList);
    if (y != NULL) {
        varDecl->children.push_back(y);
        return varDecl;
    }

    return NULL;
}
Node* parseWhileStatement(vector<Token> lexerList) {

    Node* whileStatement = new Node();
    whileStatement->name = "whileStatement";

    if (match("whileToken", lexerList)) {

        auto x = parseBoolExpr(lexerList);
        if (x != NULL)
            whileStatement->children.push_back(x);
        auto y = parseBlock(lexerList);
        if (y != NULL) {
            whileStatement->children.push_back(y);

            return whileStatement;
        }
    }

    return NULL;
}


Node* parseIntExpr(vector<Token> lexerList) {

    Node* intExpr = new Node();
    intExpr->name = "intExpr";

    string token = lexerList.at(current).tokenType;

    int counter = current + 1;
    if (token == "digitToken") {
        if (lexerList[counter].tokenType == "addToken") {
            if (match("digitToken", lexerList)) {
                auto x = parseIntOp(lexerList);
                if (x != NULL) {
                    intExpr->children.push_back(x);
                }
                auto y = parseExpr(lexerList);
                if (y != NULL) {
                    intExpr->children.push_back(y);

                    return intExpr;
                }
            }
        }
        else
            if (match("digitToken", lexerList)) {
                return intExpr;
        }
    }

    return NULL;
}



Node* parseStringExpr(vector<Token> lexerList) {

    Node* stringExpr = new Node();
    stringExpr->name = "stringExpr";

    if (match("quoteToken", lexerList)) {
        auto x = parseCharList(lexerList);
        if ((x != NULL) && (match("quoteToken", lexerList))) {
            stringExpr->children.push_back(x);
            return stringExpr;
        }
    }

    return NULL;
}



Node* parseProgram(vector<Token> lexerList) {
    Node* root = new Node();
    root->name = "Program";

    auto x = parseBlock(lexerList);
    if ((x != NULL) && match("endProgram", lexerList)) {
        root->children.push_back(x);
        return root;
    }

    return NULL;
}

void Parser::parse(vector<Token> masterTokenStreamLexed) {
    
    current = 0;

    vector<Token> lexerList;

    //Since we are ignoring comments, create a new token stream with comment tokens removed
        //so they are not added to the CST
    for (auto i = 0; i < masterTokenStreamLexed.size(); i++)
    {
        //auto x = masterTokenStreamLexed[i].tokenType;
        if ((masterTokenStreamLexed[i].tokenType != "startComToken") && (masterTokenStreamLexed[i].tokenType != "closeComToken"))
            lexerList.push_back(masterTokenStreamLexed[i]);
    }
    
    //If the program is valid, print the tree.

    Node* root = new Node();
    auto x = parseProgram(lexerList);

    if (x != NULL) {
        root->children.push_back(x);
        printCST(root);
    }
    else
        cout << "ERROR. No CST Created." << endl;

    cout << "\n\n\n" << endl;
   
}
