import gdb
import os

class FuncTester(gdb.Command):

    def __init__(self):

        ##################################################
        # Create GDB Command name to execute invoke func #
        ##################################################
        super(FuncTester, self).__init__("test", gdb.COMMAND_USER)
    
    def invoke(self, filename, from_tty):

        #################################################
        # Executing, and fetching Return value Sequence #
        #################################################
        gdb.Breakpoint(filename)
        gdb.execute("r")
        gdb.execute("finish")
        ret = gdb.parse_and_eval("$rax")

        ################################
        # Storing Return Value to PIPE #
        ################################
        # create pipe path
        pipe_name = "/tmp/my_pipe"

        # Check if pipe name already exists
        if os.path.exists(pipe_name):
            os.unlink(pipe_name)

        # Tell OS to define pipe_name as PIPE
        os.mkfifo(pipe_name)

        # Open Pipe for communication and write value to it
        pipe = os.open(pipe_name, os.O_WRONLY)
        os.write(pipe, str(ret).encode())
        os.close(pipe)

        #############
        # Close GDB #
        #############
        gdb.execute("quit")

FuncTester()
