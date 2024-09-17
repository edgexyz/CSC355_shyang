import os, sys, re
import subprocess
import difflib

TEST_DIR = "tests"

TEST_FILES = ['t000.in', 't001.in', 't002.in', 't003.in', 't004.in', 't005.in', 't006.in', 't007.in', 't008.in', 't009.in', 't010.in', 't011.in', 't012.in', 't020.in', 't021.in', 't022.in', 't023.in', 't024.in']

def main():

    for test_file in TEST_FILES:
        path = f"{TEST_DIR}/{test_file}"
        output = subprocess.run(["./parser", path], capture_output=True, text=True)
    
        stdout_is_pass = True
        stderr_is_pass = True

        with open(f"./tests/{test_file.split('.')[0]}.out") as f:
            lines = "".join(f.readlines())

            if lines != output.stdout:
                std_out_is_pass = False

        if output.stderr:
            if not os.path.exists(f"./tests/{test_file.split('.')[0]}.err"):
                if stdout_is_pass:
                    print (f"{test_file}...........Fail (stdout pass; stderr fail)")
                else:
                    print (f"{test_file}...........Fail (stdout fail; stderr fail)")

            with open(f"./tests/{test_file.split('.')[0]}.err") as f:
                lines = "".join(f.readlines())
                
                if lines != output.stderr:
                    stderr_is_pass = False

        if stdout_is_pass and stderr_is_pass:
            print (f"{test_file}...........Pass (stdout pass; stderr pass)")
        elif stdout_is_pass and not stderr_is_pass:
            print (f"{test_file}...........Fail (stdout pass; stderr fail)")
        elif stderr_is_pass and not stdout_is_pass:
            print (f"{test_file}...........Fail (stdout fail; stderr pass)")
        else:
            print (f"{test_file}...........Fail (stdout fail; stderr fail)")

    return

if __name__ == "__main__":
    main()
