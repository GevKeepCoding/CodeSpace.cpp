import gdb  
  
# Dictionary to hold counters for functions  
function_counters = {}  
  
class FunctionCallCounter(gdb.Breakpoint):  
    def __init__(self, function_name):  
        # Initialize the breakpoint on the given function  
        super(FunctionCallCounter, self).__init__(function_name, internal=True)  
        self.function_name = function_name  
        self.silent = True  # Suppress breakpoint hit messages  
        self.call_count = 0  
  
    def stop(self):  
        # Increment the call count each time the breakpoint is hit  
        self.call_count += 1  
        # Continue execution without stopping  
        return False  
  
class StartCountingFunctionCalls(gdb.Command):  
    """Start counting calls to a function.  
  
Usage: start_counting_calls FUNCTION_NAME  
"""  
  
    def __init__(self):  
        super(StartCountingFunctionCalls, self).__init__("start_counting_calls", gdb.COMMAND_USER)  
  
    def invoke(self, arg, from_tty):  
        function_name = arg.strip()  
        if not function_name:  
            print("Usage: start_counting_calls FUNCTION_NAME")  
            return  
  
        if function_name in function_counters:  
            print(f"Already counting calls to '{function_name}'.")  
            return  
  
        # Create a new counter for the function  
        counter = FunctionCallCounter(function_name)  
        function_counters[function_name] = counter  
        print(f"Started counting calls to '{function_name}'.")  
  
class ShowCallCounts(gdb.Command):  
    """Show the number of times functions have been called.  
  
Usage: show_call_counts  
"""  
  
    def __init__(self):  
        super(ShowCallCounts, self).__init__("show_call_counts", gdb.COMMAND_USER)  
  
    def invoke(self, arg, from_tty):  
        if not function_counters:  
            print("No functions are being counted.")  
            return  
        for function_name, counter in function_counters.items():  
            print(f"Function '{function_name}' has been called {counter.call_count} times.")  
  
# Register the commands with GDB  
StartCountingFunctionCalls()  
ShowCallCounts()  
