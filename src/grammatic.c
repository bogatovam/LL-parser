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
	g->FollowSets = NULL;
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
	char** waiting = calloc(2, sizeof(char*));
	bool* ready = calloc(g->countNterm, sizeof(bool));

	waiting[0] = calloc(g->countNterm*g->countNterm + g->countNterm, sizeof(char));
	waiting[1] = calloc(g->countNterm*g->countNterm + g->countNterm, sizeof(char));

	int j = 0;	char currNterm = g->nterm[0];

	for (int numNterm = 0; numNterm < g->countNterm; numNterm++, currNterm = g->nterm[numNterm]) {
		char** tmp = FindRecordTab(&g->product, currNterm);
		g->FirstSets[g->product.currPos] = calloc(g->countTerm*g->countNterm, sizeof(char)); //!!!
		g->FirstSets[g->product.currPos][0] = '\0';

		if (tmp != NULL) {
			int count = g->product.recs[g->product.currPos]->countValues;

			//ѕроход по продукци€м нетерминала
			int oldIndex = j; 
			for (int k = 0; k < count; k++) {
				char c_proc = tmp[k][0];

				if (IsNterm(g, c_proc)) {
					waiting[0][j] = currNterm;
					waiting[1][j++] = c_proc;
				}
				else strncat(g->FirstSets[g->product.currPos], &c_proc, 1);
			}
			if (j == oldIndex) ready[g->product.currPos] = true;
			else ready[g->product.currPos] = false;
		}
	}
	int i = 0, countReady = 0;
	while (1) {
		if (waiting[0][i] != -1) {
			FindRecordTab(&g->product, waiting[1][i]);
			int currPos = g->product.currPos;
			if (ready[currPos]) {
				FindRecordTab(&g->product, waiting[0][i]);
				strcat(g->FirstSets[g->product.currPos], g->FirstSets[currPos]);

				char tmp = waiting[0][i];
				
				waiting[0][i] = -1;

				if (strchr(waiting[0], tmp) == NULL)
					ready[g->product.currPos] = true;

				countReady++;
			}
		}
		if (countReady == j) break;
		i = (i + 1) % j;
	}
}

void CreateFollowSets(struct Grammatic *g) {
	bool flag = true; //были изменени€ на этой итерации
	bool* changed = calloc(g->countNterm, sizeof(bool));
	char** waiting = calloc(2, sizeof(char*));

	waiting[0] = calloc(g->countNterm*g->countNterm + g->countNterm, sizeof(char));
	waiting[1] = calloc(g->countNterm*g->countNterm + g->countNterm, sizeof(char));

	for (int i = 0; i < g->countNterm; i++) {
		changed[i] = false;
		g->FollowSets[i] = calloc(g->countTerm*g->countNterm + 888, sizeof(char));
		g->FollowSets[i][0]= '\0';
	}

	FindRecordTab(&g->product, g->startNterm);
	strcat(g->FollowSets[g->product.currPos], "$");
	//ƒобавить необходимые множества ферст в фаллов терминалов
	char currNterm = g->nterm[0];
	int j = 0;

	for (int numNterm = 0; numNterm < g->countNterm; numNterm++, currNterm = g->nterm[numNterm]) {
		char** tmp = FindRecordTab(&g->product, currNterm);
		if (tmp != NULL) {
			int count = g->product.recs[g->product.currPos]->countValues;
			for (int k = 0; k < count; k++) {
				char* c_proc = tmp[k];
				while (*c_proc != '\0') {
					if (IsNterm(g, *c_proc))
						if (*(c_proc + 1) != '\0') {
							FindRecordTab(&g->product, *c_proc);
							int tmpPos = g->product.currPos;
							strcat(g->FollowSets[tmpPos], FIRST(g, *(c_proc + 1)));
							changed[tmpPos] = true;
						}
						else if (*c_proc != currNterm) {
							waiting[0][j] = *c_proc;
							waiting[1][j++] = currNterm;
						}
						c_proc++;
				}
			}
		}
	}



	while (flag) {
		flag = false;
		int i = 0, countReady = 0;
		while (1) {
			FindRecordTab(&g->product, waiting[1][i]);
			if (changed[g->product.currPos]) {
				char* from = g->FollowSets[g->product.currPos];

				FindRecordTab(&g->product, waiting[0][i]);
				char* to = g->FollowSets[g->product.currPos];

				while (*from != '\0') {
					if (strchr(to, *from) == NULL) {
						strncat(to, from, 1);
						changed[g->product.currPos] = true;
						flag = true;
					}
					from++;
				}

				countReady++;
			}

			if (countReady == j) break;
			i = (i + 1) % j;
		}
	}
}

char* FIRST(struct Grammatic *g, char c) {
	if (IsNterm(g, c)) {
		if (g->FirstSets == NULL) {
			g->FirstSets = calloc(g->countNterm, sizeof(char*));
			CreateFirstSets(g);
		}
		FindRecordTab(&g->product, c);
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
		FindRecordTab(&g->product, c);
		return g->FollowSets[g->product.currPos];
	}
}