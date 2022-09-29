#!/usr/bin/env python3

import random


def main():
    alphabets = ["0", "1"]
    sizes = [9999, 10000]
    with open("benchmark/benchmark.input.in", "w") as fi, open(
            "benchmark/benchmark.output.in", "w") as fo:
        selected_sizes = random.choices(sizes, k=1000)
        inputs = [''.join(random.choices(alphabets, k=size))
                  for size in selected_sizes]
        results = ["yes" if size % 3 == 0 else "no" for size in selected_sizes]
        fi.write(",".join(inputs) + "\n")
        fo.write(",".join(results) + "\n")


if __name__ == "__main__":
    main()
