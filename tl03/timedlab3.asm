
;;====================================================
;; CS 2110 - Fall 2018
;; Timed Lab 3
;; timedlab3.asm
;;====================================================
;; Name: Zachary Panzarino
;;====================================================

.orig x3000

LD R0, STR_ADDR
AND R1, R1, 0
LD R5, SPACE
LD R6, LOW_OFFSET
LD R7, HIGH_OFFSET

LOOPS
  AND R2, R2, 0
  ADD R2, R1, R0
  LDR R3, R2, 0
  BRz DONE
  AND R4, R4, 0
  ADD R4, R4, R6
  ADD R4, R4, R3
  BRn SWAP
  AND R4, R4, 0
  ADD R4, R4, R7
  ADD R4, R4, R3
  BRp SWAP
  ADD R1, R1, 1
  BR LOOPS

SWAP
  AND R4, R4, 0
  ADD R4, R4, R5
  STR R4, R2, 0
  BR LOOPS

DONE HALT

SPACE .fill 32
LOW_OFFSET .fill -48
HIGH_OFFSET .fill -57
STR_ADDR .fill x5000
.end

.orig x5000
  .stringz "asdfasdfasdf"
.end
