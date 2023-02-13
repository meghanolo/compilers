#include <fstream>
#include <regex>
#include <iostream>
#include <string>

using namespace std;

class Lexer {

public:

	Lexer();
	~Lexer();


	void lex();

private:
	
};

Lexer::~Lexer()
= default;

void Lexer::lex()
{
	regex openBrac("{");
	regex closeBrac("}");
	regex printReg("print");
	regex openParen("(");
	regex closeParen(")");
	regex assignment("=");
	regex whileReg("while");
	regex ifReg("if");
	regex spaceReg(" ");
	regex intReg("int");
	regex stringReg("string");
	regex boolReg("boolean");
	regex charReg("[a-z]");
	regex digitReg("[0-9]");
	regex boolEqual("==");
	regex boolNot("!=");
	regex falseReg("false");
	regex trueReg("true");
	regex addition("+");
	regex startComment("/*");
	regex endComment("*/");
	regex endProgram("$");

	string codeString;
	
	if (regex_search(codeString, openBrac))
		cout << "openBracToken" << endl;
	else if (regex_search(codeString, closeBrac))
		cout << "closeBracToken" << endl;
	else if (regex_search(codeString, printReg))
		cout << "printToken" << endl;
	else if (regex_search(codeString, openParen))
		cout << "openParenToken" << endl;
	else if (regex_search(codeString, closeParen))
		cout << "closeParenToken" << endl;
	else if (regex_search(codeString, assignment))
		cout << "assignmentToken" << endl;
	else if (regex_search(codeString, whileReg))
		cout << "whileToken" << endl;
	else if (regex_search(codeString, spaceReg))
		cout << "spaceToken" << endl;
	else if (regex_search(codeString, intReg))
		cout << "intToken" << endl;
	else if (regex_search(codeString, stringReg))
		cout << "stringToken" << endl;
	else if (regex_search(codeString, boolReg))
		cout << "boolToken" << endl;
	else if (regex_search(codeString, charReg))
		cout << "charToken" << endl;
	else if (regex_search(codeString, digitReg))
		cout << "digitToken" << endl;
	else if (regex_search(codeString, boolEqual))
		cout << "boolEqualToken" << endl;
	else if (regex_search(codeString, boolNot))
		cout << "boolNotToken" << endl;
	else if (regex_search(codeString, falseReg))
		cout << "falseToken" << endl;
	else if (regex_search(codeString, trueReg))
		cout << "trueToken" << endl;
	else if (regex_search(codeString, addition))
		cout << "addToken" << endl;
	else if (regex_search(codeString, startComment))
		cout << "startComToken" << endl;
	else if (regex_search(codeString, endComment))
		cout << "endComToken" << endl;
	else if (regex_search(codeString, endProgram))
		cout << "endProgramToken" << endl;
	else
		cout << "ERROR!" << endl;

	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "[VERBOSE] Beginning Lexer " << std::endl;
	std::cout << "------------------------------------------------------------------" << std::endl;
}