import subprocess
import re
import matplotlib.pyplot as plt
import argparse
import numpy as np

def parse_output(output):
    """Parses the output and extracts working times and input values."""
    times = []
    
    # Поиск чисел после working time:
    time_matches = re.findall(r'working time: (\d+\.?\d*)', output)


    if not time_matches:
      return [], []

    times = [float(time_match) for time_match in time_matches]

    return times, input_values

def run_binary(binary_path, args):
    """Executes the binary with given arguments and captures the output."""
    try:
      result = subprocess.run([binary_path] + args, capture_output=True, text=True, check=True)
      return result.stdout
    except subprocess.CalledProcessError as e:
        print(f"Error executing binary: {e}")
        return ""


def plot_times(times, input_values):
  """Plots working time vs binary input argument"""

  if not times or not input_values:
    print("No data to plot.")
    return

  print(len(times))
  print(len(input_values))

  times = np.array(times) / 1000000

  plt.figure(figsize=(10, 6))
  plt.plot(input_values, times, marker='o')
  plt.xlabel('Binary input arguments')
  plt.ylabel('Working Time (seconds)')
  plt.title('Working time vs binary input')
  plt.grid(True)
  plt.show()



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run a binary and plot working time vs input arguments.")
    parser.add_argument("binary_path", help="Path to the binary file.")
    parser.add_argument("--start", type=int, default=1, help="Start of loop")
    parser.add_argument("--end", type=int, default=10, help="End of loop")
    parser.add_argument("--step", type=int, default=1, help="Step of loop")

    args = parser.parse_args()

    binary_path = args.binary_path
    times = []
    input_values = []

    for input_val in range(args.start, args.end + 1, args.step):
      output = run_binary(binary_path, [str(input_val)])
      new_times, new_input_values = parse_output(output)
      times.extend(new_times)
      input_values.append(input_val)
      print(f"get data from paralleled on {input_val} threads, times {times}")


    plot_times(times, input_values)