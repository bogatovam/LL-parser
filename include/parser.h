#pragma once
#include "grammar.h"
#include "hashtable.h"
#include "stack.h"

typedef struct LL_Parser {
	HashTable parserTable;
} LL_Parser;

void InitParser(LL_Parser *p, Grammar *g);
void ClearParser(LL_Parser *p);

void CreateParserTable(LL_Parser *p, Grammar *g);
bool Parse(LL_Parser *p,Grammar *g, char* str);

void PrintParserTable(LL_Parser *p, Grammar *g);
