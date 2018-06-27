#ifndef TRANSLATENODES_H
#define TRANSLATENODES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Compiler/nodes.h"
#include "translateNodes.h"
#include "typeChecks.h"

FILE * test();

void translateProgramNode(program_node * program);

void translateType(variableType type);

void translateDefines(defines_node * defines);

void translateFunctionDefinitions(functionList * functions);

void translateFunctionDefinition(functionNode * function);

void translateParameters(variableList * arguments);

void translateFunctions(functions_node * functions);

void declareVariables(variableList * variables);

void translateSentences(sentences_node * sentences);

void translateSentence(sentence_node * sentence);

void translateIf(if_node * ifNode);

void translateElse(if_node * elseNode);

void translateCondition(condition_node * condition);

void translateExpression(expression_node * expression);

void translateOperation(expression_node * expression);

void translateExpressionFunctionCall(function_execute_node * function);

void translateFor(for_node * forNode);

void translateForVariableOperation(variable_opration_node * variableOperation);

void translateWhile(while_node * whileNode);

void translateFunctionCall(sentence_node * sentence);

void translateCallParameters(call_parameters_node * parameters);

void translateCallParameter(call_parameter_node * parameter);

void translateVariableOperation(sentence_node * sentence);

void translateAssignment(assignment_node * assignment);

void translateQueueStack(assignment_node * assignment);

void translateElementList(assignment_node * assignment);

void translateReturn(sentence_node * sentence);

void translateSentenceEnd(sentence_node * sentence);

#endif