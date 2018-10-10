;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Zachary Panzarino
;;====================================

.orig x3000

  AND R0, R0, 0 ; clear R0 (sum)
  AND R1, R1, 0 ; clear R1 (index)
  LD R3, ARRAY_LEN ; R3 = ARRAY_LEN
  NOT R3, R3 ; reverse R3 (to make it negative)
  ADD R3, R3, 1 ; add 1 to R3

  LOOPS ; start of loop

    ADD R4, R1, R3 ; checks value of index - LENGTH
    BRZP EXIT ; if index >= LENGTH, exit loop

    LD R2, ARRAY_ADDR ; R2 = ARRAY_ADDR
    ADD R2, R2, R1 ; R2 = address of index
    LDR R2, R2, 0 ; R2 = array[index]

    BRZP SKIP ; if array[index] >= 0, don't increment count
    ADD R0, R0, R2 ; adds negative value
    SKIP
    ADD R1, R1, 1 ; indrement index

  BR LOOPS ; go to start of loop

  EXIT

  ST R0, ANSWER ; ANSWER = sum

HALT


ARRAY_ADDR .fill x4000
ARRAY_LEN  .fill 10

ANSWER     .blkw 1

.end

.orig x4000
  .fill 7
  .fill -18
  .fill 0
  .fill 5
  .fill -9
  .fill 25
  .fill 1
  .fill -2
  .fill 10
  .fill -6
.end
