#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct listCDT* listADT;

listADT newList();

void freeList(listADT list);

int queue(listADT list, void * element, size_t size);

int queueInt(listADT list, int element);

void * dequeue(listADT list);

int dequeueInt(listADT list);

int push(listADT list, void * element, size_t size);

int pushInt(listADT list, int element);

void * pop(listADT list);

int popInt(listADT list);

int isEmpty(listADT list);

#endif
