;-----------------------------------------------;
; Test foreground and background colors and     ;
;display with number of that color in that color;
;-----------------------------------------------;
format binary as 'dex'
use32
	ORG   0x1A00000 			      ; where our program is loaded to
	jmp   start				     ; jump to the start of program.
	db    'DEX6'				     ; We check for this, to make sure it a valid DexOS file.

msg1:	db    'Color Test ',13,10,24h

 ;----------------------------------------------------;
 ; Start of program.                                  ;
 ;----------------------------------------------------;

start:
	mov	ax,18h				      ; set ax to nonlinear base
	mov	ds,ax				      ; add them to ds
	mov	es,ax				      ; and es.

 ;----------------------------------------------------;
 ; Get call table address.                            ;
 ;----------------------------------------------------;
	mov	edi,Functions			      ; fill the function table
	mov	al,0				      ; so we have some usefull functions
	mov	ah,0x0a
	int	50h

 ;----------------------------------------------------;
 ; Try printing colors                                ;
 ;----------------------------------------------------;

	mov	esi,msg1			      ; this point's to our string.
	call	[PrintString_$] 			; this call the print function.
	mov	al,0				      ; clear al
colors:
	call	[TextColor]			      ; change color of text to al
	call	[WriteHex16]			      ; print al in al color
	inc	al				      ; add 1 to al
	cmp	al,255				      ; check if al is maxed out
	jne	colors				      ; if not max try another color
	call	[WaitForKeyPress]		      ; is the wait for keypress function.
	ret					      ; This returns to the CLI/GUI

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;

include 'Dex.inc'				      ; Here is where we include our "DexFun.inc" file
