import subprocess
import statistics
import math

NO_OF_SAMPLES = 100     # Number of samples used

# Compiling the source codes in C
def compileAll():
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linkedlistSerial', 'linkedlistSerial.c'])
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linkedlistMutex', 'linkedlistMutex.c', '-lm', '-lpthread'])
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linkedlistRWlock', 'linkedlistRWlock.c', '-lm', '-lpthread'])

# Execution of given process and calculation of average and standard deviation
def execute(command):
    elapsed_times = []
    for i in range(NO_OF_SAMPLES):
        time = subprocess.Popen(command, stdout=subprocess.PIPE).communicate()[0]
        elapsed_times.append(float(time))

    print('Average: ' + str(statistics.mean(elapsed_times)))
    print('Standard Deviation: ' + str(statistics.stdev(elapsed_times)))

    avg = statistics.mean(elapsed_times)
    standardDeviation = statistics.stdev(elapsed_times)
    samples = math.ceil(math.pow(((100 * 1.96 * standardDeviation) / (5 * avg)), 2))

    print('Samples: ' + str(samples))

# Execution of a list of commands
def executeCommands(cmds):
    for i in range(len(cmds)):
        print("Number of Threads: " + str(2**i))
        execute(cmds[i])
        print("")

# Commands to be executed
serial = [['./linkedlistSerial', '1000', '10000', '0.99', '0.005', '0.005'], ['./linkedlistSerial', '1000', '10000', '0.9', '0.05', '0.05'], ['./linkedlistSerial', '1000', '10000', '0.5', '0.25', '0.25']]
mutex_1 = [['./linkedlistMutex', '1000', '10000', '0.99', '0.005', '0.005', '1'], ['./linkedlistMutex', '1000', '10000', '0.99', '0.005', '0.005', '2'], ['./linkedlistMutex', '1000', '10000', '0.99', '0.005', '0.005', '4'], ['./linkedlistMutex', '1000', '10000', '0.99', '0.005', '0.005', '8']]
mutex_2 = [['./linkedlistMutex', '1000', '10000', '0.9', '0.05', '0.05', '1'], ['./linkedlistMutex', '1000', '10000', '0.9', '0.05', '0.05', '2'], ['./linkedlistMutex', '1000', '10000', '0.9', '0.05', '0.05', '4'], ['./linkedlistMutex', '1000', '10000', '0.9', '0.05', '0.05', '8']]
mutex_3 = [['./linkedlistMutex', '1000', '10000', '0.5', '0.25', '0.25', '1'], ['./linkedlistMutex', '1000', '10000', '0.5', '0.25', '0.25', '2'], ['./linkedlistMutex', '1000', '10000', '0.5', '0.25', '0.25', '4'], ['./linkedlistMutex', '1000', '10000', '0.5', '0.25', '0.25', '8']]
rw_1 = [['./linkedlistRWlock', '1000', '10000', '0.99', '0.005', '0.005', '1'], ['./linkedlistRWlock', '1000', '10000', '0.99', '0.005', '0.005', '2'], ['./linkedlistRWlock', '1000', '10000', '0.99', '0.005', '0.005', '4'], ['./linkedlistRWlock', '1000', '10000', '0.99', '0.005', '0.005', '8']]
rw_2 = [['./linkedlistRWlock', '1000', '10000', '0.9', '0.05', '0.05', '1'], ['./linkedlistRWlock', '1000', '10000', '0.9', '0.05', '0.05', '2'], ['./linkedlistRWlock', '1000', '10000', '0.9', '0.05', '0.05', '4'], ['./linkedlistRWlock', '1000', '10000', '0.9', '0.05', '0.05', '8']]
rw_3 = [['./linkedlistRWlock', '1000', '10000', '0.5', '0.25', '0.25', '1'], ['./linkedlistRWlock', '1000', '10000', '0.5', '0.25', '0.25', '2'], ['./linkedlistRWlock', '1000', '10000', '0.5', '0.25', '0.25', '4'], ['./linkedlistRWlock', '1000', '10000', '0.5', '0.25', '0.25', '8']]

mutex = [mutex_1, mutex_2, mutex_3]
rw = [rw_1, rw_2, rw_3]

# Compile all the files
compileAll()

# Execute and print the output
for i in range(1,4):
    print('=============== CASE: ' + str(i) + ' ===============')
    print('Serial linked list ')
    print('=======')
    execute(serial[i-1])
    print('')
    print('Mutex linked list ')
    print('=======')
    executeCommands(mutex[i-1])
    print('')
    print('ReadWrite linked list')
    print('=======')
    executeCommands(rw[i-1])
