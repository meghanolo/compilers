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
	void lex(string, int, int);

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


list<Token> codeStringAnalysis(string codeStringFull) {

	list<Token> dataSet;

	regex printReg("print");
	regex whileReg("while");
	regex stringReg("string");
	regex boolReg("boolean");
	regex ifReg("if");
	regex intReg("int");
	regex charReg("[a-z]");
	regex digitReg("[0-9]");
	regex falseReg("false");
	regex trueReg("true");
	string codeString = "";

	for (int i = 0; i < codeStringFull.length(); i++) {
		codeString += codeStringFull.substr(i, 1);
		if (regex_search(codeString, boolReg)) {
			dataSet.insert(dataSet.end(), Token("boolToken", codeString));
			//cout << "boolToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, stringReg)) {
			dataSet.insert(dataSet.end(), Token("stringToken", codeString));
			//cout << "stringToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, printReg)) {
			dataSet.insert(dataSet.end(), Token("printToken", codeString));
			//cout << "printToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, whileReg)) {
			dataSet.insert(dataSet.end(), Token("whileToken", codeString));
			//cout << "whileToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, falseReg)) {
			dataSet.insert(dataSet.end(), Token("falseToken", codeString));
			//cout << "falseToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, trueReg)) {
			dataSet.insert(dataSet.end(), Token("trueToken", codeString));
			//cout << "trueToken" << endl;
			codeString = "";
		}
		else if (regex_search(codeString, intReg)) {
			dataSet.insert(dataSet.end(), Token("intToken", codeString));
			//cout << "intToken" << endl;
			codeString = "";
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
	regex boolNot("[!=]");
	regex addition("[\\+]");
	regex startComment("/\\*");
	regex endProgram("[\\$]");
	regex quotes("[\"]");

	if (regex_match(symbolString, openBrac)) {
		return "openBracToken";
	}
	else if (regex_match(symbolString, startComment)) {
		return "startComToken";
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

void Lexer::lex(string program, int lineNum, int lineCount) {
	list<Token> masterTokenStream;

	int counter = 0;
	string symbol;
	bool inQuotes = false;
	bool isComment = false;
	string codeString = "";
	string symbolString = "";
	regex symbolReg("[\\{\\$\\+\\}\\(\\)=!/\\*\"]");
	regex doubleSymbolReg("[=!/\\*]");
	regex charReg("[a-z]");
	regex spaceReg(" ");
	regex digitReg("[0-9]");
	regex closeComment("\\*/");
	regex quoteReg("[\"]");

	for (int i = 0; i < program.length(); i++) {

		string single = program.substr(i, 1);
		string iAmComment;

		if (isComment == true) {
			for (int m = i; m < program.length(); m++) {
				iAmComment += program[m];
				if (regex_search(iAmComment, closeComment)) {
					masterTokenStream.push_back(Token("closeComToken", "*/"));
					//cout << "closeComToken" << endl;
					isComment = false;
					i = m;
					break;
				}
			}
		}
		else if (inQuotes == true) {
			if (regex_search(single, charReg)) {
				masterTokenStream.push_back(Token("charToken", single));
				//cout << "charToken" << single << endl;
			}
			else if (regex_search(single, spaceReg)) {
				masterTokenStream.push_back(Token("spaceToken", " "));
				//cout << "spaceToken" << endl;
			}
			else if (regex_search(single, quoteReg)) {
				masterTokenStream.push_back(Token("quoteToken"));
				//cout << "quoteToken" << endl;
				inQuotes = false;
			}
			else
				masterTokenStream.push_back(Token("ERROR", single));
		}
		else if (regex_search(single, symbolReg)) {

			if (codeString != "") {
				for (const Token token : codeStringAnalysis(codeString)) {
					cout << "hit" << endl;
					cout << codeString << endl;
					masterTokenStream.push_back(token);
					codeString = "";
				}
			}
			else if (symbolString == "") {
				symbolString += program[i];
				if (regex_search(symbolString, doubleSymbolReg)) {
					continue;
				}
				else {
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString));
					//cout << symbol << endl;
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
			else {
				symbolString += program[i];
				symbol = symbolAnalysis(symbolString);
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes == true)
						inQuotes = false;
					else
						inQuotes = true;
				}
				//cout << symbol << endl;
				masterTokenStream.push_back(Token(symbol, symbolString));
				symbolString = "";
			}			
			
		}
		else if (regex_search(single, charReg)) {
			if (symbolString != "") {
				symbol = symbolAnalysis(symbolString);
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes == true)
						inQuotes = false;
					else
						inQuotes = true;
				}
				//cout << symbol << endl;
				masterTokenStream.push_back(Token(symbol, symbolString));
				symbolString = "";
			}
			codeString += program[i];
		}

		else if (regex_search(single, digitReg)) {
			if (symbolString != "") {
				symbol = symbolAnalysis(symbolString);
				if (symbol == "startComToken") {
					isComment = true;
				}
				if (symbol == "quoteToken") {
					if (inQuotes == true)
						inQuotes = false;
					else
						inQuotes = true;
				}
				//cout << symbol << endl;
				masterTokenStream.push_back(Token(symbol, symbolString));
				symbolString = "";
			}
			codeString += program[i];
		}
		else if (regex_search(single, spaceReg)) {
		continue;
		}
		else
			masterTokenStream.push_back(Token("ERROR", single));
	}

	cout << "------------------------------------------------------------------" << endl;
	cout << "[VERBOSE] Beginning Lexer " << endl;

	cout << "------------------------------------------------------------------" << endl;

	if (lineCount == 1)
		lineNum = lineNum;
	else
		lineNum = lineNum + lineCount;

	for (auto const& i : masterTokenStream) {
		cout << "LEXER --> | " << i.tokenType << " [ " << i.value << " ] on line " << lineNum << endl;
	}
	
	
}