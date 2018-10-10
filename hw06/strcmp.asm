;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Zachary Panzarino
;;====================================

.orig x3000

LD R0, STR_ADDR_1 ; R0 = STR_ADDR_1
LD R1, STR_ADDR_2 ; R1 = STR_ADDR_2
AND R2, R2, 0 ; R2 = 0 (index)

LOOPS ; loop
  AND R3, R3, 0 ; clear R3
  ADD R3, R1, R2 ; R3 = difference of characters
  LDR R3, R3, 0 ; R3 = current character in second string
  AND R5, R5, 0 ; clear R5
  ADD R5, R3, 0 ; R5 = R3
  NOT R3, R3 ; R3 = !R3 (make negative)
  ADD R3, R3, 1 ; add 1 to R3 (make negative)
  ADD R4, R0, R2 ; R4 = R0 + R2 (address of current character first string)
  LDR R4, R4, 0 ; R4 = current character in first string
  ADD R5, R4, R5 ; R5 = sum of current characters
  BRz ZERO ; if 0 store 0
  ADD R3, R3, R4 ; R3 = difference of current characters
  BRn NEGATIVE ; if negative store -1
  BRp POSITIVE ; if positive store 1
  ADD R2, R2, 1 ; increment R2 (index)
  BR LOOPS ; restart loop

NEGATIVE ; save -1 as answer
  AND R6, R6, 0 ; clear R6
  ADD R6, R6, -1 ; R6 = -1
  ST R6, ANSWER ; ANSWER = R6
  BR DONE ; break loop

ZERO ; save 0 as answer
  AND R6, R6, 0 ; R6 = 0
  ST R6, ANSWER ; ANSWER = R6
  BR DONE ; break loop

POSITIVE ; save 1 as answer
  AND R6, R6, 0 ; clear R6
  ADD R6, R6, 1 ; R6 = 1
  ST R6, ANSWER ; ANSWER = R6
  BR DONE ; break loop

DONE ; go here when done
HALT

STR_ADDR_1 .fill x4000
STR_ADDR_2 .fill x4050

ANSWER     .blkw 1

.end

.orig x4000
  .stringz "This is a test"
.end

.orig x4050
  .stringz "This is a rest"
.end
