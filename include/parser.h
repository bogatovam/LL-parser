#pragma once
#include "grammatic.h"
#include "hashtable.h"
#include "stack.h"

struct LL_Parser
{
	struct HashTable parserTable;
};

void InitParser(struct LL_Parser *p, struct Grammatic *g);

void CreateParserTable(struct LL_Parser *p, struct Grammatic *g);
bool Parse(struct LL_Parser *p, char* str);
