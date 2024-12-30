#include <stdio.h>
#include <stdlib.h>
#include <process.h> // Äëÿ _spawnl è _getpid
#include <windows.h> // Äëÿ Sleep
#include <string.h>
#include <errno.h>

// Function to create child processes and print information
void create_processes(int n, int level, FILE* output_file) {
    if (n <= 0) {
        return;
    }

    DWORD* child_pids = (DWORD*)malloc(n * sizeof(DWORD)); // Array to store child PIDs

    // Create child processes
    for (int i = 0; i < n; i++) {
        DWORD pid = _spawnl(_P_NOWAIT, _pgmptr, _pgmptr, NULL);
        if (pid == (DWORD)-1) {
            perror("_spawnl");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            // Child process
            create_processes(n - 1, level + 1, output_file);
            Sleep((10 + level) * 1000); // Delay before exiting
            exit(EXIT_SUCCESS);
        }
        else {
            child_pids[i] = pid;
        }
    }

    // Print information about the process and its child processes
    fprintf(output_file, "PID: %d ", _getpid());
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%d ", child_pids[i]);
    }
    fprintf(output_file, "\n");

    // Wait for child processes to finish
    for (int i = 0; i < n; i++) {
        _cwait(NULL, child_pids[i], WAIT_CHILD);
    }

    free(child_pids);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s N [output_file]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "N must be a positive integer\n");
        return EXIT_FAILURE;
    }

    FILE* output_file;
    if (argc > 2) {
        errno_t err = fopen_s(&output_file, argv[2], "w");
        if (err != 0 || !output_file) {
            perror("fopen_s");
            return EXIT_FAILURE;
        }
    }
    else {
        output_file = stdout;
    }

    int total_processes = 0;

    // Call the function to create the hierarchy of processes
    create_processes(n, 0, output_file);

    // Calculate total number of processes
    int temp_n = n;
    int num_processes = 1;
    while (temp_n > 0) {
        total_processes += num_processes;
        num_processes *= temp_n;
        temp_n--;
    }

    // Print greeting from the main process
    fprintf(output_file, "Hello from Ivan Konovalov! Created %d processes.\n", total_processes);

    if (output_file != stdout)
    {
        fclose(output_file);
    }


    return EXIT_SUCCESS;
}