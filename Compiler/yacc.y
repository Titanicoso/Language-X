%{
	/*void yyerror (char *s);*/

	/* C declarations used in actions*/
	#include <stdio.h>
	#include <stdlib.h>
	#include "nodes.h"
	#include "../Utilities/typeChecks.h"
	#include "../Utilities/translateNodes.h"

	extern void yyerror(char *);
	extern int yylex();
%}

%union {
	char * text;
	int value;
	basicTypes type;

	program_node* program_node;
	defines_node* defines_node;
	define_node* define_node;
	functions_node* functions_node;
	function_node* function_node;
	type_node * type_node;
	parameters_node* parameters_node;
	sentences_node* sentences_node;
	sentence_node* sentence_node;
	declaration_node * declaration_node;
	variable_opration_node* variable_opration_node;
	assignment_node* assignment_node;
	queue_stack_node* queue_stack_node;
	elements_node* elements_node;
	element_node* element_node;
	if_node* if_node;
	while_node* while_node;
	for_node* for_node;
	condition_node* condition_node;
	expression_node* expression_node;
	function_execute_node* function_execute_node;
	call_parameters_node* call_parameters_node;
	call_parameter_node* call_parameter_node;
	return_node* return_node;
}


%token SEMICOLON COLON COMMA OPEN_CURLY_BRACES CLOSE_CURLY_BRACES LESS_THAN GREATER_THAN OPEN_PARENTHESES CLOSE_PARENTHESES OPEN_BRACKET CLOSE_BRACKET
%token PLUS MINUS MULTIPLY DIVIDE MOD EQUAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL AND OR
%token RETURN DEFINE FOR WHILE IF ELSE MAIN BOOLEAN_TYPE INTEGER_TYPE STRING_TYPE

%token <value> BOOLEAN
%token <value> INTEGER
%token <text> NAME
%token <text> STRING

%type <program_node> Program
%type <defines_node> Defines
%type <define_node> Define
%type <functions_node> Functions
%type <function_node> Function Main
%type <type_node> Type CompoundType
%type <type> BasicType
%type <parameters_node> Arguments Parameters
%type <sentences_node> Block Sentences
%type <sentence_node> Sentence
%type <text> SentenceEnd AssignmentOperation LogicalOperation Increment Decrement
%type <declaration_node> Declaration
%type <variable_opration_node> VariableOperation
%type <assignment_node> Assignment
%type <queue_stack_node> Queue Stack
%type <elements_node> ElementList Elements
%type <element_node> Element
%type <if_node> If Else
%type <while_node> While
%type <for_node> For
%type <condition_node> Condition
%type <expression_node> Expression
%type <function_execute_node> FunctionExecute
%type <call_parameters_node> CallArguments CallParameters
%type <call_parameter_node> CallParameter
%type <return_node> Return


%start Program

%left PLUS MINUS MULTIPLY DIVIDE MOD
%left EQUAL NOT_EQUAL GREATER_OR_EQUAL GREATER_THAN LESS_THAN LESS_OR_EQUAL
%left OR AND

/* Descriptions of expected inputs corresponding actions (in C)*/
%%

Program: Defines Functions  {$$ = new_program_node($1, $2); translateProgramNode($$);}

Defines: Define Defines  { $$ = new_defines_node($1, $2); }
        | /* empty */ {$$ = NULL; createFunction();}

Define: DEFINE NAME BOOLEAN { $$ = new_define_node(DEFINE_INTEGER, $2, $3, NULL); }
        | DEFINE NAME INTEGER {$$ = new_define_node(DEFINE_INTEGER, $2, $3, NULL); }
        | DEFINE NAME STRING {$$ = new_define_node(DEFINE_STRING, $2, 0, $3); }

Functions: Function Functions {$$ = new_functions_node($1, $2); }
        | Main {$$ = new_functions_node($1, NULL);}

Function: Type NAME OPEN_PARENTHESES Arguments CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_function_node($1, $2, $4, $7);}

BasicType: INTEGER_TYPE {$$ = INTEGER_T;}
			| BOOLEAN_TYPE {$$ = BOOLEAN_T;}
			| STRING_TYPE {$$ = STRING_T;}

CompoundType: OPEN_BRACKET BasicType CLOSE_BRACKET {$$ = new_type_node($2, QUEUE_T);}
			| LESS_THAN BasicType GREATER_THAN {$$ = new_type_node($2, STACK_T);}

Type: BasicType {$$ = new_type_node($1, NONE);}
		| CompoundType {$$ = $1;}

Main: Type MAIN OPEN_PARENTHESES CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_function_node($1, "main", NULL, $6); }

Arguments: /* empty */	{$$ = NULL;}
				|	Parameters	{$$ = $1;}

Parameters: Type NAME {$$ = new_parameters_node($1, $2, NULL); }
				| Type NAME COMMA Parameters {$$ = new_parameters_node($1, $2, $4); }

Block: /* empty */ {$$ = NULL; }
				| Sentences {$$ = $1;}

Sentences: Sentence {$$ = new_sentences_node($1, NULL); }
				| Sentence Sentences {$$ = new_sentences_node($1, $2); }

Sentence: Declaration SentenceEnd { $$ = new_sentence_node(SENTENCE_DECLARATION, $1, NULL, $2, NULL, NULL, NULL, NULL, NULL); }
				| VariableOperation SentenceEnd { $$ = new_sentence_node(SENTENCE_VARIABLE, NULL, $1, $2, NULL, NULL, NULL, NULL, NULL); }
				| For {$$ = new_sentence_node(SENTENCE_FOR, NULL, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
				| While {$$ = new_sentence_node(SENTENCE_WHILE, NULL, NULL, NULL, NULL, $1, NULL, NULL, NULL); }
				| If {$$ = new_sentence_node(SENTENCE_IF, NULL, NULL, NULL, NULL, NULL, $1, NULL, NULL); }
				| FunctionExecute SentenceEnd {$$ = new_sentence_node(SENTENCE_FUNCTION, NULL, NULL, $2, NULL, NULL, NULL, $1, NULL); }
				| Return SentenceEnd {$$ = new_sentence_node(SENTENCE_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL, $1); }

SentenceEnd: /* empty */ {$$ = NULL; }
				| SEMICOLON {$$ = ";";}

Declaration: Type NAME {$$ = new_declaration_node($1, $2);}

VariableOperation: Assignment {$$ = new_variable_opration_node(VARIABLE_ASSIGNMENT, $1, NULL); }
				| Increment {$$ = new_variable_opration_node(VARIABLE_INCREMENT, NULL, $1);}
				| Decrement {$$ = new_variable_opration_node(VARIABLE_DECREMENT, NULL, $1);}

Assignment: NAME EQUAL STRING {$$ = new_assignment_node(ASSIGNMENT_STRING, $1, $3, NULL, NULL, NULL); }
				| NAME EQUAL Queue {$$ = new_assignment_node(ASSIGNMENT_QUEUE, $1, NULL, $3, NULL, NULL); }
				| NAME EQUAL Stack {$$ = new_assignment_node(ASSIGNMENT_STACK, $1, NULL, $3, NULL, NULL); }
				| NAME AssignmentOperation Expression {$$ = new_assignment_node(ASSIGNMENT_EXPRESSION, $1, NULL, NULL, $2, $3);}

Queue: OPEN_BRACKET ElementList CLOSE_BRACKET {$$ = new_queue_stack_node($2); }

Stack: LESS_THAN ElementList GREATER_THAN {$$ = new_queue_stack_node($2); }

ElementList: /* empty */ {$$ = NULL; }
				| Elements {$$ = $1; }

Elements: Element COMMA Elements {$$ = new_elements_node($1, $3); }
				| Element {$$ = new_elements_node($1, NULL); }

Element: BOOLEAN {$$ = new_element_node(ELEMENT_BOOLEAN, $1, NULL); }
				| STRING {$$ = new_element_node(ELEMENT_STRING, 0, $1);}
				| INTEGER {$$ = new_element_node(ELEMENT_INTEGER, $1, NULL);}
				| NAME {$$ = new_element_node(ELEMENT_VARIABLE, 0, $1);}

AssignmentOperation: PLUS EQUAL {$$ = "+="; }
 				| EQUAL {$$ = "="; }
				| MINUS EQUAL {$$ = "-="; }
				| DIVIDE EQUAL {$$ = "/=";}
				| MULTIPLY EQUAL {$$ = "*=";}

LogicalOperation: AND {$$ = "&&";}
				| OR {$$ = "||";}
				| NOT_EQUAL {$$ = "!=";}
				| EQUAL EQUAL {$$ = "==";}
				| GREATER_OR_EQUAL {$$ = ">=";}
				| GREATER_THAN {$$ = ">";}
				| LESS_OR_EQUAL {$$ = "<=";}
				| LESS_THAN {$$ = "<";}

Increment: NAME PLUS PLUS {$$ = $1;}

Decrement: NAME MINUS MINUS {$$ = $1;}

Expression: BOOLEAN {$$ = new_expression_node(EXPRESSION_BOOLEAN, NULL, 0, NULL, NULL, $1, NULL); }
				| NAME {$$ = new_expression_node(EXPRESSION_VARIABLE, NULL, 0,  NULL, NULL, 0, $1); }
				| INTEGER {$$ = new_expression_node(EXPRESSION_INTEGER, NULL, 0, NULL, NULL, $1, NULL);}
				| FunctionExecute { $$ = new_expression_node(EXPRESSION_FUNCTION, NULL, 0, NULL, $1, 0, NULL); }
				| Expression PLUS Expression {$$ = new_expression_node(EXPRESSION_OPERATION, $1, '+', $3, NULL, 0, NULL);}
				| Expression MINUS Expression {$$ = new_expression_node(EXPRESSION_OPERATION, $1, '-', $3, NULL, 0, NULL);} /*Para queue y array cuantos queres sacar - "Se redefine segun el tipo de dato" esto iria en {}, nos fijamos que tipo de dato estamos manejando y segun eso que es lo que hacemos ... */
				| Expression MOD Expression {$$ = new_expression_node(EXPRESSION_OPERATION, $1, '%', $3, NULL, 0, NULL);}
				| Expression DIVIDE Expression {$$ = new_expression_node(EXPRESSION_OPERATION, $1, '/', $3, NULL, 0, NULL);}
				| Expression MULTIPLY Expression {$$ = new_expression_node(EXPRESSION_OPERATION, $1, '*', $3, NULL, 0, NULL);}

For: FOR OPEN_PARENTHESES Assignment SEMICOLON Condition SEMICOLON VariableOperation CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_for_node(REGULAR_FOR, $3, $5, $7, $10, NULL, NULL); }
				| FOR OPEN_PARENTHESES NAME COLON NAME CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_for_node(FOR_EACH, NULL, NULL, NULL, $8, $3, $5);}

Condition: Expression LogicalOperation Expression {$$ = new_condition_node(CONDITION_LOGICAL, $1, $2, $3, NULL); }
				| Expression {$$ = new_condition_node(CONDITION_EXPRESSION, $1, NULL, NULL, NULL);}
				| OPEN_PARENTHESES Condition CLOSE_PARENTHESES {$$ = new_condition_node(CONDITION_PARENTHESES, NULL, NULL, NULL, $2); }

While: WHILE OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_while_node($3, $6); }

If: IF OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {$$ = new_if_node($3, $6, $8); }

Else: ELSE OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_if_node(NULL, $3, NULL); }
				| ELSE OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {$$ = new_if_node($3, $6, $8); }
				| /* empty */{$$ = NULL; }

FunctionExecute: NAME OPEN_PARENTHESES CallArguments CLOSE_PARENTHESES {$$ = new_function_execute_node($1, $3); }

CallArguments: /* empty */	{$$ = NULL; }
				|	CallParameters	{$$ = $1;}

CallParameters: CallParameter {$$ = new_call_parameters_node($1, NULL); }
				| CallParameter COMMA CallParameters {$$ = new_call_parameters_node($1, $3);}

CallParameter: STRING {$$ = new_call_parameter_node(PARAMERER_STRING, $1, NULL); }
				| Expression {$$ = new_call_parameter_node(PARAMETER_EXPRESSION, NULL, $1);}

Return: RETURN Expression {$$ = new_return_node(RETURN_EXPRESSION, NULL, $2); }
			| RETURN STRING {$$ = new_return_node(RETURN_STRING, $2, NULL); }

%%
