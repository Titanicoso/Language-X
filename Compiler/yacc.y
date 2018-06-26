%{
	/*void yyerror (char *s);*/

	/* C declarations used in actions*/
	#include <stdio.h>
	#include <stdlib.h>
	#include "nodes.h"
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

Program: Defines Functions  {$$ = new_program_node(1, $1, $2); }

Defines: Define Defines  { $$ = new_defines_node(2, $1, $2); }
        | /* empty */ {$$ = NULL;}

Define: NUMERAL DEFINE NAME BOOLEAN { $$ = new_define_node(3, $1, $2, $3, NULL, $4); }
        | NUMERAL DEFINE NAME INTEGER {$$ = new_define_node(3, $1, $2, $3, NULL, $4); }
        | NUMERAL DEFINE NAME STRING {$$ = new_define_node(3, $1, $2, $3, $4, NULL); } /*medio feo solo para chequear constantes, no?*/

Functions: Function Functions {$$ = new_functions_node(4, $1, $2, NULL); }
        | Main {$$ = new_functions_node(4, NULL, NULL, $1); }

Function: NAME OPEN_PARENTHESES Arguments CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_function_node(5, $3, $6); }

Main: MAIN OPEN_PARENTHESES Arguments CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_main_node(6, $3, $6); }

Arguments: /* empty */	{$$ = new_arguemnts_node(7, NULL); }
				|	Parameters	{$$ = new_arguemnts_node(7, $1); }

Parameters: NAME {$$ = new_parameters_node(8, NULL, $1); }
				| NAME COMMA Parameters {$$ = new_parameters_node(8, $3, $1); }

Block: /* empty */ {$$ = new_block_node(9, NULL); }
				| Sentences {$$ = new_block_node(9, $1);}

Sentences: Sentence {$$ = new_sentences_node(10, $1, NULL); }
				| Sentence Sentences {$$ = new_sentences_node(10, $1, $2); }

Sentence: VariableOperation SentenceEnd { $$ = new_sentence_node(11, $1, $2, NULL, NULL, NULL, NULL, NULL); }
				| For {$$ = new_sentence_node(11, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
				| While {$$ = new_sentence_node(11, NULL, NULL, NULL, $1, NULL, NULL, NULL); }
				| If {$$ = new_sentence_node(11, NULL, NULL, NULL, NULL, $1, NULL, NULL); }
				| FunctionExecute SentenceEnd {$$ = new_sentence_node(11, NULL, $2, NULL, NULL, NULL, $1, NULL); }
				| Return SentenceEnd {$$ = new_sentence_node(11, NULL, $2, NULL, NULL, NULL, NULL, $1); }

SentenceEnd: /* empty */ {$$ = new_sentence_end_node(12, NULL); }
				| SEMICOLON {$$ = new_sentence_end_node(12, $1); }

VariableOperation: Assignments {$$ = new_variable_operation_node(13, $1, NULL, NULL); }
				| Increment {$$ = new_variable_operation_node(13, NULL, $1, NULL);}
				| Decrement {$$ = new_variable_operation_node(13, NULL, NULL, $1);}

Assignments: Assignment COMMA Assignments {$$ = new_assignments_node(14, $1, $3); }
				| Assignment {$$ = new_assignments_node(14, $1, NULL); }

Assignment: NAME EQUAL STRING {$$ = new_assignment_node(15, $1, $3, NULL, NULL, NULL, NULL); }
				| NAME EQUAL Queue {$$ = new_assignment_node(15, $1, NULL, $3, NULL, NULL, NULL); }
				| NAME EQUAL Stack {$$ = new_assignment_node(15, $1, NULL, NULL, $3, NULL, NULL); }
				| NAME AssignmentOperation Expression {$$ = new_assignment_node(15, $1, NULL, NULL, NULL, $2, $3);}

Queue: OPEN_BRACKET ElementList CLOSE_BRACKET {$$ = new_queue_stack_node(16, $2); }

Stack: LESS_THAN ElementList GREATER_THAN {$$ = new_queue_stack_node(17, $2); }

ElementList: /* empty */ {$$ = new_element_list_node(18, NULL); }
				| Elements {$$ = new_element_list_node(18, $1); }

Elements: Element COMMA Elements {$$ = new_elements_node(19, $1, $2, $3); }
				| Element {$$ = new_elements_node(19, $1, NULL, NULL); }

Element: BOOLEAN {$$ = new_element_node(20, NULL, $1); }
				| STRING {$$ = new_element_node(20, $1, NULL);}
				| INTEGER {$$ = new_element_node(20, $1, NULL);}
				| NAME {$$ = new_element_node(20, NULL, $1);}

/*No estoy segggura de que esto sea asi, entre un poco en duda*/
AssignmentOperation: PLUS EQUAL {$$ = '+='; }
 				| EQUAL {$$ = '='; }
				| MINUS EQUAL {$$ = '-='; }
				| DIVIDE EQUAL {$$ = '/='}
				| MULTIPLY EQUAL {$$ = '*='}

LogicalOperation: AND {$$ = '&&'}
				| OR {$$ = '||'}
				| NOT_EQUAL {$$ = '!='}
				| EQUAL EQUAL {$$ = '=='}
				| GREATER_OR_EQUAL {$$ = '>='}
				| GREATER_THAN {$$ = '>'}
				| LESS_OR_EQUAL {$$ = '<='}
				| LESS_THAN {$$ = '<'}

Increment: NAME PLUS PLUS {}

Decrement: NAME MINUS MINUS {}

Expression: BOOLEAN {$$ = new_expression_node(25, NULL, NULL, NULL, NULL, $1, NULL); }
				| NAME {$$ = new_expression_node(25, NULL, NULL,  NULL, NULL, NULL, $1); }
				| INTEGER {$$ = new_expression_node(25, NULL, NULL, NULL, NULL, $1, NULL);}
				| FunctionExecute { $$ = new_expression_node(25, NULL, NULL, NULL, $1, NULL, NULL); }
				| Expression PLUS Expression {$$ = new_expression_node(25, $1, '+', $3, NULL, NULL, NULL);}
				| Expression MINUS Expression {$$ = new_expression_node(25, $1, '-', $3, NULL, NULL, NULL);} /*Para queue y array cuantos queres sacar - "Se redefine segun el tipo de dato" esto iria en {}, nos fijamos que tipo de dato estamos manejando y segun eso que es lo que hacemos ... */
				| Expression MOD Expression {$$ = new_expression_node(25, $1, '%', $3, NULL, NULL, NULL);}
				| Expression DIVIDE Expression {$$ = new_expression_node(25, $1, '/', $3, NULL, NULL, NULL);}
				| Expression MULTIPLY Expression {$$ = new_expression_node(25, $1, '*', $3, NULL, NULL, NULL);}

For: FOR OPEN_PARENTHESES Assignments SEMICOLON Condition SEMICOLON VariableOperation CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_for_node(26, $3, $5, $7, $10); }
				| FOR OPEN_PARENTHESES NAME COLON NAME CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_for_node(26, NULL, NULL, NULL, $7); }

Condition: Expression LogicalOperation Expression {$$ = new_condition_node(27, $1, $2, $3, NULL); }
				| Expression {$$ = new_condition_node(27, $1, NULL, NULL, NULL);}
				| OPEN_PARENTHESES Condition CLOSE_PARENTHESES {$$ = new_condition_node(27, NULL, NULL, NULL, $2); }

While: WHILE OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_while_node(28, $3, $6); }

If: IF OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {$$ = new_if_node(29, $3, $6, $8); }

Else: ELSE OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES	{$$ = new_if_node(30, NULL, $3, NULL); }
				| ELSE_IF OPEN_PARENTHESES Condition CLOSE_PARENTHESES OPEN_CURLY_BRACES Block CLOSE_CURLY_BRACES Else {$$ = new_if_node(30, $3, $6, $8); }
				| /* empty */{$$ = new_if_node(30, NULL, NULL, NULL); }

FunctionExecute: NAME OPEN_PARENTHESES CallArguments CLOSE_PARENTHESES {$$ = new_function_execute_node(31, $3); }

CallArguments: /* empty */	{$$ = new_call_arguments_node(32, NULL); }
				|	CallParameters	{$$ = new_call_arguments_node(32, $1); }

CallParameters: CallParameter {$$ = new_call_parameters_node(33, $1, NULL); }
				| CallParameter COMMA CallParameters {$$ = new_call_parameters_node(33, $1, $3);}

CallParameter: STRING {$$ = new_call_parameter_node(34, $1, NULL); }
				| Expression {$$ = new_call_parameter_node(34, NULL, $1); }

Return: RETURN Expression {$$ = new_return_node(35, NULL, $1); }
			| RETURN STRING {$$ = new_return_node(35, $2, NULL); }

%%
