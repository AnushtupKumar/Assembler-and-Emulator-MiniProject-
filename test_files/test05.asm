; Student Name: Anushtup Kumar
; User ID: 2501CS32
; Declaration of Authorship: I declare that this code is my own work.

; This file tests label-related error handling.

0invalid:               ; ERROR: bogus label name (cannot start with a number)
        ldc 10

valid_label:
        adc 5

valid_label:            ; ERROR: duplicate label definition
        add

        br missing_lbl  ; ERROR: no such label (missing_lbl is never defined)
        
        HALT