;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Zachary Panzarino
;;====================================

.orig x3000

  LD R0, ARRAY_ADDR ; R0 = ARRAY_ADDR
  LD R1, ARRAY_LEN ; R1 = ARRAY_LEN

  AND R2, R2, 0 ; clear R2
  AND R3, R3, 0 ; clear R3
  ADD R3, R3, R0 ; load address into R3

  LOOP1 ; first loop
    AND R0, R0, 0 ; clear R0
    ADD R0, R3, 0 ; add R3 to R0
    ADD R1, R1, -1 ; decrement length value
    BRN DONE ; if loop value is 0 break loop

    LDR R2, R3, 0 ; load current array value to R2
    AND R4, R4, 0 ; clear R4
    ADD R4, R1, 0 ; R4 = remaining array length
    ADD R0, R0, R4 ; R0 = current array index

  LOOP2 ; second loop
    LDR R5, R0, 0 ; R5 = current array value
    NOT R6, R5 ; R6 = !R5
    ADD R6, R6, 1 ; increment R6 by 1
    ADD R6, R6, R2 ; find difference between R5 (through R6) and R2
    BRP SWAP ; if positive swap

  COUNTER ; decrement counter
    ADD R0, R0, -1 ; decrement current index
    ADD R4, R4, -1 ; decrement remaining index length
    BRN INCR3 ; if negative go to increment R3
    BR LOOP2 ; restart loop 2

  SWAP ; swap values
    STR R5, R3, 0 ; R5 = current
    STR R2, R0, 0 ; R2 = index value
    AND R2, R2, 0 ; clear R2
    ADD R2, R2, R5 ; R2 = R5
    BR COUNTER ; go to counter

  INCR3 ; increment R3
    ADD R3, R3, 1 ; increment R3
    BR LOOP1 ; restart loop 1

DONE ; go to when done
HALT

ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

.end

.orig x4000
  .fill 7
  .fill 18
  .fill 0
  .fill 5
  .fill 9
  .fill 25
  .fill 1
  .fill 2
  .fill 10
  .fill 6
.end
