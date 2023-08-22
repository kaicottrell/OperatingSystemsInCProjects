#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
 // Kai Cottrell - CS3100
bool isInteger(const char* str) {
    if (str == NULL || *str == '\0') {
        return false;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

struct Process {
    int process_id;
    int active;
    int arrival_time;
    int total_cpu_time;
    int remaining_cpu_time;
    int turnaround_time;
};

// Part 2: Create and initialize the simulation table (processes) 
void create_process(struct Process* process, int k, int min_time, int max_time) {
    process->arrival_time = rand() % (k + 1);

    if (max_time < min_time) {
        printf("Max_time must be greater than or equal to Min_time\n");
    } else {
        process->total_cpu_time = min_time + rand() % (max_time - min_time + 1);
        process->remaining_cpu_time = process->total_cpu_time;
        process->active = (process->arrival_time == 0) ? 1 : 0;
        process->turnaround_time = 0;
    }
}
void print_processes(struct Process processes[], int numProcesses){
    printf("PID  Arrival  Active(0/1)  Total  Remain Turnaround\n");
    for(int i = 0; i < numProcesses; i++){
        printf("%d\t%d\t%d\t   %d\t   %d\t  %d\n", processes[i].process_id, processes[i].arrival_time, processes[i].active, processes[i].total_cpu_time, processes[i].remaining_cpu_time, processes[i].turnaround_time );
    }
    
}
void reset_remaining_time(struct Process processes[], int numProcesses){
    for(int i = 0; i < numProcesses; i++){
        processes[i].remaining_cpu_time = processes[i].total_cpu_time;
        processes[i].turnaround_time = 0;
    }
}
void calc_average_turnaround_time(struct Process processes[], int numProcesses){
    int total_turnaround = 0;
    for(int i = 0; i < numProcesses; i++){
        total_turnaround += processes[i].turnaround_time;
    }
    double average_turnaround = total_turnaround / numProcesses;
    printf("The Average Turnaround is: %f\n\n", average_turnaround);
}
void bubbleSortByArrivalTime(struct Process processes[], int numProcesses){
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap the processes
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}
void bubbleSortByTotalCPUTime(struct Process processes[], int numProcesses) {
    for (int i = 0; i < numProcesses - 1; i++) {
        for (int j = 0; j < numProcesses - i - 1; j++) {
            if (processes[j].total_cpu_time > processes[j + 1].total_cpu_time) {
                // Swap the processes
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}


void FCFS(struct Process processes[], int numProcesses) {
    print_processes(processes, numProcesses);
    bubbleSortByArrivalTime(processes,numProcesses);
    int time = 0;
    bool allProcessesCompleted = false;

    while (!allProcessesCompleted) {
        bool processExecuted = false;
        bool idlePrinted = true;  // Flag to check if IDLE has been printed

        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0 && processes[i].arrival_time <= time) {
                // Execute the process
                processes[i].active = 1;
                while (processes[i].remaining_cpu_time > 0) {
                    processes[i].remaining_cpu_time--;
                    printf("Time %d: - Process %d\n", time, processes[i].process_id);
                    time++;
                }
                if (processes[i].remaining_cpu_time < 0)
                    processes[i].remaining_cpu_time = 0;

                // Process has completed, update its active flag and calculate turnaround time
                processes[i].active = 0;
                processes[i].turnaround_time = time - processes[i].arrival_time;

                processExecuted = true;
                idlePrinted = false;
                break;
            }
        }

        if (!processExecuted && idlePrinted) {
            printf("Time %d: - IDLE\n", time);
            time++;
        }

        // Check if all processes have completed
        allProcessesCompleted = true;
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0) {
                allProcessesCompleted = false;
                break;
            }
        }
    }
    

    printf("Turnaround time for FCFS\n");
    calc_average_turnaround_time(processes, numProcesses);
    reset_remaining_time(processes, numProcesses);
}
//FCFS just based giving priority to processes with shorter CPU Times instead of arrival times
void SJF(struct Process processes[], int numProcesses) {
    bubbleSortByTotalCPUTime(processes,numProcesses);
    print_processes(processes, numProcesses);
    int time = 0;
    bool allProcessesCompleted = false;

    while (!allProcessesCompleted) {
        bool processExecuted = false;
        bool idlePrinted = true;  // Flag to check if IDLE has been printed

        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0 && processes[i].arrival_time <= time) {
                // Execute the process
                processes[i].active = 1;
                while (processes[i].remaining_cpu_time > 0) {
                    processes[i].remaining_cpu_time--;
                    printf("Time %d: - Process %d\n", time, processes[i].process_id);
                    time++;
                }
                if (processes[i].remaining_cpu_time < 0)
                    processes[i].remaining_cpu_time = 0;

                // Process has completed, update its active flag and calculate turnaround time
                processes[i].active = 0;
                processes[i].turnaround_time = time - processes[i].arrival_time;

                processExecuted = true;
                idlePrinted = false;
                break;
            }
        }

        if (!processExecuted && idlePrinted) {
            printf("Time %d: - IDLE\n", time);
            time++;
        }

        // Check if all processes have completed
        allProcessesCompleted = true;
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0) {
                allProcessesCompleted = false;
                break;
            }
        }
    }
    

    printf("Turnaround time for SJF\n");
    calc_average_turnaround_time(processes, numProcesses);
    reset_remaining_time(processes, numProcesses);
}

void SRT(struct Process processes[], int numProcesses) {
    print_processes(processes, numProcesses);
    int time = 0;
    bool allProcessesCompleted = false;
    int executingProcess = -1;

    while (!allProcessesCompleted) {
        bool processExecuted = false;
        int shortestIndex = -1;
        int shortestRemainingTime = INT_MAX;

        // Find the process with the shortest remaining time among the available processes
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0 && processes[i].arrival_time <= time) {
                if (processes[i].remaining_cpu_time < shortestRemainingTime) {
                    shortestRemainingTime = processes[i].remaining_cpu_time;
                    shortestIndex = i;
                }
            }
        }

        if (executingProcess == -1) {
            // No process is currently executing, so select the process with the shortest remaining time
            executingProcess = shortestIndex;
        } else if (shortestIndex != -1 && shortestIndex != executingProcess) {
            // A shorter process has arrived, so preempt the currently executing process
            printf("Time %d: - Preempt Process %d\n", time, processes[executingProcess].process_id);
            executingProcess = shortestIndex;
        }
        if (executingProcess != -1) {
            // Execute the process
            processes[executingProcess].active = 1;
            processes[executingProcess].remaining_cpu_time--;

            printf("Time %d: - Process %d\n", time, processes[executingProcess].process_id);
            time++;

            // Check if the current process has completed
            if (processes[executingProcess].remaining_cpu_time == 0) {
                // Process has completed, update its active flag and calculate turnaround time
                processes[executingProcess].active = 0;
                processes[executingProcess].turnaround_time = time - processes[executingProcess].arrival_time;
                processExecuted = true;
                executingProcess = -1;
            }
        } else {
            // No process is currently executing, so increment time
            time++;
        }
        // Check if all processes have completed
        allProcessesCompleted = true;
        for (int i = 0; i < numProcesses; i++) {
            if (processes[i].remaining_cpu_time > 0) {
                allProcessesCompleted = false;
                break;
            }
        }
    }
    printf("Turnaround time for SRT\n");
    calc_average_turnaround_time(processes, numProcesses);
    reset_remaining_time(processes, numProcesses);
}

int main(int argc, char* argv[]) {
    //Part 1
    struct Process* processes;
    int n;
    int k = 100;
    int min_time = 7;
    int max_time = 15;
    if (argc == 1) {
        n = 10;
        processes = malloc(n * sizeof(struct Process));
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            processes[i].process_id = i;
            create_process(&processes[i], k, min_time, max_time);
        }
        //Part 3
        FCFS(processes,n);
        SJF(processes,n);
        SRT(processes,n);
    } else if (argc != 5) {
        printf("Not the correct number of arguments entered. Please enter 4 integers and try again.\n");
        return 1;
    } else if (isInteger(argv[1]) && isInteger(argv[2]) && isInteger(argv[3]) && isInteger(argv[4])) {
        n = atoi(argv[1]);
        k = atoi(argv[2]);
        min_time = atoi(argv[3]);
        max_time = atoi(argv[4]);
        processes = malloc(n * sizeof(struct Process));
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            processes[i].process_id = i;
            create_process(&processes[i], k, min_time, max_time);
        }
        //Part 3
        FCFS(processes,n);
        SJF(processes,n);
        SRT(processes,n);
    } else {
        printf("Please enter integers.\n");
        return 1;
    }
    free(processes);
    return 0;
}
