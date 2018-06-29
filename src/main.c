#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logger.h"
#include "grammar.h"
#include "parser.h"

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

	Grammar g;
	InitGrammar(&g,'S', "012", 3, "SB", 2);
	AddProduct(&g, 'S', "B2");
	AddProduct(&g, 'B', "1|0");

	char* c = g.nterm;
	for (; *c != '\0'; c++) {
		lprintf("%c: ", *c);
		lprintf("FIRST: %s\t", FIRST(&g, *c));
		lprintf("FOLLOW: %s\n", FOLLOW(&g, *c));
	}
	lprintf("\n");

	LL_Parser p;
	InitParser(&p,&g);
	PrintParserTable(&p, &g);
	Parse(&p, &g, str);
	return EXIT_SUCCESS;
}