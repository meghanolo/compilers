#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>

using namespace std;

//I used the same tutor as Zach Garfield who basically explained it the same way in C++

//ScopeNode* symbolTree;
int genErrors = 0;
vector<string> codeGenErrors;
int startAddr = 0;

auto loadAConst(int digit) {
    vector<string> holder;
    holder.push_back("A9");
    //holder.push_back("0x" + digit.toString(16).insert(0, 2 - digit.toString(16).size()(), '0'));
    return holder;
}


auto loadXConst(int num) {
    vector<string> holder;
    holder.push_back("A2");
    holder.push_back(num);
    return holder;
}

auto compareXmem(int num) {
    vector<string> holder;
    holder.push_back("EC");
    return holder;
}
auto toNegative(int num) {
    return ~num & 0xff
}

string int_to_hex(int i)
{
    std::stringstream stream;
    stream << "0x"
        << std::setfill('0')
        << std::hex << i;
    return stream.str();
}

struct ScopeNode {
public:
    int scope;
    string value;
    string type;
    string address;
    vector<ScopeNode*> children;
    ScopeNode(string x, string y, string z) {
        value = x;
        type = y;
        address = z;
    }
    ScopeNode();
};

int startAddress = 0;
ScopeNode* codeGenVars = new ScopeNode;
//Node struct
struct Node {
    string name;
    string value;
    vector<Node*> children;
};

class ASTNode {
public:
    string type;
    string value;
    vector<ASTNode*> children;
    void visit(Visitor v) {
        v.visitNode(this);
    }
    
};



class Visitor {

public: 
    vector<string> opCodes;
    vector<string> variables;
    vector<string> strings;

    string lookUp(string x) {
        for (int i = 0; i < codeGenVars.children.size(); i++) {
            if (x == codeGenVars.children[i]->value) {
                return codeGenVars.children[i]->type;
            }
        }

    }

    string lookUpAdd(string x) {
        for (int i = 0; i < codeGenVars.children.size(); i++) {
            if (x == codeGenVars.children[i]->value) {
                return codeGenVars.children[i].address;
            }
        }
    }
    void visitNode(ASTNode* node) {
        auto nodeType = node->type;

        if (nodeType == "block") {
            visitBlock(node);
        }
        else if (nodeType == "varDecl") {
            visitVarDecl(node);
        }
        else if (nodeType == "assignmentStatement") {
            visitAssign(node);
        }
        else if (nodeType == "printStatement") {
            visitPrintStatement(node);
        }
        else if (nodeType == "whileStatement") {
            visitWhile(node);
        }
        else if (nodeType == "ifStatement") {
            visitIf(node);
        }
        else if (nodeType == "idNode") {
            visitID(node);
        }
        else if (nodeType == "String") {
            visitString(node);
        }
        else if (nodeType == "boolVal") {
            visitBool(node);
        }
        else if (nodeType == "intExpr") {
            visitIntExpr(node);
        }

    }
    void visitBlock(ASTNode* node) {
        for (ASTNode* child : node->children) {
            visitNode(child);
        }
    }
    void visitVarDecl(ASTNode* node) {

        codeGenVars.children.push_back(ScopeNode(node->children[0]->value, node->children[0]->type, startAddress));
        
        startAddr++;

        //increment vars array by 1 in length
        //mem location of variable is location in vars array
        //placeholder for x
        //second pass fills in placeholders - note that its a var
        //associate mem address w variable

    }
    void visitPrintStatement(ASTNode* node) {

        string typeOfPrint = node->children[0]->type;
        cout << typeOfPrint << endl;

        auto isInt = false;
      
        if (typeOfPrint == "digitToken") {

            opCodes.push_back("A2");                  
            opCodes.push_back("01")                        
            visit(node->children[0]);
            opCodes.push_back("8D");    
            opCodes.push_back("AC");     
            
            opCodes.push_back("FF");

            //loadAReg w the constant
            //use 00 as temp 
            //copy a to 00 from 00 to Y
            //put 1 into x for print int
            //FF
       }
        if (typeOfPrint == "stringExpr") {
            opCodes.push_back("A2")                        
           opCodes.push_back("02")                       
            visit(node->children[0]);
            opCodes.push_back("8D");  
            opCodes.push_back("AC");     
            opCodes.push_back("FF");                  
            //refer to as address of sa=tarting byte
            //value in x is starting Address of string
            //visit firsdt child of node
            //puts address in a register
            //copy a to 00 from 00 to Y
            //put 2 into x for print string
            //FF

        }
        if (typeOfPrint == "id") {
           
            string varType = lookUp(node->children[1]);
            if (varType == NULL) {
                cout << "CODE GEN - ERROR - Variable not found." << endl;
            }
            else if (varType == "string") {
                isInt = false;
            }
            else {
                isInt = true;
            }
        }

    }
    void visitAssignmentStatement(ASTNode* node) {
        visit(node->children[0]);                  
        auto varType = lookUp(node->value);
        auto varAdd = lookUpAdd(node->value);
        if (varType == NULL) {
            cout << "CODE GEN - ERROR - Variable not found." << endl;
        }
        opCodes.push_back("Var" + varAdd);
        stoMem(varAdd);
    }

    void visitIfStatement(ASTNode* node) {


            visit(node->children[0]);      
                                                        
            opCodes.push_back("8D");        
            opCodes.push_back(loadXMem(0));      
           
    opCodes.push_back("8D");        
    opCodes.push_back(compareXmem(0));   
   
    if (visit(node->children[1]) == "==") {
        opCodes.push_back("D0");                       
        auto jumpIndex = opCodes.size();
        opCodes.push_back("00");
        visit(node->children[1]);
        opCodes[jumpIndex] = (opCodes.size() - jumpIndex);
    }
        else {
        opCodes.push_back("D0");                       
            auto jumpIndex = opCodes.size();
            opCodes.push_back("00");
            opCodes.push_back(loadXConst(1));
            opCodes.push_back(loadAConst(0));
            opCodes.push_back("8D");
            opCodes.push_back(compareXmem(0));
            opCodes.push_back("D0");
            auto afterIfIndex = opCodes.size();
            opCodes.push_back("00");
            opCodes[jumpIndex] = (opCodes.size() - jumpIndex);
            visit(node->children[1]);
            opCodes[afterIfIndex] = (opCodes.size() - afterIfIndex);
        }
    }

    void visitIntExpr(ASTNode* node) {
        opCodes.push_back(loadAConst(node->children[0]->value));    //load accumulator with left
        opCodes.push_back("8D");                                //store that into memeory
       visit(node->children[0]);                                              //load acumulator with expr 
        opCodes.push_back(("6D"));
    }
    


private :

};

class CodeGeneration {
public:
    ~CodeGeneration();
    int createCode(ASTNode* ast);

private:

};

CodeGeneration::~CodeGeneration()
= default;

//visitor = way to process a tree





int CodeGeneration::createCode(ASTNode* node) {

    ASTNode* AST = new ASTNode;

    codeGenErrors = 0;

    visit(node);

    if (genErrors == 0) {
        cout << "\n\n\n" << endl;
        cout << "Code" << endl;
        for (string x : codeGenErrors) {
            cout << x << endl;
        }
        cout << "" << endl;
        cout << "\nCode Generation Completed with 0 errors." << endl;
    }
    else {
        cout << "\n\n\n" << endl;
        cout << "Code Generation" << endl;
        for (string x : codeGenErrors) {
            cout << x << endl;
        }
        cout << "\nCode Generation Completed with " << codeGenErrors << " errors." << endl;
    }


    cout << "\n\n\n" << endl;

    return 0;
}
