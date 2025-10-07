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

int main( int argc, char *argv[] ) {
	signal(SIGINT, SIG_IGN);
	if ( argc > 1 ) {
		FILE *file = fopen( argv[1], "r" );
		if ( file == 0) { printf("File Does Not Exist!\n");	} else {
			char line[256];
			while (fgets(line, sizeof(line), file)) {
				sprintf(fullsystem, "/bin/%s", line);
				system(fullsystem); 
			}
		}
	fclose(file);
	return 0;
	}
	while (1) {
		getcwd(cwd, sizeof(cwd));
		memset(cmd,0,512);
		char prt[256];
		sprintf(prt, "%s%s%s-%s%s%s$ ", AC_MAGENTA, "BinSH", AC_WHITE, AC_YELLOW, cwd, AC_WHITE);		
		char *temp = readline(prt);
		strncpy(cmd, temp, sizeof(cmd) - 1);
		add_history(cmd);
		free(temp);
		cmd[strcspn(cmd, "\n")] = 0;
		if (strcmp(cmd, "exit") == 0) {	return 0; } 
		if (strncmp(cmd, "cd ", 3) == 0) { chdir(cmd + 3); } 
		else if (strncmp(cmd, "cd", 2) == 0){ chdir(getenv("HOME")); }
		else {
			sprintf(fullsystem, "/bin/%s", cmd);
			system(fullsystem); 
		}
	}
}
