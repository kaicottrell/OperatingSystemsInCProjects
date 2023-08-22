#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

struct Process {
    int process_id;
    int active;
    int arrival_time;
    int total_cpu_time;
    int remaining_cpu_time;
    int turnaround_time;
};

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

void create_simulation_row(struct Process* process, int k, int min_time, int max_time) {
    process->arrival_time = rand() % (k + 1);

    if (max_time < min_time) {
        printf("Max_time must be greater than or equal to Min_time\n");
    } else {
        process->total_cpu_time = min_time + rand() % (max_time - min_time + 1);
        process->remaining_cpu_time = process->total_cpu_time;
        process->active = (process->arrival_time == 0) ? 1 : 0;

        printf("%-7d%-8d%-8d%-8d%-9d%d\n", process->process_id, process->active, process->arrival_time,
               process->total_cpu_time, process->remaining_cpu_time, process->turnaround_time);
    }
}

int fcfs_algorithm(struct Process* processes, int current_process_id, int current_time_step) {
    int n = sizeof(processes) / sizeof(processes[0]);

    if (current_process_id == -1) {
        for (int i = 0; i < n; i++) {
            if (processes[i].active == 1) {
                return i;
            }
        }
    }

    return current_process_id;
}

int sjf_algorithm(struct Process* processes, int current_process_id, int current_time_step) {
    int n = sizeof(processes) / sizeof(processes[0]);
    int shortest_remaining_time = INT_MAX;
    int shortest_process_id = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].active == 1 && processes[i].remaining_cpu_time < shortest_remaining_time) {
            shortest_remaining_time = processes[i].remaining_cpu_time;
            shortest_process_id = i;
        }
    }

    return shortest_process_id;
}

int srt_algorithm(struct Process* processes, int current_process_id, int current_time_step) {
    int n = sizeof(processes) / sizeof(processes[0]);
    int shortest_remaining_time = INT_MAX;
    int shortest_process_id = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].active == 1 && processes[i].remaining_cpu_time < shortest_remaining_time) {
            shortest_remaining_time = processes[i].remaining_cpu_time;
            shortest_process_id = i;
        }
    }

    if (current_process_id != -1) {
        if (processes[current_process_id].remaining_cpu_time < shortest_remaining_time) {
            return current_process_id;
        }
    }

    return shortest_process_id;
}

void simulate(struct Process* processes, int n, const char* algorithm_name) {
    int current_process_id = -1;
    int current_time_step = 0;

    printf("Simulation using %s algorithm:\n", algorithm_name);

    while (true) {
        bool all_processes_terminated = true;

        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_cpu_time > 0) {
                all_processes_terminated = false;

                if (processes[i].arrival_time == current_time_step) {
                    processes[i].active = 1;
                }
            }
        }

        if (all_processes_terminated) {
            break;
        }

        if (current_process_id == -1) {
            printf("Time: %d - IDLE\n", current_time_step);
            current_time_step++;
            continue;
        }

        printf("Time: %d - PID: %d\n", current_time_step, current_process_id);

        processes[current_process_id].remaining_cpu_time--;

        if (processes[current_process_id].remaining_cpu_time == 0) {
            processes[current_process_id].active = 0;
            processes[current_process_id].turnaround_time = current_time_step + 1 - processes[current_process_id].arrival_time;
            current_process_id = -1;
        }

        current_time_step++;
    }

    double total_turnaround_time = 0.0;

    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
    }

    double average_turnaround_time = total_turnaround_time / n;
    printf("Average Turnaround Time: %.2f\n", average_turnaround_time);
    printf("\n");
}

int main(int argc, char* argv[]) {
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
            create_simulation_row(&processes[i], k, min_time, max_time);
        }
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
            create_simulation_row(&processes[i], k, min_time, max_time);
        }
    } else {
        printf("Please enter integers.\n");
        return 1;
    }

    simulate(processes, n, "FCFS");
    simulate(processes, n, "SJF");
    simulate(processes, n, "SRT");

    free(processes);

    return 0;
}
