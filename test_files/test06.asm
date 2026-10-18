; Student Name: Anushtup Kumar
; User ID: 2501CS32
; Declaration of Authorship: I declare that this code is my own work.

; This file tests instruction syntax and operand validation.

        ldc             ; ERROR: missing operand (ldc requires a value)
        
        add 15          ; ERROR: unexpected operand (add does not take an operand)
        
        fake_inst 5     ; ERROR: bogus mnemonic (fake_inst is not in instruction set)
        
        ldc 10 extra    ; ERROR: extra on end of line (only one operand allowed)
        
        HALT