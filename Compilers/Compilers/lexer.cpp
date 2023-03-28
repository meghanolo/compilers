#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
		linePos++;
		if (codeString.substr(0, 7) == "boolean") {
			//cout << "boolToken" << endl;
			codeString.erase(0, 7);
			dataSet.insert(dataSet.end(), Token("boolToken", "boolean", linePos));
		}
		else if (codeString.substr(0, 6) == "string") {
			//cout << "stringToken" << endl;
			codeString.erase(0, 6);
			dataSet.insert(dataSet.end(), Token("stringToken", "string", linePos));
		}
		else if (codeString.substr(0, 5) == "print") {
			//cout << "printToken" << endl;
			codeString.erase(0, 5);
			dataSet.insert(dataSet.end(), Token("printToken", "print", linePos));
		}
		else if (codeString.substr(0, 5) == "while") {
			//cout << "whileToken" << endl;
			codeString.erase(0, 5);
			dataSet.insert(dataSet.end(), Token("whileToken", "while", linePos));
		}
		else if (codeString.substr(0, 2) == "if") {
			//cout << "ifToken" << endl;
			codeString.erase(0, 2);
			dataSet.insert(dataSet.end(), Token("ifToken", "if", linePos));
		}
		else if (codeString.substr(0, 5) == "false") {
			//cout << "falseToken" << endl;
			codeString.erase(0, 5);
			dataSet.insert(dataSet.end(), Token("falseToken", "false", linePos));
		}
		else if (codeString.substr(0, 4) == "true") {
			//cout << "trueToken" << endl;
			codeString.erase(0, 4);
			dataSet.insert(dataSet.end(), Token("trueToken", "true", linePos));
		}
		else if (codeString.substr(0, 3) == "int") {
			codeString.erase(0, 3);
			dataSet.insert(dataSet.end(), Token("intToken", "int", linePos));
		}
		else if (regex_match(codeString.substr(0, 1), charReg)) {
			dataSet.insert(dataSet.end(), Token("charToken", codeString.substr(0, 1), linePos));
			//cout << "trueToken" << endl;
			codeString.erase(0, 1);
		}
		else if (regex_match(codeString.substr(0, 1), digitReg)) {
			dataSet.insert(dataSet.end(), Token("digitToken", codeString.substr(0, 1), linePos));
			//cout << "intToken" << endl;
			codeString.erase(0, 1);
		}
		else {
			dataSet.insert(dataSet.end(), Token("ERROR", codeString, linePos));
			//cout << "ERROR" << endl;
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
		return "ERROR";
};

vector<Token> Lexer::lex(string program, int lineNum) {
	vector<Token> masterTokenStream;
	vector<Token> temp;

	int linePos = 0;
	int counter = 0;
	string symbol;
	bool inQuotes = false;
	bool isComment = false;
	string codeString = "";
	string symbolString = "";
	regex symbolReg("[\\{\\$\\+\\}\\(\\)=!/\\*\"]");
	regex doubleSymbolReg("[=!/]");
	regex charReg("[a-z]");
	regex spaceReg(" ");
	regex digitReg("[0-9]");
	regex closeComment("\\*/");
	regex quoteReg("[\"]");
	regex newLine("[\n]");

	for (int i = 0; i < program.length(); i++) {

		linePos++;

		string single = program.substr(i, 1);
		string iAmComment;

		if (isComment == true) {
			for (int m = i; m < program.length(); m++) {
				iAmComment += program[m];
				if (regex_search(iAmComment, closeComment)) {
					masterTokenStream.push_back(Token("closeComToken", "*/", linePos));
					isComment = false;
					i = m;
					break;
				}
			}
		}
		else if (inQuotes == true) {
			if (regex_search(single, charReg)) {
				masterTokenStream.push_back(Token("charToken", single, linePos));
			}
			else if (regex_search(single, spaceReg)) {
				masterTokenStream.push_back(Token("spaceToken", " ", linePos));
			}
			else if (regex_search(single, quoteReg)) {
				masterTokenStream.push_back(Token("quoteToken", "\"", linePos));
				inQuotes = false;
			}
			else
				masterTokenStream.push_back(Token("ERROR", single, linePos));
		}
		else if (regex_search(single, charReg) || regex_search(single, digitReg)) {
			codeString += single;
		}
		else if (regex_search(single, symbolReg)) {
			symbolString += single;
			if (regex_search(symbolString, doubleSymbolReg)) {
				if (regex_search(program.substr(i + 1, 1), symbolReg)) {
					symbolString += program.substr(i + 1, 1);
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, linePos));
					symbolString = "";
					i++;
				}
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes)
						inQuotes = false;
					else
						inQuotes = true;
				}
				if (codeString != "") {
					temp = codeStringAnalysis(codeString, linePos);
					for (const Token token : temp) {
						masterTokenStream.push_back(token);
					}
					codeString = "";
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, linePos));
					if (symbol == "startComToken") {
						isComment = true;
					}
					if (symbol == "quoteToken") {
						if (inQuotes)
							inQuotes = false;
						else
							inQuotes = true;
					}
					symbolString = "";
				}
			}
			else if (codeString != "") {
				temp = codeStringAnalysis(codeString, linePos);
				for (const Token token : temp) {
					masterTokenStream.push_back(token);
				}
				codeString = "";
				symbol = symbolAnalysis(symbolString);
				masterTokenStream.push_back(Token(symbol, symbolString, linePos));
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes)
						inQuotes = false;
					else
						inQuotes = true;
				}
				symbolString = "";
			}
			else {
				symbol = symbolAnalysis(symbolString);
				masterTokenStream.push_back(Token(symbol, symbolString, linePos));
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes)
						inQuotes = false;
					else
						inQuotes = true;
				}
				symbolString = "";
			}


		}
		else if (regex_search(single, spaceReg))
			continue;
		else
			masterTokenStream.push_back(Token("ERROR", single, linePos));
	}

	cout << "------------------------------------------------------------------" << endl;
	cout << "[VERBOSE] Beginning Lexer " << endl;

	cout << "------------------------------------------------------------------" << endl;

	for (auto const& i : masterTokenStream) {
		cout << "LEXER --> | " << i.tokenType << " [ " << i.value << " ] on line [" << lineNum << ":" << i.linePosition << "] " << endl;
	}

	cout << "\n\n" << endl;
	return masterTokenStream;
}