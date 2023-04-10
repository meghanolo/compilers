#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool isComment;
bool inQuotes;
int errors = 0;

class Token {
public:
	string tokenType;
	string value;
	int linePosition;
	Token(string x, string y, int z) {
		tokenType = x;
		value = y;
		linePosition = z;
	}
	Token(string x) {
		tokenType = x;
	}
};


class Lexer {

public:
	~Lexer();
	vector<Token> lex(string, int);

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


vector<Token> codeStringAnalysis(string codeString, int linePos) {

	vector<Token> dataSet;

	regex charReg("[a-z]");
	regex digitReg("[0-9]");

	while (codeString.length() > 0) {
		if (codeString.substr(0, 7) == "boolean") {
			//cout << "boolToken" << endl;
			codeString.erase(0, 7);
			dataSet.push_back(Token("boolToken", "boolean", (linePos-7)));
		}
		else if (codeString.substr(0, 6) == "string") {
			//cout << "stringToken" << endl;
			codeString.erase(0, 6);
			dataSet.push_back(Token("stringToken", "string", (linePos-6)));
		}
		else if (codeString.substr(0, 5) == "print") {
			//cout << "printToken" << endl;
			codeString.erase(0, 5);
			dataSet.push_back(Token("printToken", "print", (linePos-5)));
		}
		else if (codeString.substr(0, 5) == "while") {
			//cout << "whileToken" << endl;
			codeString.erase(0, 5);
			dataSet.push_back(Token("whileToken", "while", (linePos-5)));
		}
		else if (codeString.substr(0, 2) == "if") {
			//cout << "ifToken" << endl;
			codeString.erase(0, 2);
			dataSet.push_back(Token("ifToken", "if", (linePos-2)));
		}
		else if (codeString.substr(0, 5) == "false") {
			//cout << "falseToken" << endl;
			codeString.erase(0, 5);
			dataSet.push_back(Token("falseToken", "false", (linePos-5)));
		}
		else if (codeString.substr(0, 4) == "true") {
			//cout << "trueToken" << endl;
			codeString.erase(0, 4);
			dataSet.push_back(Token("trueToken", "true", (linePos-4)));
		}
		else if (codeString.substr(0, 3) == "int") {
			codeString.erase(0, 3);
			dataSet.push_back(Token("intToken", "int", (linePos-3)));
		}
		else if (regex_match(codeString.substr(0, 1), charReg)) {
			dataSet.push_back(Token("charToken", codeString.substr(0, 1), (linePos-1)));
			//cout << "trueToken" << endl;
			codeString.erase(0, 1);
		}
		else if (regex_match(codeString.substr(0, 1), digitReg)) {
			dataSet.push_back(Token("digitToken", codeString.substr(0, 1), (linePos-1)));
			//cout << "intToken" << endl;
			codeString.erase(0, 1);
		}
		else {
			dataSet.push_back(Token("ERROR", codeString, linePos));
			//cout << "ERROR" << endl;
			errors++;
			codeString.erase(0);
		}
	}
	return dataSet;
}


string symbolAnalysis(string symbolString) {

	regex openBrac("[\\{]");
	regex closeBrac("[\\}]");
	regex openParen("[\\(]");
	regex closeParen("[\\)]");
	regex assignment("=");
	regex boolEqual("==");
	regex boolNot("!=");
	regex addition("[\\+]");
	regex startComment("/\\*");
	regex endComment("\\*/");
	regex endProgram("[\\$]");
	regex quotes("\"");

	if (regex_match(symbolString, openBrac)) {
		return "openBracToken";
	}
	else if (regex_match(symbolString, startComment)) {
		isComment = true;
		return "startComToken";
	}
	else if (regex_match(symbolString, endComment)) {
		return "closeComToken";
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
		inQuotes = true;
		return "quoteToken";
	}
	else {
		errors++;
		return "ERROR";
	}
};

vector<Token> Lexer::lex(string program, int lineNum) {
	vector<Token> masterTokenStream;
	vector<Token> temp;

	int linePos = 0;
	errors = 0;
	int counter = 0;
	string symbol;
	string codeString = "";
	string symbolString = "";
	regex symbolReg("[\\{\\$\\+\\}\\(\\)=!/\\*\"]");
	regex doubleSymbolReg("[=!/\\*]");
	regex charReg("[a-z]");
	regex spaceReg(" ");
	regex digitReg("[0-9]");
	regex closeComment("\\*/");
	regex quoteReg("[\"]");
	regex newLine("[\n]");
	string iAmComment;
	string single = "";
	isComment = false;
	inQuotes = false;

	for (int i = 0; i < program.length(); i++) {

		single = program.substr(i, 1);

		if (isComment == true) {
			for (int m = i; m < program.length(); m++) {
				iAmComment = program.substr(m, 2);
				if (regex_search(iAmComment, closeComment)) {
					masterTokenStream.push_back(Token("closeComToken", "*/", m));
					isComment = false;
					i = ++m;
					break;
				}
			}
		}
		else if (inQuotes == true) {
			if (regex_search(single, quoteReg)) {
				masterTokenStream.push_back(Token("quoteToken", "\"", i));
				inQuotes = false;
			}
			else if (regex_search(single, charReg)) {
				masterTokenStream.push_back(Token("charToken", single, i));
			}
			else if (regex_search(single, spaceReg)) {
				masterTokenStream.push_back(Token("spaceToken", " ", i));
			}
			else {
				masterTokenStream.push_back(Token("ERROR", single, i));
				errors++;
			}
		}
		else if (regex_search(single, symbolReg)) {
			symbolString += single;
			if (codeString != "") {
				temp = codeStringAnalysis(codeString, i);
				for (const Token token : temp) {
					masterTokenStream.push_back(token);
				}
				codeString = "";
			}
			if (regex_search(symbolString, doubleSymbolReg)) {
				if (regex_search(program.substr(i + 1, 1), symbolReg)) {
					symbolString += program.substr(i + 1, 1);
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i));
					symbolString = "";
					i++;
				}
				else {
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i));
					symbolString = "";
				}
			}
			else {
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i));
					symbolString = "";
			}

					
				}

		else if (regex_search(single, spaceReg))
			continue;
		else
			codeString += single;
	
}

	cout << "------------------------------------------------------------------" << endl;
	cout << "[VERBOSE] Beginning Lexer " << endl;

	cout << "------------------------------------------------------------------" << endl;

	for (auto const& i : masterTokenStream) {
		cout << "LEXER --> | " << i.tokenType << " [ " << i.value << " ] on line [" << lineNum << ":" << i.linePosition << "] " << endl;
	}
	if (errors == 0)
		cout << "SUCCESS! Lexer completed with 0 errors." << endl;
	else
		cout << "Lexer completed with " << errors << " errors." << endl;
	cout << "\n\n" << endl;
	return masterTokenStream;
}