# Learn Assembly Language

## 10 Most Used Assembly Instructions

1. **MOV**
   - Transfers data from one location to another.
   - Example: `MOV AX, BX` (Moves the value in BX to AX).

2. **ADD**
   - Adds two values and stores the result in the destination operand.
   - Example: `ADD AX, BX` (Adds BX to AX).

3. **SUB**
   - Subtracts the source operand from the destination operand.
   - Example: `SUB AX, BX` (Subtracts BX from AX).

4. **MUL**
   - Multiplies unsigned integers.
   - Example: `MUL BX` (Multiplies AX by BX, result in DX:AX).

5. **DIV**
   - Divides unsigned integers.
   - Example: `DIV BX` (Divides DX:AX by BX, quotient in AX, remainder in DX).

6. **CMP**
   - Compares two operands by subtracting one from the other without storing the result.
   - Example: `CMP AX, BX` (Compares AX with BX).

7. **JMP**
   - Unconditionally jumps to a specified address.
   - Example: `JMP LABEL` (Jumps to the instruction at LABEL).

8. **JE/JZ**
   - Jumps if equal or zero flag is set.
   - Example: `JE LABEL` (Jumps to LABEL if the zero flag is set).

9. **JNE/JNZ**
   - Jumps if not equal or zero flag is not set.
   - Example: `JNE LABEL` (Jumps to LABEL if the zero flag is not set).

10. **INT**
    - Triggers a software interrupt.
    - Example: `INT 21H` (Calls DOS interrupt 21H for system services).