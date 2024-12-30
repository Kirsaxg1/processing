#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        fprintf(stderr, "Number of processes must be a positive integer\n");
        return 1;
    }

    for (int i = 0; i < num_processes; i++) {
        DWORD pid = _spawnl(_P_NOWAIT, _pgmptr, _pgmptr, NULL);

        if (pid == (DWORD)-1) {
            perror("_spawnl");
            return 1;
        }
        else if (pid == 0) {
            // Child process
            printf("Hello from Ivan Konovalov! My PID: %d\n", _getpid());
            exit(0);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < num_processes; i++) {
        _cwait(NULL, -1, WAIT_CHILD);
    }

    return 0;
}