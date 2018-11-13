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
void VariableSymbol();void Letter();void Digit();

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
	}
}

void ErrrorHandling(){
		printf("Rejected at line %d column %d.\nSymbol: %s\n",errorLine, errorCol, obtainedSymbol.c_str());
		exit(0);
}

bool IsEnder(char c){
	if(inputFile.eof()) 
		return true;
	/*
	else if( c==' ' || c=='\t' || c=='!'|| c=='@'|| c=='#'|| c=='%'|| c=='^'|| c=='&'|| c=='*'|| c=='('|| c==')'|| c=='-'|| c=='+'|| c=='/'
	|| c=='"'|| c=='+'|| c=='-' || c=='[' || c==']' || c=='<' || c=='>' || c=='=' || c=='|' ){
		errorCol++;
		return true;
	}
	*/
	else if(c=='\n'){
		errorCol=defaultCol;
		errorLine++;
		return true;
	}
	else
		return false;
}

void GetSymbol(){
	inputFile.get(c);
	obtainedSymbol = "";
	SkipBlanks();
	//identifier
	if(isalpha(c) || c=='$' || c=='_'){
		obtainedSymbol += c;
		peekC = inputFile.peek();
		if(IsEnder(peekC))//if divider found
			return;
		while(isalpha(peekC)>0 || peekC=='$' || peekC=='_' || isdigit(peekC)) {//while there are still acceptable cahrs
			inputFile.get(c);
			obtainedSymbol += c;
			peekC = inputFile.peek();
			if(IsEnder(peekC))//if divider found
				return;
		}
		return;
	}
	//digit
	else if(isdigit(c)){
		obtainedSymbol += c;
		peekC = inputFile.peek();
		if(c==0 && (peekC=='x' || peekC=='X'))//if hex
		{
			inputFile.get(c);
			obtainedSymbol += c;
			peekC = inputFile.peek();
			while(isdigit(peekC)) {//while there are still acceptable cahrs
				inputFile.get(c);
				obtainedSymbol += c;
				peekC = inputFile.peek();
				if(IsEnder(peekC))//if divider found
					return;
			}
			return;
		}
		if(IsEnder(peekC))//if divider found
			return;
		while(isdigit(peekC)) {//while there are still acceptable cahrs
			inputFile.get(c);
			obtainedSymbol += c;
			peekC = inputFile.peek();
			if(IsEnder(peekC))//if divider found
				return;
		}
		return;
	}
	//inputsymbol
	else if(c=='!'|| c=='@'|| c=='#'|| c=='%'|| c=='^'|| c=='&'|| c=='*'|| c=='('|| c==')'|| c=='-'|| c=='+'|| c=='/'){
		obtainedSymbol += c;
		peekC = inputFile.peek();
		if(c==0 && (peekC=='x' || peekC=='X'))//if hex
		{
			inputFile.get(c);
			obtainedSymbol += c;
			peekC = inputFile.peek();
			while(isdigit(peekC)) {//while there are still acceptable cahrs
				inputFile.get(c);
				obtainedSymbol += c;
				peekC = inputFile.peek();
				if(IsEnder(peekC))//if divider found
					return;
			}
			return;
		}
		if(IsEnder(peekC))//if divider found
			return;
		while(isdigit(peekC)) {//while there are still acceptable cahrs
			inputFile.get(c);
			obtainedSymbol += c;
			peekC = inputFile.peek();
			if(IsEnder(peekC))//if divider found
				return;
		}
		return;
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
	printf("|%s|\n",obtainedSymbol.c_str());
	//printf("%s: ",GetSymbol());
	//printf("%s\n",obtainedSymbol.c_str());
	while(!inputFile.eof())
	{
		//printf("\n%s: ",GetSymbol());
		GetSymbol();
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

void Accept(char t){
	if(inputFile.eof())
	{
		printf("Rejected on line %d, column %d\n", errorLine, errorCol);
		printf("Reason: \nNo more character to read");
		exit(0);
	}
	else if(symbol == t)
	{
		errorCol++;
		inputFile.get(symbol);
	}
	else
	{
		printf("Rejected on line %d, column %d\n", errorLine, errorCol);
		printf("Reason:");
		if(symbol == '\n')	
			printf("Symbol found: enter\n");
		else
			printf("Symbol found: %c\n", symbol);
		printf("Symbol should be: %c\n", t);
		exit(0);
	}
}


//Rules
void NextLetter(){
	if (isalpha(symbol)){
		Letter(); NextLetter();
	} 
	else if(isdigit(symbol)){
		Digit(); NextLetter();
	}
	else if(symbol == '$' || symbol == '_'){
		VariableSymbol(); NextLetter();
	}
}
void VariableSymbol() {
	switch(symbol)
	{
		case '$' : Accept('$'); break;
		default : Accept('_');
	}
}
void Letter() {
	if(isalpha(symbol))
		Accept(symbol);
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
void Digit() {
	if(isdigit(symbol))
		Accept(symbol);
	else
	{
		printf("Rejected on line %d, column %d\n", errorLine, errorCol);
		printf("Reason:");
		if(symbol == '\n')	
			printf("Symbol found: enter\n");
		else
			printf("Symbol found: %c\n", symbol);
		printf("Symbol should be a digit\n");
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
void A(){
	while(symbol == 'f')
	{
		Accept('f');
		Accept('a');
		Accept('i');
		Accept('s');
		EscapeCharacter();
	}
}

