#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool isComment;
bool inQuotes;
int errors = 0;

//Create a Token object with a type, value, and line position
class Token {
public:
	string tokenType;
	string value;
	int linePosition;
	int lineNum;
	Token(string x, string y, int z, int a) {
		tokenType = x;
		value = y;
		linePosition = z;
		lineNum = a;
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

//Analyze a string of anything but symbols in the program and determine
	//which token should be created based on the input.
	//If the program contains a character not found in the grammar, this will return 
	//an error, otherwise, it will return a vector of Tokens to be added to the
	//larger token stream.
vector<Token> codeStringAnalysis(string codeString, int linePos, int lineNum) {

	vector<Token> dataSet;

	regex charReg("[a-z]");
	regex digitReg("[0-9]");

	//Since we are not entering this function until a symbol is found,
		//we must backtrack to get the correct line position
	int length = codeString.length();
	int size = (linePos - length);

	while (codeString.length() > 0) {
		if (codeString.substr(0, 7) == "boolean") {
			codeString.erase(0, 7);
			dataSet.push_back(Token("boolToken", "boolean", (size+7), lineNum));
		}
		else if (codeString.substr(0, 6) == "string") {
			codeString.erase(0, 6);
			dataSet.push_back(Token("stringToken", "string", (size+6), lineNum));
		}
		else if (codeString.substr(0, 5) == "print") {
			codeString.erase(0, 5);
			dataSet.push_back(Token("printToken", "print", (size+5), lineNum));
		}
		else if (codeString.substr(0, 5) == "while") {
			//cout << "whileToken" << endl;
			codeString.erase(0, 5);
			dataSet.push_back(Token("whileToken", "while", (size+5), lineNum));
		}
		else if (codeString.substr(0, 2) == "if") {
			codeString.erase(0, 2);
			dataSet.push_back(Token("ifToken", "if", (size+2), lineNum));
		}
		else if (codeString.substr(0, 5) == "false") {
			codeString.erase(0, 5);
			dataSet.push_back(Token("falseToken", "false", (size+5), lineNum));
		}
		else if (codeString.substr(0, 4) == "true") {
			codeString.erase(0, 4);
			dataSet.push_back(Token("trueToken", "true", (size+4), lineNum));
		}
		else if (codeString.substr(0, 3) == "int") {
			codeString.erase(0, 3);
			dataSet.push_back(Token("intToken", "int", (size+3), lineNum));
		}
		else if (regex_match(codeString.substr(0, 1), charReg)) {
			dataSet.push_back(Token("charToken", codeString.substr(0, 1), (size+1), lineNum));
			//cout << "lexer " << codeString.substr(0, 1) << endl;
			codeString.erase(0, 1);
		}
		else if (regex_match(codeString.substr(0, 1), digitReg)) {
			dataSet.push_back(Token("digitToken", codeString.substr(0, 1), (size+1), lineNum));
			codeString.erase(0, 1);
		}
		else {
			dataSet.push_back(Token("ERROR", codeString, size, lineNum));
			errors++;
			codeString.erase(0);
		}
	}
	return dataSet;
}

//Using regex, analyze the symbols and produce a token based on the type of symbol found in the program.
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
	//Set comment boolean to true if startComment symbol is found to ignore all that is found inside comments
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
	//Set quotes boolean to true if quoteToken symbol is found.
	else if (regex_match(symbolString, quotes)) {
		inQuotes = true;
		return "quoteToken";
	}
	else {
		errors++;
		return "ERROR";
	}
};

//Main function to perform lexical analysis on a program and return a Token stream.
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

	//Iterate over the entire program
	for (int i = 0; i < program.length(); i++) {

		//Analyze the program one character at a time by using single as a pointer
		single = program.substr(i, 1);
		
		//Check if we are inside a comment. If so, ignore all code until the comment ends and add the correct
			//comment tokens.
		if (isComment == true) {
			for (int m = i; m < program.length(); m++) {
				iAmComment = program.substr(m, 2);
				if (regex_search(iAmComment, closeComment)) {
					masterTokenStream.push_back(Token("closeComToken", "*/", m, lineNum));
					isComment = false;
					i = ++m;
					break;
				}
			}
		}

		//Check if we are in a string using quotes. If so, space tokens and char tokens can be generated until
			//quotes are found again.
		else if (inQuotes == true) {
			if (regex_search(single, quoteReg)) {
				masterTokenStream.push_back(Token("quoteToken", "\"", i, lineNum));
				inQuotes = false;
			}
			else if (regex_search(single, charReg)) {
				masterTokenStream.push_back(Token("charToken", single, i, lineNum));
			}
			else if (regex_search(single, spaceReg)) {
				masterTokenStream.push_back(Token("spaceToken", " ", i, lineNum));
			}
			else {
				masterTokenStream.push_back(Token("ERROR", single, i, lineNum));
				errors++;
			}
		}

		//Check if we have found a symbol, if so, analyze the program up to that point and create
			//respective tokens. After, determine if the symbol is one or two characters and 
			//create symbol tokens based on such.
		else if (regex_search(single, symbolReg)) {
			symbolString += single;
			if (codeString != "") {
				temp = codeStringAnalysis(codeString, i, lineNum);
				for (const Token token : temp) {
					masterTokenStream.push_back(token);
				}
				codeString = "";
			}
			//If the symbol might be two characters, check the next position before creating a token...
			if (regex_search(symbolString, doubleSymbolReg)) {
				if (regex_search(program.substr(i + 1, 1), symbolReg)) {
					symbolString += program.substr(i + 1, 1);
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i, lineNum));
					symbolString = "";
					i++;
				}
				else {
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i, lineNum));
					symbolString = "";
				}
			}
			//otherwise, create a symbol token using symbolAnalysis.
			else {
					symbol = symbolAnalysis(symbolString);
					masterTokenStream.push_back(Token(symbol, symbolString, i, lineNum));
					symbolString = "";
			}

					
				}
		//If a space is found outside quotes, ignore it.
		else if (regex_search(single, spaceReg))
			continue;
		//If any other character is found, add it to codeString to be analyzed after a symbol
			//is found in codeStringAnalysis.
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
		cout << "INFO - SUCCESS! Lexer completed with 0 errors." << endl;
	else
		cout << "INFO - Lexer completed with " << errors << " errors." << endl;
	cout << "\n\n" << endl;
	return masterTokenStream;
}