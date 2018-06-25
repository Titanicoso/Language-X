%{
	/*void yyerror (char *s);*/

	/* C declarations used in actions*/
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

/* Descriptions of expected inputs corresponding actions (in C)*/
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
				| Sentences {}

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

Assignment: NAME EQUAL STRING {$1 = $3;}
				| NAME EQUAL Queue {$1 = $3;}
				| NAME EQUAL Stack {$1 = $3;}
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

AssignmentOperation: PLUS EQUAL {;}
 				| EQUAL {;}
				| MINUS EQUAL {;}
				| DIVIDE EQUAL {;}
				| MULTIPLY EQUAL {;}

LogicalOperation: AND {;}
				| OR {;}
				| NOT_EQUAL {;}
				| EQUAL EQUAL {;}
				| GREATER_OR_EQUAL {;}
				| GREATER_THAN {;}
				| LESS_OR_EQUAL {;}
				| LESS_THAN {;}

Increment: NAME PLUS PLUS {$1++; }

Decrement: NAME MINUS MINUS {$1--; }

Expression: BOOLEAN {$$ = $1; }
				| NAME {$$ = $1;  }
				| INTEGER {$$ = $1; }
				| FunctionExecute {}
				/*| VariableOperation {}*/
				| Expression PLUS Expression {
											if($1 == INTEGER && $3 == INTEGER) { $$ = $1 + $3;}else if($1 == BOOLEAN && $3 == BOOLEAN){
											$$ = $1; /*defini la suma de booleanos*/}else if($1 == NAME && $3 == NAME){
												if($1 == STRING && $3 == STRING){}else if($1 == Queue && $3 == Queue){}else if($1 == Stack && $3 == Stack){}else{
													printf("Error, you are trying to add two different type of elements\n");
													return -1; 
												}
											}
				}
				| Expression MINUS Expression {} /*Para queue y array cuantos queres sacar - "Se redefine segun el tipo de dato" esto iria en {}, nos fijamos que tipo de dato estamos manejando y segun eso que es lo que hacemos ... */
				| Expression MOD Expression {$$ = $1 % $3;}
				| Expression DIVIDE Expression {if($3 == 0) {$$ = 0;}else {$$ = $1 / $3;}}
				| Expression MULTIPLY Expression {$$ = $1 * $3; }

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
