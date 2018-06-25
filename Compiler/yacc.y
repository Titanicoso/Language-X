%{
	/*void yyerror (char *s);*/

	/* C declarations used in actions*/
	#include <stdio.h>
	#include <stdlib.h>

	/*logical_operation_node & assignment_operation_node*/
	typedef struct node{
		struct node *left; 
		struct node *right; 
		char *token; 
	}node; 

	typedef struct if_node{
		struct condition_node *condition; 
		struct block_node *block; 
		struct if_node *else_branch; /*en else pondrias condition en null & else en null, pero en if_else es lo mismo*/
		char *token; 
	}if_node; 

	typedef struct while_node{
		struct condition_node *condition; 
		struct block_node *block; 
		char *token; /* token - de que tipo de nodo es */
	}while_node;

	typedef struct for_node
	{
		struct assignments_node *assignments; 
		struct condition_node *condition;
		struct variable_operation_node *variable_operation;  
		struct block_node *block; 
		char *token;
	}for_node;

	typedef struct condition_node{
		struct expression_node *expression_1; 
		struct logical_operation_node *logical_operation;
		struct expression_node *expression_2;
		struct condition_node *condition; 
		char *token; 
	}condition_node; 

	typedef struct condition_node{
		struct expression_node *expression_1; 
		struct logical_operation_node *logical_operation;
		struct expression_node *expression_2;
		struct condition_node *condition; 
		char *token; 
	}condition_node; 

	typedef struct expression_node{
		struct expression_node *expression_1; 
		struct expression_node *expression_2;
		struct function_expression_node *function_expression; 

		/* no estoy muy segura que tan necesarios son estos*/
		struct boolean_node *boolean; 
		struct name_node *name; 
		struct integer_node *integer; 
		struct string_node *string; 
		char *token; 
		/* BOOLEAN, NAME, INTEGER, ENTRE EN DUDA -> ¿struct node* node? */
	}expression_node; 
		
	typedef struct boolean_node{
		int boolean_value;
		char *token; 
	}boolean_node; 

	typedef struct name_node{
		string name_value; 
		char *token; 
	}name_node;

	typedef struct integer_node{
		int integer_node;
		char *token;  
	}integer_node;

	typedef struct string_node{
		string integer_node;
		char *token;  
	}string_node;

	typedef struct function_execute_node{
		char*token; 
		struct call_arguments_node *arguments; 
	}function_execute_node;

	typedef struct call_arguments_node{
		char*token; 
		struct call_parameters_node *parameters; 
	}call_arguments_node;

	typedef call_parameters_node{
		char*token; 
		struct call_parameter_node *parameters_1; 
		struct call_parameters_node *parameters_2; 
	}call_parameters_node;

	typedef struct call_parameter_node{
		char*token; 
		struct string_node *string_node; 
		struct expression_node *expression_node; 
	} call_parameter_node; 

	typedef struct return_node{
		char*token; 
		struct string_node *string_node; 
		struct expression_node *expression_node; 
	}return_node;  /* mmmm... es igual a call_parameter_node, pero no se si estaria bueno ponerlo todo junto, me parece que el return node esta bueno que este separado, por claridad*/


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
				| NOT_EQUAL {}
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
				| Expression MINUS Expression {} /*Para queue y array cuantos queres sacar - "Se redefine segun el tipo de dato" esto iria en {}, nos fijamos que tipo de dato estamos manejando y segun eso que es lo que hacemos ... */
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


