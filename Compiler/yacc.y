%{
	#include <stdio.h>
	#include <stdlib.h>
%}

%token SEMICOLON COLON COMMA OPEN_CURLY_BRACES CLOSE_CURLY_BRACES LESS_THAN GREATER_THAN OPEN_PARENTHESES CLOSE_PARENTHESES OPEN_BRACKET CLOSE_BRACKET
%token PLUS MINUS MULTIPLY DIVIDE MOD EQUAL NUMERAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL AND OR
%token RETURN DEFINE FOR WHILE IF ELSE ELSE_IF MAIN

%token BOOLEAN
%token INTEGER
%token NAME
%token STRING

%start Program

%left PLUS MINUS MULTIPLY DIVIDE MOD
%left EQUAL NOT_EQUAL GREATER_OR_EQUAL GREATER_THAN LESS_THAN LESS_OR_EQUAL
%left OR AND

%%

Program: Defines Functions  {}

Defines: Define Defines  {}
        | Define {}

Define: NUMERAL DEFINE NAME BOOLEAN {}
        | NUMERAL DEFINE NAME INTEGER {}
        | NUMERAL DEFINE NAME STRING {}

Functions: Function Functions {}
        | Main {}

Function: NAME OPEN_PARENTHESES Arguments CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}

Main: MAIN OPEN_PARENTHESES Arguments CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}

Arguments: /* empty */	{}
				|	Parameters	{}

Parameters: NAME {}
				| NAME COMMA Parameters {}

Block: /* empty */ {}
				| Sentences

Sentences: Sentence {}
				| Sentence Sentences {}

Sentence: VariableOperation SentenceEnd {}
				| For {}
				| While {}
				| If {}
				| FunctionExecute {}
				| Return {}

SentenceEnd: /* empty */ {}
				| SEMICOLON {}

VariableOperation: Assignments {}
				| Increment {}
				| Decrement {}

Assignments: Assignment COMMA Assignments {}
				| Assignment {}

Assignment: NAME EQUAL STRING {}
				| NAME EQUAL Queue {}
				| NAME EQUAL Stack {}
				| NAME AssignmentOperation Expression {}

Queue: OPEN_BRACKET ElementList CLOSE_BRACKET {}

Stack: LESS_THAN ElementList GREATER_THAN {}

ElementList: /* empty */ {}
				| Elements {}

Elements: Element COMMA Elements {}
				| Element {}

Element: BOOLEAN {}
				| STRING {}
				| INTEGER {}
				| NAME {}

AssignmentOperation: PLUS EQUAL {}
 				| EQUAL {}
				| MINUS EQUAL {}
				| DIVIDE EQUAL {}
				| MULTIPLY EQUAL {}

LogicalOperation: AND {}
				| OR {}
				|	NOT_EQUAL {}
				| EQUAL EQUAL {}
				| GREATER_OR_EQUAL {}
				| GREATER_THAN {}
				| LESS_OR_EQUAL {}
				| LESS_THAN {}

Increment: NAME PLUS PLUS {}

Decrement: NAME MINUS MINUS {}

Expression: BOOLEAN {}
				| NAME {}
				| INTEGER {}
				| FunctionExecute {}
				/*| VariableOperation {}*/
				| Expression PLUS Expression {}
				| Expression MINUS Expression {} /*Para queue y array cuantos queres sacar*/
				| Expression MOD Expression {}
				| Expression DIVIDE Expression {}
				| Expression MULTIPLY Expression {}

For: FOR OPEN_PARENTHESES Assignments SEMICOLON Condition SEMICOLON VariableOperation CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}
				| FOR OPEN_PARENTHESES NAME COLON  CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}

Condition: Expression LogicalOperation Expression {}
				| Expression {}
				| OPEN_PARENTHESES Condition CLOSE_PARENTHESES {}

While: WHILE OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}

If: IF OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {}

Else: ELSE OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{}
				| ELSE_IF OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {}
				| /* empty */

FunctionExecute: NAME OPEN_PARENTHESES CallArguments CLOSE_PARENTHESES {}

CallArguments: /* empty */	{}
				|	CallParameters	{}

CallParameters: CallParameter {}
				| CallParameter COMMA CallParameters {}

CallParameter: STRING {}
				| Expression {}

Return: RETURN Expression {}
			| RETURN STRING {}

%%
