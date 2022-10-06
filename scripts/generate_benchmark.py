#!/usr/bin/env python3

import random
from pathlib import Path

from generate_test import parenthesize, explicit_concat_op, write_examples


def main():
    alphabets = "01"

    selected_sizes = random.choices(list(range(5000, 10000)), k=500)
    inputs = [
        ''.join(random.choices(alphabets, k=size)) for size in selected_sizes]
    results = ["yes" if size % 3 == 0 else "no" for size in selected_sizes]

    out_dir = Path("benchmark")
    out_dir.mkdir()
    write_examples(
        out_dir, "benchmark",
        parenthesize(explicit_concat_op("((0+1)(0+1)(0+1))*" * 150)),
        inputs, results)


if __name__ == "__main__":
    main()
