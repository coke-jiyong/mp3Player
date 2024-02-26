
#include "string_tools.h"

int read_line (FILE*fp, char str[], int limit) {
	int ch, i = 0;
	while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
		if (i < limit - 1) {
			str[i++] = ch;
		}
	}
	str[i] = '\0';
	return i;
}

