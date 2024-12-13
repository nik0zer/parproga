import argparse
import matplotlib.pyplot as plt
import re

def parse_output(filename):
    """
    Parses the output file and extracts the b values and y(x) data.

    Args:
      filename: The name of the file containing the output data.

    Returns:
      A dictionary where keys are 'b' values and values are lists of (x, y) tuples.
    """
    data = {}
    current_b = None
    with open(filename, 'r') as f:
        for line in f:
             match_b = re.match(r'Solution for b = (\d+\.\d+) and \d+ points:', line)
             if match_b:
                current_b = float(match_b.group(1))
                data[current_b] = []
                continue
            
             match_xy = re.match(r'y\((\d+\.\d+)\) = (\d+\.\d+)', line)
             if match_xy:
                 x = float(match_xy.group(1))
                 y = float(match_xy.group(2))
                 data[current_b].append((x, y))
    return data


def plot_data(data):
    """
    Plots the y(x) data for each b value.

    Args:
      data: A dictionary of b values and their corresponding y(x) data, as returned by parse_output.
    """
    plt.figure(figsize=(10, 6))  # Adjust figure size as needed
    plt.xlabel("x")
    plt.ylabel("y(x)")
    plt.title("Solutions for different b values")

    for b, points in data.items():
        x_values = [point[0] for point in points]
        y_values = [point[1] for point in points]
        plt.plot(x_values, y_values, label=f"b = {b:.2f}")


    plt.legend()
    plt.grid(True)
    plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot y(x) data from a file.")
    parser.add_argument("filename", help="Path to the input file.")
    args = parser.parse_args()

    filename = args.filename
    parsed_data = parse_output(filename)
    plot_data(parsed_data)