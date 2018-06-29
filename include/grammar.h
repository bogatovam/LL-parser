#pragma once
#include <stdbool.h>
#include "hashtable.h"

#define e "^"

typedef struct Grammar {
	char startNterm;
	char* term;
	char* nterm;
	HashTable product;

	int countTerm;
	int countNterm;

	char** FirstSets;
	char** FollowSets;
} Grammar;

void InitGrammar(Grammar *g,char startNterm, char * term, int numTerm, char * nterm, int numNterm);
void ClearGrammar(Grammar *g);

bool IsTerm(const Grammar *g, char c);
bool IsNterm(const Grammar *g, char c);

void AddProduct(Grammar *g, char nterm, char* product);

void CreateFirstSets(Grammar *g);
void CreateFollowSets(Grammar *g);

char* FIRST(Grammar *g, char c);
char* FOLLOW(Grammar *g, char c);