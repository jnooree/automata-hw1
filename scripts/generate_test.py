#!/usr/bin/env python3

import os
import re
import sys
import random
import tempfile
import itertools
import subprocess as sp
from pathlib import Path
from collections import defaultdict
from typing import Generic, TypeVar

from xeger import Xeger
from tqdm import tqdm
from joblib import Parallel, delayed

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


def to_posix(regex: str):
    regex = regex.replace(".", "")
    regex = regex.replace("+", "|")
    regex = re.sub(r"\*+", "*", regex)
    regex = regex.replace("e", "()")
    return regex


def _generatable(regex: str):
    regex = regex.replace("*", "{0,20}")
    return regex


def generate_examples(posix):
    inp = set()
    total_length = 0
    xeger = Xeger(limit=10000)

    sub_rex = _generatable(posix)
    for _ in range(100):
        ex = xeger.xeger(sub_rex)
        if not ex or ex in inp:
            continue

        total_length = total_length + len(ex)
        inp.add(ex)
        if total_length > 15000:
            break

    inp = list(inp)

    for _ in range(1000 - len(inp)):
        k = random.randint(1, 10000)
        length_sum = total_length + k
        if length_sum > 30000:
            continue

        total_length = length_sum
        s = "".join(random.choices("01", k=k))
        inp.append(s)

    return inp


def grep_eval(regex, inp):
    with tempfile.NamedTemporaryFile("w") as f:
        f.write("\n".join(inp) + "\n")
        f.flush()
        ret = sp.run(["grep", "-Exn", regex, f.name],
                     stdout=sp.PIPE, text=True, check=False)

    out = ["no"] * len(inp)
    for line in ret.stdout.splitlines():
        n = int(line.split(":")[0]) - 1
        out[n] = "yes"
    return out


def write_examples(out_dir: Path, prefix, rex, inp, out):
    with open(out_dir / f"{prefix}.q1", "w") as fo:
        fo.write(rex + "\n")

    with open(out_dir / f"{prefix}.q2", "w") as fo:
        fo.write(f"{len(inp)} " + " ".join(inp) + "\n")

    with open(out_dir / f"{prefix}.a2", "w") as fo:
        fo.write("\n".join(out) + "\n")


def generate(out_dir, i, regex):
    try:
        pr = parenthesize(regex)
        posix = to_posix(regex)

        inp = generate_examples(posix)
        out = grep_eval(posix, inp)

        write_examples(out_dir, f"{i:04d}", pr, inp, out)
    except Exception:
        print(regex)
        raise


_uop = ["", "*"]
_bop = [".", "+"]


def _cartesian_product(*args):
    return ["".join(prod) for prod in itertools.product(*args)]


def _all_terms():
    atoms = _cartesian_product(alphabets, _uop)
    terms = _cartesian_product(atoms, _bop, atoms)
    return atoms, [f"({t})" for t in terms]


def _all_regexes():
    atoms, terms = _all_terms()
    r1 = _cartesian_product(terms, _uop)
    r2 = _cartesian_product(r1, _bop, atoms, _uop)
    return r1 + [f"({r})" for r in r2]


def main():
    src = Path(sys.argv[1])
    out_dir = src.with_name("generated")
    out_dir.mkdir(parents=True)

    i = -1
    with src.open() as f:
        for i, line in enumerate(f):
            generate(out_dir, i, explicit_concat_op(line.rstrip()))

    regexes = _all_regexes()
    Parallel(n_jobs=os.cpu_count())(
        delayed(generate)(out_dir, j, regex)
        for j, regex in enumerate(tqdm(regexes), i + 1))


if __name__ == "__main__":
    main()
