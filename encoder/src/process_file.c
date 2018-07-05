#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_file.h"
#include "remove_ext.h"

void process_file(const char *path) {
	size_t path_len = strlen(path);
	char *bin_path = malloc(sizeof(*bin_path) * (path_len + strlen(".bin") + 1));
	strcpy(bin_path, path);
	strcat(bin_path, ".bin");

	char *msg_path = malloc(sizeof(*msg_path) * (path_len + strlen(".msg") + 1));
	strcpy(msg_path, path);
	strcat(msg_path, ".msg");

	FILE *bin_f = fopen(bin_path, "rb");
	free(bin_path);

	fseek(bin_f, 0, SEEK_END);
	size_t input_size = ftell(bin_f);
	fseek(bin_f, 0, SEEK_SET);
	if (input_size == 0) {
		fclose(bin_f);
		free(msg_path);
		return;
	}

	unsigned char *input_buf = malloc(sizeof(*input_buf) * input_size);
	fread(input_buf, sizeof(*input_buf), input_size, bin_f);
	fclose(bin_f);

	unsigned char *output_buf = malloc(sizeof(*output_buf) * (input_size+4));
	for (int i = 0; i < input_size+4; i++) {
		if (i <= 1) {
			output_buf[i] = 0xFF;
		} else if (i > input_size+1) {
			output_buf[i] = 0x00;
		} else {
			output_buf[i] = input_buf[i-2];
		}
	}

	FILE *msg_f = fopen(msg_path, "w");
	free(msg_path);
	fwrite(output_buf, sizeof(*output_buf), input_size+4, msg_f);

	free(input_buf);
	free(output_buf);
	fclose(msg_f);
}
