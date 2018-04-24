.text
.section	.text,"ax",%progbits
.align 4

.global s_mmap
.global s_munmap
.global __aeabi_idiv0
.global __aeabi_unwind_cpp_pr0
.global __aeabi_unwind_cpp_pr1
.align 4

__mmap2:
MOV     R12, SP
STMFD   SP!, {R4-R7}
LDMIA   R12, {R4-R6}
MOV     R7, #0xC0
SVC     0
LDMFD   SP!, {R4-R7}
MOVS    R0, R0
BXPL    LR
B       __set_syscall_errno
s_mmap:
PUSH    {R4,R5,LR}
SUB     SP, SP, #0xC
LDR     R4, [SP,#0x1C]
LSLS    R5, R4, #0x14
CMP     R5, #0
BEQ     loc_1AFFA
BL      __errno
MOVS    R3, #0x16
STR     R3, [R0]
MOVS    R0, #1
NEGS    R0, R0
B       loc_1B006
loc_1AFFA:
LDR     R5, [SP,#0x18]
LSRS    R4, R4, #0xC
STR     R4, [SP,#4]
STR     R5, [SP,#0]
BL     __mmap2
loc_1B006:
ADD     SP, SP, #0xC
POP     {R4,R5,PC}

s_munmap:
STMFD   SP!, {R4,R7}
MOV     R7, #0x58
SVC     0
LDMFD   SP!, {R4,R7}
MOVS    R0, R0
BXPL    LR
B       __set_syscall_errno

	__set_syscall_errno:
PUSH    {R4,LR}
MOVS    R3, R0
BLT     locret_26
NEGS    R0,R0
BL      __set_errno
locret_26:
POP     {R4,PC}
 __errno:
PUSH    {R4,LR}
LDR     R3, [PC,#0x8]
BLX     R3
ADDS    R0, #8
POP     {R4,PC}
.4byte  0xFFFF0FE0
__set_errno:
PUSH    {R4,LR}
MOVS    R4, R0
BL      __errno
STR     R4, [R0]
MOVS    R0, #1
NEGS    R0, R0
POP     {R4,PC}

__aeabi_idiv0:
NOP

__aeabi_unwind_cpp_pr0:
NOP

__aeabi_unwind_cpp_pr1:
NOP