#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Compiler/nodes.h"
#include "typeChecks.h"

functionList * functions = NULL;
variableList * defines;
functionNode * current = NULL;

functionList * getFunctionList() {
  return functions;
}

int functionExists(char * name) {
  return getFunction(name) != NULL;
}

functionNode * createFunction() {
  functionNode * function = malloc(sizeof(functionNode));
	function->returnType = UNKNOWN;
	function->arguments = NULL;
	function->variables = NULL;
  current = function;
  return function;
}

int renameCurrent(char * name) {
  if(functionExists(name)) {
		return 0;
	}
  current->name = malloc(strlen(name) + 1);
  strcpy(current->name, name);

  if(strcmp(name, "main") == 0) {
    current->returnType = INTEGER;
  }
  addToList(current, &functions);
  current = createFunction();
  return 1;
}

void addToList(functionNode * function, functionList ** list) {
  functionList * node = malloc(sizeof(functionList));
  node->function = function;
  node->next = *list;
	*list = node;
}

int addReturn(variableType type) {
  if(current == NULL) {
    current = createFunction();
  }
  if(current->returnType != UNKNOWN && current->returnType != type)
    return 0;
  current->returnType = type;
  return 1;
}

int addParameterToFunction(char * name) {
  if(current == NULL) {
    current = createFunction();
  }
  if(parameterExists(name))
    return 0;

  variableList * node = malloc(sizeof(variableList));
  node->variable = createVariable(name, UNKNOWN, UNKNOWN);
  node->next = current->arguments;
  current->arguments = node;

  return 1;
}

void addToDefines(char * name, variableType type) {
  variableList * node = malloc(sizeof(variableList));
  node->variable = createVariable(name, type, VOID);
  node->next = defines;
  defines = node;
}

int existsDefine(char * name) {
  if(current == NULL) {
    current = createFunction();
  }
  variableNode * ret = getVariableFromList(name, defines);
  if(ret != NULL)
    return 1;
}

variableNode * createVariable(char * name, variableType type, variableType elementType) {
  variableNode * variable = malloc(sizeof(variableNode));
  variable->name = malloc(strlen(name) + 1);
	strcpy(variable->name, name);
	variable->type = type;
  variable->elementType = elementType;

  return variable;
}

int addVariable(char * name, variableType type) {
  if(current == NULL) {
    current = createFunction();
  }
  int exists = existsVariableTyped(name, type);
  if(exists == TYPE_DEFINED) {
    return 1;
  } else if(exists == INCOMPATIBLE_DEFINITION)
    return 0;

  variableList * node = malloc(sizeof(variableList));
  node->variable = createVariable(name, type, UNKNOWN);
  node->next = current->variables;
  current->variables = node;

  return 1;
}

int parameterExists(char * name) {
  variableNode * ret = getVariableFromList(name, defines);
  if(ret != NULL)
    return 1;
  ret = getVariableFromList(name, current->arguments);
  return ret != NULL;
}

functionNode * getFunction(char * name) {
  functionList * next = functions;
  while(next != NULL) {
    functionNode * function = next->function;
    if(strcmp(function->name, name) == 0)
      return function;
    next = next->next;
  }
  return NULL;
}

variableNode * getVariable(char * name, functionNode * function) {
  variableNode * ret = getVariableFromList(name, defines);
  if(ret != NULL)
    return ret;
  ret = getVariableFromList(name, function->arguments);
  if(ret != NULL)
    return ret;
  ret = getVariableFromList(name, function->variables);
  return ret;
}

variableNode * getVariableFromList(char * name, variableList * list) {
  variableList * next = list;
  while(next != NULL) {
    variableNode * variable = next->variable;
    if(strcmp(variable->name, name) == 0)
      return variable;
    next = next->next;
  }
  return NULL;
}

int existsVariableTyped(char * name, variableType type) {
  variableNode * ret = getVariableFromList(name, current->arguments);
  if(ret != NULL) {
    if(ret->type == UNKNOWN || ret->type == type) {
      ret->type = type;
      return TYPE_DEFINED;
    }
    return INCOMPATIBLE_DEFINITION;
  }
  ret = getVariableFromList(name, current->variables);
  if(ret != NULL) {
    if(ret->type == UNKNOWN || ret->type == type) {
      ret->type = type;
      return TYPE_DEFINED;
    }
    return INCOMPATIBLE_DEFINITION;
  }
  return 0;
}

int isIterable(char * name) {
  variableNode * ret = getVariableFromList(name, current->arguments);
  if(ret != NULL) {
    if(ret->type == QUEUE || ret->type == STACK) {
      return 1;
    }
    return 0;
  }
  ret = getVariableFromList(name, current->variables);
  if(ret != NULL) {
    if(ret->type == QUEUE || ret->type == STACK) {
      return 1;
    }
    return 0;
  }
  return 0;
}

int existsVariable(char * name) {
  if(current == NULL) {
    current = createFunction();
  }
  variableNode * ret = getVariableFromList(name, current->arguments);
  if(ret != NULL)
    return 1;
  ret = getVariableFromList(name, current->variables);
  return ret != NULL;
}
