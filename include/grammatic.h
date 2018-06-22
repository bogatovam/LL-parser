#pragma once
#include <stdbool.h>
#include "hashtable.h"

struct Grammatic
{
	char startNterm;
	char* term;
	char* nterm;
	struct HashTable product;

	int countTerm;
	int countNterm;
	int countProduct;
};

void InitGrammatic(struct Grammatic *g,char startNterm, char * term, int numTerm, char * nterm, int numNterm);
void ClearGrammatic(struct Grammatic *g);

bool IsTerm(struct Grammatic *g, char c);
bool IsNterm(struct Grammatic *g, char c);

void AddProduct(struct Grammatic *g, char nterm, char* product);

