#include <stdio.h> 
#include <stdlib.h> 
#include <fstream>
#include <ctype.h>
using namespace std;
char symbol; 
std::string obtainedSymbol; 
int errorLine; 
int errorCol; 
int evaluatedCol, evaluatedLine;
int defaultCol; 
ifstream inputFile;
char c, peekC;
bool isEnd;
//Prototypes
void VariableSymbol();void Letter();void Digit(); void Accept(std::string); void TestProcedure(); void InputCharacter();
void SkipBlanks(){
	while(c==' ' || c=='\n' || c=='\t'){
		if(c=='\n')
		{
			evaluatedCol=defaultCol;
			evaluatedLine++;
		}
		else
			evaluatedCol++;
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
	evaluatedCol++;
	obtainedSymbol = "";
	SkipBlanks();
	if(inputFile.eof()){
		isEnd = true;
		return;
	}
	//save first symbol that is not a blank
	obtainedSymbol += c;
	if(IsEnder(c))//if special character found
	{
		//ifs for super special character
		peekC = inputFile.peek();
		if(c=='=' || c=='+' || c=='-' || c=='<' || c=='>'){//for ==, ++, --, <<, >>
			if(peekC==c){
				inputFile.get(c);
				evaluatedCol++;
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
					evaluatedCol++;
					obtainedSymbol += c;
					peekC = inputFile.peek();
				}
				return;
			}
			else if(peekC=='=' && (c=='+' || c=='-')){
				inputFile.get(c);
				evaluatedCol++;
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
		if(isalpha(c) || c=='$' || c=='_'){
			while(IsEnder(peekC)==false && (isalpha(peekC) || peekC=='$' || peekC=='_')) {//while there are still no divider cahrs
				inputFile.get(c);
				evaluatedCol++;
				obtainedSymbol += c;
				peekC = inputFile.peek();
			}
		}
		else if(isdigit(c)){
			if(c=='0' && (peekC=='x' || peekC=='X')){//if hex
				inputFile.get(c);
				evaluatedCol++;
				obtainedSymbol += c;
				return;
			}
			while(IsEnder(peekC)==false && (isdigit(peekC))) {//while there are still no divider cahrs
				inputFile.get(c);
				evaluatedCol++;
				obtainedSymbol += c;
				peekC = inputFile.peek();
			}
		}
	}
}

int main(){
	isEnd = false;
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
	if(!isEnd)
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
	//*/
	//CharacterByCharacter
	//inputFile.get(symbol);

	//Check if there are more symbols */
	
	{
		if(isEnd)
			printf("\n----------------------ZUCC----------------------\n");
		else {
			printf("Rejected on line %d, column %d\n", errorLine, errorCol);
			printf("Reason:\nThere are still symbol: %s\n", obtainedSymbol.c_str());
		}
	}
	
	return 0;
}
//Prototype
void Identifier(); void DecimalNumeral();void StringCharacters();void StringLiteral();void CharacterLiteral();void DimExpr(); void Dims();
void Expression();
void Accept(std::string t){
	if(t.compare(obtainedSymbol)==0){
		errorCol+=evaluatedCol;
		errorLine+=evaluatedLine;
		evaluatedCol = evaluatedLine = 0;
		if(inputFile.eof())
			isEnd = true;
		GetSymbol();
	}
	else{
		if(inputFile.eof())
			isEnd = true;
		ErrrorHandling();
	}
}
//Rules
//148-152 joined to one because identifier rule is included in GetSymbol()
void Identifier(){
	if (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Accept(obtainedSymbol);
			if(isEnd)
				return;
		while(isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_' || isdigit(obtainedSymbol.at(0))){
			Accept(obtainedSymbol);
			if(isEnd)
					break;
		}
	}
	else
		ErrrorHandling();
}
//146-147 combined
void InputCharacter(){
	if(obtainedSymbol.length()>1 )
		ErrrorHandling();
	else if(isalpha(obtainedSymbol.at(0)))
		Accept(obtainedSymbol);
	else if (obtainedSymbol.at(0)=='!' || obtainedSymbol.at(0) == '@' || obtainedSymbol.at(0) == '#' || obtainedSymbol.at(0) == '%' || obtainedSymbol.at(0) == '^' || obtainedSymbol.at(0) == '&' || obtainedSymbol.at(0) == '*' || obtainedSymbol.at(0) == '(' || obtainedSymbol.at(0) == ')' || obtainedSymbol.at(0) == '-' || obtainedSymbol.at(0) == '+' || obtainedSymbol.at(0) == '/')
		Accept(obtainedSymbol);
	else if (obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')
		Accept(obtainedSymbol);
	else if(isdigit(obtainedSymbol.at(0)))
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//144-145 Combined
void StringCharacters(){
		if(inputFile.eof())
			return;
	while(obtainedSymbol.at(0) != '"')
	{
		Accept(obtainedSymbol);
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
	if(obtainedSymbol.compare("'")==0)//the input is ''
		return;
	else
	{
		if (obtainedSymbol.length()>1) //if there are more than 1 char
		{
			if(obtainedSymbol.at(0)==92 && obtainedSymbol.length()<3)//if 1st symbol is backslash
			{
				if((obtainedSymbol.at(1)=='n' ||obtainedSymbol.at(1)=='t'||obtainedSymbol.at(1)=='b'||obtainedSymbol.at(1)=='r'))
					GetSymbol();
				else
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
//140
void BooleanLiteral(){
	if(obtainedSymbol == "true" || obtainedSymbol == "false")
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//139
void FloatTypeSuffix(){
	if(obtainedSymbol=="f" ||obtainedSymbol=="F" || obtainedSymbol=="d" || obtainedSymbol=="D")
		Accept(obtainedSymbol);
}
//137-138
void SignedInteger(){
	if(obtainedSymbol=="+" || obtainedSymbol=="-")
		Accept(obtainedSymbol);
	DecimalNumeral();
}
//135-136
void ExponentPart(){
	if(obtainedSymbol=="e" || obtainedSymbol=="E"){
		Accept(obtainedSymbol);
		SignedInteger();
	}
}
//134
void FloatingPoint(){
	if(isdigit(obtainedSymbol.at(0)))
		DecimalNumeral();
	ExponentPart();
	FloatTypeSuffix();
}
//133
void HexDigit(){
	if(isxdigit(obtainedSymbol.at(0))){//if the first char of the first symbol is hex
		while(!isEnd && isxdigit(obtainedSymbol.at(0))){//while next symbols is still hex
			for(int i=0; i<obtainedSymbol.length(); i++){
				if(!isxdigit(obtainedSymbol.at(i)))
					ErrrorHandling();
			}
			Accept(obtainedSymbol);
		}
	}
	else
		ErrrorHandling();
}
//132
void HexNumeral(){
	if(obtainedSymbol == "0x" || obtainedSymbol=="0X"){
		Accept(obtainedSymbol);
		HexDigit();
	}
	else
		ErrrorHandling();
}
//130-131
void DecimalNumeral(){
	if(isdigit (obtainedSymbol.at(0)))//because parsing is always digits or not, check the first char is adequate
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//129
void IntegerTypeSuffix(){
	if(obtainedSymbol=="l" || obtainedSymbol=="L")
		Accept(obtainedSymbol);
}
//128
void IntegerOrFloat(){
	if(obtainedSymbol=="."){
		Accept(".");
		FloatingPoint();
	}
	else if(isdigit(obtainedSymbol.at(0))){
		DecimalNumeral();
		IntegerTypeSuffix();
	}
}
//127
void NumberLiteral(){
	if(obtainedSymbol=="0x" || obtainedSymbol=="0X"){//if hex detected
		printf("Testis: %s\n", obtainedSymbol.c_str());
		HexNumeral();
	}
	else if(obtainedSymbol=="."){ //if floating point without pre digit
			FloatingPoint();
		
	}
	else if(isdigit(obtainedSymbol.at(0))){ //if digits
		Accept(obtainedSymbol);
			if(obtainedSymbol==".") //if floating point with pre digit
				FloatingPoint();
			else //if its only decimal numeral
				IntegerTypeSuffix();
	}
}
//126
void PrimitiveLiteral(){
	if(obtainedSymbol=="true" || obtainedSymbol=="false")
		BooleanLiteral();
	else if(obtainedSymbol=="null")
		Accept("null");
	else if(obtainedSymbol=="." || isdigit(obtainedSymbol.at(0)))
		NumberLiteral();
	else
		CharacterLiteral();
}
//125
void Name(){
	Identifier();
	while(!isEnd && obtainedSymbol=="."){
		Accept(".");
		if(isEnd)
			ErrrorHandling();
		Identifier();
	}
}
//124
void ArrayAccess(){
	Name();
	DimExpr();
	while(!isEnd && obtainedSymbol=="["){
		DimExpr();
	}	
}
//123
void Dims(){
	while(!isEnd && obtainedSymbol=="["){
		Accept("[");
		Accept("]");
	}
}
//122 --------------------------------------------------------------------
void DimExpr(){
	Accept("[");
	//Expression();
	Accept("]");
}
//121 --------------------------------------------------------------------------
void ArrayCreationExpression(){
	Name();
	while(!isEnd && obtainedSymbol=="["){
		Accept("[");
		if(obtainedSymbol!="]")
			//Expression();
		Accept("]");
	}
}
//120 -----------------------------------------------------------------
void ArgumentList(){
	
}
//119----------------------------------------------------------------
void ArrayAndClassCreationExpression(){
	if(true){
		Name();
		Accept("(");
		ArgumentList();
		Accept(")");
	}
	else
		ArrayCreationExpression();
}
//118----------------------------------------------------------------------
void MethodAccess(){
	
}
//117------------------------------------------------------------------
void MethodOrArrayAccess(){
	
}
//116 -----------------------------------------------------------------------
void Primary(){
	
}
//115
void PrePostSymbol(){
	if(obtainedSymbol=="++" || obtainedSymbol=="--")
		Accept(obtainedSymbol);
}
//114
void Operand(){
	Name();
	while(!isEnd && obtainedSymbol=="["){
		DimExpr();
	}
}
//113
void PostCrementExpression(){
	Operand();
	PrePostSymbol();
}
//112 -------------------------------------------------------------
void UnaryExpression2(){
	
}
//111 --------------------------------------------------------- unaryExpression
void PreCrementExpression(){
	
}
//86
void AssignmentOperator(){
	if(obtainedSymbol=="="||obtainedSymbol=="*="||obtainedSymbol=="/="||obtainedSymbol=="%="||obtainedSymbol=="+="||obtainedSymbol=="-="||obtainedSymbol=="<<="||obtainedSymbol==">>="||obtainedSymbol==">>>="||obtainedSymbol=="&="||obtainedSymbol=="^="||obtainedSymbol=="|=")
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
void TestProcedure()
{
	Operand();
}

