## [Getting started with Natvis](https://aka.ms/vsnatvis)


10,h ->  to show hexidecimal
!    -> forces the raw view of the value. Ignores any visualization
na   -> Suppresses memory address of a pointer
nd   -> Suppresses any info for more derived classes.
en   -> Suppresses underlying integer value of Enum.

```natvis
<Type Name="MyCoolType">
 <Expand HideRawView="true">
  <Item Name="MyInt"> MyInt</Item>
  <Item Name="MyFloat">MyFloat</Item>
 </Expand>
</Type>
<Type Name="MyCoolType" IncludeView="Hex">
 <Expand HideRawView="true">
  <Item Name="MyInt"> MyInt,x</Item>
  <Item Name="MyFloat">*(int*)&amp;MyFloat,x</Item>
 </Expand>
</Type>
```
In locals window **myCoolVar,view(Hex)**

```xml
<Type Name="MyCoolType">
 <Expand HideRawView="true">
  <Item Name="MyInt"> MyInt</Item>
  <Item Name="MyFloat">MyFloat</Item>
  <Synthetic Name="FakeObject">
   <DisplayString> Not in This Type </DisplayString>
    <Expand>
     <Item Name="OtherFakeVar"> MyInt + MyFloat</Item>
    </Expand>
   </Synthetic>
 </Expand>
</Type>
```

```xml
<Type Name="FreeListVector&lt;*&gt;">
 <DisplayString> Size={m_Size}</DisplayString>
 <Expand>
  <Item Name="[Size]">m_Size</Item>
  <CustomListItems>
   <Variable Name="Index" InitialValue="0"/>
   <Variable Name="ItemIndex" InitialValue="0"/>
    <Loop Condition="Index &lt; m_Size">
     <If Condition="m_AliveList._Mypair._Myval2._Myfirst[Index]==1">
      <Item Name="[{ItemIndex}]">
       m_Data._Mypair._Myval2._Myfirst[Index]
      </Item>
      <Exec>++ItemIndex</Exec>
     </If>
     <Exec>++Index</Exec>
    </Loop>
  </CustomListItems>
 </Expand>
</Type>
```

---

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






