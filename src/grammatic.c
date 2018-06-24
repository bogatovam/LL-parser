#define _CRT_SECURE_NO_WARNINGS

#include "grammatic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitGrammatic(struct Grammatic *g, char startNterm, char * term, int numTerm, char * nterm, int numNterm)
{
	g->term = term;
	g->nterm = nterm;
	g->startNterm = startNterm;

	g->countNterm = numNterm;
	g->countTerm = numTerm;

	//лучше проинициализировать сразу всеми нетерминалами - поиск несуществующих будет предсказуемым
	InitTab(&g->product, numNterm);
	for (int i = 0; i < g->countNterm; i++)
		InitInsert(&g->product, nterm[i]);

	g->FirstSets = NULL;
	g->FollowSets = calloc(numNterm, sizeof(char*));
}

bool IsTerm(struct Grammatic *g, char c)
{
	for (int i = 0; i < g->countTerm; i++)
		if (g->term[i] == c) return true;
	return false;
}

bool IsNterm(struct Grammatic *g, char c)
{
	for (int i = 0; i < g->countNterm; i++)
		if (g->nterm[i] == c) return true;
	return false;
}

void AddProduct(struct Grammatic *g, char nterm, char * product)
{
	int i = 0, prevI = 0;
	for (i = 0; product[i] != '\0'; i++) {
		if (product[i + 1] == '|' || product[i + 1] == '\0') {
			int size = i - prevI + 1;
			char* arrTmp = (char*)calloc(size, sizeof(char));

			strncpy(arrTmp, product + prevI, size);
			arrTmp[size] = '\0';
			InsRecordTab(&g->product, nterm, arrTmp);
			prevI = i + 2;
		}
	}
}

void CreateFirstSets(struct Grammatic *g) {
	//Ќет проверки на порождение "пустого" терминала
	char* c;
	for (c = g->nterm; *c != '\0'; c++) {
		char** tmp = FindRecordTab(&g->product, *c);
		int count = g->product.recs[g->product.currPos]->countValues;

		if (tmp == NULL) continue;

		char* res = calloc(count + 1, sizeof(char));
		for (int i = 0; i < count; i++)
			res[i] = tmp[i][0];
		res[count] = '\0';
		g->FirstSets[g->product.currPos] = res;
	}
}

void CreateFollowSets(struct Grammatic *g) {

	char*** tmpFollowSet; //ћассив  где дл€ каждого нетерминала содержитс€ массив указателей
	char c = g->startNterm;

	tmpFollowSet = calloc(g->countNterm, sizeof(char*));
	for (int i = 0; i < g->countNterm; i++) {
		tmpFollowSet[i] = calloc(g->countNterm * 2 + 1, sizeof(char));
		g->FollowSets[i] = calloc(g->countTerm, sizeof(char));
	}

	FindRecordTab(&g->product, c);
	strcat(g->FollowSets[g->product.currPos],"$");

	//ѕроход по нетерmиналам
	for (int i = 0; i < g->countNterm; c = g->nterm[i], i++) {
		char** tmp = FindRecordTab(&g->product, c);

		if (tmp != NULL) {
			int count = g->product.recs[g->product.currPos]->countValues;
			//ѕроход по продукци€м нетерминала
			for (int k = 0; k < count; k++) {
				char* c_proc = tmp[i][k];
				while (*c_proc != '\0') {
					if (IsNterm(g, c_proc)) {
						if (*(c_proc + 1) == '\0') {
							int j = 0;
							FindRecordTab(&g->product, *c_proc);
							while (tmp[g->product.currPos][j] != NULL) j++;
							tmp[g->product.currPos][j] = g->FollowSets[g->product.currPos];
						} else {
							int j = 0;
							FindRecordTab(&g->product, *c_proc);
							while (tmp[g->product.currPos][j] != NULL) j++;
							tmp[g->product.currPos][j] = FIRST(g, *(c_proc + 1));
						}
					}
					c_proc++;
				}
			}
		}
	}

	free(tmpFollowSet);
}

char* FIRST(struct Grammatic *g, char c) {
	if (IsNterm(g, c)) {
		if (g->FirstSets == NULL) {
			g->FirstSets = calloc(g->countNterm, sizeof(char*));
			CreateFirstSets(g);
		}
		char** tmp = FindRecordTab(&g->product, c);

		if (tmp == NULL) 
			return NULL;
		else 
			return g->FirstSets[g->product.currPos];
	}
	else if (IsTerm(g, c)) {
		char res[2];
		res[0] = c; res[1] = '\0';
		return res;
	}
}

char* FOLLOW(struct Grammatic *g, char c) {
	if (IsNterm(g, c)) {
		if (g->FollowSets == NULL) {
			g->FollowSets = calloc(g->countNterm, sizeof(char*));
			CreateFollowSets(g);
		}
		char** tmp = FindRecordTab(&g->product, c);

		if (tmp == NULL)
			return NULL;
		else
			return g->FollowSets[g->product.currPos];
	}
}