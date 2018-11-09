#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

char symbol;
int errorLine;
int errorCol;
int defaultCol;
ifstream inputFile;

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

int main()
{
	defaultCol = 1;
	errorLine = defaultCol;
	errorCol = defaultCol;
	inputFile.open("input.txt");
	if(!inputFile) {
		printf("Error: File not found");
		return 1;
	}

	inputFile.get(symbol);
	A();

	if(inputFile.eof())
		printf("\n----------------------ZUCC----------------------\n");
	else {
		printf("Rejected on line %d, column %d\n", errorLine, errorCol);
		printf("Reason:\nThere are still symbol: %c\n", symbol);
	}
	return 0;
}

