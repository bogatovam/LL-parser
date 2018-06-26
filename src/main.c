#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "grammatic.h"

#define MAX_STRING_LENGTH 10

int main(int argc, char *argv[]) {
	char* str =(char*)calloc(MAX_STRING_LENGTH, sizeof(char));
	
	if (argc == 1) {
		printf("Enter a string for parse: \n");
		scanf("%s", str);
	}
	else if (argc == 2) {
		str = argv[1];
	} 
	else {
		printf("Incorrect arguments!\n");
		exit(EXIT_FAILURE);
	}

	struct Grammatic g;
	//InitGrammatic(&g,'S', "012'", 4, "SB", 2);
	//AddProduct(&g, 'S', "B2");
	//AddProduct(&g, 'B', "1|2");

	InitGrammatic(&g,'E', "+*()i", 5, "EKTMF", 5);
	AddProduct(&g, 'E', "TK");
	AddProduct(&g, 'K', "+TK");
	AddProduct(&g, 'T', "FM");
	AddProduct(&g, 'M', "*FM");
	AddProduct(&g, 'F', "(E)|i");
	char* c = g.nterm;
	for (; *c != '\0'; c++) {
		printf("%c ", *c);
		printf("FOLLOW: %s,\t", FOLLOW(&g, *c));
		printf("FIRST: %s\n", FIRST(&g, *c));
	}
	return EXIT_SUCCESS;
}