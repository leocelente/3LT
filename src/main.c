#include "ipc.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define PERMISSION 0666

int main(const int argc, char **argv) {

	char target[] = "./build/target-mock";// target peripheral
	char lib_ut[] = "./build/library-tests";// library under test

	mkfifo(POCI, PERMISSION);
	mkfifo(COPI, PERMISSION);

	int child_pid = fork();
	if (child_pid == 0) {
		   execl(target, "", NULL);
		while (1) {}
	} else {
		execl(lib_ut, (char *)&child_pid, NULL);
	}
	wait(NULL);
}
