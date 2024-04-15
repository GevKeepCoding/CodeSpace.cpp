import gdb

bt_storage = {}

class BTCollectorBreakpoint(gdb.Breakpoint):
    def __init__(self, funcname):
        super(BTCollectorBreakpoint, self).__init__(spec=funcname, internal=False)
        self.funcname = funcname
        gdb.write("BTCollectorBreakpoint: breakpoint set for function '%s'\n" % funcname)

    def stop(self):
        global bt_storage
        try:
            bt_output = gdb.execute("bt", to_string=True)
        except Exception as e:
            gdb.write("ERROR: Could not capture backtrace: %s\n" % str(e))
            return False

        if self.funcname not in bt_storage:
            bt_storage[self.funcname] = set()

        if bt_output not in bt_storage[self.funcname]:
            bt_storage[self.funcname].add(bt_output)
            gdb.write("Collected a new unique backtrace at %s.\n" % self.funcname)
        return False

class CollectBTCommand(gdb.Command):
    """collect_bt <function_name>
    Set a breakpoint at <function_name>. Each time that function is entered,
    the backtrace is collected if it is unique.
    Example:
    (gdb) collect_bt my_function
    """
    def __init__(self):
        super(CollectBTCommand, self).__init__("collect_bt", gdb.COMMAND_USER)

    def invoke(self, argument, from_tty):
        arg = argument.strip()
        if not arg:
            gdb.write("Usage: collect_bt <function_name>\n")
            return
        BTCollectorBreakpoint(arg)
        gdb.write("BT collection enabled for function '%s'.\n" % arg)

class DumpBTCommand(gdb.Command):
    """dump_bt <function_name>
    Print all unique backtraces collected for <function_name> so far.
    Example:
    (gdb) dump_bt my_function
    """
    def __init__(self):
        super(DumpBTCommand, self).__init__("dump_bt", gdb.COMMAND_USER)

    def invoke(self, argument, from_tty):
        global bt_storage
        arg = argument.strip()
        if not arg:
            gdb.write("Usage: dump_bt <function_name>\n")
            return
        if arg not in bt_storage or not bt_storage[arg]:
            gdb.write("No backtraces collected for function '%s'.\n" % arg)
            return
        gdb.write("Dumping unique backtraces for function '%s':\n" % arg)
        for idx, bt in enumerate(bt_storage[arg], start=1):
            gdb.write("---- Backtrace #%d ----\n" % idx)
            gdb.write(bt)
            gdb.write("\n")
        gdb.write("---- End of backtraces for '%s' ----\n" % arg)


class WriteBTCommand(gdb.Command):
    """write_bt
    Write all collected backtraces for each function into a log file.
    For each function, a file named "collect_bt_<function_name>.log" is created,
    containing all the unique normalized backtraces collected.
    Example:
    (gdb) write_bt
    """
    def init(self):
        super(WriteBTCommand, self).init("write_bt", gdb.COMMAND_USER)

    def invoke(self, argument, from_tty):
        global bt_storage
        arg = argument.strip()
        if not arg:
            gdb.write("Usage: write_bt <function_name>\n")
            return
        if arg not in bt_storage or not bt_storage[arg]:
            gdb.write("No backtraces collected for function '%s'.\n" % arg)
            return
        gdb.write("Dumping unique backtraces for function '%s':\n" % arg)
        file_name = "collect_bt_%s.log" % arg
        try:
            with open(file_name, "w") as logfile:
                logfile.write("Collected unique backtraces for function '%s':\n\n" % arg)
                for idx, bt in enumerate(bt_storage[arg], start=1):
                    logfile.write("---- Backtrace #%d ----\n" % idx)
                    logfile.write(bt)
                    logfile.write("\n")
                logfile.write("---- End of backtraces for '%s' ----\n" % arg)
            gdb.write("Wrote backtraces for '%s' to file '%s'.\n" % (arg, file_name))
        except Exception as e:
            gdb.write("Error writing to file '%s': %s\n" % (file_name, e))
        
        # delete the backtrace storage for the function
        del bt_storage[arg]
        gdb.write("All backtraces for function '%s' have been written to file '%s'.\n" % (arg, file_name))
        gdb.write("All backtraces for function '%s' have been cleared from memory.\n" % arg)

    
# Register the commands
CollectBTCommand()
DumpBTCommand()
WriteBTCommand()
