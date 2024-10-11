#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

// Maximum value of random numbers
#define MAX_VALUE 65535  // 2^16 - 1 = 65535

// linked list node
struct ListNode {
    int    data;
    struct ListNode* next;
};

// Variables
struct ListNode* head_p = NULL;
pthread_rwlock_t rwlock;
int no_Of_Operations;
int noOfOperationsPerThread;
int no_Of_Delete_Per_Thread;
int no_Of_Member_Per_Thread;
int no_Of_Insert_Per_Thread;

// Method declarations
int  Insert(int value);
int  Member(int value);
int  Delete(int value);
void initialize(int);
long get_Current_Time(void);
void initialize_Operation(int *no);
void swap(int *a, int *b);
void* doOperations(void*);
void randomize(int *no, int n);
int generate_Random(void);
void Free_list();



/*
* Main method to execute the program.
*/
int main(int arc, char *argv[]) {
    // Variables
    long start, finish, elapsed;
    long thread;
    pthread_t* threadHandles;

    // Collect and interpret the arguments
    int noOfVariables = atoi(argv[1]);
    int noOfThreads = atoi(argv[6]);
    no_Of_Operations = atoi(argv[2]);
    noOfOperationsPerThread = atoi(argv[2]) / noOfThreads;
    no_Of_Member_Per_Thread = strtod(argv[3], NULL) * noOfOperationsPerThread;
    no_Of_Insert_Per_Thread = strtod(argv[4], NULL) * noOfOperationsPerThread;
    no_Of_Delete_Per_Thread = strtod(argv[5], NULL) * noOfOperationsPerThread;

    threadHandles = (pthread_t*) malloc (noOfThreads * sizeof(pthread_t));
    pthread_rwlock_init(&rwlock, NULL);

    // Initialize the linked list
    initialize(noOfVariables);
    //Initialize the random operations
    int operations[noOfOperationsPerThread];
    initialize_Operation(operations);


    // Get the starting time
    start = get_Current_Time();

    // Do the operations
    for (thread = 0; thread < noOfThreads; thread++) {
//        int start = thread * noOfOperationsPerThread;
        pthread_create(&threadHandles[thread], NULL, doOperations, (void*)(operations));
    }

    for (thread = 0; thread < noOfThreads; thread++) {
        pthread_join(threadHandles[thread], NULL);
    }

    // Get the ending time
    finish = get_Current_Time();

    // Calculate the elapsed time
    elapsed = finish - start;
//    free(operations);
    Free_list();
    // Print the time to stdout
    printf("%ld", elapsed);

    return 0;
}

/*
* Generate the current time in milliseconds.
*/
long get_Current_Time() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000; // caculate milliseconds
    return milliseconds;
}

/*
* Generate random number within the range.
*/
int generate_Random() {
    int value = rand() % MAX_VALUE;
    return value;
}

/*
* Initialize the array using random numbers.
*/
void initialize(int noOfVariables) {
    srand (time(NULL));
    int inserted = 0;
    int i;
    for (i = 0; i < noOfVariables; i++) {
        inserted = Insert(generate_Random());
        if (!inserted) {
            i--;
        }
    }
}

/*
 * A function to generate a random permutation of arr[]
 */
void randomize(int *no, int n) {
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand(time(NULL));

    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n - 1; i > 0; i--) {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);

        // Swap arr[i] with the element at random index
        swap((no+i), (no+j));
    }
}


void initialize_Operation(int *no) {
    int i;
    for (i = 0; i < noOfOperationsPerThread; i++) {
        if (i < no_Of_Insert_Per_Thread) {
            *(no + i) = 1;
        } else if (i < no_Of_Insert_Per_Thread + no_Of_Delete_Per_Thread) {
            *(no + i) = -1;
        } else {
            *(no + i) = 0;
        }
    }
    randomize(no, noOfOperationsPerThread);
}

/*
 *  A utility function to swap to integers
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


/*
* Do operations.
*/
void* doOperations(void* para) {
    int *start = para;
    int i;
    for (i = 0; i < noOfOperationsPerThread; i++) {
        if (*(start + i) == 1) {
            int value = generate_Random();
            pthread_rwlock_wrlock(&rwlock);
            Insert(value);
//            printf("++++++ in %p\n", start);
            pthread_rwlock_unlock(&rwlock);
        } else if (*(start + i) == -1) {
            int value = generate_Random();
            pthread_rwlock_wrlock(&rwlock);
            Delete(value);
//            printf("------ in %p\n", start);
            pthread_rwlock_unlock(&rwlock);
        } else {
            int value = generate_Random();
            pthread_rwlock_rdlock(&rwlock);
            Member(value);
//            printf("====== in %p\n", start);
            pthread_rwlock_unlock(&rwlock);
        }
    }
    return NULL;
}

/*
* Check whether the number is available.
*/
int  Member(int value) {
    struct ListNode* curr_p;

    curr_p = head_p;
    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}


/*
* Insert a number to linked list.
*/
int Insert(int value) {
    struct ListNode* curr_p = head_p;
    struct ListNode* pred_p = NULL;
    struct ListNode* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct ListNode));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            head_p = temp_p;
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}


/*
* Delete the given number from the linked list.
*/
int Delete(int value) {
    struct ListNode* curr_p = head_p;
    struct ListNode* pred_p = NULL;

    /* Find value */
    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            head_p = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else {
        return 0;
    }
}

void Free_list() {
    struct ListNode* succ_p;
    struct ListNode *curr_p;

    if (head_p == NULL) return;
    curr_p = head_p;
    while (curr_p != NULL) {
        succ_p = curr_p->next;
        free(curr_p);
        curr_p = succ_p;
    }

}