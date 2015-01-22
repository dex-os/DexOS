align 4
iniciomemoria1:
	dd 0,0,0,0,0,0

align 4
iniciomemoria:
	dd -(iniciomemoria+4),-(iniciomemoria+4),(iniciomemoria+4),.l1,0
     .l1 dd 0
iniciomemoriaPacker rb 100
align 4
fin:	    rb	200000h
