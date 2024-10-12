import subprocess
import statistics
import math

NO_OF_SAMPLES = 100  # Number of samples used

# Function to compile source codes with error handling
def compile_all():
    sources = [
        ('linkedlistSerial.c', 'linkedlistSerial'),
        ('linkedlistMutex.c', 'linkedlistMutex'),
        ('linkedlistRWlock.c', 'linkedlistRWlock')
    ]

    for source, output in sources:
        try:
            subprocess.check_call(['gcc', '-g', '-Wall', '-o', output, source, '-lm', '-lpthread'])
            print(f"Compiled {source} successfully.")
        except subprocess.CalledProcessError:
            print(f"Error compiling {source}. Please check the source code.")

# Function to execute a given command, calculate statistics, and return results
def execute(command):
    elapsed_times = []
    
    for i in range(NO_OF_SAMPLES):
        try:
            # Capture the execution time from the subprocess
            time = subprocess.Popen(command, stdout=subprocess.PIPE).communicate()[0]
            elapsed_times.append(float(time))
        except Exception as e:
            print(f"Error executing command {command}: {e}")
            return None
    
    # Calculate statistics
    avg = statistics.mean(elapsed_times)
    std_dev = statistics.stdev(elapsed_times)
    required_samples = math.ceil(math.pow(((100 * 1.96 * std_dev) / (5 * avg)), 2))

    print(f"Average: {avg:.4f}")
    print(f"Standard Deviation: {std_dev:.4f}")
    print(f"Required Samples: {required_samples}")
    
    return avg, std_dev, required_samples

# Function to execute a list of commands
def execute_commands(commands, label):
    for i, command in enumerate(commands):
        print(f"{label} - Number of Threads: {2 ** i}")
        execute(command)
        print("")  # Print new line for readability

# Main function to handle execution flow
def main():
    # Compile the C source files
    compile_all()

    # Commands to be executed
    serial_commands = [
        ['./linkedlistSerial', '1000', '10000', '0.99', '0.005', '0.005'],
        ['./linkedlistSerial', '1000', '10000', '0.9', '0.05', '0.05'],
        ['./linkedlistSerial', '1000', '10000', '0.5', '0.25', '0.25']
    ]

    mutex_commands = [
        [
            ['./linkedlistMutex', '1000', '10000', '0.99', '0.005', '0.005', str(threads)]
            for threads in [1, 2, 4, 8]
        ],
        [
            ['./linkedlistMutex', '1000', '10000', '0.9', '0.05', '0.05', str(threads)]
            for threads in [1, 2, 4, 8]
        ],
        [
            ['./linkedlistMutex', '1000', '10000', '0.5', '0.25', '0.25', str(threads)]
            for threads in [1, 2, 4, 8]
        ]
    ]

    rw_commands = [
        [
            ['./linkedlistRWlock', '1000', '10000', '0.99', '0.005', '0.005', str(threads)]
            for threads in [1, 2, 4, 8]
        ],
        [
            ['./linkedlistRWlock', '1000', '10000', '0.9', '0.05', '0.05', str(threads)]
            for threads in [1, 2, 4, 8]
        ],
        [
            ['./linkedlistRWlock', '1000', '10000', '0.5', '0.25', '0.25', str(threads)]
            for threads in [1, 2, 4, 8]
        ]
    ]

    # Run experiments
    for case_idx in range(1, 4):
        print(f"=============== CASE {case_idx} ===============")
        print("Serial Linked List")
        print("=======")
        execute(serial_commands[case_idx - 1])
        print()

        print("Mutex Linked List")
        print("=======")
        execute_commands(mutex_commands[case_idx - 1], "Mutex")
        print()

        print("ReadWrite Linked List")
        print("=======")
        execute_commands(rw_commands[case_idx - 1], "RWLock")
        print()

if __name__ == '__main__':
    main()