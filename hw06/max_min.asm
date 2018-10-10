;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Zachary Panzarino
;;====================================

.orig x3000

  LD R0, HEAD_ADDR ; R0 = HEAD_ADDR
  BRz EMPTY ; if 0 go to empty

  AND R1, R1, 0 ; R1 = 0
  AND R2, R2, 0 ; R2 = 0
  ADD R1, R0, 1 ; R1 = node + 1
  LDR R1, R1, 0 ; R1 = value at R1 (max)
  ADD R2, R0, 1 ; R2 = node + 1
  LDR R2, R2, 0 ; R2 = value at R2 (min)

  LOOPS
    LDR R0, R0, 0 ; R0 = value of node
    BRz DONE ; if 0 go to done
    NOT R4, R1 ; R4 = !R1
    ADD R4, R4, 1 ; add 1 to R4 (calculate negative)
    AND R5, R5, 0 ; clear R5
    ADD R5, R0, 1 ; R5 = node + 1
    LDR R5, R5, 0 ; R5 = value at R5
    ADD R4, R5, R4 ; R4 = R5 - R4 (difference between current and max)
    BRp SETMAX ; if positive set new max
    AND R4, R4, 0 ; clear R4
    NOT R4, R2 ; R4 = NOT R2
    ADD R4, R4, 1 ; add 1 to R4 (calculate negative)
    AND R5, R5, 0 ; clear R5
    ADD R5, R0, 1 ; R5 = node + 1
    LDR R5, R5, 0 ; R5 = value at R5
    ADD R4, R5, R4 ; R4 = R5 - R4 (difference between current and min)
    BRn SETMIN ; if negative set new min
    BR LOOPS ; restart loop

  EMPTY ; when empty set outputs
    LD R6, MAX_INT ; R6 = MAX_INT
    LD R7, MIN_INT ; R7 = MIN_INT
    ST R6, ANSWER_MAX ; ANSWER_MAX = R6
    ST R7, ANSWER_MIN ; ANSWER_MIN = R7
    HALT

  SETMAX ; set max value
    AND R1, R1, 0 ; clear R1
    ADD R1, R5, R1 ; R1 = R5 (max)
    BR LOOPS ; restart loop

  SETMIN ; set min value
    AND R2, R2, 0 ; clear R2
    ADD R2, R5, R2 ; R2 = R5 (min)
    BR LOOPS ; restart loop

  DONE ; when done set outputs
    ST R1, ANSWER_MAX ; ANSWER_MAX = R1
    ST R2, ANSWER_MIN ; ANSWER_MIN = R2

HALT

HEAD_ADDR  .fill x4000

MAX_INT    .fill x7FFF
MIN_INT    .fill x8000

ANSWER_MAX .blkw 1
ANSWER_MIN .blkw 1

.end

.orig x4000
  .fill x4002         ;; Node 1
  .fill 4
  .fill x4004         ;; Node 2
  .fill 5
  .fill x4006         ;; Node 3
  .fill 25
  .fill x4008         ;; Node 4
  .fill 1
  .fill x0000         ;; Node 5
  .fill 10
.end
