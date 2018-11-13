#include <stdio.h> 
#include <stdlib.h> 
#include <fstream>
#include <ctype.h>
using namespace std;
char symbol; 
std::string obtainedSymbol; 
int errorLine; 
int errorCol; 
int defaultCol; 
ifstream inputFile;
char c, peekC;
//Prototypes
void VariableSymbol();void Letter();void Digit(); void Accept(std::string); void TestProcedure();

void SkipBlanks(){
	while(c==' ' || c=='\n' || c=='\t'){
		if(c=='\n')
		{
			errorCol=defaultCol;
			errorLine++;
		}
		else
			errorCol++;
		inputFile.get(c);
		if(inputFile.eof())
			return;
	}
}

void ErrrorHandling(){
	if(inputFile.eof())
		printf("Rejected at line %d column %d.\nNo more symbols\n",errorLine, errorCol, obtainedSymbol.c_str());
	else
		printf("Rejected at line %d column %d.\nSymbol: %s\n",errorLine, errorCol, obtainedSymbol.c_str());
	exit(0);
}

bool IsEnder(char c){
	if(inputFile.eof()) 
		return true;
	
	else if( c==' ' || c=='\t' || c=='!'|| c=='@'|| c=='#'|| c=='%'|| c=='^'|| c=='&'|| c=='*'|| c=='('|| c==')'|| c=='-'|| c=='+'|| c=='/'
	|| c=='"'|| c=='+'|| c=='-' || c=='[' || c==']' || c=='<' || c=='>' || c=='=' || c=='|' || c=='.' || c==';' ){
		return true;
	}
	
	else if(c=='\n'){
		return true;
	}
	else
		return false;
}

void GetSymbol(){
	//printf("Accepted Symbol: %s\n",obtainedSymbol.c_str());//check what is accepted before
	inputFile.get(c);
	errorCol++;
	obtainedSymbol = "";
	SkipBlanks();
	if(inputFile.eof())
		return;
	//save first symbol that is not a blank
	obtainedSymbol += c;
	if(IsEnder(c))//if special character found
	{
		//ifs for super special character
		peekC = inputFile.peek();
		if(c=='=' || c=='+' || c=='-' || c=='<' || c=='>'){//for ==, ++, --, <<, >>
			if(peekC==c){
				inputFile.get(c);
				errorCol++;
				obtainedSymbol += c;
				peekC = inputFile.peek();
				if(c=='>' && peekC=='>')//for >>
				{
					inputFile.get(c);
					errorCol++;
					obtainedSymbol += c;
					peekC = inputFile.peek();
				}
				if((c=='<' ||c=='>') && peekC == '=')//for <<=, >>>=
				{
					inputFile.get(c);
					errorCol++;
					obtainedSymbol += c;
					peekC = inputFile.peek();
				}
				return;
			}
			else if(peekC=='=' && (c=='+' || c=='-')){
				inputFile.get(c);
				errorCol++;
				obtainedSymbol += c;
				return;
			}
		}
		else if(c=='*' || c=='/' || c=='&' || c=='^' || c=='|' || c=='%'){
			if(peekC=='='){
				inputFile.get(c);
				obtainedSymbol += c;
				return;
			}
		}
	}
	else//for every other "regular" symbols
	{
		peekC = inputFile.peek();
		if(IsEnder(peekC))//if divider found
			return;
		while(IsEnder(peekC)==false) {//while there are still no divider cahrs
			inputFile.get(c);
			errorCol++;
			obtainedSymbol += c;
			peekC = inputFile.peek();
		}
	}
}

int main(){
	defaultCol = 1;
	errorLine = defaultCol;
	errorCol = defaultCol;
	inputFile.open("input.txt");
	if(!inputFile) {
		printf("Error: File not found");
		return 1;
	}
	
	//GetOneWord
	GetSymbol();
	//TestProcedure();
	
	//Check GetSymbol Parsings
	{
		while(!inputFile.eof())
		{
			printf("|%s|\n",obtainedSymbol.c_str());
			GetSymbol();
		}
		printf("|%s|\n",obtainedSymbol.c_str());
	}
	//CharacterByCharacter
	//inputFile.get(symbol);

	//Check if there are more symbols 
	/*
	{
		if(inputFile.eof())
			printf("\n----------------------ZUCC----------------------\n");
		else {
			printf("Rejected on line %d, column %d\n", errorLine, errorCol);
			printf("Reason:\nThere are still symbol: %c\n", symbol);
		}
	}
	*/
	return 0;
}

void Accept(std::string t){
	if(t.compare(obtainedSymbol)==0){
		GetSymbol();
	}
	else{
		ErrrorHandling();
	}
}
void Identifier(); void DecimalNumeral();
void TestProcedure()
{
	Identifier();
	Accept("=");
	DecimalNumeral();
	Accept("*");
	DecimalNumeral();
	Accept(";");
}
//Rules
//148-152 joined to one because identifier rule is included in GetSymbol
void Identifier(){
	if (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')
	{
		GetSymbol();
	}
	else
		ErrrorHandling();
}
//
void DecimalNumeral(){
	for(int i = 0; i<obtainedSymbol.length(); i++)
	{
		if(isdigit (obtainedSymbol.at(i))==false)
		{
			ErrrorHandling();
		}
	}
	GetSymbol();
}

void VariableSymbol() {
	switch(symbol)
	{
		case '$' : Accept("$"); break;
		default : Accept("_");
	}
}
void Letter() {
	if(isalpha(symbol))
	{
	}
	else
	{
		printf("Rejected on line %d, column %d\n", errorLine, errorCol);
		printf("Reason:");
		if(symbol == '\n')	
			printf("Symbol found: enter\n");
		else
			printf("Symbol found: %c\n", symbol);
		printf("Symbol should be a letter\n");
		exit(0);
	}
}

//TestRules
void EscapeCharacter(){
	while (symbol == '\n')
	{
		errorLine++;
		errorCol = defaultCol;
		inputFile.get(symbol);
		if(inputFile.eof())
			break;
	}
}

