enum productions
{
	Program=1, Defines, Define, Functions, Function, Main, Arguments, Parameters, Block, 
	Sentences, Sentence, SentenceEnd, VariableOperation, Assignments, Assignment, Queue,
	Stack, ElementList, Elements, Element, AssignmentOperation, LogicalOperation, Increment, 
	Decrement, Expression, For, Condtion, While, If, Else, FunctionExecute, CallArguments, 
	CallParameters, CallParameter, Return
};

/* los puse en char -> logical_operation_node & assignment_operation_node. O usamos el node ? */
	typedef struct node{
		struct node *left; 
		struct node *right; 
		int token; 
	}node; 

	typedef struct program_node{
		int token; 
		struct defines_node *defines_node; 
		struct functions_node *functions_node; 
	}program_node;

	typedef struct defines_node{
		int token; 
		struct defines_node *defines_node; 
		struct define_node *define_node; 
	}defines_node; 

	typedef struct define_node{
		int token; 
		char*numeral; 
		char*define; 
		char*name; 
		char*string; 
		int*boolean_number; 
	}define_node; 

	typedef struct functions_node{
		int token; 
		struct fucntion_node * fucntion_node; 
		struct functions_node * functions_node; 
		struct main_node * main_node; 
	}functions_node; 

	typedef struct function_node{
		int token; 
		struct arguments_node *arguments_node; 
		struct block_node *block_node; 
	}function_node;

	typedef struct main_node{
		int token;
		struct arguments_node *arguments_node; 
		struct block_node *block_node; 
	}

	typedef struct arguments_node{
		int token; 
		struct parameters_node *parameters_node; 
	}arguments_node; 

	typedef struct parameters_node{
		int token; 
		struct parameters_node *parameters_node; 
		char* name; /*NAME*/ 
	};

	typedef struct block_node{
		int token; 
		struct sentences_node *sentences_node; 
	}block_node;

	typedef struct sentences_node{
		int token; 
		struct sentence_node *sentence_node; 
		struct sentences_node *sentences_node; 
	}sentences_node; 

	typedef struct sentence_node{
		int token; 
		struct variable_opration_node *variable_opration_node; 
		struct senetence_end_node *senetence_end_node; 
		struct for_node *for_node; 
		struct while_node *while_node; 
		struct if_node *if_node; 
		struct function_execute_node* function_execute_node; 
		struct return_node*return_node; 
	}sentence_node;

	/*lo termine agregando por cuestion de prolijidad*/
	typedef struct sentence_end_node{
		int token; 
		char semicolon; 
	}sentence_end_node; 

	typedef struct variable_opration_node{
		int token; 
		struct assignments_node *assignments_node; 
		struct increment_node * increment_node; 
		struct decrement_node * decrement_node; 
	}variable_opration_node; 

	typedef struct assignments_node{
		int token; 
		struct assignment_node *assignment_node; 
		struct assignments_node *assignments_node; 
	}assignments_node; 

	typedef struct assignment_node{
		int token; 
		char*name;
		char*string; 
		struct queue_node *queue_node; 
		struct stack_node *stack_node; 
		char assignment_operation; 
		struct expression_node *expression_node; 
	}assignment_node; 

	typedef struct queue_stack_node{
		int token; 
		struct element_list_node *element_list_node; 
	}queue_stack_node; 

	typedef struct element_list_node{
		int token; 
		struct elements_node *elements_node; 
	}element_list_node;

	typedef struct elements_node{
		int token; 
		struct element_node *element_node;
		char comma;  
		struct elements_node *elements_node; 
	}elements_node; 

	/*repito lo pongo solo por prolijidad, podriamos directamente poner la info en elements_node*/
	typedef struct element_node{
		int token; 
		char *string_name; /*BOOLEAN, STRING, INTEGER, NAME*/
		integer *boolean_number; 
	}element_node; 

	typedef struct if_node{
		int token; 
		struct condition_node *condition; 
		struct block_node *block; 
		struct if_node *else_branch; /*en else pondrias condition en null & else en null, pero en if_else es lo mismo*/
	}if_node; 

	typedef struct while_node{
		int token; 
		struct condition_node *condition_node; 
		struct block_node *block_node; 
	}while_node;

	typedef struct for_node
	{
		int token; 
		struct assignments_node *assignments_node; 
		struct condition_node *condition_node;
		struct variable_operation_node *variable_operation_node;  
		struct block_node *block_node; 
	}for_node;

	typedef struct condition_node{
		int token; 
		struct expression_node *expression_1; 
		char *logical_operation;
		struct expression_node *expression_2;
		struct condition_node *condition_node; 
	}condition_node; 

	typedef struct condition_node{
		int token; 
		struct expression_node *expression_1; 
		char *logical_operation;
		struct expression_node *expression_2;
		struct condition_node *condition_node; 
	}condition_node; 

	typedef struct expression_node{
		int token; 
		struct expression_node *expression_1; 
		char op; 
		struct expression_node *expression_2;
		struct function_expression_node *function_expression_node; 

		int boolean_number; 
		char * string_name;  
		/* BOOLEAN, NAME, INTEGER, ENTRE EN DUDA -> ¿struct node* node? */
	}expression_node; 
		
	typedef struct boolean_node{
		int token; 
		int boolean_value;
	}boolean_node; 

	typedef struct name_node{
		int token; 
		char* name_value; 
	}name_node;

	typedef struct integer_node{
		int token; 
		int integer_node; 
	}integer_node;

	typedef struct string_node{
		int token; 
		char* integer_node; 
	}string_node;

	typedef struct function_execute_node{
		int token; 
		struct call_arguments_node *call_arguments_node; 
	}function_execute_node;

	typedef struct call_arguments_node{
		int token; 
		struct call_parameters_node *call_parameters_node; 
	}call_arguments_node;

	typedef call_parameters_node{
		int token; 
		struct call_parameter_node *parameters_1; 
		struct call_parameters_node *parameters_2; 
	}call_parameters_node;

	typedef struct call_parameter_node{
		int token; 
		struct string_node *string_node; 
		struct expression_node *expression_node; 
	} call_parameter_node; 

	typedef struct return_node{
		int token; 
		struct string_node *string_node; 
		struct expression_node *expression_node; 
	}return_node;  /* mmmm... es igual a call_parameter_node, pero no se si estaria bueno ponerlo todo junto, me parece que el return node esta bueno que este separado, por claridad*/