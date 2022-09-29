#!/usr/bin/env python3
import sys
from pathlib import Path


def main():
    inp = Path(sys.argv[1])
    with open(inp) as f:
        for i, line in enumerate(f, 1):
            cases = line.strip().split(",")
            with open(inp.with_name(f"{i}.q2"), "w") as f:
                f.write(f"{len(cases)} {' '.join(cases)}\n")

    out = Path(sys.argv[2])
    with open(out) as f:
        for i, line in enumerate(f, 1):
            results = line.strip().split(",")
            with open(out.with_name(f"{i}.a2"), "w") as f:
                f.write("\n".join(results) + "\n")


if __name__ == "__main__":
    main()
