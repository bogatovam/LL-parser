#pragma once
#include <stdbool.h>

typedef struct Link {
	void* value;
	struct Link* next;
} Link;

typedef struct Stack {
	Link* first;
	int dataCount; 
} Stack;

void InitStack(Stack* st);
bool IsStackEmpty(Stack* st);

void Push(Stack* st, void* val);
void* Pop(Stack* st);
void* Top(Stack* st);
