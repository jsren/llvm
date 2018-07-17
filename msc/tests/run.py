#!/usr/bin/python3
import os
import re
import subprocess
from typing import List

compiler = "/source/build-llvm/bin/clang++"
default_args = [compiler, '-Wno-everything', '-fzcexceptions', '-g', '-O0', '-std=c++17']

def split_alpha_num(s):
    vs = re.split('([0-9]+)', s)
    if 1 < len(vs) and vs[1]: vs[1] = int(vs[1])
    return vs

class Result:
    def __init__(self, outfile : str, args : List[str], succeeded : bool, msg : str):
        self.outfile = outfile
        self.args = args
        self.succeeded = succeeded
        self.msg = msg

def run(filename : str, outfile : str, expected_rc : int):
    args = list(default_args)
    args.extend(['-o', outfile, filename])
    try:
        proc = subprocess.run(args, timeout=10, check=True)
    except subprocess.TimeoutExpired:
        return Result(outfile, args, False, "Timeout exceeded when compiling")
    except subprocess.CalledProcessError as e:
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

if __name__ == "__main__":
    import re
    spec = re.compile("//\\s*expected:\\s*(\\d+).*")

    dir = os.path.dirname(__file__)
    for file in sorted(os.listdir(dir), key=split_alpha_num):
        filepath = os.path.join(dir, file)
        expected_rc = 0
        if filepath.endswith(".cpp"):
            with open(filepath, "r") as fs:
                for line in fs:
                    if not line:
                        continue
                    res = spec.match(line)
                    if res:
                        expected_rc = int(res.group(1))
                    else:
                        print("[WARN] Missing expected RC:", filepath)
                    break
            
            outfile = os.path.join(dir, "bin", file) + '.exe'
            res = run(filepath, outfile, expected_rc)
            if res.succeeded:
                print("[PASS]", file)
            else:
                print("[FAIL]", file, res.msg)
