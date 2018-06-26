#ifndef NODES_H
#define NODES_H

enum productions
{
	DEFINE_INTEGER, DEFINE_STRING, SENTENCE_VARIABLE, SENTENCE_FOR, SENTENCE_WHILE, SENTENCE_IF, SENTENCE_FUNCTION, SENTENCE_RETURN,
	VARIABLE_ASSIGNMENT, VARIABLE_INCREMENT, VARIABLE_DECREMENT, ASSIGNMENT_STRING, ASSIGNMENT_QUEUE, ASSIGNMENT_STACK, ASSIGNMENT_EXPRESSION,
	ELEMENT_BOOLEAN, ELEMENT_STRING, ELEMENT_VARIABLE, ELEMENT_INTEGER, FOR_EACH, REGULAR_FOR, CONDITION_LOGICAL, CONDITION_EXPRESSION, CONDITION_PARENTHESES,
	EXPRESSION_BOOLEAN, EXPRESSION_VARIABLE, EXPRESSION_INTEGER, EXPRESSION_FUNCTION, EXPRESSION_OPERATION, PARAMERER_STRING, PARAMETER_EXPRESSION,
	RETURN_STRING, RETURN_EXPRESSION
};

	typedef struct program_node{
		struct defines_node * defines;
		struct functions_node * functions;
	} program_node;

	typedef struct defines_node{
		struct define_node * define;
		struct defines_node * next;
	} defines_node;

	typedef struct define_node{
		enum productions production;
		char* name;
		char* string_name;
		int value;
	} define_node;

	typedef struct functions_node{
		struct function_node * function;
		struct functions_node * next;
	} functions_node;

	typedef struct function_node{
		struct parameters_node * parameters;
		struct sentences_node * sentences;
		char * name;
	} function_node;

	typedef struct parameters_node {
		struct parameters_node * next;
		char* name; /*NAME*/
	} parameters_node;

	typedef struct sentences_node {
		struct sentence_node * sentence_node;
		struct sentences_node * sentences_node;
	}sentences_node;

	typedef struct sentence_node{
		enum productions production;
		struct variable_opration_node *variable_opration_node;
		struct for_node *for_node;
		struct while_node *while_node;
		struct if_node *if_node;
		struct function_execute_node* function_execute_node;
		struct return_node*return_node;
		char sentenceEnd; //Si es ; o nada.
	}sentence_node;

	typedef struct variable_opration_node{
		enum productions production;
		struct assignment_node *assignment_node;
		char * increment_decrement_name;
	}variable_opration_node;

	typedef struct assignment_node{
		enum productions production;
		char* name;
		char* string;
		struct queue_stack_node * queue_stack;
		char* assignment_operation;
		struct expression_node *expression_node;
	}assignment_node;

	typedef struct queue_stack_node{
		struct elements_node * elements;
	}queue_stack_node;

	typedef struct elements_node{
		struct element_node * element;
		struct elements_node * next;
	}elements_node;

	typedef struct element_node{
		enum productions production;
		char * string_name;
		int value;
	}element_node;

	typedef struct if_node{
		struct condition_node *condition;
		struct sentences_node *sentences;
		struct if_node *else_branch; /*en else pondrias condition en null & else en null, pero en if_else es lo mismo*/
	}if_node;

	typedef struct while_node{
		struct condition_node *condition;
		struct sentences_node *sentences;
	}while_node;

	typedef struct for_node	{
		enum productions production;
		struct assignment_node *assignment_node;
		struct condition_node *condition_node;
		struct variable_operation_node *variable_operation_node;
		struct sentences_node *sentences;
		char * variable;
		char * structure;
	}for_node;

	typedef struct condition_node{
		enum productions production;
		struct expression_node *expression_1;
		char * logical_operation;
		struct expression_node *expression_2;
		struct condition_node *condition_node;
	}condition_node;

	typedef struct expression_node{
		enum productions production;
		struct expression_node *expression_1;
		char op;
		struct expression_node *expression_2;
		struct function_execute_node *function_execute_node;

		int boolean_number;
		char * name;
	}expression_node;

	typedef struct function_execute_node{
		char * name;
		struct call_parameters_node * parameters;
	}function_execute_node;

	typedef struct call_parameters_node{
		struct call_parameter_node * parameter;
		struct call_parameters_node * next;
	} call_parameters_node;

	typedef struct call_parameter_node{
		enum productions production;
		char* string;
		struct expression_node *expression_node;
	} call_parameter_node;

	typedef struct return_node {
		enum productions production;
		char* string;
		struct expression_node *expression_node;
	} return_node;

#endif
