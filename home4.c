#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 

#define p_1 3
#define c_1 10
#define n_1 500
#define r_1 150

// Pipe for inter-process communication
int pipe_fd[2]; 
// Mutex for synchronization
pthread_mutex_t pipe_mutex = PTHREAD_MUTEX_INITIALIZER; 

// Signal handler for the child process to start reading
volatile sig_atomic_t start_processing = 0;

void signal_handler(int signum) {
    // Set flag to indicate child should start processing
    start_processing = 1;
}
void *generate_numbers(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);
    int numbers[n_1];
    // Generate random numbers
    for (int i = 0; i < n_1; i++) {
        numbers[i] = rand() % 1001; 
    }
    pthread_mutex_lock(&pipe_mutex);
    if (write(pipe_fd[1], numbers, sizeof(numbers)) == -1) {
        perror("write");
    }
    pthread_mutex_unlock(&pipe_mutex);
    printf("Parent thread %d generated and sent numbers to the pipe.\n", thread_id);
    fflush(stdout);
    return NULL;
}

void *process_numbers(void *arg) {
    int thread_id = *(int *)arg;
    free(arg);
    while (!start_processing) {
        // Child waits for the signal to start processing
        sleep(1);  
    }
    int numbers[r_1];
    int sum = 0;
    if (read(pipe_fd[0], numbers, sizeof(numbers)) == -1) {
        perror("read");
        pthread_exit(NULL);
    }
    // Calculate the sum of numbers
    for (int i = 0; i < r_1; i++) {
        sum += numbers[i];
    }
    printf("Child thread %d calculated sum: %d\n", thread_id, sum);
    fflush(stdout); 
    return (void *)(intptr_t)sum;
}
int main() {
    // Set up the signal handler for child process
    signal(SIGUSR1, signal_handler);
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pthread_t parent_threads[p_1];
    // Create parent threads to generate numbers
    for (int i = 0; i < p_1; i++) {
        int *thread_id = malloc(sizeof(int));
        *thread_id = i;
        if (pthread_create(&parent_threads[i], NULL, generate_numbers, thread_id) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    // Wait for parent threads to finish
    for (int i = 0; i < p_1; i++) {
        pthread_join(parent_threads[i], NULL);
    }
    // Fork the child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        pthread_t child_threads[c_1];
        int total_sum = 0;
        // Create child threads to process numbers
        for (int i = 0; i < c_1; i++) {
            int *thread_id = malloc(sizeof(int));
            *thread_id = i;
            if (pthread_create(&child_threads[i], NULL, process_numbers, thread_id) != 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
        }
        // Collect results from child threads
        for (int i = 0; i < c_1; i++) {
            void *retval;
            pthread_join(child_threads[i], &retval);
            total_sum += (intptr_t)retval;
        }
        // Calculate and display the average
        double average = total_sum / (double)c_1;
        printf("Average of sums: %.2f\n", average);
        fflush(stdout); 
        // Writing the average to a file
        FILE *output = fopen("average.txt", "w");
        if (output) {
            fprintf(output, "%.2f\n", average);
            fclose(output);
        } else {
            perror("fopen");
        }
        exit(0);
    }
    kill(pid, SIGUSR1);
    waitpid(pid, NULL, 0);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    return 0;
}

