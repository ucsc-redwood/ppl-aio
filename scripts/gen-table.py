import pandas as pd
import matplotlib.pyplot as plt

# Specify the path to your CSV file
csv_file = "benchmark_results.csv"

# Determine where the CSV data starts by finding the line that begins with 'name,iterations'
with open(csv_file, "r") as f:
    lines = f.readlines()

# Find the index of the line that starts with 'name,iterations'
csv_start_line = None
for idx, line in enumerate(lines):
    if line.strip().startswith("name,iterations"):
        csv_start_line = idx
        break

if csv_start_line is None:
    raise ValueError("Could not find CSV header in the file.")

# Read the CSV data from the line where the actual data starts
df = pd.read_csv(csv_file, skiprows=csv_start_line)


# Parse the 'name' column to extract 'Stage' and 'Threads'
def parse_benchmark_name(name):
    parts = name.split("/")
    if len(parts) >= 3:
        stage = parts[1]
        threads_str = parts[2]
        try:
            threads = int(threads_str)
        except ValueError:
            threads = 1  # Default to 1 if parsing fails
        return stage, threads
    else:
        return name, 1


df["Stage"], df["Threads"] = zip(*df["name"].map(parse_benchmark_name))


# Convert 'real_time' to milliseconds based on 'time_unit'
def convert_time(time_value, time_unit):
    if time_unit == "ms":
        return time_value
    elif time_unit == "s":
        return time_value * 1000
    elif time_unit == "us":
        return time_value / 1000
    elif time_unit == "ns":
        return time_value / 1e6
    else:
        return time_value  # Assume milliseconds if unit is unknown


df["Time_ms"] = df.apply(
    lambda row: convert_time(row["real_time"], row["time_unit"]), axis=1
)

# Plot the data
stages = df["Stage"].unique()

plt.figure(figsize=(12, 8))

for stage in stages:
    stage_df = df[df["Stage"] == stage]
    stage_df = stage_df.sort_values("Threads")
    plt.plot(stage_df["Threads"], stage_df["Time_ms"], marker="o", label=stage)

plt.xlabel("Number of Threads")
plt.ylabel("Time (ms)")
plt.title("Benchmark Results")
plt.legend()
plt.grid(True)
plt.tight_layout()

# plt.show()


# Save the plot as an image file instead of displaying it
output_file = 'benchmark_results.png'
plt.savefig(output_file)
print(f"Plot saved as {output_file}")
