;;====================================
;; CS 2110 - Fall 2018
;; Homework 6
;;====================================
;; Name: Zachary Panzarino
;;====================================

.orig x3000

  AND R0, R0, 0 ; clear R0
  LD R0, A ; R0 = A
  NOT R0, R0 ; A = !A

  AND R1, R1, 0 ; clear R1
  LD R1, B ; R1 = B
  NOT R1, R1 ; B = !B

  AND R2, R0, R1 ; R7 = A & B
  NOT R2, R2 ; R7 = !R7
  ST R2, ANSWER ; answer = R7

HALT

A      .fill x1010
B      .fill x0404

ANSWER .blkw 1

.end
