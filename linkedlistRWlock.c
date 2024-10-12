#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_RANDOM 65535 // Maximum random number

// Node for linked list
struct Node {
    int value;
    struct Node* next;
};

// Global variables
struct Node* listHead = NULL;
pthread_rwlock_t rwLock;
int totalOperations;
int operationsPerThread;
int deleteOpsPerThread;
int memberOpsPerThread;
int insertOpsPerThread;

// Function prototypes
int addNode(int value);
int searchNode(int value);
int removeNode(int value);
void initializeList(int numValues);
long getCurrentTime(void);
void setOperations(int* ops);
void shuffle(int* arr, int size);
void* executeOperations(void* params);
int generateRandomValue(void);
void freeList();

/*
* Main function
*/
int main(int argc, char *argv[]) {
    long startTime, endTime, totalTime;
    long threadIndex;
    pthread_t* threads;

    // Get inputs from command line arguments
    int initialValues = atoi(argv[1]);
    int numThreads = atoi(argv[6]);
    totalOperations = atoi(argv[2]);
    operationsPerThread = totalOperations / numThreads;
    memberOpsPerThread = strtod(argv[3], NULL) * operationsPerThread;
    insertOpsPerThread = strtod(argv[4], NULL) * operationsPerThread;
    deleteOpsPerThread = strtod(argv[5], NULL) * operationsPerThread;

    threads = (pthread_t*) malloc(numThreads * sizeof(pthread_t));
    pthread_rwlock_init(&rwLock, NULL);

    // Initialize linked list with random values
    initializeList(initialValues);

    // Initialize random operations
    int operations[operationsPerThread];
    setOperations(operations);

    // Start timer
    startTime = getCurrentTime();

    // Create threads to perform operations
    for (threadIndex = 0; threadIndex < numThreads; threadIndex++) {
        pthread_create(&threads[threadIndex], NULL, executeOperations, (void*)(operations));
    }

    // Wait for threads to finish
    for (threadIndex = 0; threadIndex < numThreads; threadIndex++) {
        pthread_join(threads[threadIndex], NULL);
    }

    // End timer
    endTime = getCurrentTime();

    // Calculate elapsed time
    totalTime = endTime - startTime;
    freeList();

    // Output the elapsed time
    printf("%ld", totalTime);

    return 0;
}

/*
* Get the current time in milliseconds
*/
long getCurrentTime() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    long milliseconds = currentTime.tv_sec * 1000LL + currentTime.tv_usec / 1000;
    return milliseconds;
}

/*
* Generate a random value
*/
int generateRandomValue() {
    return rand() % MAX_RANDOM;
}

/*
* Initialize linked list with random values
*/
void initializeList(int numValues) {
    srand(time(NULL));
    for (int i = 0; i < numValues; i++) {
        if (!addNode(generateRandomValue())) {
            i--;
        }
    }
}

/*
* Randomize the order of operations
*/
void shuffle(int* arr, int size) {
    srand(time(NULL));
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

/*
* Set up operations array
*/
void setOperations(int* ops) {
    for (int i = 0; i < operationsPerThread; i++) {
        if (i < insertOpsPerThread) {
            ops[i] = 1; // Insert
        } else if (i < insertOpsPerThread + deleteOpsPerThread) {
            ops[i] = -1; // Delete
        } else {
            ops[i] = 0; // Member (search)
        }
    }
    shuffle(ops, operationsPerThread);
}

/*
* Perform operations in threads
*/
void* executeOperations(void* params) {
    int* operations = (int*) params;
    for (int i = 0; i < operationsPerThread; i++) {
        int value = generateRandomValue();
        if (operations[i] == 1) {
            pthread_rwlock_wrlock(&rwLock);
            addNode(value);
            pthread_rwlock_unlock(&rwLock);
        } else if (operations[i] == -1) {
            pthread_rwlock_wrlock(&rwLock);
            removeNode(value);
            pthread_rwlock_unlock(&rwLock);
        } else {
            pthread_rwlock_rdlock(&rwLock);
            searchNode(value);
            pthread_rwlock_unlock(&rwLock);
        }
    }
    return NULL;
}

/*
* Search for a node in the list
*/
int searchNode(int value) {
    struct Node* current = listHead;
    while (current != NULL && current->value < value)
        current = current->next;

    if (current == NULL || current->value > value)
        return 0;
    else
        return 1;
}

/*
* Add a node to the linked list
*/
int addNode(int value) {
    struct Node* current = listHead;
    struct Node* previous = NULL;
    struct Node* newNode;

    while (current != NULL && current->value < value) {
        previous = current;
        current = current->next;
    }

    if (current == NULL || current->value > value) {
        newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->value = value;
        newNode->next = current;
        if (previous == NULL)
            listHead = newNode;
        else
            previous->next = newNode;
        return 1;
    }
    return 0;
}

/*
* Remove a node from the linked list
*/
int removeNode(int value) {
    struct Node* current = listHead;
    struct Node* previous = NULL;

    while (current != NULL && current->value < value) {
        previous = current;
        current = current->next;
    }

    if (current != NULL && current->value == value) {
        if (previous == NULL) {
            listHead = current->next;
        } else {
            previous->next = current->next;
        }
        free(current);
        return 1;
    }
    return 0;
}

/*
* Free the linked list
*/
void freeList() {
    struct Node* current = listHead;
    struct Node* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}
