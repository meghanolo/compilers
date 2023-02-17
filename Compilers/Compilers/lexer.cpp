#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Token {       
	public:             
		string tokenType;
		string value;
		Token(string x, string y) {
			tokenType = x;
			value = y;
		}
		Token(string x) {
			tokenType = x;
		}
};


class Lexer {

public:
	~Lexer();
	void lex(string);

private:
	
};

Lexer::~Lexer()
= default;

/*class Position {
	int idx;
	int ln;
	int col;
	string code;

	void advance(int);
};*/


list<Token> codeStringAnalysis(string codeString) {

	list<Token> dataSet;

	regex printReg("print");
	regex whileReg("while");
	regex stringReg("string");
	regex boolReg("boolean");
	regex ifReg("if");
	regex spaceReg(" ");
	regex intReg("int");
	regex charReg("[a-z]");
	regex digitReg("[0-9]");
	
	regex falseReg("false");
	regex trueReg("true");

	if (regex_search(codeString, printReg)) {
		dataSet.insert(dataSet.end(), Token("printToken", codeString));
		cout << "printToken" << endl;
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

	else if (regex_search(codeString, falseReg)) {
		dataSet.insert(dataSet.end(), Token("falseToken", codeString));
		cout << "falseToken" << endl;
	}
	else if (regex_search(codeString, trueReg)) {
		dataSet.insert(dataSet.end(), Token("trueToken", codeString));
		cout << "trueToken" << endl;
	}
	else
		cout << "ERROR!" << endl;
	return dataSet;
}

string symbolAnalysis(string symbolString) {

	regex openBrac("[\\{]");
	regex closeBrac("[\\}]");
	regex openParen("[\\(]");
	regex closeParen("[\\)]");
	regex assignment("[=]");
	regex boolEqual("==");
	regex boolNot("[!=]");
	regex addition("[\\+]");
	regex startComment("[/\\*]");
	regex endComment("[\\*/]");
	regex endProgram("[\\$]");
	regex quotes(" ");

	if (regex_match(symbolString, openBrac)) {
		return "openBracToken";
	}
	else if (regex_match(symbolString, startComment)) {
		return "startComToken";
	}
	else if (regex_match(symbolString, endComment)) {
		return "endComToken";
	}
	else if (regex_match(symbolString, endProgram)) {
		return "endProgram";
	}
	else if (regex_match(symbolString, boolEqual)) {
		return "boolEqualToken";
	}
	else if (regex_match(symbolString, boolNot)) {
		return "boolNotToken";
	}
	else if (regex_match(symbolString, closeBrac)) {
		return "closeBracToken";
	}
	else if (regex_match(symbolString, openParen)) {
		return "openParenToken";
	}
	else if (regex_match(symbolString, closeParen)) {
		return "closeParenToken";
	}
	else if (regex_match(symbolString, assignment)) {
		return "assignmentToken";
	}
	else if (regex_match(symbolString, addition)) {
		return "addToken";
	}
	else if (regex_match(symbolString, quotes)) {
		return "quoteToken";
	}
	else
		return "";
};

void Lexer::lex(string program) {
	list<Token> masterTokenStream;

	int counter = 0;
	string symbol;
	bool inQuotes = false;
	string codeString = "";
	string symbolString = "";
	//regex symbolReg("[{}()=!/*$]");
	regex symbolReg("[\\{\\$\\+\\}\\(\\)=!/]");
	regex doubleSymbolReg("[=!/\\*]");
	regex charReg("[a-z]");
	regex digitReg("[0-9]");

	for (int i = 0; i < program.length(); i++) {

		string single = program.substr(i, 1);
		
		if (regex_search(single, symbolReg)) {

			if (symbolString == "") {
				symbolString += program[i];
				if (regex_search(symbolString, doubleSymbolReg)) {
					continue;
				}
				else {
					symbol = symbolAnalysis(symbolString);
					cout << symbol << endl;
					symbolString = "";
				}
			}
			else {
				symbolString += program[i];
				symbol = symbolAnalysis(symbolString);
				cout << symbol << endl;
				symbolString = "";
			}
				
			
			if (codeString != "") {
				//codeStringAnalysis(codeString);
				cout << "hit" << endl;
			}
		}

		else if (regex_search(single, charReg)) {
			if (symbolString != "") {
				symbol = symbolAnalysis(symbolString);
				if (symbol == "quoteToken") {
					if (inQuotes)
						inQuotes = false;
					else
						inQuotes = true;
				}
				cout << symbol << endl;

				masterTokenStream.push_back(Token(symbol));
				symbolString = "";
			}
			codeString += program[i];
		}

		else if (regex_search(single, digitReg)) {
			if (symbolString != "") {
				symbol = symbolAnalysis(symbolString);
				if (symbol == "quoteToken") {
					if (inQuotes)
						inQuotes = false;
					else
						inQuotes = true;
				}
				cout << symbol << endl;
				masterTokenStream.push_back(Token(symbol));
				symbolString = "";
			}
			codeString += program[i];
		}


	}

	for (auto const& i : masterTokenStream) {
		cout << i.tokenType << endl;
	}
	
	cout << "------------------------------------------------------------------" << endl;
	cout << "[VERBOSE] Beginning Lexer " << endl;

	cout << "------------------------------------------------------------------" << endl;
}