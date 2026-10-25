; Student Name: Anushtup Kumar
; User ID: 2501CS32
; Declaration of Authorship: I declare that this code is my own work.

; Sum array of [10, 20, 30] using ldnl offset reading
ldc array
ldnl 0      ; A = 10
ldc array
ldnl 1      ; B = 10, A = 20
add         ; A = 30
ldc array
ldnl 2      ; B = 30, A = 30
add         ; A = 60
HALT

array: 
data 10
data 20
data 30