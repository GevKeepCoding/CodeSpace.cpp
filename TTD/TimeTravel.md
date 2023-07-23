
## [Pacific++ 2018: James McNellis "Time Travel Debugging"](https://www.youtube.com/watch?v=BVslyei0804)




---
* esp and ebp should be close to each other.
if they are far away then this means we have currapted the stack.

To find who has currapted the stack pointer.
```
ba w 4 ValueOf(esp)
```
---

## to see all unique stacks.
```
!uniqstack



dx @$calls = @$cursession.TTD.Calls("kernelbase!GetLastError", "user32!MessageBoxW")

dx -g @$calls
dx -g @$calls.OrderBy(x => x.TimeStart).Where(x => x.ThreadId == 0x1634).Where(x=> x.ReturnValue != 0 || x.Function.Contains("MessageBox"))
dx @$calls.OrderBy(x => x.TimeStart).Where(x => x.ThreadId == 0x1634)
.Where(x=> x.ReturnValue != 0 || x.Function.Contains("MessageBox")).Count()

n10 to change base to decimal

dx @$calls.OrderBy(x => x.TimeStart).Where(x => x.ThreadId == 0x1634)
.Where(x=> x.ReturnValue != 0 || x.Function.Contains("MessageBox")).Skip(2900)

```
---
## To find who has currapted our memory
```
dx &myObject_Memory_That_Is_Currapted
dx -g @$cursession.TTD.Memory(StartAddress, StartAddress+8, "w")


dx (uint64_t*) Address | will show object at this address as if it was uint64_t object.
```


# create function breakpoint
```
bp kernelbase!CreateFileW "dx @$t0"
```

# create function breakpoint on my exe's class A member function fun.
```
bp myexe!A::fun "dx @$t0"
```

# q: windbg command to search for member function that when I don't know in which dll module this class is?
```
x myexe!A::*

```
// how to see all the threads in my app
```
~*k
```



