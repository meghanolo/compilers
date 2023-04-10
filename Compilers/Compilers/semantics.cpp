#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;

int scope = 0;

Node* createNode(string value);

struct Node {
    string name;
    string value;
    int scope;
    vector<Node*> children;
};

struct Scope {
    string name;
    int indent;
};


class Semantics {

public:
    ~Semantics();
    void semanticAnalysis(vector<Node> lexerList);

private:

};

Semantics::~Semantics()
= default;

Node* createNode(int scope, string value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->scope = scope;
    return newNode;
}

// Function to build the AST
Node* buildAST(vector<Node> lexerList) {
    vector<Node*> stack;
    int i = 0;
    while (i < lexerList.size()) {
        auto temp = lexerList[i].name;
        if (temp == "Block") {
            scope++;
            Scope* newScope = new Scope;
            Node* numberNode = createNode(scope, temp);
            stack.push_back(numberNode);

            
        }
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            string op = "";
            op += expression[i];
            Node* opNode = createNode(op);
            i++;
            opNode->children.push_back(stack.back());
            stack.pop_back();
            opNode->children.push_back(stack.back());
            stack.pop_back();
            stack.push_back(opNode);
        }
        else {
            i++;
        }
    }
    return stack.back();
}

void printAST(Node* root, int depth) {
    for (int i = 0; i < depth; i++) {
        cout << " ";
    }
    cout << root->value << endl;
    for (int i = 0; i < root->children.size(); i++) {
        printAST(root->children[i], depth + 1);
    }
}


void semanticAnalysis(vector<Node> lexerList) {
    Node* ast = buildAST(lexerList);
    printAST(ast, 0);
    return 0;
}