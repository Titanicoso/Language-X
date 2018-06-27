#ifndef TYPECHECKS_H
#define TYPECHECKS_H

#define TYPE_DEFINED 2
#define INCOMPATIBLE_DEFINITION 1

typedef enum {FUNCTION_REPETITION_ERROR} errorType;

typedef enum {BOOLEAN, STRING, QUEUE, STACK, INTEGER, VOID, UNKNOWN} variableType;

typedef struct {
  char * name;
  variableType type;
  variableType elementType;
} variableNode;

typedef struct variableList {
  struct variableList * next;
  variableNode * variable;
} variableList;

typedef struct {
  char * name;
  variableList * variables;
  variableList * arguments;
  variableType returnType;
} functionNode;

typedef struct functionList {
  struct functionList * next;
  functionNode * function;
} functionList;

functionList * getFunctionList();

int functionExists(char * name);

void createFunction(char * name);

void addToList(functionNode * function, functionList * list);

int addReturn(variableType type);

int addParameterToFunction(char * name);

void addToDefines(char * name, variableType type);

int existsDefine(char * name);

variableNode * createVariable(char * name, variableType type, variableType elementType);

int addVariable(char * name, variableType type);

int parameterExists(char * name);

functionNode * getFunction(char * name);

variableNode * getVariable(char * name, functionNode * function);

variableNode * getVariableFromList(char * name, variableList * list);

int existsVariableTyped(char * name, variableType type);

int existsVariable(char * name);

#endif
