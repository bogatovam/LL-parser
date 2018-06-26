#include "parser.h"

void InitParser(struct LL_Parser *p, struct Grammatic *g) {
	for (int i = 0; i < g->countNterm; i++)
		InitInsert(&p->parserTable, g->nterm[i]);
}

void CreateParserTable(struct LL_Parser *p, struct Grammatic *g) {

}

bool Parse(struct LL_Parser *p, char* str) {

}
