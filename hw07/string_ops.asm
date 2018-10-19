;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; string_ops.asm
;;====================================
;; Name: Zachary Panzarino
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt

strlen
    ADD R6, R6, -3   ;initialize space for everything
    STR R7, R6, 1    ;store RA
    STR R5, R6, 0    ;store old FP
    ADD R6, R6, -4   ;initialize space for registers
    ADD R5, R6, 3    ;place new FP
    STR R0, R6, 3    ;store R0 on the stack
    STR R1, R6, 2    ;store R1 on the stack
    STR R2, R6, 1    ;store R2 on the stack
    STR R3, R6, 0    ;store R3 on the stack

    LDR R0, R5, 4    ;load the address of string into R0
    AND R1, R1, 0    ;reset R1 to 0, it'll be the length

LOOPSTRING
    ADD R2, R0, R1   ;R2 -> address of current character
    LDR R2, R2, 0    ;R2 -> value of current character

    BRZ ENDSTRING    ;if value of character is 0, reached the end

    ADD R1, R1, 1    ;increment string length
    BR LOOPSTRING

ENDSTRING
    STR R1, R5, 3    ;store strlen at the return value spot


TEARDOWN
    LDR R3, R6, 0   ;reload R3
    LDR R2, R6, 1   ;reload R2
    LDR R1, R6, 2   ;reload R1
    LDR R0, R6, 3   ;reload R0
    ADD R6, R6, 4   ;add the stack pointer
    LDR R5, R6, 0   ;reload old FP
    LDR R7, R6, 1   ;reload return address
    ADD R6, R6, 2   ;put the stack pointer at the rv
    RET



count_occurrence

    ADD R6, R6, -3 ; create space for everything
    STR R7, R6, 1 ; save RA
    STR R5, R6, 0 ; save old FP
    ADD R6, R6, -5 ; create register space
    ADD R5, R6, 4 ; new FP
    STR R0, R6, 4 ; R0 -> stack
    STR R1, R6, 3 ; R1 -> stack
    STR R2, R6, 2 ; R2 -> stack
    STR R3, R6, 1 ; R3 -> stack
    STR R4, R6, 0 ; R4 -> stack

    AND R0, R0, 0 ; R0 = 0 (count)
    LDR R1, R5, 4 ; R1 = string addr
    LDR R2, R5, 5 ; R2 = character
    STR R1, R6, -1 ; put string at top of stack
    ADD R6, R6, -1 ; R6 - 1 (decrement)
    JSR strlen ; call strlen
    LDR R3, R6, 0 ; get return value (length of string)
    ADD R6, R6, 2 ; remove return value and string from stack

LOOPS
    ADD R3, R3, 0 ; load R3 for BR
    BRnz EXIT ; if R3 <= 0 exit
    LDR R4, R1, 0 ; load char into R4
    NOT R4, R4 ; reverse (for subtraction)
    ADD R4, R4, 1 ; add 1 (for subtraction)
    ADD R4, R2, R4 ; subtract R4 from R2
    BRnp DECREMENT ; if R4 >= 0 then decrement counter
    ADD R0, R0, 1 ; increment count

DECREMENT
    ADD R3, R3, -1 ; decrement counter
    ADD R1, R1, 1 ; increment string addr
    BR LOOPS ; restart loop

EXIT
    STR R0, R5, 3 ; store return value in stack
    LDR R4, R6, 0 ; reload R4
    LDR R3, R6, 1 ; reload R3
    LDR R2, R6, 2 ; reload R2
    LDR R1, R6, 3 ; reload R1
    LDR R0, R6, 4 ; reload R0
    ADD R6, R6, 5 ; add stack pointer
    LDR R5, R6, 0 ; reload old FP
    LDR R7, R6, 1 ; reload return
    ADD R6, R6, 2 ; change stack pointer to return value
    RET ; return


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
