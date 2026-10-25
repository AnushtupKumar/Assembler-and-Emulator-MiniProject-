; Student Name: Anushtup Kumar
; User ID: 2501CS32
; Declaration of Authorship: I declare that this code is my own work.

; Compute 2x^2 + 3x + 4 for x = 5
ldc 5       ; Load x = 5
ldc 5       
mul         ; A = 25 (x^2)
ldc 2       
mul         ; A = 50 (2x^2)

ldc term1   ; Load address of term1
stnl 0      ; Store 50 into memory at term1

ldc 5       ; Load x = 5
ldc 3       
mul         ; A = 15 (3x)

ldc term1   
ldnl 0      ; Load 50 from term1. B = 15, A = 50
add         ; A = 65 (2x^2 + 3x)

ldc 4       ; B = 65, A = 4
add         ; A = 69 (Final Result)
HALT

term1: data 0