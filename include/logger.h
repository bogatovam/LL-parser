#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

void lprintf(const char *format, ...);

void lprintf(const char *format, ...)
{
	static first = true;
	FILE *file;
	if (file = fopen("log.txt", first ? "w" : "a")) {
		first = false;
		va_list args;
		va_start(args, format);
		vfprintf(file, format, args);
		va_end(args);
		fclose(file);
	}
}