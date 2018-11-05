#include <stdio.h>
#include <fstream>

using namespace std;

char symbol;
bool done;
ifstream inputFile;

void Accept(char t)
{
	if(symbol == t)
	{
		printf("Accepted symbol: %c\n", symbol);
		inputFile.get(symbol);
	}
	else
	{
		if(symbol == '\n')	
			printf("Rejected. Symbol: enter\n");
		else
			printf("Rejected. Symbol: %c\n", symbol);
		printf("Symbol should be: %c\n", t);
		exit(0);
	}
}

void EscapeCharacter()
{
	if(symbol == '\n')
		Accept('\n');
}

void A()
{
	Accept('f');
	Accept('a');
	Accept('i');
	Accept('s');
	EscapeCharacter();
}

int main()
{
	done = false;
	char test;
	inputFile.open("input.txt");
	if(!inputFile)
	{
		printf("File not found");
		return 1;
	}

	inputFile.get(symbol);
	A();

	if(inputFile.peek()	== EOF)
	{
		printf("----------------------ZUCC----------------------\n\n\n");
		done = true;
	}
	else
	{
		inputFile.get(symbol);
		printf("Rejected. There are still symbol: %c\n", symbol);
	}
	return 0;
}

