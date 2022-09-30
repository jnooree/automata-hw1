#!/usr/bin/env python3

import os
import random


def main():
    alphabets = "01"
    sizes = [9999, 10000]

    selected_sizes = random.choices(sizes, k=1000)
    inputs = [
        ''.join(random.choices(alphabets, k=size)) for size in selected_sizes]
    results = ["yes" if size % 3 == 0 else "no" for size in selected_sizes]

    os.makedirs("benchmark", exist_ok=True)
    with open("benchmark/benchmark.in", "w") as f:
        f.write("((0+1)(0+1)(0+1))*" * 10 + "\n")
        f.write(",".join(inputs) + "\n")
        f.write(",".join(results) + "\n")


if __name__ == "__main__":
    main()
