// Compilers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "lexer.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    //std::cout << "Hello World!\n";
	
	fstream myfile;
	string fileName = argv[1];

	myfile.open(fileName,ios::in);

	regex endProgramReg("$");

	if (myfile.is_open()) { //checking whether the file is open
		//Lexer myLex; //  Stack allocation. To allocate on the heap use "new Lexer()".
		string tp;
		//int lineCount = 0;
		bool endProgram = false;
		while (!endProgram) {
			string program = "";
			while (getline(myfile, tp)) { //read data from file object and put it into string.
				//++lineCount;
				program += tp;
				if (regex_search(program, endProgramReg)) {
					endProgram = true;
					cout << program << "\n"; //print the data of the string
					program = "";
				}
			}
		}
		
		myfile.close(); //close the file object.
	}

	/*
	myLex.lex();
	*/
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
