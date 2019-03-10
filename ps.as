; file ps.as
.entry LENGTH 
.extern W
MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
L1: inc k
.entry LOOP
.extern LOL
jmp W 
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
k: .data 22
.extern L3
prn -5 
bne W
sub @r1, @r4 
bne L3

