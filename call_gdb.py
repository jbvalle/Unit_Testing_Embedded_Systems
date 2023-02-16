import sys
import subprocess

#####################
# Check num of args #
#####################
if len(sys.argv) != 3:
    print("ERROR: Expected arg1: main, arg2: test_func")

##############
# Parse Args #
##############
test_bin  = sys.argv[1]
test_func = sys.argv[2]
python_gdb_script = "func_tester.py"

###############
# Execute GDB #
###############
cmd = "gdb {} -batch -x {} -ex \"test {}\"".format(test_bin, python_gdb_script, test_func)

subprocess.check_output(cmd, shell=True)
