bt_storage = {}

def normalize_backtrace(bt_text):
    """
    Normalize a backtrace by extracting only function names from each stack frame.
    
    Example backtrace line:  
       "#0  my_function(int, char*) at source.cpp:10"  
    This function extracts "my_function" from such a line.  
    """  
    
    normalized_frames = []  
    # Regex: after frame number, take the first token up to the first '('.  
    pattern = re.compile(r'#\d+\s+(?P<func>[^\s(]+)')  
    for line in bt_text.splitlines():  
        m = pattern.search(line)  
        if m:  
            normalized_frames.append(m.group("func"))  
    return "->".join(normalized_frames)  


class NBTCollectorBreakpoint(gdb.Breakpoint):

    def init(self, funcname):
        # Set a breakpoint for the given function.
        super(NBTCollectorBreakpoint, self).init(spec=funcname, internal=False)
        self.funcname = funcname
        gdb.write("NBTCollectorBreakpoint: breakpoint set for function '%s'\n" % funcname)

    def stop(self):  
        """Called when the breakpoint is hit."""  
        global bt_storage  
        try:  
            bt_output = gdb.execute("bt", to_string=True)  
        except Exception as e:  
            gdb.write("ERROR: Could not capture backtrace: %s\n" % str(e))  
            return False  
    
        normalized = normalize_backtrace(bt_output)  
    
        if self.funcname not in bt_storage:  
            bt_storage[self.funcname] = {}  
    
        if normalized not in bt_storage[self.funcname]:  
            bt_storage[self.funcname][normalized] = bt_output  
            gdb.write("Collected a new unique (normalized) backtrace at '%s'.\n" % self.funcname)  
        return False  


class CollectNBTCommand(gdb.Command):
    """collect_nbt <function_name>
    Set a breakpoint at <function_name> that collects backtraces when hit.
    Only the function names (ignoring argument differences) are used for uniqueness.
    Example:
    (gdb) collect_nbt my_function
    """
    def init(self):
        super(CollectNBTCommand, self).init("collect_bt", gdb.COMMAND_USER)
    
    def invoke(self, argument, from_tty):  
        arg = argument.strip()  
        if not arg:  
            gdb.write("Usage: collect_bt <function_name>\n")  
            return  
        NBTCollectorBreakpoint(arg)  
        gdb.write("BT collection enabled for function '%s'.\n" % arg)  

class DumpNBTCommand(gdb.Command):
    """dump_nbt <function_name>
    Dump all unique backtraces (with function argument differences ignored) for <function_name>.
    Example:
    (gdb) dump_nbt my_function
    """
    def init(self):
        super(DumpNBTCommand, self).init("dump_bt", gdb.COMMAND_USER)

def invoke(self, argument, from_tty):  
    global bt_storage  
    arg = argument.strip()  
    if not arg:  
        gdb.write("Usage: dump_bt <function_name>\n")  
        return  
    if arg not in bt_storage or not bt_storage[arg]:  
        gdb.write("No backtraces collected for function '%s'.\n" % arg)  
        return  
    gdb.write("Dumping unique normalized backtraces for function '%s':\n" % arg)  
    for idx, (norm, bt) in enumerate(bt_storage[arg].items(), start=1):  
        gdb.write("---- Backtrace #%d ----\n" % idx)  
        gdb.write("Normalized signature: %s\n" % norm)  
        gdb.write(bt)  
        gdb.write("\n")  
    gdb.write("---- End of backtraces for '%s' ----\n" % arg)  

class WriteNBTCommand(gdb.Command):
    """write_bt
    Write all collected backtraces for each function into a log file.
    For each function, a file named "collect_bt_<function_name>.log" is created,
    containing all the unique normalized backtraces collected.
    Example:
    (gdb) write_bt
    """
    def init(self):
        super(WriteNBTCommand, self).init("write_bt", gdb.COMMAND_USER)

    def invoke(self, argument, from_tty):  
        global bt_storage  
        if not bt_storage:  
            gdb.write("No backtraces have been collected.\n")  
            return  
    
        for funcname, bt_dict in bt_storage.items():  
            file_name = "collect_bt_%s.log" % funcname  
            try:  
                with open(file_name, "w") as logfile:  
                    logfile.write("Collected unique backtraces for function '%s':\n\n" % funcname)  
                    for idx, (norm, bt) in enumerate(bt_dict.items(), start=1):  
                        logfile.write("---- Backtrace #%d ----\n" % idx)  
                        logfile.write("Normalized signature: %s\n" % norm)  
                        logfile.write(bt)  
                        logfile.write("\n")  
                    logfile.write("---- End of backtraces for '%s' ----\n" % funcname)  
                gdb.write("Wrote backtraces for '%s' to file '%s'.\n" % (funcname, file_name))  
            except Exception as e:  
                gdb.write("Error writing to file '%s': %s\n" % (file_name, e))  

# Register the commands.

CollectNBTCommand()
DumpNBTCommand()
WriteNBTCommand()