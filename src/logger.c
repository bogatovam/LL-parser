#define _CRT_SECURE_NO_WARNINGS

#include "logger.h"

void lprintf(const char * format, ...) {
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
