; Student Name: Anushtup Kumar
; User ID: 2501CS32

; Sorts an array of 5 integers in ascending order

        ; 1. Initialize outer loop counter N
        ldc 5           
        stl 0           ; Store N at SP+0 

outer:  ldl 0
        ldc 1
        sub
        stl 0           ; N = N - 1
        ldl 0
        brz done        ; If N == 0, sorting is complete
        
        ldc 0           
        stl 1           ; Store 0 at SP+1 (Inner loop counter j)

inner:  ldl 1           ; Load j
        ldl 0           ; Load N
        sub             ; j - N
        brz outer       ; if j == N, inner loop is done, go to outer

        ; Load array[j]
        ldc array
        ldl 1
        add             ; Address of array[j]
        ldnl 0          ; A = array[j]
        stl 2           ; Store array[j] at SP+2

        ; Load array[j+1]
        ldc array
        ldl 1
        add             ; A = array + j
        ldc 1
        add             ; A = array + j + 1 (Correctly calculated!)
        ldnl 0          ; A = array[j+1]
        
        ; Compare array[j] and array[j+1]
        ldl 2           ; B = array[j+1], A = array[j]
        sub             ; A = array[j+1] - array[j]
        brlz swap       ; if array[j+1] < array[j], swap them
        br next         

swap:   ; Calculate and store Addr(j)
        ldc array
        ldl 1
        add             
        stl 3           ; Store Addr(j) at SP+3

        ; Calculate and store Addr(j+1)
        ldc array
        ldl 1
        add             
        ldc 1
        add             
        stl 4           ; Store Addr(j+1) at SP+4

        ; Perform swap in memory
        ldl 4
        ldnl 0          ; A = array[j+1]
        ldl 3           ; B = array[j+1], A = Addr(j)
        stnl 0          ; memory[Addr(j)] = array[j+1]

        ldl 2           ; A = original array[j]
        ldl 4           ; B = original array[j], A = Addr(j+1)
        stnl 0          ; memory[Addr(j+1)] = array[j]

next:   ldl 1
        ldc 1
        add
        stl 1           ; j = j + 1
        br inner        

done:   HALT

array:  data 45
        data 12
        data 78
        data 3
        data 22