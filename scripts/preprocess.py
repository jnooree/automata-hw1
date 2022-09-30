#!/usr/bin/env python3

import sys
from pathlib import Path
from collections import defaultdict
from typing import Generic, TypeVar

_T = TypeVar("_T")
alphabets = "e01"


class Stack(Generic[_T]):
    def __init__(self, *initial: _T):
        super().__init__()
        self._data = list(initial)

    def push(self, item: _T):
        self._data.append(item)

    def peek(self) -> _T:
        return self._data[-1]

    def pop(self) -> _T:
        return self._data.pop()

    def __len__(self) -> int:
        return len(self._data)


def explicit_concat_op(regex: str) -> str:
    chars = []
    prev = None
    for curr in regex:
        if prev:
            # Concat when:
            #   - current part is '(' and previous part is not '(', or
            #   - current part is alphabet and previous part is
            #     ')' or '*' or alphabet
            if ((curr == "(" and prev != "(")
                    or (curr in alphabets and prev in f")*{alphabets}")):
                chars.append(".")
        chars.append(curr)
        prev = curr
    return "".join(chars)


_precedence = defaultdict(int, {
    "*": 3,
    ".": 2,
    "+": 1,
})


def parenthesize(regex: str) -> str:
    regex = explicit_concat_op(regex)

    result = Stack[str]()
    ops = Stack[str]()

    def pop_join():
        op = ops.pop()
        rhs = result.pop()
        if op == "*":
            ret = f"({rhs}{op})"
        else:
            ret = f"({result.pop()}{op}{rhs})"
        result.push(ret)

    i = 0
    while i < len(regex):
        ch = regex[i]
        if ch in alphabets:
            result.push(ch)
        elif not ops or ch in "(*":
            ops.push(ch)
        elif ch == ")":
            while ops.peek() != "(":
                pop_join()
            ops.pop()
        else:
            preced = _precedence[ch]
            if preced > _precedence[ops.peek()]:
                ops.push(ch)
            else:
                while ops and preced <= _precedence[ops.peek()]:
                    pop_join()
                ops.push(ch)
        i += 1

    while ops:
        pop_join()
    return result.pop()


def split_cases(cases: str):
    return cases.strip().split(",")


def main():
    src = Path(sys.argv[1])
    with open(src, "r") as fi:
        lines = fi.read().splitlines()

    for i in range(0, len(lines) // 3):
        lineno = i * 3
        regex, inp, out = lines[lineno:lineno + 3]

        regex = parenthesize(regex)
        inp = split_cases(inp)
        out = split_cases(out)

        if len(inp) != len(out):
            raise ValueError("Input and output lengths do not match")

        with open(src.with_name(f"{i}.q1"), "w") as fo:
            fo.write(regex + "\n")

        with open(src.with_name(f"{i}.q2"), "w") as fo:
            fo.write(f"{len(inp)} {' '.join(inp)}\n")

        with open(src.with_name(f"{i}.a2"), "w") as fo:
            fo.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
