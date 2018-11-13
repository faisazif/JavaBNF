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
void VariableSymbol();void Letter();void Digit(); void Accept(std::string); void TestProcedure(); void InputCharacter();
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
	//if(inputFile.eof())
	//	printf("Rejected at line %d column %d.\nNo more symbol to read\n",errorLine, errorCol, obtainedSymbol.c_str());
	//else
		printf("Rejected at line %d column %d.\nSymbol: %s\n",errorLine, errorCol, obtainedSymbol.c_str());
	exit(0);
}
bool IsEnder(char c){
	if(inputFile.eof()) 
		return true;
	
	else if( c==' ' || c=='\t' || c=='!'|| c=='@'|| c=='#'|| c=='%'|| c=='^'|| c=='&'|| c=='*'|| c=='('|| c==')'|| c=='-'|| c=='+'|| c=='/'
	|| c==39 || c=='"'|| c=='+'|| c=='-' || c=='[' || c==']' || c=='<' || c=='>' || c=='=' || c=='|' || c=='.' || c==';' ){
		return true;
	}
	
	else if(c=='\n'){
		return true;
	}
	else
		return false;
}
void GetSymbol(){
	printf("Accepted Symbol: %s\n",obtainedSymbol.c_str());//check what is accepted before
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
	TestProcedure();
	/*
	//Check GetSymbol Parsings
	{
		while(!inputFile.eof())
		{
			printf("|%s|\n",obtainedSymbol.c_str());
			GetSymbol();
		}
		printf("|%s|\n",obtainedSymbol.c_str());
	}
	*/
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
void Identifier(); void DecimalNumeral();void StringCharacters();void StringLiteral();void CharacterLiteral();
void TestProcedure()
{
	CharacterLiteral();
}
//Rules
//148-152 joined to one because identifier rule is included in GetSymbol()
void Identifier(){
	if (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')
		GetSymbol();
	else
		ErrrorHandling();
}
//146-147 combined
void InputCharacter(){
	if(obtainedSymbol.length()>1 )
		ErrrorHandling();
	else if(isalpha(obtainedSymbol.at(0)))
		GetSymbol();
	else if (obtainedSymbol.at(0)=='!' || obtainedSymbol.at(0) == '@' || obtainedSymbol.at(0) == '#' || obtainedSymbol.at(0) == '%' || obtainedSymbol.at(0) == '^' || obtainedSymbol.at(0) == '&' || obtainedSymbol.at(0) == '*' || obtainedSymbol.at(0) == '(' || obtainedSymbol.at(0) == ')' || obtainedSymbol.at(0) == '-' || obtainedSymbol.at(0) == '+' || obtainedSymbol.at(0) == '/')
		GetSymbol();
	else if (obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')
		GetSymbol();
	else if(isdigit(obtainedSymbol.at(0)))
		GetSymbol();
	else
		ErrrorHandling();
}
//144-145 Combined
void StringCharacters(){
		if(inputFile.eof())
			return;
	while(obtainedSymbol.at(0) != '"')
	{
		GetSymbol();
		if(inputFile.eof())
			break;
	}
}
//143
void StringLiteral(){
	std::string petik = "";
	char x = '"';
	petik += x;
	Accept(petik);
	if(obtainedSymbol.compare(petik)==0)
		return;
	else
		StringCharacters();
	Accept(petik);
}
//141-142 Combined
void CharacterLiteral(){
	Accept("'");
	if(obtainedSymbol.compare("'")==0)
		return;
	else
	{
		if (obtainedSymbol.length()>1) //if there are more than 1 char
		{
			if(obtainedSymbol.at(0)==92)//if 1st symbol is backslash
			{
				if(obtainedSymbol.at(1)!='n' ||obtainedSymbol.at(1)!='t')
					ErrrorHandling();
			}
			else
				ErrrorHandling();
		}
		else
			GetSymbol();
	}
	Accept("'");
}
//130. Rule 131 Digit also modeled here
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

