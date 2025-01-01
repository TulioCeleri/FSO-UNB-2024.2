#include <time.h> 
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	char script_source_line[255], argument[255];
	float real_time, total_time = 0.0;
	struct timespec start, stop;
	int return_value;
	while (scanf("%s %s", script_source_line, argument) != EOF) {
		fflush(stdout);
		timespec_get(&start, TIME_UTC);
		if (!fork()) {
			execl(script_source_line, script_source_line, argument, (char*) NULL);
			printf("> Erro: %s\n", strerror(errno));
			return_value = errno;
			fclose(stdin);
			exit(errno);
		} else {
		wait(&return_value);
		timespec_get(&stop, TIME_UTC);
		real_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
		printf("> Demorou %.1f segundos, retornou %d\n", real_time, WEXITSTATUS(return_value));
		total_time += real_time;
		}
	}
	printf(">> O tempo total foi de %.1f segundos\n", total_time);
	return 0;
}