#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void InitParser(struct LL_Parser *p, struct Grammatic *g) {
	InitTab(&p->parserTable, g->countNterm);
	for (int i = 0; i < g->countNterm; i++) {
		struct HashTable* terms;
		terms =(struct HashTable*)calloc(1, sizeof(struct HashTable));
		InitTab(terms, g->countTerm + 1);
		InsRecordTab(&p->parserTable, g->nterm[i], terms);
	}
	CreateParserTable(p, g);
}

void CreateParserTable(struct LL_Parser *p, struct Grammatic *g) {
	char currNterm = g->nterm[0];

	for (int numNterm = 0; numNterm < g->countNterm; numNterm++, currNterm = g->nterm[numNterm]) {
		char** tmp = (char**)FindRecordTab(&g->product, currNterm);
		if (tmp != NULL) {
			int count = g->product.recs[g->product.currPos]->countValues;
			for (int k = 0; k < count; k++) {
				char* first = FIRST(g, tmp[k][0]);
				while (*first != '\0') {
					struct HashTable* strTerm=*FindRecordTab(&p->parserTable, currNterm);
					InsRecordTab(strTerm, *first, tmp[k]);

					if (*first == *e) {
						char* follow = FOLLOW(g, currNterm);
						while (*follow != '\0') {
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

bool Parse(struct LL_Parser *p, struct Grammatic *g, const char* str) {
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
			printf("Error!\n");
			break;
		}
		else {
			struct HashTable* ht = *FindRecordTab(&p->parserTable, x);
			char** tmp = FindRecordTab(ht, a);
			if (tmp == NULL) {
				printf("Error!\n");
				break;
			}
			else {
				Pop(&st);
				for (int i = strlen(tmp[0]) - 1; i > -1; i--) {
					if (tmp[0][i] != *e) Push(&st, &tmp[0][i]);
				}
			}
		}
	}
	return false;
}
