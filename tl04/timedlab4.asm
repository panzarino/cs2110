
;;====================================================
;; CS 2110 - Fall 2018
;; Timed Lab 4
;; converge.asm
;;====================================================
;; Name: Zachary Panzarino
;;====================================================

.orig x3000

;; Don't try to run this code directly, since it only contains
;; subroutines that need to be invoked using the LC-3 calling
;; convention. Use Debug > Setup Test or Simulate Subroutine
;; Call in complx instead.
;;
;; Do not remove this line or you will break...
;; 'Simulate Subroutine Call'

halt

converge

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

    LDR R0, R5, 4 ; r0 = n

    AND R1, R1, 0
    ADD R1, R0, -1 ; check if n - 1 = 0 (n = 1)
    BRz ZERO

    AND R2, R2, 0
    ADD R2, R2, 2
    LD R4, DIV_ADDR

    STR R0, R6, -2
    STR R2, R6, -1
    ADD R6, R6, -2

    JSRR R4

    LDR R3, R6, 0 ; r3 = divide return value
    ADD R6, R6, 3

    AND R2, R2, 0
    ADD R2, R2, 1
    AND R2, R0, R2 ; r2 = mod
    BRz MOD
    BR ELSE

MOD
    STR R3, R6, -1
    ADD R6, R6, -1
    JSR converge
    LDR R4, R6, 0
    ADD R6, R6, 2
    ADD R4, R4, 1
    STR R4, R5, 3
    BR DONE

ELSE
    AND R4, R4, 0
    ADD R4, R4, R0
    ADD R4, R4, R0
    ADD R4, R4, R0
    ADD R4, R4, 1
    STR R4, R6, -1
    ADD R6, R6, -1
    JSR converge
    LDR R4, R6, 0
    ADD R6, R6, 2
    ADD R4, R4, 1
    STR R4, R5, 3
    BR DONE

ZERO
    AND R1, R1, 0
    STR R1, R5, 3
    BR DONE

DONE
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

STACK    .fill xF000
DIV_ADDR .fill x6000 ;; Call the divide subroutine at
                     ;; this address!

.end

;;====================================================
;;   divide(n, d): Takes a numerator (n) and
;;                 denominator (d), returning n / d.
;;====================================================
.orig x6000
divide  ;; DO NOT call JSR with this label! Use DIV_ADDR instead!
  .fill x1DBD
  .fill x7F81
  .fill x7B80
  .fill x1BBF
  .fill x1DBB
  .fill x7140
  .fill x737F
  .fill x757E
  .fill x777D
  .fill x797C
  .fill x6144
  .fill x6345
  .fill x54A0
  .fill x1620
  .fill x987F
  .fill x1921
  .fill x1903
  .fill x0805
  .fill x14A1
  .fill x987F
  .fill x1921
  .fill x16C4
  .fill x0FF7
  .fill x7543
  .fill x6140
  .fill x637F
  .fill x657E
  .fill x677D
  .fill x697C
  .fill x1D61
  .fill x6B80
  .fill x1DA1
  .fill x6F80
  .fill x1DA1
  .fill xC1C0
.end
