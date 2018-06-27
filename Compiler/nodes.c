#include "nodes.h"

program_node*
new_program_node(defines_node * defines, 
				functions_node * functions){

	program_node * node = malloc(sizeof(program_node)); 

	node->defines = defines; 
	node->functions = functions; 

	return node; 
}

defines_node*
new_defines_node(define_node * define, defines_node * next){

	defines_node * node = malloc(sizeof(defines_node)); 

	node -> define = define; 
	node -> next = next; 

	return node; 
}

define_node* 
new_define_node(enum productions production, char*name, int value, char* string_name){

	char * name_aux = malloc(sizeof(name)); 
	strcpy(name_aux, name); 

	if(exists_variable(name_aux)){
		error(); 
	}else{

		define_node * node = malloc(sizeof(define_node)); 
		node -> name = name_aux; 
		node -> production = production; 
		if(production == DEFINE_INTEGER){
			node -> string_name = NULL; 
			node -> value = value; 
		}else if(production == DEFINE_STRING){
			node -> value = 0; 
			char* string_name_aux = malloc(sizeof(string_name)); 
			strcpy(string_name_aux, string_name); 
			node -> string_name = string_name_aux; 
		}else{
			error(); 
		}
	}

	return node; 	
}

functions_node*
new_functions_node(function_node * function, functions_node * next){

	functions_node * node = malloc(sizeof(functions_node)); 

	node -> function = function;
	if(next ! = NULL){node -> next = next; }else{
		node -> next = NULL; /*No me acuerdo que tan necesario era chequear esto en C, de ultima lo sacamos*/
	}

	return node; 
}

function_node*
new_function_node(char* name, parameters_node * parameters, sentences_node * sentences){

	function_node * node = malloc(sizeof(function_node)); 
	char * name_aux = malloc(sizeof(name) + 1);
	strcpy(name_aux, name); 

	if(exists_function_name(name_aux)){
		error(); /*Printf + exit*/
	}else{
		node -> parameters = parameters;
		node -> sentences = sentences;
		node -> name = name_aux;   
	}
	return node; /*total la idea es que el error() se encargue de terminar antes si es necesario*/
}

parameters_node*
new_parameters_node(char*name, parameters_node * next){
	
	parameters_node * node = malloc(sizeof(parameters_node)); 
	char * name_aux = malloc(sizeof(name)); 
	strcpy(name_aux, name); 

	if(exists_parameter_in_function()){
		/*chequear que los nombres de los 
		parametros pertenecesientes a dicha 
		funcion son unicos - Podriamos decir
		que en nuestro lenguaje no se puede
		repetir en ningun momento nombres, si seria
		un chino de lenguaje para usar, pero nos ahorraria
		problemas a nosotros*/
		error(); 
	}else{
		node -> name = name_aux; 
		if(next != NULL){
			node -> next = next; 
		}else{
			node -> next = NULL; 
		}
		return node; 
	}
	return node; 
}

sentences_node*
new_sentences_node(sentence_node * sentence, sentences_node * sentences){

	sentences_node * node = malloc(sizeof(sentences_node)); 

	node -> sentence = sentence;
	if(sentences != NULL){
		node -> sentences = sentences;
	}else{
		node -> sentences = NULL; 
	}

	return node;  
}

sentence_node*
new_sentence_node(enum productions production, variable_operation_node * variable_operation, 
	for_node * for_node, while_node * while_node, if_node * if_node, 
	function_execute_node* function_execute, return_node*return_node, char sentenceEnd){

	sentence_node * node = malloc(sizeof(sentence_node)); 

	node -> production = production; 
	node -> variable_operation = variable_operation; 
	node -> for_node = for_node; 
	node -> while_node = while_node; 
	node -> if_node = if_node; 
	node -> function_execute = function_execute; 
	node -> return_node = return_node; 

	if(sentenceEnd != NULL && sentenceEnd!=';'){
		error(); 
	}else{
		node -> sentenceEnd = sentenceEnd; 
	}

	return node; 
}

variable_operation_node*
new_variable_operation_node(enum productions production, assignment_node * assignment, char * increment_decrement_name){

	variable_operation_node * node = malloc(sizeof(variable_operation_node)); 
	char * name_aux = malloc(sizeof(increment_decrement_name)); 
	strcpy(name_aux, increment_decrement_name); 

	node -> production = production; 
	if(production == VARIABLE_ASSIGNMENT) {
		node -> assignment = assignment; 
		node -> increment_decrement_name = NULL; 
	}else if(production == VARIABLE_INCREMENT || production == VARIABLE_DECREMENT){
		node -> assignment == NULL; 
		node -> increment_decrement_name = name_aux; 
	}else{
		error(); 
	}
	return node; 
}

assignment_node*
new_assignment_node(enum productions production, char * name, 
	char * string, queue_stack_node * queue_stack, char* assignment_operation, 
	expression_node * expression){

	assignment_node * node = malloc(sizeof(assignment_node)); 
	char * name_aux = malloc(sizeof(name)); 

	node -> production = production; 
	node -> name = name_aux; 

	if(production == ASSIGNMENT_STRING){
		/*solo lo creo si lo necesito, si no directamente, null*/
		char * string_aux = malloc(sizeof(string)); 
		strcpy(string_aux, string); 
		node -> string = string_aux; 
		node -> queue_stack = NULL; 
		node -> assignment_operation = NULL; 
		node -> expression = NULL; 
	}else if(production == ASSIGNMENT_QUEUE || production == ASSIGNMENT_STACK){

		node -> string = NULL; 
		node -> queue_stack = queue_stack; 
		node -> assignment_operation = NULL; 
		node -> expression = NULL; 
	}else if(production == ASSIGNMENT_EXPRESSION){

		node -> string = NULL; 
		node -> queue_stack = NULL; 
		
		char * assignment_operation_aux = malloc(sizeof(assignment_operation)); 
		strcpy(assignment_operation_aux, assignment_operation); 
		node -> assignment_operation = assignment_operation_aux; 

		node -> expression = expression; 
	}else {
		error(); 
	}

	return node; 
}

queue_stack_node *
new_queue_stack_node(elements_node * elements){

	queue_stack_node * node = malloc(sizeof(queue_stack_node)); 

	node -> elements = elements; 

	return node; 
}

elements_node * 
new_elements_node (element_node * element, elements_node * next){

	elements_node * node = malloc(sizeof(elements_node)); 

	node -> element = element; 
	if(next!= NULL){
		node -> next = next; 
	}else{
		node -> NULL; 
	}

	return node; 
}

element_node * 
new_element_node(enum productions production, int value, char * string_name){

	element_node * node = malloc(sizeof(element_node)); 

	node -> production = production; 

	if(production == ELEMENT_BOOLEAN || production == ELEMENT_INTEGER){
		node -> value = value; 
		node -> string_name = NULL; 
	}else if(production == ELEMENT_STRING || production == ELEMENT_VARIABLE){
		node -> value = 0; 
		char * string_name_aux = malloc(sizeof(string_name)); 
		strcpy(string_name_aux, string_name); 
		node -> string_name = string_name_aux; 
	}else{
		error(); 
	}

	return node; 
}

if_node * 
new_if_node(condition_node * condition, sentences_node * sentences, if_node * else_branch){

	if_node * node = malloc(sizeof(if_node)); 

	if(condition != NULL){
		node -> condition = condition; 
	}else{
		node -> condition = NULL; 
	}

	node -> sentences = sentences; 

	if(else_branch !=NULL){
		node -> else_branch = else_branch; 
	}else{
		node -> else_branch = NULL; 
	}

	return node; 
}

while_node*
new_while_node(condition_node * condition, sentences_node * sentences){

	while_node * node = malloc(sizeof(while_node)); 

	node -> condition = condition;
	node -> sentences = sentences; 

	return node;  
}

for_node*
new_for_node(enum productions production, assignment_node *assignment, condition_node *condition, 
			variable_operation_node *variable_operation, sentences_node *sentences, char * variable, 
		char * structure){

	for_node * node = malloc(sizeof(for_node)); 

	node -> production = production; 
	if(production == REGULAR_FOR){
		node -> assignment = assignment; 
		node -> condition = condition; 
		node -> variable_operation = variable_operation; 
		node -> sentences = sentences; 
		node -> variable = NULL;  
		node -> structure = NULL; 
	}else if(production == FOR_EACH){
		node -> assignment = NULL; 
		node -> condition = NULL; 
		node -> variable_operation = NULL; 
		node -> sentences = sentences; 

		char * variable_aux = malloc(sizeof(variable)); 
		strcpy(variable_aux, variable); 
		node -> variable = variable_aux; 

		char * structure_aux = malloc(sizeof(structure)); 
		strcpy(structure_aux, structure); 
		node -> structure = structure_aux; 
	}else{
		error(); 
	}

	return node; 
}

condition_node*
new_condition_node(enum productions production, expression_node *expression_1, char * logical_operation, 
	expression_node *expression_2, condition_node *condition){

	condition_node * node = malloc(sizeof(condition_node)); 

	node -> production = production; 

	if(production == CONDITION_LOGICAL){
		node -> expression_1 = expression_1; 

		char * logical_operation_aux = malloc(sizeof(logical_operation)); 
		strcpy(logical_operation_aux, logical_operation); 
		node -> logical_operation = logical_operation_aux; 

		node -> expression_2 = expression_2; 
		node -> condition = NULL; 

	}else if(production == CONDITION_EXPRESSION){
		node -> expression_1 = expression_1; 
		node -> logical_operation = NULL; 
		node -> expression_2 = NULL; 
		node -> condition = NULL; 

	}else if(production == CONDITION_PARENTHESES){
		node -> expression_1 = NULL; 
		node -> logical_operation = NULL; 
		node -> expression_2 = NULL; 
		node -> condition = condition; 
	}else{
		erro(); 
	}
	return node; 
}

expression_node*
new_expression_node(enum productions production, 
		expression_node *expression_1, 
		char op, expression_node *expression_2, 
		function_execute_node *function_execute, 
		int boolean_number, char * name){

	expression_node * node = malloc(sizeof(expression_node)); 

	node -> production = production; 

	if(production == EXPRESSION_OPERATION){
		node -> expression_1 = expression_1; 
		node -> expression_2 = expression_2; 
		node -> op = op; 
		node -> function_execute = NULL; 
		node -> boolean_number = 0; 
		node -> name = NULL; 
	}else if(production == EXPRESSION_BOOLEAN || production == EXPRESSION_VARIABLE || production == EXPRESSION_INTEGER || production == EXPRESSION_FUNCTION){
		node -> expression_1 = NULL; 
		node -> expression_2 = NULL; 
		node -> op = NULL; 
		node -> function_execute = NULL; 
		if(production == EXPRESSION_BOOLEAN || production == EXPRESSION_INTEGER){
			node -> boolean_number = boolean_number; 
			node -> name = NULL; 	
		}else if(production == EXPRESSION_VARIABLE){
			node -> boolean_number = 0; 

			char * name_aux = malloc(sizeof(name)); 
			strcpy(name_aux, name); 
			node -> name = name_aux; 
		}else{
			node -> boolean_number = 0; 
			node -> name = NULL; 
			node -> function_execute = function_execute; 
		}
	}else{
		error(); 
	}

	return node; 
}

function_execute_node*
new_function_execute_node(char * name, call_parameters_node * parameters){

	function_execute_node * node = malloc(sizeof(function_execute_node)); 
	char * name_aux = malloc(sizeof(name)); 
	strcpy(name_aux, name); 

	node -> name = name_aux; 
	node -> parameters = parameters; 
}

call_parameters_node * 
new_call_parameters_node(call_parameter_node * parameter, 
	call_parameters_node * next){

	call_parameters_node * node = malloc(sizeof(call_parameters_node)); 

	node -> parameter = parameter; 
	node -> next = next; 

	return node; 
}

call_parameter_node*
new_call_parameter_node(enum productions production, 
	char* string, 
	expression_node *expression){

	call_parameter_node * node = malloc(sizeof(call_parameter_node)); 

	node -> production = production; 

	if(production == PARAMERER_STRING){
		char * string_aux = malloc(sizeof(string)); 
		strcpy(string_aux, string); 
		node -> string = string_aux; 
		node -> expression = NULL; 
	}else if(production == PARAMETER_EXPRESSION){
		node -> string = NULL; 
		node -> expression = expression; 
	}else{
		error(); 
	}

	return node; 
}

return_node * 
new_return_node(enum productions production, 
		char* string, 
		expression_node *expression){

	return_node * node = malloc(sizeof(return_node)); 

	node -> production = production; 
	if(production == RETURN_EXPRESSION){
		node -> string = NULL; 
		node -> expression = expression; 
	}else if(production == RETURN_STRING){
		char*string_aux = malloc(sizeof(string)); 
		strcpy(string_aux, string); 
		node -> string = string_aux; 
		node -> expression = NULL; 
	}else{
		error(); 
	}

	return node; 
}



