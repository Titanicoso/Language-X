#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

typedef struct listNodeT{
    void * value;
    size_t size;
    struct listNodeT * next;
} listNodeT;

typedef struct listCDT{
    listNodeT * first;
    listNodeT * last;
} listCDT;


listNodeT * createNode(void * element, size_t size);

listADT newList() {
  return calloc(1, sizeof(listCDT));
}

void freeList(listADT list) {
    if(list == NULL)
        return;

    listNodeT * node = list->first;
    listNodeT * aux = NULL;

    while(node != NULL) {
      free(node->value);
      aux = node;
      node = node->next;
      free(aux);
    }
    free(list);
}

int queueInt(listADT list, int element) {
  int aux = element;
  int * ptr = &aux;
  return queue(list, ptr, sizeof(int));
}

int queue(listADT list, void * element, size_t size) {
  if(list == NULL) {
    return 0;
  }

  listNodeT * node = createNode(element, size);
  if(node == NULL) {
    return 0;
  }

  if(list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    list->last = node;
  }

  return 1;
}

void * dequeue(listADT list) {
  void * element;
  listNodeT * node;

  if(list == NULL || list->first == NULL) {
    return NULL;
  }

  node = list->first;
  element = malloc(node->size);
  if(element == NULL) {
    return NULL;
  }

  memcpy(element, node->value, node->size);

  if(list->last == node) {
    list->last = NULL;
  }

  list->first = node->next;
  free(node->value);
  free(node);

  return element;
}

int dequeueInt(listADT list) {
  int* aux = (int *) dequeue(list);
  return *aux;
}

int push(listADT list, void * element, size_t size) {
  if(list == NULL) {
    return 0;
  }

  listNodeT * node = createNode(element, size);
  if(node == NULL) {
    return 0;
  }

  if(list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first = node;
  }

  return 1;
}

int pushInt(listADT list, int element) {
  int aux = element;
  int * ptr = &aux;
  return push(list, ptr, sizeof(int));
}

void * pop(listADT list) {
  return dequeue(list);
}

int popInt(listADT list) {
  return dequeueInt(list);
}

listNodeT * createNode(void * element, size_t size) {
  listNodeT * node;
  void * value;

  if(element == NULL)
    return NULL;

  node = malloc(sizeof(listNodeT));
  if(node == NULL)
    return NULL;

  value = malloc(size);
  if(value == NULL)
    return NULL;

  memcpy(value, element, size);
  node->value = value;
  node->size = size;
  node->next = NULL;

  return node;
}

int isEmpty(listADT list) {
  return list == NULL || list->first == NULL;
}
