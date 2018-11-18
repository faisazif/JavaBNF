#include <stdio.h> 
#include <stdlib.h> 
#include <fstream>
#include <ctype.h>
using namespace std;
char symbol; 
std::string obtainedSymbol; 
std::string shouldBeSymbol; 
int errorLine; 
int errorCol; 
int evaluatedCol, evaluatedLine;
int defaultCol; 
int testIndex;
ifstream inputFile;
char c, peekC;
bool isEnd, haveSpace, isReserved;
//Prototypes
void Identifier(); void DecimalNumeral();void StringCharacters();void StringLiteral();void CharacterLiteral();void DimExpr(); void Dims();
void Expression();void DataType(); void Type(); void UnaryExpression(); void MultiplicativeExpression(); void AdditiveExpression();
void ShiftExpression(); void RelationalExpression(); void EqualityExpression(); void AndExpression(); void OrExpression();
void ConditionalAndExpression(); void ConditionalOrExpression(); void ConditionalExpression(); void ArgumentList(); void Block();
void Accept(std::string); void TestProcedure(); void UnaryExpression2(); void CastExpression(); void Parameter(); void StatementExpression();
void Statement(); void BlockStatement(); void BlockStatements(); void VariableDeclarations(); void ParameterList(); void MethodDeclarator();
void Throws(); void VariableDeclaration(); void FieldDeclaration(); void ReturnType();
void SkipBlanks(){
	while(c==' ' || c=='\n' || c=='\t'){
		haveSpace = true;
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
	if(isEnd)
		printf("Rejected at line %d column %d.\nNo more symbol to read\n",errorLine, errorCol, obtainedSymbol.c_str());
	else if(isReserved)
		printf("Rejected at line %d column %d.\nSymbol: %s is reserved\n",errorLine, errorCol, obtainedSymbol.c_str());
	else{
		printf("Rejected at line %d column %d.\nSymbol: %s\nSymbol should be: %s",errorLine, errorCol, obtainedSymbol.c_str(), shouldBeSymbol.c_str());
	}
	exit(0);
}
bool IsReserveWord(){
	if(obtainedSymbol=="abstract"||obtainedSymbol=="assert"||obtainedSymbol=="boolean"||obtainedSymbol=="break"||obtainedSymbol=="byte"||obtainedSymbol=="case"||obtainedSymbol=="catch"||obtainedSymbol=="char"||obtainedSymbol=="class"||obtainedSymbol=="const"||obtainedSymbol=="default"||obtainedSymbol=="do"||obtainedSymbol=="double"||obtainedSymbol=="else"||obtainedSymbol=="enum"||obtainedSymbol=="extends"||obtainedSymbol=="false"||obtainedSymbol=="final"||obtainedSymbol=="finally"||obtainedSymbol=="float"||obtainedSymbol=="for"||obtainedSymbol=="goto"||obtainedSymbol=="if"||obtainedSymbol=="implements"||obtainedSymbol=="import"||obtainedSymbol=="instanceof"||obtainedSymbol=="int"||obtainedSymbol=="interface"||obtainedSymbol=="long"||obtainedSymbol=="native"||obtainedSymbol=="new"||obtainedSymbol=="null"||obtainedSymbol=="package"||obtainedSymbol=="private"||obtainedSymbol=="protected"||obtainedSymbol=="public"||obtainedSymbol=="return"||obtainedSymbol=="short"||obtainedSymbol=="static"||obtainedSymbol=="strictfp"||obtainedSymbol=="super"||obtainedSymbol=="switch"||obtainedSymbol=="synchronized"||obtainedSymbol=="this"||obtainedSymbol=="throw"||obtainedSymbol=="throws"||obtainedSymbol=="transient"||obtainedSymbol=="true"||obtainedSymbol=="try"||obtainedSymbol=="void"||obtainedSymbol=="volatile"||obtainedSymbol=="while"||obtainedSymbol=="continue"){
		isReserved = true;
		return true;
	}
	else
		return false;
}
bool IsEnder(char c){
	if(inputFile.eof()) 
		return true;
	
	else if( c==' ' || c=='\t' || c=='!'|| c=='@'|| c=='#'|| c=='%'|| c=='^'|| c=='&'|| c=='*'|| c=='('|| c==')'|| c=='-'|| c=='+'|| c=='/'
	|| c==39 || c=='"'|| c=='+'|| c=='-' || c=='[' || c==']' || c=='<' || c=='>' || c=='=' || c=='|' || c=='.' || c==',' || c==';' ){
		return true;
	}
	
	else if(c=='\n'){
		return true;
	}
	else
		return false;
}
void GetSymbol(){
	evaluatedCol = evaluatedLine = 0;
	//printf("Accepted Symbol: %s\n",obtainedSymbol.c_str());//check what is accepted before
	inputFile.get(c);
	evaluatedCol++;
	obtainedSymbol = "";
	haveSpace = false;
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
		if(c=='=' || c=='+' || c=='-' || c=='<' || c=='>' || c=='|' || c=='&'){//for ==, ++, --, <<, >>
			if(peekC==c){
				inputFile.get(c);
				evaluatedCol++;
				obtainedSymbol += c;
				peekC = inputFile.peek();
				if(c=='>' && peekC=='>')//for >>
				{
					inputFile.get(c);
					evaluatedCol++;
					obtainedSymbol += c;
					peekC = inputFile.peek();
					if(peekC == '=')//for <<=, >>>=
					{
						inputFile.get(c);
						evaluatedCol++;
						obtainedSymbol += c;
						peekC = inputFile.peek();
					}
				}
				else if(c=='<' && peekC=='<')//for <<
				{
					inputFile.get(c);
					evaluatedCol++;
					obtainedSymbol += c;
					peekC = inputFile.peek();
					if(peekC == '=')//for <<=, >>=
					{
						inputFile.get(c);
						evaluatedCol++;
						obtainedSymbol += c;
						peekC = inputFile.peek();
					}
					else if(c=='>' && peekC=='>') //for >>>
					{
						inputFile.get(c);
						evaluatedCol++;
						obtainedSymbol += c;
						peekC = inputFile.peek();
						if(peekC == '=')//for >>>=
						{
							inputFile.get(c);
							evaluatedCol++;
							obtainedSymbol += c;
							peekC = inputFile.peek();
						}
					}
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
		else if(c=='*' || c=='/' || c=='&' || c=='^' || c=='|' || c=='%'){//for *=, /=, &=, etc,
			if(peekC=='='){
				inputFile.get(c);
				evaluatedCol++;
				obtainedSymbol += c;
				return;
			}
		}
		else if(c=='.'){//for .*
			if(peekC=='*'){
				inputFile.get(c);
				evaluatedCol++;
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
			if(obtainedSymbol=="else" && peekC==' '){
				inputFile.get(c);
				evaluatedCol++;
				peekC = inputFile.peek();
				if(peekC=='i'){
					inputFile.get(c);
					evaluatedCol++;
					peekC = inputFile.peek();
					if(peekC=='f'){
						inputFile.get(c);
						evaluatedCol++;
						obtainedSymbol+=" if";
						peekC = inputFile.peek();
						while(IsEnder(peekC)==false && (isalpha(peekC) || peekC=='$' || peekC=='_')) {//while there are still no divider cahrs
							inputFile.get(c);
							evaluatedCol++;
							obtainedSymbol += c;
							peekC = inputFile.peek();
						}
					}
					else{
						inputFile.putback('i');
						evaluatedCol--;
					}
				}
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
	haveSpace = false;
	isReserved = false;
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
	//if(!isEnd)
	//	TestProcedure();
	///*
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
		else
			shouldBeSymbol=t;
		ErrrorHandling();
	}
}
//Rules
//148-152 joined to one because identifier rule is included in GetSymbol()
void Identifier(){
	if(isEnd)
		ErrrorHandling();
	if(IsReserveWord())
		ErrrorHandling();
	if (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Accept(obtainedSymbol);
			if(isEnd)
				return;
		while(!haveSpace && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_' || isdigit(obtainedSymbol.at(0)))){
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
	if(isEnd)
		return;
	while(obtainedSymbol.at(0) != '"')
	{
		Accept(obtainedSymbol);
		if(isEnd)
			return;
	}
}
//143
void StringLiteral(){
	if(isEnd)
		ErrrorHandling();
	std::string petik = "";
	char x = '"';
	petik += x;
	Accept(petik);
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
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol == "true" || obtainedSymbol == "false")
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//139
void FloatTypeSuffix(){
	if(isEnd)
		return;
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
		HexNumeral();
	}
	else if(obtainedSymbol=="."){ //if floating point without pre digit
		Accept(obtainedSymbol);
		FloatingPoint();
	}
	else if(isdigit(obtainedSymbol.at(0))){ //if digits
		Accept(obtainedSymbol);
			if(obtainedSymbol=="."){ //if floating point with pre digit
				Accept(obtainedSymbol);
				FloatingPoint();
			}
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
//122
void DimExpr(){
	Accept("[");
	Expression();
	Accept("]");
}
//121 
void ArrayCreationExpression(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="("){
		Accept("(");
		ArgumentList();
		Accept(")");
	}
	else if(obtainedSymbol=="["){
		DimExpr();
		while(!isEnd && obtainedSymbol=="["){
			DimExpr();
		}
		Dims();
	}
	else
		ErrrorHandling();
}
//120
void ArgumentList(){
	if(isEnd)
		return;
	std::string petik = "";
	char x = '"';
	petik += x;
	if(obtainedSymbol=="this" || obtainedSymbol=="(" || obtainedSymbol=="new" || obtainedSymbol=="super" || obtainedSymbol==petik ||
	obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0)) ||
	isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_' || obtainedSymbol=="+" || obtainedSymbol=="-"
	|| obtainedSymbol=="~" || obtainedSymbol=="!" || obtainedSymbol=="(" ||isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || 
	obtainedSymbol.at(0) == '_'){
		Expression();
		while(!isEnd && obtainedSymbol==","){
			Accept(",");
			Expression();
		}
	}
}
//119
void ArrayAndClassCreationExpression(){
	Name();
	ArrayCreationExpression();
}
//118
void MethodAccess(){
	if(!isEnd){
		while(!isEnd && obtainedSymbol=="."){
			Accept(".");
			Identifier();
		}
		Accept("(");
		ArgumentList();
		Accept(")");
	}
	else
		return;
}
//117
void MethodOrArrayAccess(){
	if(isEnd)
		return;
	if(obtainedSymbol=="["){
		DimExpr();
		while(!isEnd && obtainedSymbol=="["){
			DimExpr();
		}
		MethodAccess();
	}
	else if(obtainedSymbol=="("){
		Accept("(");
		ArgumentList();
		Accept(")");
	}
}
//116 
void Primary(){
	if(isEnd)
		ErrrorHandling();
	std::string petik = "";
	char x = '"';
	petik += x;
	if(obtainedSymbol=="this"){
		Accept(obtainedSymbol);
		MethodAccess();
	}
	else if(obtainedSymbol=="("){
		Accept("(");
		Expression();
		Accept(")");
	}
	else if(obtainedSymbol=="new"){
		Accept(obtainedSymbol);
		ArrayAndClassCreationExpression();
		MethodAccess();
	}
	else if(obtainedSymbol=="super"){
		Accept(obtainedSymbol);
		MethodAccess();
	}
	else if(obtainedSymbol==petik){
		StringLiteral();
	}
	else if(obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0))){
		PrimitiveLiteral();
	}
	else
		ErrrorHandling();
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
//112 
void UnaryExpression2(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="~"){
		Accept(obtainedSymbol);
		PostCrementExpression();
	}
	else if(obtainedSymbol=="!"){
		Accept(obtainedSymbol);
		BooleanLiteral();
	}
	else if(obtainedSymbol=="("){
		CastExpression();
	}
	else{
		PostCrementExpression();
	}
}
//111 
void PreCrementExpression(){
	if(obtainedSymbol=="--" || obtainedSymbol=="++"){
		Accept(obtainedSymbol);
		UnaryExpression();
	}
}
//110 
void UnaryExpression(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="+"||obtainedSymbol=="-"){
		Accept(obtainedSymbol);
		Operand();
	}
	else
		UnaryExpression2();
}
//109 
void CastExpression(){
	Accept("(");
	DataType();
	Accept(")");
	UnaryExpression();
}
//109
void Multi(){
	if(isEnd)
		return;
	if(obtainedSymbol=="*" || obtainedSymbol=="/" || obtainedSymbol=="%"){
		Accept(obtainedSymbol);
		MultiplicativeExpression();
	}
}
//108
void MultiplicativeExpression(){
	UnaryExpression();
	Multi();
}
//107
void Add(){
	if(isEnd)
		return;
	if(obtainedSymbol=="+" || obtainedSymbol=="-"){
		Accept(obtainedSymbol);
		AdditiveExpression();
	}
}
//106
void AdditiveExpression(){
	MultiplicativeExpression();
	Add();
}
//105
void Shift(){
	if(isEnd)
		return;
	if(obtainedSymbol=="<<" || obtainedSymbol==">>" || obtainedSymbol==">>>"){
		Accept(obtainedSymbol);
		ShiftExpression();
	}
}
//104
void ShiftExpression(){
	AdditiveExpression();
	Shift();
}
//103
void InstanceOf(){
	if(isEnd)
		return;
	if(obtainedSymbol=="instanceof"){
		Accept(obtainedSymbol);
		Type();
	}
}
//102
void InstanceOfExpression(){
	RelationalExpression();
	InstanceOf();
}
//101
void Relational(){
	if(isEnd)
		return;
	if(obtainedSymbol=="<" || obtainedSymbol==">" || obtainedSymbol=="<=" || obtainedSymbol==">="){
		Accept(obtainedSymbol);
		RelationalExpression();
	}
}
//100
void RelationalExpression(){
	ShiftExpression();
	Relational();
}
//99
void Equal(){
	if(obtainedSymbol=="==" || obtainedSymbol=="!="){
		Accept(obtainedSymbol);
		EqualityExpression();
	}
}
//98
void EqualityExpression(){
	InstanceOfExpression();
	Equal();
}
//97
void And(){
	if(obtainedSymbol=="&"){
		Accept(obtainedSymbol);
		AndExpression();
	}
}
//96
void AndExpression(){
	EqualityExpression();
	And();
}
//95
void Or(){
	if(obtainedSymbol=="^"){
		Accept(obtainedSymbol);
		OrExpression();
	}
}
//94
void OrExpression(){
	AndExpression();
	Or();
}
//93
void ConditionalAnd(){
	if(isEnd)
	return;
	if(obtainedSymbol=="&&"){
		Accept(obtainedSymbol);
		ConditionalAndExpression();
	}
}
//92
void ConditionalAndExpression(){
	OrExpression();
	ConditionalAnd();
}
//91
void ConditionalOr(){
	if(obtainedSymbol=="||"){
		Accept(obtainedSymbol);
		ConditionalOrExpression();
	}
}
//90
void ConditionalOrExpression(){
	ConditionalAndExpression();
	ConditionalOr();
}
//89
void Conditional(){
	if(isEnd)
		return;
	if(obtainedSymbol=="?"){
		Accept("?");
		Expression();
		Accept(":");
		ConditionalExpression();
	}
}
//88
void ConditionalExpression(){
	ConditionalOrExpression();
	Conditional();
}
//87
void AssignmentOperator(){
	if(obtainedSymbol=="="||obtainedSymbol=="*="||obtainedSymbol=="/="||obtainedSymbol=="%="||obtainedSymbol=="+="||obtainedSymbol=="-="||obtainedSymbol=="<<="||obtainedSymbol==">>="||obtainedSymbol==">>>="||obtainedSymbol=="&="||obtainedSymbol=="^="||obtainedSymbol=="|=")
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//86
void Assignment(){
	std::string petik = "";
	char x = '"';
	petik += x;
	if(obtainedSymbol=="this"||obtainedSymbol=="("||obtainedSymbol=="new"||obtainedSymbol=="super"||obtainedSymbol==petik||
	obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0))){
		Primary();
		AssignmentOperator();
		Expression();
	}
}
//85
void Expression(){
	if(isEnd)
		ErrrorHandling();
	std::string petik = "";
	char x = '"';
	petik += x;
	if(obtainedSymbol=="this" || obtainedSymbol=="(" || obtainedSymbol=="new" || obtainedSymbol=="super" || obtainedSymbol==petik ||
	obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0)) ||
	isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Assignment();
	}
	else{
		ConditionalExpression();
	}
}
//84
void Finally(){
	if(isEnd)
		return;
	if(obtainedSymbol=="finally"){
		Accept(obtainedSymbol);
		Block();
	}
}
//83
void CatchClause(){
	Accept("catch");
	Accept("(");
	Parameter();
	Accept(")");
	Block();
}
//82
void Catches(){
	while(!isEnd && obtainedSymbol=="catch"){
		CatchClause();
	}
}
//81
void TryStatement(){
	Accept("try");
	Block();
	Catches();
	Finally();
}
//80
void SynchronizedStatement(){
	Accept("synchronized");
	Accept("(");
	Expression();
	Accept(")");
	Block();
}
//79
void ThrowsStatement(){
	Accept("throw");
	Expression();
	Accept(";");
}
//78
void Expr(){
	if(isEnd)
		return;
	std::string petik = "";
	char x = '"';
	petik += x;
	if(obtainedSymbol=="this" || obtainedSymbol=="(" || obtainedSymbol=="new" || obtainedSymbol=="super" || obtainedSymbol==petik ||
	obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0)) ||
	isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_' || obtainedSymbol=="+" || obtainedSymbol=="-"
	|| obtainedSymbol=="~" || obtainedSymbol=="!" || obtainedSymbol=="(" ||isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || 
	obtainedSymbol.at(0) == '_'){
		Expression();
	}
}
//77
void ReturnStatement(){
	Accept("return");
	Expr();
	Accept(";");
}
//76
void Cont(){
	if(isEnd)
		return;
	if (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Identifier();
	}
}
//75
void ContinueStatement(){
	Accept("continue");
	Cont();
	Accept(";");
}
//74
void BreakStatement(){
	Accept("break");
	Cont();
	Accept(";");
}
//73
void StatementExpressionList(){
	StatementExpression();
	while(!isEnd && obtainedSymbol==","){
		Accept(",");
		StatementExpression();
	}
}
//72
void ForUpdate(){
	
}
//71
void ForInit(){
	
}
//70
void ForStatement(){
	Accept("for");
	Accept("(");
	ForInit();
	Accept(";");
	Expr();
	Accept(";");
	ForUpdate();
	Accept(")");
	Statement();
}
//69
void DoStatement(){
	Accept("do");
	Statement();
	Accept("while");
	Accept("(");
	Expression();
	Accept(")");
	Accept(";");
}
//68
void WhileStatement(){
	Accept("while");
	Accept("(");
	Expression();
	Accept(")");
	Statement();
}
//67
void SwitchLabel(){
	Accept("case");
	Expression();
	Accept(":");
}
//66
void SwitchLabels(){
	if(isEnd)
		return;
	if(obtainedSymbol=="default"){
		Accept(obtainedSymbol);
		Accept(":");
	}
}
//65
void SwitchBlockStatementGroups(){
	while(!isEnd && (obtainedSymbol=="case" || obtainedSymbol=="default")){
		SwitchLabel();
		BlockStatements();
	}
}
//64
void SwitchBlock(){
	while(!isEnd && obtainedSymbol=="case"){
		SwitchBlockStatementGroups();
		SwitchLabels();
	}
}
//63
void SwitchStatement(){
	Accept("switch");
	Accept("(");
	Expression();
	Accept(")");
	SwitchBlock();
}
//62
void FinalElseStatement(){
	if(isEnd)
		return;
	if(obtainedSymbol=="else"){
		Accept(obtainedSymbol);
		Statement();
	}
}
//61
void ElseIfStatements(){
	while(!isEnd && obtainedSymbol=="else if"){
		Accept(obtainedSymbol);
		Accept("(");
		Expression();
		Accept(")");
		StatementExpressionList();
	}
}
//60
void IfStatement(){
	Accept("if");
	Accept("(");
	Expression();
	Accept(")");
	Statement();
	ElseIfStatements();
	FinalElseStatement();
}
//59
void Statement(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol==";")
		Accept(";");
	else if(obtainedSymbol=="switch")
		SwitchStatement();
	else if(obtainedSymbol=="do")
		DoStatement();
	else if(obtainedSymbol=="break")
		BreakStatement();
	else if(obtainedSymbol=="continue")
		ContinueStatement();
	else if(obtainedSymbol=="return")
		ReturnStatement();
	else if(obtainedSymbol=="synchronized")
		SynchronizedStatement();
	else if(obtainedSymbol=="throw")
		ThrowsStatement();
	else if(obtainedSymbol=="try")
		TryStatement();
	else if(obtainedSymbol=="if")
		IfStatement();
	else if(obtainedSymbol=="while")
		WhileStatement();
	else if(obtainedSymbol=="for")
		ForStatement();
	else
		Block();
}
//61
void PostCrementMethodArray2(){
	if(isEnd)
		return;
	if(obtainedSymbol=="["){
		DimExpr();
		MethodAccess();
	}
	else if(obtainedSymbol=="++" || obtainedSymbol=="--"){
		PrePostSymbol();
	}
}
//60
void PostCrementMethodArray(){
	if(isEnd)
		return;
	if(obtainedSymbol=="["|| obtainedSymbol=="++" || obtainedSymbol=="--"){
		while(!isEnd && (obtainedSymbol=="[")){
			DimExpr();
		}
		PostCrementMethodArray2();
	}
	else if(obtainedSymbol=="("){
		Accept("(");
		ArgumentList();
		Accept(")");
	}
}
//58
void StatementExpression(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="--" || obtainedSymbol=="++"){
		PreCrementExpression();
	}
	else if(!IsReserveWord() && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')){
		Name();
		PostCrementMethodArray();
	}
	else{
		Assignment();
	}
}
//57
void LocalVariableDeclaration(){
	Type();
	VariableDeclarations();
	Accept(";");
}
//56
void BlockStatement(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean" || isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		DataType();
	}
	else
		Statement();
}
//55
void BlockStatements(){
	
}
//54
void Block(){
	
}
//53
void DataType(){
	if(obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean")
		Accept(obtainedSymbol);
	else
		ErrrorHandling();
}
//52
void Type(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean"){
		DataType();
		Dims();
	}
	else if (!IsReserveWord() && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')){
		Name();
		Dims();
	}
	else
		ErrrorHandling();
}
//50
void ArrayInitializer(){
	std::string petik = "";
	char x = '"';
	petik += x;
	while(!isEnd && (obtainedSymbol=="this" || obtainedSymbol=="(" || obtainedSymbol=="new" || obtainedSymbol=="super" || obtainedSymbol==petik ||
	obtainedSymbol=="true" || obtainedSymbol=="false" || obtainedSymbol=="null" || obtainedSymbol=="." || isdigit(obtainedSymbol.at(0)) ||
	isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_' || obtainedSymbol=="+" || obtainedSymbol=="-"
	|| obtainedSymbol=="~" || obtainedSymbol=="!" || obtainedSymbol=="(" ||isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || 
	obtainedSymbol.at(0) == '_')){
		ArgumentList();
	}
}
//49
void ConstantDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="final"){
		Accept(obtainedSymbol);
		Type();
		VariableDeclaration();
		Accept(";");
	}
	else{
		Type();
		VariableDeclaration();
		Accept(";");
	}
}
//48
void ConstantAndAbstractMethod4(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="("){
		Accept("(");
		ParameterList();
		Accept(")");
	}
}
//47
void ConstantAndAbstractMethod3(){
	Identifier();
	ConstantAndAbstractMethod4();
}
//46
void ConstantAndAbstractMethod2(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="void"){
		Accept(obtainedSymbol);
		MethodDeclarator();
		Throws();
		Accept(";");
	}
	else{
		Type();
		ConstantAndAbstractMethod3();
	}
}
//46
void AbstractMethodDeclaration(){
	ReturnType();
	MethodDeclarator();
	Throws();
	Accept(";");
}
//45
void ConstantAndAbstractMethod(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="static"){
		Accept(obtainedSymbol);
		ConstantDeclaration();
	}
	else if(obtainedSymbol=="abstract"){
		Accept(obtainedSymbol);
		AbstractMethodDeclaration();
	}
	else{
		ConstantAndAbstractMethod2();
	}
}
//44
void InterfaceMemberDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="public")
		Accept(obtainedSymbol);
	ConstantAndAbstractMethod();
}
//43
void InterfaceBody(){
	while(!isEnd && (obtainedSymbol=="public"||obtainedSymbol=="static"||obtainedSymbol=="abstract"||obtainedSymbol=="public"||obtainedSymbol=="void"||
	obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean"||
	(!IsReserveWord() && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')))){
		InterfaceMemberDeclaration();
	}
}
//42
void ExtendsInterfaces(){
	if(isEnd)
		return;
	if(isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Name();
		while(!isEnd && obtainedSymbol==","){
			Accept(obtainedSymbol);
			Name();
		}
	}
}
//41
void InterfaceDeclaration(){
	Accept("interface");
	Identifier();
	Accept("extends");
	ExtendsInterfaces();
	InterfaceBody();
}
//40
void ReturnType(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="void")
		Accept(obtainedSymbol);
	else
		Type();
}
//39
void ParameterList(){
	if(isEnd)
		return;
	if(obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean"){
		Type();
		Identifier();
		while(!isEnd && obtainedSymbol==","){
			Accept(obtainedSymbol);
			Type();
			Identifier();
		}
	}
	else if(!IsReserveWord() && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')){
		Type();
		Identifier();
		while(!isEnd && obtainedSymbol==","){
			Accept(obtainedSymbol);
			Type();
			Identifier();
		}
	}
}
//38
void MethodDeclarator(){
	Identifier();
	Accept("(");
	ParameterList();
	Accept(")");
}
//36
void AbstractMethodHeader(){
	ReturnType();
	MethodDeclarator();
	Throws();
	Accept(";");
}
//35
void MethodHeader(){
	MethodDeclarator();
	Throws();
}
//34
void MethodDeclaration(){
	MethodHeader();
	Block();
}
//33
void VariableDeclaration3(){
	if(isEnd)
		return;
	if(obtainedSymbol=="="){
		Accept(obtainedSymbol);
		ArrayInitializer();
	}
}
//32
void VariableDeclaration2(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="="){
		Accept(obtainedSymbol);
		Expression();
	}
	else if(obtainedSymbol=="["){
		Accept("[");
		Accept("]");
		VariableDeclaration3();
	}
}
//31
void VariableDeclaration(){
	Identifier();
	VariableDeclaration2();
}
//30
void VariableDeclarations(){
	VariableDeclaration();
	while(!isEnd && obtainedSymbol==","){
		Accept(",");
		VariableDeclaration();
	}
}
//29
void FieldOrMethod(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="("){
		Accept("(");
		ParameterList();
		Accept(")");
		Throws();
		Block();
	}
	else{
		VariableDeclaration2();
		while(!isEnd && obtainedSymbol==","){
			VariableDeclaration();
		}
	}
}
//28
void FieldOrMethodDeclaration(){
	Identifier();
	FieldOrMethod();
}
//27
void FieldVariableMethod(){
	if(isEnd)
		ErrrorHandling();
	if(!IsReserveWord() && (isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_')){
		FieldOrMethodDeclaration();
		Accept(";");
	}
	else{
		VariableDeclaration2();
		while(!isEnd && obtainedSymbol==","){
			VariableDeclaration();
		}
	}
}
//27
void MemberDeclaration4(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="void"){
		Accept(obtainedSymbol);
		MethodDeclaration();
	}
	else{
		Identifier();
		FieldVariableMethod();
	}
}
//26
void MemberDeclaration3(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="synchronized"){
		Accept(obtainedSymbol);
		MethodDeclaration();
	}
	else{
		MemberDeclaration4();
	}
}
//25
void MemberDeclaration2(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="final"){
		Accept(obtainedSymbol);
		MemberDeclaration3();
	}
	else if(obtainedSymbol=="volatile"){
		Accept(obtainedSymbol);
		FieldDeclaration();
	}
	else
		MemberDeclaration3();
}
//24
void FieldDeclaration(){
	Type();
	VariableDeclarations();
	Accept(";");
}
//23
void ConstructorInvocation(){
	if(isEnd)
		return;
	if(obtainedSymbol=="this" || obtainedSymbol=="super"){
		Accept(obtainedSymbol);
		Accept("(");
		ArgumentList();
		Accept(")");
		Accept(";");
	}
}
//22
void ConstructorBody(){
	Accept("{");
	ConstructorInvocation();
	BlockStatements();
	Accept("}");
}
//21
void Throws(){
	if(obtainedSymbol=="throws"){
		Accept(obtainedSymbol);
		Name();
		while(!isEnd && obtainedSymbol==","){
			Accept(obtainedSymbol);
			Name();
		}
	}
}
//20
void Parameter(){
	if(isEnd)
		return;
	if(obtainedSymbol=="float" ||obtainedSymbol=="double"||obtainedSymbol=="int"||obtainedSymbol=="long"||obtainedSymbol=="short"||obtainedSymbol=="char"||obtainedSymbol=="boolean"||isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'){
		Type();
		Identifier();
		while(obtainedSymbol==","){
			Accept(",");
			Type();
			Identifier();
		}
	}
}
//19
void ConstructorDeclarator(){
	Identifier();
	Accept("(");
	Parameter();
	Accept(")");
}
//18
void ConstructorDeclaration(){
	ConstructorDeclarator();
	Throws();
	ConstructorBody();
}
//17
void MemberDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="static" || obtainedSymbol=="transient"){
		Accept(obtainedSymbol);
		MemberDeclaration2();
	}
	else if(obtainedSymbol=="final" || obtainedSymbol=="volatile" || obtainedSymbol=="synchronized" || obtainedSymbol=="void"){
		MemberDeclaration2();
	}
	else
		ConstructorDeclaration();
}
//16
void StaticMemberDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="final" || obtainedSymbol=="volatile" || obtainedSymbol=="synchronized" || obtainedSymbol=="void"){
		MemberDeclaration2();
	}
	else
		Block();
}
//15
void ClassBodyDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="public" || obtainedSymbol=="protected" ||obtainedSymbol=="private"){
		while(!isEnd && (obtainedSymbol=="public" || obtainedSymbol=="protected" ||obtainedSymbol=="private")){
			Accept(obtainedSymbol);
			MemberDeclaration();
		}
	}
	else if(obtainedSymbol=="static"){
		while(!isEnd && obtainedSymbol=="static"){
			Accept(obtainedSymbol);
			StaticMemberDeclaration();
		}
	}
	else if(obtainedSymbol=="final" || obtainedSymbol=="volatile" || obtainedSymbol=="synchronized" || obtainedSymbol=="void" || ((isalpha(obtainedSymbol.at(0)) || obtainedSymbol.at(0)=='$' || obtainedSymbol.at(0) == '_'))){
	printf("Testis: %s\n", obtainedSymbol.c_str());
		MemberDeclaration2();
	}
	else {
		ConstructorDeclaration();
	}
}
//14
void ClassBody(){
	Accept("{");
	ClassBodyDeclaration();
	Accept("}");
}
//13
void ImplementDeclaration(){
	if(isEnd)
		return;
	if(obtainedSymbol=="implements"){
	Accept("implements");
	Name();
	while(!isEnd && obtainedSymbol==","){
		Accept(",");
		Name();
		}
	}
}
//12
void SuperDeclaration(){
	if(isEnd)
		return;
	if(obtainedSymbol=="extends"){
	Accept("extends");
	Name();
	}
}
//11
void SuperAndInterfaceDeclaration(){
	SuperDeclaration();
	ImplementDeclaration();
}
//10
void FinalModifier(){
	if(obtainedSymbol=="final")
		Accept(obtainedSymbol);
}
//9
void ClassDeclaration(){
	Accept("class");
	Identifier();
	SuperAndInterfaceDeclaration();
	ClassBody();
}
//8
void ClassOrInterface(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="class")
		ClassDeclaration();
	else if(obtainedSymbol=="interface")
		InterfaceDeclaration();
}
//7
void TypeDeclaration2(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="abstract"){
		Accept(obtainedSymbol);
		ClassOrInterface();
	}
	else if(obtainedSymbol=="final"){
		Accept(obtainedSymbol);
		ClassDeclaration();
	}
	else if(obtainedSymbol=="class" || obtainedSymbol=="interface"){
		ClassOrInterface();
	}
}
//5
void TypeDeclaration(){
	if(isEnd)
		ErrrorHandling();
	if(obtainedSymbol=="public")
		Accept(obtainedSymbol);
	TypeDeclaration2();
}
//4
void ImportAll(){
	if(obtainedSymbol==".*"){
		Accept(obtainedSymbol);
	}
}
//3
void ImportDeclaration(){
	Accept("import");
	Name();
	ImportAll();
	Accept(";");
}
//2
void PackageDeclaration(){
	if(obtainedSymbol=="package"){
		Accept(obtainedSymbol);
		Name();
		Accept(";");
	}
}
//1
void CompilationUnit(){
	if(isEnd)
		return;
	PackageDeclaration();
	while(!isEnd && obtainedSymbol=="import"){
		ImportDeclaration();
	}
	while(!isEnd && (obtainedSymbol=="public" || obtainedSymbol=="abstract" || obtainedSymbol=="final" || obtainedSymbol=="class" || obtainedSymbol=="interface")){
		TypeDeclaration();
	}
}
void TestProcedure()
{
	CompilationUnit();
}
