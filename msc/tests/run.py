#!/usr/bin/python3
import os
import re
import subprocess
from typing import List, Tuple, Any, Union

compiler = "/source/build-llvm/bin/clang++"
default_args = [compiler, '-Wno-everything', '-g', '-std=c++17']
variant_args = [('-fzcexceptions', '-fexceptions'), ('-O0', '-O3')]

def split_alpha_num(s) -> Tuple[Union[str, int]]:
    vs = re.split('([0-9]+)', s)
    if 1 < len(vs) and vs[1]: vs[1] = int(vs[1])
    return vs

class Result:
    def __init__(self, outfile : str, args : List[str], succeeded : bool, msg : str):
        self.outfile = outfile
        self.args = args
        self.succeeded = succeeded
        self.msg = msg

def run(filename : str, outfile : str, expected_rc : int, args : List[str]) -> Result:
    try:
        proc = subprocess.run(args, timeout=10, check=True)
    except subprocess.TimeoutExpired:
        return Result(outfile, args, False, "Timeout exceeded when compiling")
    except subprocess.CalledProcessError:
        return Result(outfile, args, False, "Compilation failed")
    try:
        proc = subprocess.run([outfile], timeout=5)
    except subprocess.TimeoutExpired:
        return Result(outfile, args, False, "Timeout exceeded when running")
    else:
        if proc.returncode != expected_rc:
            return Result(outfile, args, False, "Expected RC '{}', got '{}'".format(expected_rc, proc.returncode))
        else:
            return Result(outfile, args, True, "Passed")

def variant_iter(variants : List[Tuple[Any]]) -> List[Any]:
    indexes = [0] * len(variants)
    while True:
        yield [variants[i][indexes[i]] for i in range(len(indexes))]
        for i in range(len(indexes)):
            if indexes[i] < (len(variants[i]) - 1):
                indexes[i] += 1
                break
            else:
                indexes[i] = 0
                if i == (len(variants) - 1):
                    raise StopIteration()

def run_variants(filename : str, outfile : str, expected_rc : int):
    for var in variant_iter(variant_args):
        args = list(default_args)
        args.extend(var)
        args.extend(['-o', outfile, filename])
        yield run(filename, outfile, expected_rc, args)


import re
rc_spec = re.compile("//\\s*expected:\\s*(\\d+).*")

def test(filepath : str, outfile : str):
    expected_rc = 0
    with open(filepath, "r") as fs:
        for line in fs:
            if not line:
                continue
            res = rc_spec.match(line)
            if res:
                expected_rc = int(res.group(1))
            else:
                print("[WARN] Missing expected RC:", filepath)
            break
    for res in run_variants(filepath, outfile, expected_rc):
        if res.succeeded:
            print("[PASS]", filepath)
        else:
            print("[FAIL]", filepath, res.msg)


if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        for filepath in sys.argv[1:]:
            dir = os.path.dirname(filepath)
            filename = os.path.basename(filepath)
            outfile = os.path.join(dir, "bin", filename)
            test(filepath, outfile)
    else:
        dir = os.path.dirname(__file__)
        for file in sorted(os.listdir(dir), key=split_alpha_num):
            if file.endswith(".cpp"):
                filepath = os.path.join(dir, file)
                outfile = os.path.join(dir, "bin", file) + '.exe'
                test(filepath, outfile)
