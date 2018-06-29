#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

void InitParser(LL_Parser *p, Grammar *g) {
	InitTab(&p->parserTable, g->countNterm);
	for (int i = 0; i < g->countNterm; i++) {
		HashTable* terms;
		terms =(HashTable*)calloc(1, sizeof(HashTable));
		InitTab(terms, g->countTerm + 1);
		InsRecordTab(&p->parserTable, g->nterm[i], terms);
	}
	CreateParserTable(p, g);
}

void ClearParser(LL_Parser * p) {
	ClearTable(&p->parserTable);
}

void CreateParserTable(LL_Parser *p, Grammar *g) {
	char currNterm = g->nterm[0];

	for (int numNterm = 0; numNterm < g->countNterm; numNterm++, currNterm = g->nterm[numNterm]) {
		char** tmp = (char**)FindRecordTab(&g->product, currNterm);
		if (tmp != NULL) {
			int count = g->product.recs[g->product.currPos]->countValues;
			for (int k = 0; k < count; k++) {
				char* first = FIRST(g, tmp[k][0]);
				while (*first != '\0') {
					HashTable* strTerm=*FindRecordTab(&p->parserTable, currNterm);
					
					InsRecordTab(strTerm, *first, tmp[k]);

					if (*first == *e) {
						char* follow = FOLLOW(g, currNterm);
						while (*follow != '\0') {int count_ht = strTerm->recs[strTerm->currPos]->countValues;
							InsRecordTab(strTerm, *follow, tmp[k]);
							follow++;
						}
					}
					first++;
				}
			}
		}
	}
}

bool Parse(LL_Parser *p, Grammar *g,char* str) {
	lprintf("The string to be parsed: %s \n", str);
	struct Stack st;
	InitStack(&st);
	Push(&st, "$");
	Push(&st, &g->startNterm);
	char* stream = str;
	stream[strlen(str)] = '$';

	while (!IsStackEmpty(&st)) {
		char a = *stream;
		char x = *(char*)Top(&st);
		if ( x == a) {
			Pop(&st);
			stream++;
		}
		else if (IsTerm(g, x)) {
			lprintf("Error! Ñharacter expected: %ñ \n", x);
			return false;
		}
		else {
			HashTable* ht = *FindRecordTab(&p->parserTable, x);
			char** tmp = (char**)FindRecordTab(ht, a);
			if (tmp == NULL) {
				lprintf("Error! String not derived from grammar\n");
				return false;
			}
			else {
				Pop(&st);
				for (int i = strlen(tmp[0]) - 1; i > -1; i--) {
					if (tmp[0][i] != *e) Push(&st, &tmp[0][i]);
				}
			}
		}
	}
	stream[strlen(str)] = '\0';

	lprintf("Success! String derived from the grammar \n");
	return true;
}

void PrintParserTable(LL_Parser * p, Grammar * g)
{
	lprintf("---------------------\n");
	lprintf("-----Parser Table----\n");
	lprintf("---------------------\n\n");

	if (IsTabEmpty(&p->parserTable)) CreateParserTable(p, g);

	char* c = g->nterm;
	lprintf("\t");
	for (int j = 0; j < g->countTerm; j++)
		lprintf("%c\t ", g->term[j]);
	lprintf("$\t ");
	lprintf("\n");

	for (; *c != '\0'; c++) {
		lprintf("%c\t", *c);
		HashTable* ht = *FindRecordTab(&p->parserTable, *c);
		for (int j = 0; j < g->countTerm; j++) {
			char** tmp = (char**)FindRecordTab(ht, g->term[j]);

			if (tmp != NULL) {
				int count_ht = ht->recs[ht->currPos]->countValues;
				if (count_ht > 1)
				{
					lprintf("This grammar is not ll(1)!\n");
					exit(EXIT_FAILURE);
				}
				lprintf("%c->%s\t", *c, tmp[0]);
			}
			else lprintf("err\t");
		}
		char** tmp = (char**)FindRecordTab(ht, '$');
		if (tmp != NULL) {
			lprintf("$->%s\t", tmp[0]);
		}
		else lprintf("err\t");
		lprintf("\n");
	}
	lprintf("\n\n");
}
