#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Maximum random value
#define MAX_RANDOM_VALUE 65535

// Struct for linked list node
struct Node {
  int value;
  struct Node *next;
};

// Global variables
struct Node *list_head = NULL;
int total_operations;
int insert_operations;
int member_operations;
int delete_operations;

// Function declarations
int insert_value(int val);
int check_member(int val);
int delete_value(int val);
void perform_operations(int *op_arr);
void initialize_list(int num_values);
void initialize_operations(int *op_arr);
void swap_values(int *x, int *y);
void shuffle_array(int *arr, int size);
long get_current_time_ms(void);
int generate_random_value(void);
void free_linked_list();

int main(int argc, char *argv[]) {
  long start_time, end_time, elapsed_time;

  // Parse command line arguments
  int num_values = atoi(argv[1]);
  total_operations = atoi(argv[2]);
  member_operations = strtod(argv[3], NULL) * total_operations;
  insert_operations = strtod(argv[4], NULL) * total_operations;
  delete_operations = strtod(argv[5], NULL) * total_operations;

  // Initialize the linked list
  initialize_list(num_values);

  // Initialize operations array
  int operations[total_operations];
  initialize_operations(operations);

  // Start timing
  start_time = get_current_time_ms();

  // Perform operations
  perform_operations(operations);

  // End timing
  end_time = get_current_time_ms();

  // Calculate elapsed time
  elapsed_time = end_time - start_time;

  // Free resources
  free_linked_list();

  // Output elapsed time
  printf("%ld\n", elapsed_time);

  return 0;
}

/* Get current time in milliseconds */
long get_current_time_ms() {
  struct timeval time_val;
  gettimeofday(&time_val, NULL);
  long millisecs = time_val.tv_sec * 1000LL + time_val.tv_usec / 1000;
  return millisecs;
}

/* Generate random number */
int generate_random_value() { return rand() % MAX_RANDOM_VALUE; }

/* Initialize linked list with random values */
void initialize_list(int num_values) {
  srand(time(NULL));
  for (int i = 0; i < num_values; i++) {
    if (!insert_value(generate_random_value())) {
      i--; // Retry if insertion fails
    }
  }
}

/* Initialize operations array */
void initialize_operations(int *op_arr) {
  for (int i = 0; i < total_operations; i++) {
    if (i < insert_operations) {
      op_arr[i] = 1;
    } else if (i < insert_operations + delete_operations) {
      op_arr[i] = -1;
    } else {
      op_arr[i] = 0;
    }
  }
  shuffle_array(op_arr, total_operations);
}

/* Swap two integers */
void swap_values(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

/* Shuffle the array using random permutation */
void shuffle_array(int *arr, int size) {
  srand(time(NULL));
  for (int i = size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    swap_values(&arr[i], &arr[j]);
  }
}

/* Insert a value into the linked list */
int insert_value(int val) {
  struct Node *curr = list_head;
  struct Node *prev = NULL;
  struct Node *new_node;

  while (curr != NULL && curr->value < val) {
    prev = curr;
    curr = curr->next;
  }

  if (curr == NULL || curr->value > val) {
    new_node = malloc(sizeof(struct Node));
    new_node->value = val;
    new_node->next = curr;
    if (prev == NULL) {
      list_head = new_node;
    } else {
      prev->next = new_node;
    }
    return 1;
  } else {
    return 0;
  }
}

/* Check if a value exists in the list */
int check_member(int val) {
  struct Node *curr = list_head;

  while (curr != NULL && curr->value < val) {
    curr = curr->next;
  }

  return (curr != NULL && curr->value == val);
}

/* Perform insert, delete, or member operations */
void perform_operations(int *op_arr) {
  for (long i = 0; i < total_operations; i++) {
    int val = generate_random_value();
    if (op_arr[i] == 1) {
      insert_value(val);
    } else if (op_arr[i] == -1) {
      delete_value(val);
    } else {
      check_member(val);
    }
  }
}

/* Delete a value from the linked list */
int delete_value(int val) {
  struct Node *curr = list_head;
  struct Node *prev = NULL;

  while (curr != NULL && curr->value < val) {
    prev = curr;
    curr = curr->next;
  }

  if (curr != NULL && curr->value == val) {
    if (prev == NULL) {
      list_head = curr->next;
    } else {
      prev->next = curr->next;
    }
    free(curr);
    return 1;
  } else {
    return 0;
  }
}

/* Free the linked list */
void free_linked_list() {
  struct Node *curr = list_head;
  struct Node *next_node;

  while (curr != NULL) {
    next_node = curr->next;
    free(curr);
    curr = next_node;
  }
}
