#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_RANDOM 65535  // Maximum random value: 2^16 - 1

// Node structure for linked list
struct Node {
    int data;
    struct Node* next;
};

// Global variables
struct Node* head = NULL;
pthread_mutex_t lock;
int opsPerThread, memberOpsPerThread, totalOps;
int insertOpsPerThread, deleteOpsPerThread;

// Function declarations
int insertNode(int value);
void initializeList(int numNodes);
int checkMembership(int value);
void initializeOpsArray(int *opsArray);
int deleteNode(int value);
void swapValues(int *a, int *b);
void* executeOps(void* ops);
void shuffleOps(int *opsArray, int n);
int generateRandomValue(void);
long getCurrentTimestamp(void);
void freeList();

/*
* Main function to run the program
*/
int main(int argc, char *argv[]) {
    long startTime, endTime, elapsedTime;
    int numThreads;
    pthread_t* threads;

    // Parse input arguments
    int numNodes = atoi(argv[1]);
    int numOperations = atoi(argv[2]);
    totalOps = numOperations;
    numThreads = atoi(argv[6]);
    opsPerThread = totalOps / numThreads;
    memberOpsPerThread = strtod(argv[3], NULL) * opsPerThread;
    insertOpsPerThread = strtod(argv[4], NULL) * opsPerThread;
    deleteOpsPerThread = strtod(argv[5], NULL) * opsPerThread;

    threads = (pthread_t*) malloc (numThreads * sizeof(pthread_t));
    pthread_mutex_init(&lock, NULL);

    // Initialize linked list and random operations
    initializeList(numNodes);
    int opsArray[opsPerThread];
    initializeOpsArray(opsArray);

    // Record the start time
    startTime = getCurrentTimestamp();

    // Create threads to perform operations
    for (int t = 0; t < numThreads; t++) {
        pthread_create(&threads[t], NULL, executeOps, (void*)(opsArray));
    }

    for (int t = 0; t < numThreads; t++) {
        pthread_join(threads[t], NULL);
    }

    // Record the end time and calculate elapsed time
    endTime = getCurrentTimestamp();
    elapsedTime = endTime - startTime;

    freeList();
    printf("%ld", elapsedTime);
    return 0;
}

/*
* Get the current timestamp in milliseconds
*/
long getCurrentTimestamp() {
    struct timeval te;
    gettimeofday(&te, NULL);
    long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
    return milliseconds;
}

/*
* Generate a random value within the defined range
*/
int generateRandomValue() {
    return rand() % MAX_RANDOM;
}

/*
* Initialize the linked list with random values
*/
void initializeList(int numNodes) {
    srand(time(NULL));
    for (int i = 0; i < numNodes; i++) {
        if (!insertNode(generateRandomValue())) {
            i--;  // Retry if insertion fails
        }
    }
}

/*
* Shuffle the operation array using Fisher-Yates algorithm
*/
void shuffleOps(int *opsArray, int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swapValues(&opsArray[i], &opsArray[j]);
    }
}

/*
* Initialize the operations array
*/
void initializeOpsArray(int *opsArray) {
    for (int i = 0; i < opsPerThread; i++) {
        if (i < insertOpsPerThread) {
            opsArray[i] = 1;  // Insert operation
        } else if (i < insertOpsPerThread + deleteOpsPerThread) {
            opsArray[i] = -1;  // Delete operation
        } else {
            opsArray[i] = 0;  // Membership check
        }
    }
    shuffleOps(opsArray, opsPerThread);
}

/*
* Swap two integer values
*/
void swapValues(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
* Execute operations (insert, delete, member check)
*/
void* executeOps(void *ops) {
    int *opsArray = (int*) ops;
    for (int i = 0; i < opsPerThread; i++) {
        int value = generateRandomValue();
        pthread_mutex_lock(&lock);
        if (opsArray[i] == 1) {
            insertNode(value);
        } else if (opsArray[i] == -1) {
            deleteNode(value);
        } else {
            checkMembership(value);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

/*
* Check if a value exists in the linked list
*/
int checkMembership(int value) {
    struct Node* current = head;
    while (current != NULL && current->data < value) {
        current = current->next;
    }
    return (current != NULL && current->data == value);
}

/*
* Insert a value into the linked list in sorted order
*/
int insertNode(int value) {
    struct Node* current = head;
    struct Node* prev = NULL;
    struct Node* newNode;

    while (current != NULL && current->data < value) {
        prev = current;
        current = current->next;
    }

    if (current == NULL || current->data > value) {
        newNode = (struct Node*) malloc(sizeof(struct Node));
        newNode->data = value;
        newNode->next = current;
        if (prev == NULL) {
            head = newNode;
        } else {
            prev->next = newNode;
        }
        return 1;
    }
    return 0;  // Value already exists
}

/*
* Delete a value from the linked list
*/
int deleteNode(int value) {
    struct Node* current = head;
    struct Node* prev = NULL;

    while (current != NULL && current->data < value) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->data == value) {
        if (prev == NULL) {
            head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
        return 1;
    }
    return 0;
}

/*
* Free all nodes in the linked list
*/
void freeList() {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
