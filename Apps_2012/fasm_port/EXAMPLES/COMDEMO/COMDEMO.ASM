
; fasm example of writing 16-bit COM program

	org	100h			; code starts at offset 100h
	use16				; use 16-bit code

display_text = 9

	mov	ah,display_text
	mov	dx,hello
	int	21h

	int	20h

hello db 'Hello world!',24h
