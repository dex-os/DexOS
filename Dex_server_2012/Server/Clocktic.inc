Timer_handler:
	pushad
	push  es
	push  ds
	MOV   AX,18H
	MOV   ES,AX
	MOV   DS,AX
	call  [stack_handler]
	pop   ds
	pop   es
	popad
	ret