#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void InitStack(struct Stack* st) {
	st->first = NULL;
	st->dataCount = 0;
}

bool IsStackEmpty(struct Stack* st) {
	return st->dataCount == 0;
}

void Push(struct Stack* st, void* val) {
	struct Link* tmp = calloc(1, sizeof(struct Link));

	tmp->value = val; 
	tmp->next = st->first;
	st->first = tmp;

	st->dataCount++;
}
void* Pop(struct Stack* st) {
	if (!IsStackEmpty(st)) {
		struct Link* tmp = st->first;

		void* tmpVal = st->first->value;
		st->first = st->first->next;
		st->dataCount--;

		free(tmp);
		return tmpVal;
	}
	return NULL;
}
void* Top(struct Stack* st) {
	if (!IsStackEmpty(st)) {
		return st->first->value;
	}
	return NULL;
}