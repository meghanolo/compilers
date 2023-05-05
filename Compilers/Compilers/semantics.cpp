#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>

using namespace std;

// Credit to Juice Compiler for help with logic

int pos = 0;
int semanticErrors = 0;
int currentScope;
bool continueParsing = true;

struct Node {
    string name;
    string value;
    vector<Node*> children;
};

// AST node class
class ASTNode {
public:
    string type;
    string value;
    vector<ASTNode*> children;
};

struct ScopeNode {
public:
    int scope;
    string value;
    string type;
    vector<ScopeNode*> children;
 /*   ScopeNode(int a, string x, string y, vector<ScopeNode*> z) {
        scope = a;
        value = x;
        type = y;
        children = z;
    }
    ScopeNode(int a, string x, string y) {
        scope = a;
        value = x;
        type = y;
        children = z;
    }*/
};

ScopeNode* scopeTree;
ScopeNode* symbolTree;
ASTNode* root = new ASTNode;

// Recursive descent parser class
class Semantics {
public:
    ~Semantics();
    ASTNode* working(Node* cst);

private:

};

Semantics::~Semantics()
= default;

/*void addId(string id, string type, int linePos, int lineNum) {
   
   symbolTree->children.push_back(ScopeNode(currentScope, id, type));
}

void enterBlock() {
    currentScope += 1;
}

void leaveBlock() {
    currentScope -= 1;
}

bool checkTable(string id) {
    for (int i = 0; i < this->symbols.size(); i++) {
        if (id == this->symbols[i].id) {
            return true;
        }
    }
    return false;
}

bool scopeCheck(int scope, std::string id) {
    for (int i = 0; i < this->symbols.size(); i++) {
        if (scope == this->symbols[i].scope && id == this->symbols[i].id) {
            return true;
        }
    }
    return false;
}
};*/


void printAST(ASTNode* node, int indent = 0) {
    for (int i = 0; i < indent; i++) {
        cout << "-";
    }
    cout << node->type << endl;
    if (node->value != "") {
        for (int i = 0; i < indent; i++) {
            cout << "-";
        }
        cout << node->value << endl;
    }
    
    for (int i = 0; i < node->children.size(); i++) {
        printAST(node->children[i], indent + 1);
    }
}

ASTNode* buildAST(Node* node) {
    
    string current = node->name;
    if (current == "Program") {
       root->type = "program";
       root->children.push_back(buildAST(node->children[0]));
       continueParsing = false;
    }
    else if (current == "Block") {
       ASTNode* blockNode = new ASTNode;
       blockNode->type = "block";

       ScopeNode* blockScope = new ScopeNode;
       blockScope->scope = currentScope;
       scopeTree->children.push_back(blockScope);

       currentScope++;
       
       for (int i = 0; i < node->children.size(); i++) {
           auto fly = buildAST(node->children[i]);
           blockNode->children.push_back(fly);
       }
       root->children.push_back(blockNode);

     

        }
        else if (current == "varDecl") {
            //cout << "vardecl" << endl;
            ASTNode* varDecl = new ASTNode;
            varDecl->type = "varDecl";

            string varType = node->children[0]->name;

            ASTNode* varNode = new ASTNode;
            varNode->type = varType;

            varDecl->children.push_back(varNode);

            string varId = node->children[1]->name;

            ASTNode* idNode = new ASTNode;
            idNode->type = varId;

            varDecl->children.push_back(idNode);

            root->children.push_back(varDecl);


            /*if () {
                //this->scopeTree.curr.value.buckets[id.value] = new JuiceC::ScopeVariable(id.value, token);
                scopeNode* symbol = new ScopeNode;
                symbol->scope = currentScope;
                symbol->type = varType;
                symbol->value = idNode;
                //this->symbols.push_back(symbol);
            }
            else {
                errors++;
                cout << "DEBUG - SEMANTIC - ERROR: Duplicate Variable"" << endl;
            }*/

        }
        else if (current == "printStatement") {
             //cout << "hitprint" << endl;

            ASTNode* printStatement = new ASTNode;
            printStatement->type = "printStatement";

            printStatement->children.push_back(buildAST(node->children[0]));
            root->children.push_back(printStatement);
        }
        else if (current == "assignmentStatement") {
            //cout << "hit assign" << endl;
            ASTNode* assignmentStatement = new ASTNode;
            assignmentStatement->type = "assignmentStatement";

            auto id = node->children[0]->value;
            
            //id.scopeId = this.scopeTree.curr.value.id;
            //this.ast.addNode(node.children[0].children[0].value);
            // Check if id is in scope and get its type
            //idType = this.checkScopes(node.children[0].children[0]);
            
            // Find the expression and get the type returned by the expression
            auto expressionType = node->children[1]->name;

            //this.checkTypeMatch(node.children[0].children[0].value, idType, expressionType, node.children[2].lineNum, node.children[2].colNum);
            // Update scope tree node object initialized flag. variable 
            // has been initialized.
            //this.markAsInitialized(node.children[0].children[0]);
            
            root->children.push_back(assignmentStatement);


        }
        else if (current == "whileStatement") {
            //cout << "hit while" << endl;
            ASTNode* whileStatement = new ASTNode;
            whileStatement->type = "whileStatement";
            
            whileStatement->children.push_back(buildAST(node->children[0]));
            whileStatement->children.push_back(buildAST(node->children[1]));
            root->children.push_back(whileStatement);
        }
        else if (current == "ifStatement") {
            //cout << "hit if" << endl;
            ASTNode* ifStatement = new ASTNode;
            ifStatement->type = "ifStatement";
            
            ifStatement->children.push_back(buildAST(node->children[0]));
            ifStatement->children.push_back(buildAST(node->children[1]));
            root->children.push_back(ifStatement);
        }
        else if (current == "id") {
            //cout << "hit id" << endl;

            ASTNode* idNode = new ASTNode;
            idNode->type = "idNode";

           
            auto id = node->children[0]->value;
            idNode->value = id;
            // Set the scope on the id
            //id.scopeId = this.scopeTree.curr.value.id;
            root->children.push_back(idNode);
            
            // Check if variable declared in current or parent scopes
            // If we find it in scope, return the type of the variable
            //var foundType = this.checkScopes(node.children[0]);
            // Mark id as used
            //this.markAsUsed(node.children[0]);
            // Look for used but uninitialized variables
            //this.checkUsedButUninit(node.children[0]);
            

        }
        else if (current == "intExpr") {
            //cout << "hit int" << endl;
            // Check if it is not a digit
            if (node->children.size() > 1) {
                ASTNode* intExpr = new ASTNode;
                intExpr->type = "intExpr";
                //intExpr->value = node->children[0]->value;
                root->children.push_back(intExpr);
              

                auto exprType = node->children[1]->value;
       
                if (exprType != "digitToken") {
                    errors++;
                    cout << "DEBUG - SEMANTIC - ERROR: Incorrect Int Expression" << endl;
                }
       
            }
            // If it is a digit
            else {
               // this.ast.addNode(node.children[0].children[0].value);
                
            }


        }
        else if (current == "boolExpr") {

        //cout << "boolExpr" << endl;


            ASTNode* boolVal = new ASTNode;
            boolVal->type = "boolVal";

            if (node->children.size() > 1) {
                if (node->children[1]->value == "==") {
                    boolVal->value = "Equals";
                    root->children.push_back(boolVal);
                }
                else {
                    boolVal->value = "notEquals";
                    root->children.push_back(boolVal);
                }

                string leftExpr = node->children[0]->value;
                string rightExpr = node->children[2]->value;

                if (leftExpr != rightExpr) {
                    semanticErrors++;
                    cout << "SEMANTIC --> | ERROR: Incorrect Type Comparison" << endl;
                }
                else {
                    root->children.push_back(buildAST(node->children[0]));
                    root->children.push_back(buildAST(node->children[2]));
                }

            }

            else {
                root->children.push_back(buildAST(node->children[0]));
            }

        }
        else if (current == "stringExpr") {

        //cout << "hitString" << endl;
           
            string currChar = ""; 
            string creatingString = "\"";

            auto currCharList = node->children[0];
            auto lastCharList = false;
        
            while (!lastCharList) {
                    currChar = currCharList->children[0]->value;
                   // cout << currChar << endl;
                    creatingString += currChar;
                    currCharList = currCharList->children[1];
                    if (currCharList->children.size() == 0) {
                        lastCharList = true;
                    }
            }
            
            creatingString += "\"";
            //cout << creatingString << endl;

            ASTNode* newString = new ASTNode;
            newString->type = "String";
            newString->value = creatingString;
            root->children.push_back(newString);

        }
        else {
        if (continueParsing) {
            for (int i = 0; i < node->children.size(); i++) {
                buildAST(node->children[i]);
            }
        }

        }
    
    
    return root;
}

ASTNode* Semantics::working(Node* node) {

    ASTNode* AST = new ASTNode;

    scopeTree = new ScopeNode();

    vector<Token> lexerList;

    currentScope = 0;

    AST = (buildAST(node));

    printAST(AST);
    
    cout << "\n\n\n" << endl;

    return AST;
}
