#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "remove_ext.h"

char **find_unprocessed_files(const char *dir) {
	DIR *d = opendir(dir);
	size_t dirlen = strlen(dir);
	struct dirent *entry = NULL;

	int cnt = 1;
	int bin_cnt = 1;
	int proc_cnt = 1;
	size_t len = -1;
	char *path = NULL;
	char **bins = calloc(1, sizeof(*bins) * bin_cnt);
	char **procs = calloc(1, sizeof(*procs) * proc_cnt);
	char **paths = calloc(1, sizeof(*paths) * cnt);
	
	while ((entry = readdir(d))) {
		const char *fname = entry->d_name;
		if (strstr(fname, ".bin")) {
			bin_cnt++;
			char *bin_name = remove_ext(fname, ".bin");
			bins = realloc(bins, bin_cnt * sizeof(*bins));

			bins[bin_cnt-1] = NULL;
			bins[bin_cnt-2] = bin_name; 
		} else if (strstr(fname, ".proc")) {
			proc_cnt++;
			char *proc_name = remove_ext(fname, ".proc");
			procs = realloc(procs, proc_cnt * sizeof(*procs));

			procs[proc_cnt-1] = NULL;
			procs[proc_cnt-2] = proc_name;
		}
	}
	
	for (int i = 0; bins[i]; i++) {
		char *bin_name = bins[i];
		for (int j = 0; procs[j]; j++) {
			char *proc_name = procs[j];
			if (strcmp(bin_name, proc_name) == 0) {
				goto cont;
			}
		}
		cnt++;
		paths = realloc(paths, cnt * sizeof(*paths));

		len = strlen(bin_name) + dirlen + strlen("/.proc") + 1;
		path = malloc(sizeof(*path) * len);
		sprintf(path, "%s/%s", dir, bin_name);
		paths[cnt-1] = NULL;
		paths[cnt-2] = path;
		cont: 
			free(bins[i]);
			continue;
	}
	for (int i = 0; procs[i]; i++) {
		free(procs[i]);
	}

	free(bins);
	free(procs);
	closedir(d);
	return paths;
}
