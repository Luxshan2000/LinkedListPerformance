import matplotlib.pyplot as plt

# Data for each case
data = {
    "Case 1": {
        "Serial": [20.91],
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
    "Case 2": {
        "Serial": [25.78],
        "Mutex": {
            1: 26.22,
            2: 38.41,
            4: 51.55,
            8: 53.34,
        },
        "ReadWrite": {
            1: 26.28,
            2: 27.25,
            4: 25.01,
            8: 26.98,
        },
    },
    "Case 3": {
        "Serial": [53.5],
        "Mutex": {
            1: 55.98,
            2: 74.65,
            4: 89.0,
            8: 90.96,
        },
        "ReadWrite": {
            1: 53.2,
            2: 94.87,
            4: 97.73,
            8: 102.17,
        },
    },
}


# Function to save plot as PNG
def save_plot(case_name, case_data, filename):
    plt.figure(figsize=(10, 6))

    # Serial Linked List
    serial_avg = case_data["Serial"][0]
    plt.axhline(y=serial_avg, color="r", linestyle="--", label="Serial")

    # Mutex Linked List
    mutex_threads = list(case_data["Mutex"].keys())
    mutex_avgs = list(case_data["Mutex"].values())
    plt.plot(mutex_threads, mutex_avgs, marker="o", label="Mutex", color="b")

    # ReadWrite Linked List
    rw_threads = list(case_data["ReadWrite"].keys())
    rw_avgs = list(case_data["ReadWrite"].values())
    plt.plot(rw_threads, rw_avgs, marker="o", label="ReadWrite", color="g")

    # Plot settings
    plt.title(f"Average Execution Time vs Number of Threads - {case_name}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Average Execution Time (ms)")
    plt.legend()
    plt.grid(True)

    # Save the plot
    plt.savefig(f"plots/{filename}.png")
    plt.close()


# Save plots for each case
for case, case_data in data.items():
    save_plot(case, case_data, case.replace(" ", "_"))
