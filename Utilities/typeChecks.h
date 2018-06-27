#ifndef TYPECHECKS_H
#define TYPECHECKS_H

typedef enum {BOOLEAN, STRING, QUEUE, STACK, INTEGER, VOID} variableType;

typedef struct {
  char * name;
  variableType type;
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

functionNode * getFunction(char * name);

variableNode * getVariable(char * name, functionNode * function);

#endif
