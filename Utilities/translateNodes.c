#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Compiler/nodes.h"
#include "translateNodes.h"
#include "typeChecks.h"

static FILE* file = NULL;
functionNode* funCurrent = NULL;

void translateProgramNode(program_node * program) {
  file = fopen("../Compiler/compiled.c", "w+");
	if (file != NULL)	{
	  fprintf(file,"#include <stdio.h>\n");
	  fprintf(file,"#include <stdlib.h>\n");
	  fprintf(file,"#include <string.h>\n");
	  fprintf(file,"#include \"../Utilities/linkedList.h\"\n\n");

    translateDefines(program->defines);
    fprintf(file, "\n");
    translateFunctionDefinitions(getFunctionList());
    fprintf(file, "\n");

    translateFunctions(program->functions);

	  fclose(file);
  } else
    perror("Output file error\n");
}

void translateType(variableType type) {
  switch (type) {
    case BOOLEAN: case INTEGER: fprintf(file, "int "); break;
    case STRING: fprintf(file, "char * "); break;
    case VOID: fprintf(file, "void"); break;
    case QUEUE: case STACK: fprintf(file, "listADT "); break;
  }
}

void translateDefines(defines_node * defines) {
  defines_node * next = defines;
  while(next != NULL) {
    define_node * define = next->define;
    fprintf(file, "#define %s ", define->name);
    switch (define->production) {
      case DEFINE_INTEGER: fprintf(file, "%d\n", define->value); break;
      case DEFINE_STRING: fprintf(file, "%s\n", define->string_name); break;
    }
    next = next->next;
  }
}

void translateFunctionDefinitions(functionList * functions) {
  functionList * next = functions;
  while(next != NULL) {
    functionNode * function = next->function;
    translateFunctionDefinition(function);
    fprintf(file, ";\n");
    next = next->next;
  }
}

void translateFunctionDefinition(functionNode * function) {
  translateType(function->returnType);
  fprintf(file, "%s(", function->name);
  translateParameters(function->arguments);
  fprintf(file, ")");
}

void translateParameters(variableList * arguments) {
  variableList * next = arguments;
  while(next != NULL) {
    variableNode * variable = next->variable;
    translateType(variable->type);
    fprintf(file, "%s", variable->name);
    next = next->next;
    if(next != NULL)
      fprintf(file, ", ");
  }
}

void translateFunctions(functions_node * functions) {
  functions_node * next = functions;
  while(next != NULL) {
    function_node * function = next->function;
    functionNode * fun = getFunction(function->name);
    funCurrent = fun;
    translateFunctionDefinition(fun);
    fprintf(file, " {\n");
    declareVariables(fun->variables);
    translateSentences(function->sentences);
    fprintf(file, "}\n");
    next = next->next;
  }
}

void declareVariables(variableList * variables) {
  variableList * next = variables;
  while(next != NULL) {
    variableNode * variable = next->variable;
    translateType(variable->type);
    fprintf(file, "%s;\n", variable->name);
    next = next->next;
  }
}

void translateSentences(sentences_node * sentences) {
  sentences_node * next = sentences;
  while(next != NULL) {
    sentence_node * sentence = next->sentence;
    translateSentence(sentence);
    next = next->sentences;
  }
}

void translateSentence(sentence_node * sentence) {
  switch (sentence->production) {
    case SENTENCE_IF: translateIf(sentence->if_node); break;
    case SENTENCE_FOR: translateFor(sentence->for_node); break;
    case SENTENCE_WHILE: translateWhile(sentence->while_node); break;
    case SENTENCE_FUNCTION: translateFunctionCall(sentence); break;
    case SENTENCE_VARIABLE: translateVariableOperation(sentence); break;
    case SENTENCE_RETURN: translateReturn(sentence); break;
  }
}

void translateIf(if_node * ifNode) {
  fprintf(file, "if(");
  translateCondition(ifNode->condition);
  fprintf(file, ") {\n");
  translateSentences(ifNode->sentences);
  fprintf(file, "}\n");
  translateElse(ifNode->else_branch);
}

void translateElse(if_node * elseNode) {
  if(elseNode == NULL)
    return;

  fprintf(file, "else {\n");

  if(elseNode->condition != NULL) {
    translateIf(elseNode);
  } else {
    translateSentences(elseNode->sentences);
  }
  fprintf(file, "}\n");
  translateElse(elseNode->else_branch);
}

void translateCondition(condition_node * condition) {
  switch (condition->production) {
    case CONDITION_LOGICAL: translateExpression(condition->expression_1);
                            fprintf(file, " %s ", condition->logical_operation);
                            translateExpression(condition->expression_2); break;
    case CONDITION_PARENTHESES: fprintf(file, "(");
                                translateCondition(condition->condition);
                                fprintf(file, ")"); break;
    case CONDITION_EXPRESSION: translateExpression(condition->expression_1); break;
  }
}

void translateExpression(expression_node * expression) {
  switch (expression->production) {
    case EXPRESSION_BOOLEAN: case EXPRESSION_INTEGER: fprintf(file, "%d", expression->boolean_number); break;
    case EXPRESSION_VARIABLE: fprintf(file, "%s", expression->name); break;
    case EXPRESSION_FUNCTION: translateExpressionFunctionCall(expression->function_execute); break;
    case EXPRESSION_OPERATION:translateOperation(expression); break;
  }
}

void translateOperation(expression_node * expression) {
  //TODO: DEQUEUE Y POP
  translateExpression(expression->expression_1);
  fprintf(file, " %c ", expression->op);
  translateExpression(expression->expression_2);
}

void translateExpressionFunctionCall(function_execute_node * function) {
  fprintf(file, "%s(", function->name);
  translateCallParameters(function->parameters);
  fprintf(file, ")");
}

void translateFor(for_node * forNode) {
  if(forNode->production == REGULAR_FOR) {
    translateAssignment(forNode->assignment);
    fprintf(file, "for(; ");
    translateCondition(forNode->condition);
    fprintf(file, "; ");
    translateForVariableOperation(forNode->variable_operation);
    fprintf(file, ") {\n");
    translateSentences(forNode->sentences);
    fprintf(file, "}\n");
    return;
  }

  fprintf(file, "while(!isEmpty(%s)) {\n", forNode->structure);
  //TODO: AGREGAR SENTENCIA DE DEQUEUE;
  translateSentences(forNode->sentences);
  fprintf(file, "}\n");
}

void translateForVariableOperation(variable_opration_node * variableOperation) {
  switch (variableOperation->production) {
    case VARIABLE_INCREMENT:fprintf(file, "%s++", variableOperation->increment_decrement_name);
                            break;
    case VARIABLE_DECREMENT: fprintf(file, "%s--", variableOperation->increment_decrement_name);
                            break;
  }
}

void translateWhile(while_node * whileNode) {
  fprintf(file, "while(");
  translateCondition(whileNode->condition);
  fprintf(file, ") {\n");
  translateSentences(whileNode->sentences);
  fprintf(file, "}\n");
}

void translateFunctionCall(sentence_node * sentence) {
  function_execute_node * function = sentence->function_execute;
  functionNode * fun = getFunction(function->name);
  if(sentence->sentenceEnd != ';') {
    switch (fun->returnType) {
      case STRING: fprintf(file, "printf(\"%%s\\n\", "); break;
      case BOOLEAN: case INTEGER: fprintf(file, "printf(\"%%d\\n\", "); break;
    }
  }
  fprintf(file, "%s(", function->name);
  translateCallParameters(function->parameters);
  fprintf(file, ")");
  if(sentence->sentenceEnd != ';') {
    switch (fun->returnType) {
      case STRING: case BOOLEAN: case INTEGER: fprintf(file, ")"); break;
      case QUEUE: fprintf(file, ";\n printf(\"Queue\n\")"); break;
      case STACK: fprintf(file, ";\n printf(\"Stack\n\")"); break;
    }
  }
  fprintf(file, ";\n");
}

void translateCallParameters(call_parameters_node * parameters) {
  call_parameters_node * next = parameters;
  while(next != NULL) {
    call_parameter_node * parameter = next->parameter;
    translateCallParameter(parameter);
    next = next->next;
    if(next != NULL)
      fprintf(file, ", ");
  }
}

void translateCallParameter(call_parameter_node * parameter) {
  if(parameter->production == PARAMERER_STRING) {
    fprintf(file, "%s", parameter->string);
  } else {
    translateExpression(parameter->expression);
  }
}

void translateVariableOperation(sentence_node * sentence) {
  variable_opration_node * variableOperation = sentence->variable_opration;
  switch (variableOperation->production) {
    case VARIABLE_ASSIGNMENT: translateAssignment(variableOperation->assignment); break;
    case VARIABLE_INCREMENT:fprintf(file, "%s++;\n", variableOperation->increment_decrement_name);
                            break;
    case VARIABLE_DECREMENT: fprintf(file, "%s--;\n", variableOperation->increment_decrement_name);
                            break;
  }
  translateSentenceEnd(sentence);
}

void translateAssignment(assignment_node * assignment) {
  switch (assignment->production) {
    case ASSIGNMENT_STRING: fprintf(file, "%s = malloc(strlen(%s) + 1);\n", assignment->name, assignment->string);
                            fprintf(file, "strcpy(%s, %s);\n", assignment->name, assignment->string); break;
    case ASSIGNMENT_QUEUE: case ASSIGNMENT_STACK: translateQueueStack(assignment); break;
    case ASSIGNMENT_EXPRESSION: fprintf(file, "%s %s ", assignment->name, assignment->assignment_operation);
                                translateExpression(assignment->expression);
                                fprintf(file, ";\n"); break;
  }
}

void translateQueueStack(assignment_node * assignment) {
  fprintf(file, "%s = newList();\n", assignment->name);
  translateElementList(assignment);
}

void translateElementList(assignment_node * assignment) {
  elements_node * next = assignment->queue_stack->elements;
  variableNode * variable;
  int isQueue = assignment->production == ASSIGNMENT_QUEUE;
  while(next != NULL) {
    element_node * element = next->element;
    char* aux = "";
    if(element->production == ELEMENT_BOOLEAN || element->production == ELEMENT_INTEGER)
      aux = "Int";
    if(isQueue) {
      fprintf(file, "queue%s(%s, ", aux, assignment->name);
    } else {
      fprintf(file, "push%s(%s, ", aux, assignment->name);
    }

    switch (element->production) {
      case ELEMENT_STRING: fprintf(file, "%s, strlen(%s) + 1);\n", element->string_name, element->string_name); break;
      case ELEMENT_VARIABLE: variable = getVariable(element->string_name, funCurrent);
                            if(variable->type == STRING)
                              fprintf(file, "%s, strlen(%s) + 1);\n", element->string_name, element->string_name);
                            else
                              fprintf(file, "%s, sizeof(%s));\n", element->string_name, element->string_name);
                            break;
      case ELEMENT_BOOLEAN: case ELEMENT_INTEGER: fprintf(file, "%d);\n", element->value); break;
    }
    next = next->next;
  }
}

void translateReturn(sentence_node * sentence) {
  fprintf(file, "return ");
  return_node * returnNode = sentence->return_node;
  if(returnNode->production == RETURN_STRING) {
    fprintf(file, "%s", returnNode->string);
  } else {
    translateExpression(returnNode->expression);
  }
  fprintf(file, ";\n");
}

void translateSentenceEnd(sentence_node * sentence) {
  if(sentence->sentenceEnd == ';') {
    return;
  }

  if(sentence->production == SENTENCE_VARIABLE) {
    variable_opration_node * variableOperation = sentence->variable_opration;
    char * var;
    switch (variableOperation->production) {
      case VARIABLE_ASSIGNMENT: var = variableOperation->assignment->name; break;
      case VARIABLE_INCREMENT: var = variableOperation->increment_decrement_name; break;
    }
    variableNode * variable = getVariable(var, funCurrent);

    switch (variable->type) {
      case STRING: fprintf(file, "printf(\"%%s\\n\", %s)", var); break;
      case BOOLEAN: case INTEGER: fprintf(file, "printf(\"%%d\\n\", %s)", var); break;
      case QUEUE: fprintf(file, "printf(\"Queue\n\")"); break;
      case STACK: fprintf(file, "printf(\"Stack\n\")"); break;
    }
  }
}
