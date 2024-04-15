# Debugging using gdb


---

## Setting breakpoints

* To set address breakpoint(this is when we don't have the source code):
gdb: `break *0x4005f0`
lldb: `br s -a 0x4005f0 -s app`


* to skip current assembly instruction and jump to other instruction:
lldb: `jump *next_address_where_we_want_to_jump`
gdb: `jump *next_address_where_we_want_to_jump`??

* to set breakpoint only on current thread:
gdb: `break 10 if $_thread == pthread_self()`
lldb: `br s -t pthread_self() -f 10`


* gdb: `save breakpoints` | to save breakpoints to a script
* gdb: info line foo.c:42 | show PC for line
* gdb: info line *$pc | show line begin/end for current program counter


* tbreak  temporary breakpoint
* rbreak  reg-ex breakpoint
* command list of commands to be executed when breakpoint hit

---


## Watchpoints

1. watch foo | stop when foo is modified
1. watch -l foo | watch location
1. rwatch foo   | stop when foo is read
1. watch foo thread 3 | stop when thread 3 modifies foo
1. watch foo if foo > 10 

## Useful commands

* to attach to not yet started process:
lldb: `process attach --name a.out --waitfor`
* to find symbols if you have the address:
lldb: `image lookup -a 0x4005f0`
gdb:  ``

## Dynamic printf

* dprintf fun, "m is %p m->magic is %u\n", m,m->magic
* set dprintf-style gdb|call|agent
* set dprintf-function fprintf
* set dprintf-channel mylog





