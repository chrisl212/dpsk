#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "find_unprocessed_files.h"
#include "process_file.h"

int main(int argc, char **argv) {
	pid_t pid = fork(); //fork process to background

	if (pid > 0) {
		//exit from main process
		return EXIT_SUCCESS;
	}

	char *log_path = "./log.txt"; //default log file is in current directory
	char *directory = "./"; //default directory is current directory
	
	for (int i = 1; i < argc; i++) {
		char *arg = argv[i]; //parse each option
		if (arg[0] == '-') {
			switch (arg[1]) {
				case 'd':
					//directory path option
					directory = argv[++i];
					break;
				case 'l':
					//log file path option
					log_path = argv[++i];
					break;
				default:
					printf("Unkown option %s\n", arg);
			}
		}
	}

	freopen(log_path, "w", stdout); //reopens stdout as the log file
	char **results = NULL; //initializes the results to NULL
	while (1) {
		results = find_unprocessed_files(directory); //finds the unprocessed .bin files in the directory 
		char **temp = results;
		while (*temp) {
			//loops through unprocessed files and processes them
			printf("Found unprocessed file: %s\n", *temp);
			process_file(*temp);
			free(*temp);
			temp++;
			fflush(stdout);
		}
		free(results);
		sleep(1);
	}
	
	fclose(stdout);
	return EXIT_SUCCESS;
}
