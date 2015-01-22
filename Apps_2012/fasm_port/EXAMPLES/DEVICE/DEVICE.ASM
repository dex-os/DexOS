
; fasm example of writing DOS device driver

format binary as 'SYS'

dd 0FFFFFFFFh				; reserved for next device pointer
dw 0A000h				; device attributes
dw strategy				; offset of strategy routine
dw interrupt				; offset of interrupt routine
db 'DEVICE  '				; blank-padded device name

strategy:
	mov	word [cs:request_header],bx
	mov	word [cs:request_header+2],es
	retf

interrupt:
	pusha
	push	ds es
	push	cs
	pop	ds
	les	bx,[request_header]
	mov	al,byte [es:bx+2]
	or	al,al
	jz	initialize_device
	cmp	al,4
	je	device_input
    interrupt_done:
	mov	word [es:bx+3],100h
	pop	es ds
	popa
	retf

initialize_device:
	mov	word [es:bx+0Eh],device_end
	mov	word [es:bx+10h],cs
	jmp	interrupt_done

device_input:
	push	es
	mov	cx,[es:bx+12h]
	les	di,[es:bx+0Eh]
	mov	al,1Ah
	rep	stosb
	pop	es
	jmp	interrupt_done

request_header dd ?

device_end:
