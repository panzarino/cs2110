;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; buildheap.asm
;;====================================
;; Name: Zachary Panzarino
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt

heapify

    ADD R6, R6, -3 ; initialize space
    STR R7, R6, 1 ; save old RA
    STR R5, R6, 0 ; save old FP
    ADD R6, R6, -5 ; move R6 to store reg on stack
    ADD R5, R6, 5 ; current FP
    STR R0, R6, 4 ; R0 -> stack
    STR R1, R6, 3 ; R1 -> stack
    STR R2, R6, 2 ; R2 -> stack
    STR R3, R6, 1 ; R3 -> stack
    STR R4, R6, 0 ; R4 -> stack

    LDR R0, R5, 5 ; R0 = i (largest)
    LDR R1, R5, 5 ; R1 = i
    ADD R1, R1, R1 ; R1 = 2 * R1 (2 * i)
    ADD R1, R1, 1 ; R1 + 1 (left child)
    LDR R3, R5, 4 ; R3 = n
    LDR R4, R5, 3 ; R4 = array
    NOT R3, R3 ; reverse R3 (to make neg)
    ADD R3, R3, 1 ; R3 += 1 (neg)
    ADD R3, R1, R3 ; R3 = R3 - R1 (n - left child)
    BRzp RIGHT ; if R3 >= 0
    AND R3, R3, 0 ; clear R3
    ADD R3, R1, R4 ; R3 = R1 + R4 (left child + array)
    LDR R3, R3, 0 ; R3 = array[left child]
    ADD R2, R0, R4 ; R2 = R0 + R4 (largest + array)
    LDR R2, R2, 0 ; R2 = array[largest]
    NOT R3, R3 ; reverse R3 (to make neg)
    ADD R3, R3, 1 ; R3 += 1 (neg)
    ADD R3, R3, R2 ; R2 - R3 (check if largest)
    BRn SETLEFT ; if R3 < 0 set left largest

RIGHT
    LDR R2, R5, 5 ; R2 = i
    ADD R2, R2, R2 ; R2 = 2 * R2 (2 * i)
    ADD R2, R2, 2 ; R2 += 2 (right child)
    LDR R3, R5, 4 ; R3 = n
    NOT R3, R3 ; reverse R3
    ADD R3, R3, R2 ; R3 = R3 - R2
    BRzp LNR ; if R3 >= 0
    AND R3, R3, 0 ; clear R3
    ADD R3, R2, R4 ; R3 = R2 + R4 (right child + array)
    LDR R3, R3, 0 ; R3 = array[right child]
    ADD R1, R0, R4 ; R1 = R0 + R4 (largest + array)
    LDR R1, R1, 0 ; R0 = array[largest]
    NOT R3, R3 ; reverse R3 (neg)
    ADD R3, R3, 1 ; R3 += 1 (neg)
    ADD R3, R3, R1 ; R3 - R1 (check if largest)
    BRn SETRIGHT ; if R3 < 0 set right largest

LNR
    LDR R2, R5, 5 ; R2 = i
    NOT R2, R2 ; reverse R2 (neg)
    ADD R2, R2, 1 ; R2 += 1 (neg)
    ADD R2, R2, R0 ; R2 += R0 (largest - i)
    BRz EXIT ; if R2 == 0 then exit
    BR SWAP ; otherwise swap

SETLEFT
    AND R0, R0, 0 ; clear R0
    ADD R0, R0, R1 ; set R0 = R1 (largest = left child)
    BR RIGHT ; go to right

SETRIGHT
    AND R0, R0, 0 ; clear R0
    ADD R0, R0, R2 ; set R0 = R2
    BR LNR ; go to lnr

SWAP
    ADD R1, R0, R4 ; R1 = R0 + R4 (largest + array)
    LDR R3, R1, 0 ; R3 = array[largest]
    LDR R2, R5, 5 ; R2 = i
    ADD R2, R2, R4 ; R2 += R4 (array + i)
    LDR R4, R2, 0 ; R4 = array[i]
    STR R3, R2, 0 ; swap
    STR R4, R1, 0 ; swap
    LDR R3, R5, 4 ; R3 = n
    LDR R4, R5, 3 ; R4 = array
    ADD R6, R6, -3 ; move R6 by 3
    STR R4, R6, 0 ; save array
    STR R3, R6, 1 ; save n
    STR R0, R6, 2 ; save largest
    JSR heapify ; call heapify
    ADD R6, R6, 4 ; move R6 back

EXIT
    STR R0, R5, 3 ; place return value in stack
    LDR R4, R6, 0 ; reload R4
    LDR R3, R6, 1 ; reload R3
    LDR R2, R6, 2 ; reload R2
    LDR R1, R6, 3 ; reload R1
    LDR R0, R6, 4 ; reload R0
    ADD R6, R6, 5 ; stack pointer
    LDR R5, R6, 0 ; reload FP
    LDR R7, R6, 1 ; reload RA
    ADD R6, R6, 2 ; put pointer at return value
    RET ; return


buildheap

    ADD R6, R6, -3 ; initialize space
    STR R7, R6, 1 ; save old RA
    STR R5, R6, 0 ; save old FP
    ADD R6, R6, -5 ; move R6 to store reg on stack
    ADD R5, R6, 5 ; current FP
    STR R0, R6, 4 ; R0 -> stack
    STR R1, R6, 3 ; R1 -> stack
    STR R2, R6, 2 ; R2 -> stack
    STR R3, R6, 1 ; R3 -> stack
    STR R4, R6, 0 ; R4 -> stack

    LDR R0, R5, 3 ; R0 = array
    LDR R1, R5, 4 ; R1 = n
    AND R2, R2, 0 ; R2 = i
    ADD R2, R1, R2 ; R2 = R1 + R2 (n + 1)

LOOPS
    BRn EXIT ; if negative then exit
    ADD R6, R6, -3 ; move R6 by 3
    STR R0, R6, 0 ; save array
    STR R1, R6, 1 ; save n
    STR R2, R6, 2 ; save i
    JSR heapify ; call heapify
    ADD R6, R6, 4 ; move R6 back
    ADD R2, R2, -1 ; dec i
    BR LOOPS ; loop

EXIT
    STR R0, R5, 3 ; place return value in stack
    LDR R4, R6, 0 ; reload R4
    LDR R3, R6, 1 ; reload R3
    LDR R2, R6, 2 ; reload R2
    LDR R1, R6, 3 ; reload R1
    LDR R0, R6, 4 ; reload R0
    ADD R6, R6, 5 ; stack pointer
    LDR R5, R6, 0 ; reload FP
    LDR R7, R6, 1 ; reload RA
    ADD R6, R6, 2 ; put pointer at return value
    RET ; return


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
