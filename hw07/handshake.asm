;;====================================
;; CS 2110 - Fall 2018
;; Homework 7
;; handshake.asm
;;====================================
;; Name: Zachary Panzarino
;;====================================

; Little reminder from your pals: don't run this directly by pressing
; ``Run'' in complx, since (look below) there's nothing put at address
; x3000. Instead, load it and use ``Debug'' -> ``Simulate Subroutine
; Call'' and choose the ``strlen'' label.

.orig x3000

halt

handshake

    ADD R6, R6, -4 ; initialize space
    STR R7, R6, 2 ; save old RA
    STR R5, R6, 1 ; save old FP
    ADD R5, R6, 0 ; current FP
    ADD R6, R6, -5 ; move R6 to store reg on stack
    STR R0, R6, 4 ; R0 -> stack
    STR R1, R6, 3 ; R1 -> stack
    STR R2, R6, 2 ; R2 -> stack
    STR R3, R6, 1 ; R3 -> stack
    STR R4, R6, 0 ; R4 -> stack
    LDR R0, R5, 4 ; R0 <= n
    ADD R2, R0, 0 ; R2 = R0
    BRz ZERO ; if R2 == 0 return 0
    ADD R6, R6, -1 ; R6 - 1
    ADD R1, R0, -1 ; R1 = R0 - 1 (n - 1)
    ADD R6, R6, 1  ; R6 + 1 (params)
    ADD R2, R0, -1 ; R2 = R0 - 1 (n - 1)
    STR R2, R6, 0  ; save R2 (n - 1) as 1st param
    JSR handshake  ; call handshake on (n - 1)
    LDR R2, R6, 0  ; get return value
    ADD R6, R6, 2  ; R6 = R6 + 2 (top of stack)
    ADD R3, R1, R2 ; R3 = R1 + R2 (n - 1 + handshake(n - 1))
    STR R3, R5, 3  ; store return value
    BR EXIT

ZERO
    STR R0, R5, 3 ; place return value in stack
    BR EXIT ; exit

EXIT
    LDR R3, R5, -4 ; reload R3
    LDR R2, R5, -3 ; reload R2
    LDR R1, R5, -2 ; reload R1
    LDR R0, R5, -1 ; reload R0
    ADD R6, R5, 0 ; R6 = R5
    LDR R5, R6, 1 ; reset FP
    LDR R7, R6, 2 ; reset RA
    ADD R6, R6, 3 ; R6 = return value
    RET ; return


; Needed by Simulate Subroutine Call in complx
STACK .fill xF000
.end

; You should not have to LD from any label, take the
; address off the stack instead :)
