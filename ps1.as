MAIN: sub 4,@r4
mov Length,Y
Y: .data -8,+9
.extern Length
bne Y
.entry HEIGHT
HEIGHT: .string "HI"
lea HEIGHT,Length
