#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void InitStack(Stack* st) {
	st->first = NULL;
	st->dataCount = 0;
}

bool IsStackEmpty(Stack* st) {
	return st->dataCount == 0;
}

void Push(Stack* st, void* val) {
	Link* tmp = calloc(1, sizeof(struct Link));

	tmp->value = val; 
	tmp->next = st->first;
	st->first = tmp;

	st->dataCount++;
}

void* Pop(Stack* st) {
	if (!IsStackEmpty(st)) {
		Link* tmp = st->first;

		void* tmpVal = st->first->value;
		st->first = st->first->next;
		st->dataCount--;

		free(tmp);
		return tmpVal;
	}
	return NULL;
}

void* Top(Stack* st) {
	if (!IsStackEmpty(st)) {
		return st->first->value;
	}
	return NULL;
}