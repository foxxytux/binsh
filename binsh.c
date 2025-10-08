#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define AC_YELLOW "\x1b[33m"
#define AC_MAGENTA "\x1b[35m"
#define AC_WHITE "\x1b[37m"

char cmd[512];
char fullsystem[1024];
char cwd[PATH_MAX];
char *paths[2048];
int path_count = 0;

void parse_path() {
	char *env_path = getenv("PATH");
	char *path_copy = malloc(strlen(env_path) + 1);
	strcpy(path_copy, env_path);
	
	char *token = strtok(path_copy, ":");
	while (token != NULL && path_count < 2048) {
		paths[path_count] = malloc(strlen(token) + 1);
		strcpy(paths[path_count], token);
		path_count++;
		token = strtok(NULL, ":");
	}
	free(path_copy);
}

void execute_command(char *command) {
	char cmd_name[512];
	strcpy(cmd_name, command);
	
	char *space = strchr(cmd_name, ' ');
	if (space) {
		*space = '\0';
	}
	
	for (int i = 0; i < path_count; i++) {
		sprintf(fullsystem, "%s/%s", paths[i], cmd_name);
		if (access(fullsystem, X_OK) == 0) {
			system(command);
			return;
		}
	}
	printf("BinSH: Command Not Found - %s\n", cmd_name);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, SIG_IGN);
	parse_path();
	
	if (argc > 1) {
		FILE *file = fopen(argv[1], "r");
		if (file == 0) {
			printf("File Does Not Exist!\n");
			return 1;
		}
		
		char line[256];
		while (fgets(line, sizeof(line), file)) {
			line[strcspn(line, "\n")] = 0;
			
			if (strlen(line) == 0) continue;
			
			if (strcmp(line, "exit") == 0) {
				break;
			} else if (strncmp(line, "cd ", 3) == 0) {
				chdir(line + 3);
			} else if (strcmp(line, "cd") == 0) {
				chdir(getenv("HOME"));
			} else {
				execute_command(line);
			}
		}
		fclose(file);
		return 0;
	}
	
	while (1) {
		getcwd(cwd, sizeof(cwd));
		memset(cmd, 0, 512);
		
		char prt[256];
		sprintf(prt, "%s%s%s-%s%s%s$ ", AC_MAGENTA, "BinSH", AC_WHITE, AC_YELLOW, cwd, AC_WHITE);
		
		char *temp = readline(prt);
		if (temp == NULL) break;
		
		strncpy(cmd, temp, sizeof(cmd) - 1);
		add_history(cmd);
		free(temp);
		
		cmd[strcspn(cmd, "\n")] = 0;
		
		if (strlen(cmd) == 0) continue;
		
		if (strcmp(cmd, "exit") == 0) {
			break;
		} else if (strncmp(cmd, "cd ", 3) == 0) {
			chdir(cmd + 3);
		} else if (strcmp(cmd, "cd") == 0) {
			chdir(getenv("HOME"));
		} else {
			execute_command(cmd);
		}
	}
	
	for (int i = 0; i < path_count; i++) {
		free(paths[i]);
	}
	
	return 0;
}
