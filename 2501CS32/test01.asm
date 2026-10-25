; Student Name: Anushtup Kumar
; User ID: 2501CS32
; Declaration of Authorship: I declare that this code is my own work.

; Compute mean of 4 numbers: (10 + 20 + 30 + 40) / 4
ldc 10
ldc 20
add         ; A = 30
ldc 30
add         ; A = 60
ldc 40
add         ; A = 100 (Sum)

ldc 4       ; B = 100 (Sum), A = 4 (Count)
div         ; A = B / A = 100 / 4 = 25
HALT