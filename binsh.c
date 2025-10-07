#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <signal.h>

// colors
#define AC_YELLOW "\x1b[33m"
#define AC_MAGENTA "\x1b[35m"
#define AC_WHITE "\x1b[37m"
#define AC_NORMAL "\x1b[m"

char cmd[512];
char fullsystem[1024];
int system(const char *command);
int flag=0;
int empty=0;
char cwd[PATH_MAX];

int main() {
	signal(SIGINT, SIG_IGN);
	while (1) {
		getcwd(cwd, sizeof(cwd));
		printf("%s%s%s-%s%s%s$ ", AC_MAGENTA, "BinSH", AC_WHITE, AC_YELLOW, cwd, AC_NORMAL);
		memset(cmd,0,512);
		fgets(cmd, sizeof(cmd), stdin);
		cmd[strcspn(cmd, "\n")] = 0;
		if (strcmp(cmd, "exit") == 0 || empty == 5) {
			return 0;	
		}
		if(strlen(cmd) == 0) {signal(SIGINT, SIG_IGN);
			empty++;
			continue;
		} else {
			empty=0;
		} 
		if (strncmp(cmd, "cd ", 3) == 0) {
			chdir(cmd + 3);
		} else if (strncmp(cmd, "cd", 2) == 0){
			chdir(getenv("HOME"));
		} else {
			sprintf(fullsystem, "/bin/%s", cmd);
			system(fullsystem); 
		}
	}
}
