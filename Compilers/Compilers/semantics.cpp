#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
//#include "codegeneration.cpp"

using namespace std;

// Credit to Juice Compiler for help with logic

int pos = 0;
int semanticErrors = 0;
int currentScope;

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
    string address;
    vector<ScopeNode*> children;
};

ScopeNode* symbolTree;
ASTNode* root = new ASTNode;

vector<string> errorsList;

class Semantics {
public:
    ~Semantics();
    ASTNode* working(Node* cst);

private:

};

Semantics::~Semantics()
= default;

bool checkTableScope(string id, int IDScope) {
    for (int i = 0; i < symbolTree->children.size(); i++) {
        if (IDScope == symbolTree->children[i]->scope) {
            if (id == symbolTree->children[i]->value) {
                return true;
            }
        }
    }
    return false;
}

bool checkTable(string id) {
    for (int i = 0; i < symbolTree->children.size(); i++) {
            if (id == symbolTree->children[i]->value) {
                return true;
        }
    }
    return false;
}


bool checkTableType(string id, int IDScope, string type) {
    for (int i = 0; i < symbolTree->children.size(); i++) {
        if (IDScope == symbolTree->children[i]->scope) {
            if (id == symbolTree->children[i]->value) {
                if (type == symbolTree->children[i]->type) {
                    return true;
                }
            }
        }
    }
    return false;
}


void printAST(ASTNode* node, int indent = 0) {
    
    if ((node->type == "String") || (node->type == "boolVal")) {
        for (int i = 0; i < indent; i++) {
            cout << "-";
        }
        cout << node->value << endl;
    }
    else {
        for (int i = 0; i < indent; i++) {
            cout << "-";
        }
        cout << node->type << endl;
    }
    
    for (int i = 0; i < node->children.size(); i++) {
        printAST(node->children[i], indent + 1);
    }
}

ASTNode* buildAST(Node* node) {
    
    string currentName = node->name;
    if (currentName == "Block") {
       ASTNode* blockNode = new ASTNode;
       blockNode->type = "block";

       ScopeNode* blockScope = new ScopeNode;
       blockScope->scope = currentScope;
       //scopeTree->children.push_back(blockScope);

       currentScope++;
       root->children.push_back(blockNode);
       
       for (int i = 0; i < node->children.size(); i++) {
           buildAST(node->children[i]);
        }
       currentScope--;
    }
    else if (currentName == "varDecl") {
            
            ASTNode* varDecl = new ASTNode;
            varDecl->type = "varDecl";

            string varType = node->children[0]->name;
            string varTypeVal = node->children[0]->value;

            ASTNode* varNode = new ASTNode;
            varNode->type = varTypeVal;

            varDecl->children.push_back(varNode);

            string varId = node->children[1]->name;
            string varIdVal = node->children[1]->value;
           
            ASTNode* idNode = new ASTNode;
            idNode->type = varIdVal;

            varDecl->children.push_back(idNode);
            root->children.push_back(varDecl);

            if (checkTableType(varId, currentScope, varType) == false) {
                ScopeNode* symbol = new ScopeNode;
                symbol->scope = currentScope;
                symbol->type = varTypeVal;
                symbol->value = varIdVal;
                symbolTree->children.push_back(symbol);
                errorsList.push_back("DEBUG - SEMANTIC - VALID - New Variable Initialized");
            }
            else {
                semanticErrors++;
                errorsList.push_back("DEBUG - SEMANTIC - ERROR: Duplicate Variable"); 
            }

        }
        else if (currentName == "printStatement") {
            
            ASTNode* printStatement = new ASTNode;
            printStatement->type = "printStatement";

            root->children.push_back(printStatement);

            buildAST(node->children[0]);
        }
        else if (currentName == "assignmentStatement") {
            
            ASTNode* assignmentStatement = new ASTNode;
            assignmentStatement->type = "assignmentStatement";

            auto id = node->children[0]->value;

            bool idCheck = checkTableScope(id, currentScope);
            
            string expressionType = node->children[1]->value;
            string expressionName = node->children[1]->name;
            
            if (expressionType == "digitToken") { 
                expressionType = "int";    }
            if (expressionType == "quoteToken") { expressionType = "string"; }
            if ((expressionType == "trueToken") || expressionType == "falseToken") { expressionType = "bool"; }
            


            if (checkTable(id)) {
                if (checkTableType(id, currentScope, expressionType)) {
                    errorsList.push_back("DEBUG - SEMANTIC - VALID - Variable " + id + " assigned to " + expressionName);
                    root->children.push_back(assignmentStatement);
                }
                else {
                    semanticErrors++;
                    errorsList.push_back("DEBUG - SEMANTIC - ERROR: Type Mismatch. Tried to assign " + id + " to " + expressionType);
                }
            }
            else {
                semanticErrors++;
                errorsList.push_back("DEBUG - SEMANTIC - ERROR: Uninitialized Variable. " + id + " is not initialized in scope. ");
            }

        }
        else if (currentName == "whileStatement") {
            ASTNode* whileStatement = new ASTNode;
            whileStatement->type = "whileStatement";

            root->children.push_back(whileStatement);
            
            buildAST(node->children[0]);
            buildAST(node->children[1]);
            
        }
        else if (currentName == "ifStatement") {
            ASTNode* ifStatement = new ASTNode;
            ifStatement->type = "ifStatement";
            root->children.push_back(ifStatement);
            buildAST(node->children[0]);
            buildAST(node->children[1]);
           
        }
        else if (currentName == "id") {

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
        else if (currentName == "intExpr") {
            //cout << "hit int" << endl;
            // Check if it is not a digit
            if (node->children.size() > 1) {
                ASTNode* intExpr = new ASTNode;
                intExpr->type = "intExpr";
                //intExpr->value = node->children[0]->value;
               root->children.push_back(intExpr);
              

                auto exprType = node->children[1]->value;
       
                if (exprType != "digitToken") {
                    semanticErrors++;
                    errorsList.push_back("DEBUG - SEMANTIC - ERROR: Incorrect Int Expression");
                }
       
            }
            // If it is a digit
            else {
               // this.ast.addNode(node.children[0].children[0].value);
                
            }


        }
        else if (currentName == "boolExpr") {

        //cout << "boolExpr" << endl;


            ASTNode* boolVal = new ASTNode;
            boolVal->type = "boolVal";

            if (node->children.size() > 1) {
                if (node->children[1]->value == "==") {
                    boolVal->value = "Equals";
                   
                }
                else {
                    boolVal->value = "notEquals";
                    

                }
                root->children.push_back(boolVal);

                string leftExpr = node->children[0]->value;
                string rightExpr = node->children[2]->value;

                if (leftExpr != rightExpr) {
                    semanticErrors++;
                    errorsList.push_back("SEMANTIC --> | ERROR: Incorrect Type Comparison");
                }
                else {
                    //addToAST(buildAST(node->children[0]));
                    //addToAST(buildAST(node->children[2]));
                }

            }

            else {
                root->children.push_back(buildAST(node->children[0]));
            }

        }
        else if (currentName == "stringExpr") {

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
           for (int i = 0; i < node->children.size(); i++) {
                buildAST(node->children[i]);
            }
        }
    
    
    return root;
}

ASTNode* Semantics::working(Node* node) {

    ASTNode* AST = new ASTNode;

    //CodeGeneration CodeString;
    errorsList = {};

    symbolTree = new ScopeNode();
    root = new ASTNode;
    semanticErrors = 0;

    currentScope = 0;

    buildAST(node);

    if (semanticErrors == 0) {
        cout << "\n\n\n" << endl;
        cout << "AST" << endl;
        for (string x : errorsList) {
            cout << x << endl;
        }
        cout << "" << endl;
        for (int i = 0; i < symbolTree->children.size(); i++) {
            cout << symbolTree->children[i]->scope << " , " << symbolTree->children[i]->value << " , " << symbolTree->children[i]->type << endl;
        }
        cout << "" << endl;
        printAST(root);
        cout << "\nSemantic Analysis Completed with 0 errors." << endl;
       // CodeString.createCode(root);
    }
    else {
        cout << "\n\n\n" << endl;
        cout << "Semantic Analysis" << endl;
        for (string x : errorsList) {
            cout << x << endl;
        }
        cout << "\nSemantic Analysis Completed with " << semanticErrors << " errors." << endl;
    }
    
    
    cout << "\n\n\n" << endl;

    return AST;
}
