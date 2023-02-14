#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Lexer {

public:

	Lexer();
	~Lexer();

	void lex(string);
	std::list<Token> dataSet;

private:
	
};

Lexer::~Lexer()
= default;

class Position {
	int idx;
	int ln;
	int col;
	string code;

	void advance(int);
};


class Token {
	int lineNum;
	int linePos;
	string tokenType;
	string value;
public:
	Token(string, string);
};

Token::Token(string type, string value) {
	tokenType = type;
	value = value;
}
string tokenType(string codeString, std::list<Token> dataSet) {

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

	if (regex_search(codeString, openBrac)) {
		dataSet.insert(dataSet.end(), Token("openBracToken", codeString));
		cout << "openBracToken" << endl;
	}
	else if (regex_search(codeString, closeBrac)) {
		dataSet.insert(dataSet.end(), Token("closeBracToken", codeString));
		cout << "closeBracToken" << endl;
	}
	else if (regex_search(codeString, printReg)) {
		dataSet.insert(dataSet.end(), Token("printToken", codeString));
		cout << "printToken" << endl;
	}
	else if (regex_search(codeString, openParen)) {
		dataSet.insert(dataSet.end(), Token("openParenToken", codeString));
		cout << "openParenToken" << endl;
	}
	else if (regex_search(codeString, closeParen)) {
		dataSet.insert(dataSet.end(), Token("closeParenToken", codeString));
		cout << "closeParenToken" << endl;
	}
	else if (regex_search(codeString, assignment)) {
		dataSet.insert(dataSet.end(), Token("assignmentToken", codeString));
		cout << "assignmentToken" << endl;
	}
	else if (regex_search(codeString, whileReg)) {
		dataSet.insert(dataSet.end(), Token("whileToken", codeString));
		cout << "whileToken" << endl;
	}
	else if (regex_search(codeString, spaceReg)) {
		dataSet.insert(dataSet.end(), Token("spaceToken", codeString));
		cout << "spaceToken" << endl;
	}
	else if (regex_search(codeString, intReg)) {
		dataSet.insert(dataSet.end(), Token("intToken", codeString));
		cout << "intToken" << endl;
	}
	else if (regex_search(codeString, stringReg)) {
		dataSet.insert(dataSet.end(), Token("stringToken", codeString));
		cout << "stringToken" << endl;
	}
	else if (regex_search(codeString, boolReg)) {
		dataSet.insert(dataSet.end(), Token("boolToken", codeString));
		cout << "boolToken" << endl;
	}
	else if (regex_search(codeString, charReg)) {
		dataSet.insert(dataSet.end(), Token("charToken", codeString));
		cout << "charToken" << endl;
	}
	else if (regex_search(codeString, digitReg)) {
		dataSet.insert(dataSet.end(), Token("digitToken", codeString));
		cout << "digitToken" << endl;
	}
	else if (regex_search(codeString, boolEqual)) {
		dataSet.insert(dataSet.end(), Token("boolEqualToken", codeString));
		cout << "boolEqualToken" << endl;
	}
	else if (regex_search(codeString, boolNot)) {
		dataSet.insert(dataSet.end(), Token("boolNotToken", codeString));
		cout << "boolNotToken" << endl;
	}
	else if (regex_search(codeString, falseReg)) {
		dataSet.insert(dataSet.end(), Token("falseToken", codeString));
		cout << "falseToken" << endl;
	}
	else if (regex_search(codeString, trueReg)) {
		dataSet.insert(dataSet.end(), Token("trueToken", codeString));
		cout << "trueToken" << endl;
	}
	else if (regex_search(codeString, addition)) {
		dataSet.insert(dataSet.end(), Token("addToken", codeString));
		cout << "addToken" << endl;
	}
	else if (regex_search(codeString, startComment)) {
		dataSet.insert(dataSet.end(), Token("startComToken", codeString));
		cout << "startComToken" << endl;
	}
	else if (regex_search(codeString, endComment)) {
		dataSet.insert(dataSet.end(), Token("endComToken", codeString));
		cout << "endComToken" << endl;
	}
	else if (regex_search(codeString, endProgram)) {
		dataSet.insert(dataSet.end(), Token("endProgram", codeString));
		cout << "endProgram" << endl;
	}
	else
		cout << "ERROR!" << endl;
}

void Lexer::lex(string program)
{
	std::list<Token> dataSet;
	std::list<Token>::iterator it;
	it = dataSet.begin();
	int next = 0;
	string codeString = "";
	for (auto start = program.begin(); start != program.end(); ++start) {
		++next;
		

	}
	
	


	std::cout << "------------------------------------------------------------------" << std::endl;
	std::cout << "[VERBOSE] Beginning Lexer " << std::endl;

	std::cout << "------------------------------------------------------------------" << std::endl;
}