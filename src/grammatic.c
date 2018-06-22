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
	g->countProduct = 0;

	InitTab(&g->product, numNterm);
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
	//char* arrTmp = NULL;
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
