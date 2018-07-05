#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_file.h"
#include "find_offset.h"
#include "remove_ext.h"

void process_file(const char *path) {
	size_t path_len = strlen(path);
	char *bin_path = malloc(sizeof(*bin_path) * (path_len + strlen(".bin") + 1));
	strcpy(bin_path, path);
	strcat(bin_path, ".bin");

	char *proc_path = malloc(sizeof(*proc_path) * (path_len + strlen(".proc") + 1));
	strcpy(proc_path, path);
	strcat(proc_path, ".proc");

	FILE *bin_f = fopen(bin_path, "rb");
	free(bin_path);

	fseek(bin_f, 0, SEEK_END);
	size_t input_size = ftell(bin_f);
	fseek(bin_f, 0, SEEK_SET);
	if (input_size == 0) {
		fclose(bin_f);
		free(proc_path);
		return;
	}

	unsigned char *input_buf = malloc(sizeof(*input_buf) * input_size);
	fread(input_buf, sizeof(*input_buf), input_size, bin_f);
	fclose(bin_f);

	int i;
	for (i = 0; i < input_size-1; i++) {
		if (input_buf[i+1] == (unsigned char)0xFF) {
			break;
		}
	}
	int left_shift;
	int right_shift;
	if (i < input_size-2 && input_buf[i+2] == (unsigned char)0xFF) {
		left_shift = right_shift = 0;
		i++;
	} else {
		left_shift = find_offset(input_buf[i]);
		right_shift = 8-left_shift;
	}

	int output_size = input_size-(i+1);
	unsigned char *output_buf = malloc(sizeof(*output_buf) * output_size);

	int j;
	i += 2;
	for (j = 0; i < input_size-1; i++, j++) {
		output_buf[j] = (input_buf[i] << left_shift) | (input_buf[i+1] >> right_shift);
		if (output_buf[j-1] == '\0' && output_buf[j] == '\0') {
			break;
		}
	}

	FILE *proc_f = fopen(proc_path, "w");
	free(proc_path);
	fwrite(output_buf, sizeof(*output_buf), j-1, proc_f);

	free(input_buf);
	free(output_buf);
	fclose(proc_f);
}
