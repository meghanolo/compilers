#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include "semantics.cpp"

using namespace std;

ScopeNode* symbolTree;
int startAddress;
vector<string> tempCode;

int genErrors = 0;
vector<string> codeGenErrors;

class CodeGeneration {
public:
    ~CodeGeneration();
    int createCode(Node* cst);

private:

};

CodeGeneration::~CodeGeneration()
= default;

auto loadAConst(int digit) {
    auto holder = [];
    holder.push_back("A9");
    holder.push_back(digit.toString(16).padStart(2, "0"));
    return holder;
}
auto loadAMem(int digit) {
    let holder = [];
    holder.push_back("AD");
    splitAddress(digit, holder);
    return holder;
}
auto stoMem(int num) {
    let holder = [];
    holder.push_back("8D");
    splitAddress(num, holder);
    return holder;
}
auto add(int num) {
    let holder = [];
    holder.push_back("6D");
    splitAddress(num, holder);
    return holder;
}
void splitAddress(int number, vector<string> holder) {
    auto val = number.str(16).padStart(4, "0");
    holder.push_back(val.substring(2, 4));
    holder.push_back(val.substring(0, 2));
}
auto loadY(int number) {
    auto holder = [];
    holder.push_back("AC");
    splitAddress(num, holder);
    return holder;
}
function loadYA() {
    let holder = [];
    holder.push_back("AC");
}
function loadXMem(num: number) {
    let holder = [];
    holder.push_back("AE");
    splitAddress(num, holder);
    return holder;
}
auto loadXConst(int num) {
    let holder = [];
    holder.push_back("A2");
    holder.push_back(num.toString(16).padStart(2, "0"));
    return holder;
}
auto convertHex(string val) {
    return val.charCodeAt(0).toString(16).padStart(2, "0");
}
auto convertHexNum(int num) {
    return num.toString(16).padStart(2, "0");
}
auto varRef(int val) {
    if (val >= 10000 && val < 20000) {
        return true;
    }
    else {
        return false;
    }
}
auto stringRef(int val) {
    if (val >= 20000 && val < 30000) {
        return true;
    }
    else {
        return false;
    }
}
auto compareXmem(int num) {
    let holder = [];
    holder.push_back("EC");
    splitAddress(num, holder);
    return holder;
}
auto toNegative(int num) {
    return ~num & 0xff
}


string codeString (Node* node) {

    string currentName = node->name;
    if (currentName == "Program") {
        tempCode.push_back("00");
    }
    else if (currentName == "Block") {
        this.topTable = new SymbolTable(this.topTable);
        for (int i = 0; i < node->children.size(); i++) {
            codeString(node->children[i]);
        }

        this.topTable = this.topTable.parent;
    }
    else if (currentName == "varDecl") {

        string varTypeVal = node->children[0]->value;
        string varIdVal = node->children[1]->value;

        ScopeNode* symbol = new ScopeNode;
        symbol->type = varTypeVal;
        symbol->value = varIdVal;
        symbol->memAddr = startAddr;
        symbolTree->children.push_back(symbol);

        startAddr++;

    }
    else if (currentName == "printStatement") {

        tempCode = tempCode.concat(loadAConst(parseInt(digitexpr.digit.toString())));

        let length = this.data.length;
        this.data = this.data.concat(string.value.substring(1, string.value.length - 1).split("").map(convertHex));
        this.data.push_back("00");
        //todo: treat the length like placeholder
        tempCode = tempCode.concat([String(20000 + length)], loadAMem(parseInt(length.toString())))

        auto isInt = false;
        auto typeOfPrint = node->children[0]->name;
        if (typeOfPrint == "charToken") {
            
            auto varInfo = this.topTable.lookUp(id.id.toString());
            if (varInfo == = null) {
                genErrors++;
                codeGenErrors.push_back("Var not found");
            }
            if (varInfo.type == = "string") {
                isInt = false;
            }
            else {
                isInt = true;
            }
        }
        if (typeOfPrint == "digitToken") {
            tempCode.push_back("A2");                       //LDX
            tempCode.push_back("01");                  //putting 01 into the x reg
            
            printstatement.expr.visit(this);

            tempCode = tempCode.concat(stoMem(0));    //Store that into 0 memory 
            tempCode = tempCode.concat(loadY(0));     //load into y register
            //Sto A in mem
            //load y from mem

            tempCode.push_back("FF");                  //SYS call
        }
        else {
            tempCode.push_back("A2");                       //LDX
            tempCode.push_back("02");                        //putting 02 into the x reg
                printstatement.expr.visit(this);
            tempCode = tempCode.concat(stoMem(0));    //Store that into 0 memory 
            tempCode = tempCode.concat(loadY(0));     //load into y register
            tempCode.push_back("FF");                        //SYS call
        }

        
    }
    else if (currentName == "assignmentStatement") {
        assignmentstatement.expr.visit(this);                   //visit expr
        let varInfo = this.topTable.lookUp(assignmentstatement.id.toString());                  //look up 
        if (varInfo == = null) {
            throw new Error("Var not found");
        }
        tempCode = tempCode.concat([String(10000 + varInfo.memAddr)], stoMem(varInfo.memAddr));  //look up var in the toptable
        //emit 8d for store with 10000 for temp addr


        

    }
    else if (currentName == "whileStatement") {
        let whileStart = tempCode.length;
        whilestatement.booleanexpr.right.visit(this);       //emit the condition which is boolexpr
        tempCode = tempCode.concat(stoMem(0));            //sto in temporary register from A
        tempCode = tempCode.concat(loadXMem(0));          //load it into the x register from that location
        whilestatement.booleanexpr.left.visit(this);        //emit left hand side of relation
        tempCode = tempCode.concat(stoMem(0));            //sto in temp register from A
        tempCode = tempCode.concat(compareXmem(0));       //if condition is false then jump to the end of while body
        if (whilestatement.booleanexpr.boolop.type == = "==") {
            tempCode.push_back("D0");                           //BNE
            let jumpIndex = tempCode.length;
            tempCode.push_back("00");
            whilestatement.block.visit(this);
            tempCode = tempCode.concat(loadXConst(1));
            tempCode = tempCode.concat(loadAConst(0));
            tempCode = tempCode.concat(stoMem(0));
            tempCode = tempCode.concat(compareXmem(0));//compare X from mem
            tempCode.push_back("D0");
            tempCode.push_back(convertHexNum(toNegative(tempCode.length - whileStart)));
            tempCode[jumpIndex] = convertHexNum(tempCode.length - jumpIndex);     //otherwise emit while loop body
        //uncondition jump back up to condition using negative offset for jump
        //emit place where you want to jump to if condition fails
        }
        else {
            tempCode.push_back("D0");                       //BNE
            let jumpIndex = tempCode.length;
            tempCode.push_back("00");
            tempCode = tempCode.concat(loadXConst(1));
            tempCode = tempCode.concat(loadAConst(0));
            tempCode = tempCode.concat(stoMem(0));
            tempCode = tempCode.concat(compareXmem(0));//compare X from mem
            tempCode.push_back("D0");
            let afterWhileIndex = tempCode.length;
            tempCode.push_back("00");
            tempCode[jumpIndex] = convertHexNum(tempCode.length - jumpIndex);
            whilestatement.block.visit(this);
            tempCode = tempCode.concat(loadXConst(1));
            tempCode = tempCode.concat(loadAConst(0));
            tempCode = tempCode.concat(stoMem(0));
            tempCode = tempCode.concat(compareXmem(0));//compare X from mem
            tempCode.push_back("D0");
            tempCode.push_back(convertHexNum(toNegative(tempCode.length - whileStart)));
            tempCode[afterWhileIndex] = convertHexNum(tempCode.length - afterWhileIndex);
        }
        

    }
    else if (currentName == "ifStatement") {    
                                    
    codeString(node->children[0]);
    tempCode = tempCode.concat(stoMem(0));        
    tempCode = tempCode.concat(loadXMem(0));      

    codeString(node->children[1]);
    tempCode = tempCode.concat(stoMem(0));        
    tempCode = tempCode.concat(compareXmem(0));   
   
        if (ifstatement.booleanExpr.boolop.type == = "==") {
            tempCode.push_back("D0");                       
            let jumpIndex = tempCode.length;
            tempCode.push_back("00");
            ifstatement.block.visit(this);
            tempCode[jumpIndex] = convertHexNum(tempCode.length - jumpIndex);
        }
        else {
            tempCode.push_back("D0");                       
            let jumpIndex = tempCode.length;
            tempCode.push_back("00");
            tempCode = tempCode.concat(loadXConst(1));
            tempCode = tempCode.concat(loadAConst(0));
            tempCode = tempCode.concat(stoMem(0));
            tempCode = tempCode.concat(compareXmem(0));
            tempCode.push_back("D0");
            let afterIfIndex = tempCode.length;
            tempCode.push_back("00");
            tempCode[jumpIndex] = convertHexNum(tempCode.length - jumpIndex);
            ifstatement.block.visit(this);
            tempCode[afterIfIndex] = convertHexNum(tempCode.length - afterIfIndex);
        }
        
    }
    else if (currentName == "id") {
    let varInfo = this.topTable.lookUp(idexpr.id.toString());                  //look up 
    if (varInfo == = null) {
        throw new Error("Var not found");
    }
    tempCode = tempCode.concat([String(10000 + varInfo.memAddr)], loadAMem(varInfo.memAddr));


    }
    else if (currentName == "intExpr") {
    tempCode = tempCode.concat(loadAConst(node->children[0].str()));    //load accumulator with left
    tempCode = tempCode.concat(stoMem(0));                                //store that into memeory
    
    codeString(node->children[1]);
    tempCode = tempCode.concat(add(0));                                  

   


    }
    else if (currentName == "boolExpr") {

      

    }
    else if (currentName == "stringExpr") {

   
    }
    else {
        for (int i = 0; i < node->children.size(); i++) {
            codeString(node->children[i]);
        }
    }


    return root;
}


void secondPass() {
    //iterate through all the elements in code and whenever 10000 is seen fix up for var memory addr 
    //if 20000 is seen fix up for string addr 
    this.varSection = [];
    for (let i = 0; i < this.startAddr; i++) {
        this.varSection.push_back("00");
    }
    let varOffset = tempCode.length + this.data.length
        let stringOffset = tempCode.length;

    for (let i = 0; i < tempCode.length; i++) {
        if (varRef(parseInt(tempCode[i].toString()))) {
            let fixedAddress = parseInt(tempCode[i].toString()) - 10000 + varOffset;  // this is the true addr of var at the very end
            //console.log(`fixedAddr = ${fixedAddress} i =  ${i} tempCode[i] =  ${tempCode[i]} varoffset = ${varOffset}`)
            let holder = [];
            splitAddress(fixedAddress, holder);
            tempCode[i + 2] = holder[0];
            tempCode[i + 3] = holder[1];
            tempCode[i] = "EA";  //EA is no OP
        }
        else if (stringRef(parseInt(tempCode[i].toString()))) {
            let fixedAddress = parseInt(tempCode[i].toString()) - 20000 + stringOffset;  // this is the true addr of string at the very end
            //console.log(`fixedAddr = ${fixedAddress} i =  ${i} tempCode[i] =  ${tempCode[i]} stringoffset = ${stringOffset}`)
            let holder = [];
            splitAddress(fixedAddress, holder);
            tempCode[i + 2] = holder[0];
            tempCode[i + 3] = holder[1];
            tempCode[i] = "EA";  //EA is no OP
        }
    }

}



int CodeGeneration::createCode(Node* node) {

    ASTNode* AST = new ASTNode;

    errorsList = {};

    symbolTree = new ScopeNode();
    root = new ASTNode;
    codeGenErrors = 0;

    codeString(node);
    secondPass();

    if (genErrors == 0) {
        cout << "\n\n\n" << endl;
        cout << "Code" << endl;
        for (string x : codeGenErrors) {
            cout << x << endl;
        }
        cout << "" << endl;
        cout << tempCode << endl;
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

 

public  addId(id : string, type : string, linePos : number, lineNum : number) {
    let newId = new Symbol(id, 0, type, linePos, lineNum);
    this.symbols.push_back(newId)
        return newId;
}
public lookUp(name: string) {
    for (let i = 0; i < this.symbols.length; i++) {
        if (name == = this.symbols[i].id) {
            return this.symbols[i];
        }
    }
    if (this.parent = null) {
        return null;
    }
    else {
        return this.parent.lookUp(name);
    }
}
}