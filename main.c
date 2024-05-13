#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHARED_MEMORY_KEY 1234
#define SHARED_MEMORY_SIZE sizeof(int)

// Global variables for shared memory and mutex
int shmid;
int *shared_counter;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function to increment shared counter
void *thread_function(void *arg) {
    while (1) {
        // Lock mutex to ensure thread safety
        pthread_mutex_lock(&mutex);
        // Increment shared counter
        (*shared_counter)++;
        // Print thread ID and current counter value
        printf("Thread %d: Counter = %d\n", *((int*)arg), *shared_counter);
        // Unlock mutex
        pthread_mutex_unlock(&mutex);
        // Simulate some work
        sleep(1);
    }
    return NULL;
}

// Cleanup function to release resources
void cleanup() {
    printf("Cleaning up and exiting...\n");
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    // Detach shared memory
    shmdt(shared_counter);
    // Delete shared memory segment
    shmctl(shmid, IPC_RMID, NULL);
    // Exit the program
    exit(0);
}

// Signal handler for SIGINT
void sigint_handler(int signum) {
    printf("\nReceived SIGINT signal. Exiting...\n");
    // Call cleanup function
    cleanup();
}

int main() {
    // Signal handling - register SIGINT handler
    signal(SIGINT, sigint_handler);

    // Shared memory initialization
    shmid = shmget(SHARED_MEMORY_KEY, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    // Attach shared memory segment
    shared_counter = (int*)shmat(shmid, NULL, 0);
    // Initialize shared counter
    *shared_counter = 0;

    // Thread initialization
    pthread_t thread1, thread2;
    int thread_id1 = 1, thread_id2 = 2;
    // Create two threads
    pthread_create(&thread1, NULL, thread_function, &thread_id1);
    pthread_create(&thread2, NULL, thread_function, &thread_id2);

    // Main thread waits for threads to finish (should never happen)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}