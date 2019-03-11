sub @r4,KK
OUT: .data 6,76
STR: .string "abc"
.entry KK
mov 5,OUT
bne OUT
.extern KK
bne KK
