#pragma once
#include <stdbool.h>

struct Link
{
	void* value;
	struct Link* next;
};
struct Stack
{
	struct Link* first;
	int dataCount; 
};

void InitStack(struct Stack* st);
bool IsStackEmpty(struct Stack* st);

void Push(struct Stack* st, void* val);
void* Pop(struct Stack* st);
void* Top(struct Stack* st);


