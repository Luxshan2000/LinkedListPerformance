# README

## Program Overview

This program performs performance testing on three linked list implementations: 
1. Serial Linked List
2. Mutex-based Linked List
3. Read-Write Lock Linked List

The program compiles C source files and executes the compiled binaries under different configurations. After execution, statistics like average time and standard deviation are printed. Additionally, you can generate plots from the results to visualize the performance.

### Files:
1. `run.py`: Runs all test cases and prints the results (average time, standard deviation, required samples).
2. `generate_plots.py`: Generates plots for visualizing performance after replacing the results in the data dictionary.

## Prerequisites

Ensure you have the following installed:
- Python 3.x
- GCC (GNU Compiler Collection)
- `matplotlib` (for generating plots)

To install `matplotlib`, run:
```bash
pip install matplotlib
```

## Running the Program

### Step 1: Compile and Run Tests
1. Open a terminal in the project directory.
2. Run the following command to execute the performance tests:
   ```bash
   python run.py
   ```
   For macOS or Linux, use:
   ```bash
   python3 run.py
   ```

This will compile the source files (`linkedlistSerial.c`, `linkedlistMutex.c`, `linkedlistRWlock.c`) and run the test cases for each of them. Results will be printed to the console with the following statistics:
- **Average Time**
- **Standard Deviation**
- **Required Number of Samples**

### Step 2: Replace Average Times in `generate_plots.py`

After obtaining the results from `run.py`, replace the values under the `avg time` section in the `generate_plots.py` file. Here's how to do it:
1. Open the `generate_plots.py` file.
2. Update the `data` dictionary with the average times from the output of `run.py`.

Example:
```python
data = {
    "Case 1": {
        "Serial": [20.91],  # Replace with actual values from run.py output
        "Mutex": {
            1: 21.59,
            2: 32.59,
            4: 40.64,
            8: 42.14,
        },
        "ReadWrite": {
            1: 21.05,
            2: 13.4,
            4: 7.96,
            8: 6.14,
        },
    },
    # Similarly update Case 2 and Case 3
}
```

### Step 3: Generate Plots
After updating the values in the `data` dictionary, run the following command to generate performance plots:
```bash
python generate_plots.py
```
For macOS or Linux:
```bash
python3 generate_plots.py
```

This will generate plots visualizing the performance across different linked list implementations.

## Notes:
- Ensure you have proper write permissions to create and save the plots.
- The number of threads for the Mutex and ReadWrite Lock linked lists varies from 1, 2, 4, and 8 threads.