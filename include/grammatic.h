#pragma once
#include <stdbool.h>
#include "hashtable.h"

#define e "^"

struct Grammatic
{
	char startNterm;
	char* term;
	char* nterm;
	struct HashTable product;

	int countTerm;
	int countNterm;

	char** FirstSets;
	char** FollowSets;
};

void InitGrammatic(struct Grammatic *g,char startNterm, char * term, int numTerm, char * nterm, int numNterm);
void InitInsert(struct HashTable* ht, TKey k);
void ClearGrammatic(struct Grammatic *g);

bool IsTerm(struct Grammatic *g, char c);
bool IsNterm(struct Grammatic *g, char c);

void AddProduct(struct Grammatic *g, char nterm, char* product);

void CreateFirstSets(struct Grammatic *g);
void CreateFollowSets(struct Grammatic *g);

char* FIRST(struct Grammatic *g, char c);
char* FOLLOW(struct Grammatic *g, char c);
