#include "remove_ext.h"
#include <string.h>

char *remove_ext(const char *fname, const char *ext) {
	char *no_ext = strdup(fname);
	char *dot = strstr(no_ext, ext);
	*dot = '\0';
	return no_ext;
}
