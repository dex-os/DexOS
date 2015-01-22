;=========================================================;
; Server and Stat's                              05/08/08 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
StartTime:
	push  eax
	push  ebx
	call  [GetTime]
	shr   eax,8
	mov   ebx,eax
	mov   al,49+3
	mov   ah,15
	call  [SetCursorPos]
	mov   al,bl
	call  [WriteHex8]
	mov   al,49
	mov   ah,15
	call  [SetCursorPos]
	mov   al,bh
	call  [WriteHex8]
	mov   al,49+2
	mov   ah,15
	call  [SetCursorPos]
	mov   al,":"
	call  [PrintChar]
	mov   al,49
	mov   ah,16
	call  [SetCursorPos]
	call  print_date
	pop   ebx
	pop   eax
	ret

CurrentTime:
	push  eax
	push  ebx
	call  [GetTime]
	shr   eax,8
	mov   ebx,eax
	mov   al,49+3
	mov   ah,13
	call  [SetCursorPos]
	mov   al,bl
	call  [WriteHex8]
	mov   al,49
	mov   ah,13
	call  [SetCursorPos]
	mov   al,bh
	call  [WriteHex8]
	mov   al,49+2
	mov   ah,13
	call  [SetCursorPos]
	mov   al,":"
	call  [PrintChar]
	mov   al,49
	mov   ah,14
	call  [SetCursorPos]
	call  print_date
	pop   ebx
	pop   eax
	ret

print_date:
	pushad
	call  [GetDate]
	mov   [month],ah
	mov   [day],al
	shr   eax,16 
	mov   [century],ah
	mov   [year],al     
	mov   al,[day]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  [PrintCharCursor]
	mov   al,bh
	call  [PrintCharCursor]
	mov   al,":"
	call  [PrintCharCursor]
	mov   al,[month]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  [PrintCharCursor]
	mov   al,bh
	call  [PrintCharCursor]
	mov   al,":"
	call  [PrintCharCursor]
	mov   al,[century]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   bx,0x3032
	mov   al,bl
	call  [PrintCharCursor]
	mov   al,bh
	call  [PrintCharCursor]
	mov   al,[year]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  [PrintCharCursor]
	mov   al,bh
	call  [PrintCharCursor]
	popad
	ret

ServerStatus:
	pushad
	cmp   [letter],0
	jne   @f

	mov   al,49
	add   al,[Progress]
	mov   ah,12
	call  [SetCursorPos]
	mov   al,177
	call  [PrintChar]
	inc   [Progress]
	cmp   [Progress],6
	jbe   ServerStatusExit
	mov   [letter],1
	jmp   ServerStatusExit
@@:
	mov   al,49
	add   al,[Progress]
	mov   ah,12
	call  [SetCursorPos]
	mov   al," "
	call  [PrintChar]
	dec   [Progress]
	cmp   [Progress],1
	jae   ServerStatusExit
	mov   [letter],0
ServerStatusExit:
	popad
	ret

 ;----------------------------------------------------;
 ; bcd_to_ASCII         ;converts bcd number to ASCII ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;         Bcd number in al.                          ;
 ;  Output:                                           ;
 ;         ASCII number in val.                       ;
 ;                                             (100%) ;
 ;....................................................;
		
bcd_to_ASCII:
       pushad						;start of convert
       mov  ah,al					;copy AL to AH
       and  ax,0f00fh					;mask bits
       mov  cl,4					;CL=04 for shift
       shr  ah,cl					;shift right AH to get unpacked BCD
       or   ax, 3030h					;combine with 30 to get ASCII
       xchg ah,al					;swap for ASCII storage convention
       mov  [val],ax					;store the ASCII value in VAL
       popad 
       ret
;====================================================;
; example test function                              ;
;====================================================;
testhex:
	pushad
	push  es
	push  ds
       ;mov   [hex_val],0x100000
	call  HexToAsciiHex32
	call  hexbin
	call  bindec
	mov   ax,18h
	mov   es,ax 
	mov   esi,numbuff
	mov   edi,numbuff2
	movsb
	mov   al,','
	stosb
	mov   cx,3
	rep   movsb
	mov   al,','
	stosb
	mov   cx,3
	rep   movsb
	mov   al,','
	stosb
	mov   cx,4
	rep   movsb
	mov   esi,numbuff2
	mov   edi,numbuff2
       ; call  strip1
	pop ds
	pop es
	popad
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; hexbin          Convert ascii hex to 32 bit binary ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      command line buffer.                          ;
 ;                                                    ;
 ; Output:                                            ;
 ;      edx.                                          ;
 ;....................................................;
hexbin:
	mov   esi,inbuff				
	xor   edx,edx					
      
aschexbin:
	lodsb
	cmp   al,'0'					
	jb    notasc					
	cmp   al,'9'					
	jbe   astrip					
	and   al,05fh					
	cmp   al,'A'					
	jb    notasc					
	cmp   al,'F'					
	ja    notasc					
	add   al,9				       
astrip:
	and   al,0fh					
	mov   cx,4					
	shl   edx,cl					
	xor   ah,ah					
	cbw
	add   edx,eax					
	jmp   aschexbin 				
notasc: ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; bindec       Convert binary 32bit to decimal ascii ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      edx.                                          ;
 ;                                                    ;
 ; Output:                                            ;
 ;      numbuff.                                      ;
 ;....................................................;

bindec:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   es,ax 
	pop   eax   
	mov   esi,divtab				
	mov   edi,numbuff				
	mov   cx,10					
sublop: xor   al,al					
sblop:	cmp   edx,[esi] 				
	jb    tolow					
	sub   edx,[esi] 				
	add   al,1					
	jmp   sblop					
tolow:	add   al,30h					
	stosb						
	add   si,4					
	loop  sublop					
	mov   al,0					
	stosb
	pop ds
	pop es
	popad
	ret

;====================================================;
; strip     Strip off leading zeroes in buffer at SI ;
;====================================================;

strip:
	lodsb						
	cmp   al,'0'					
	jne   nozero					
	jmp   strip					
nozero: dec   si					
	ret

;====================================================;
; strip     Strip off leading zeroes in buffer at SI ;
;====================================================;

strip1:
	lodsb						
	cmp   al,'0'
	je    stripit
	cmp   al,','					
	jne   nozero1					 
       ; jmp   strip
stripit:
	mov   al,0x20					 
	stosb
	jmp   strip1
nozero1: dec   si					 
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; HexToAsciiHex32  ; convert dword hex, to ascii hex ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      hex_val                                       ;
 ;                                                    ;
 ; Output:                                            ;
 ;      inbuff                                        ;
 ;....................................................;
HexToAsciiHex32eax:
	mov   dword[hex_val],eax
HexToAsciiHex32:
	mov   ecx,8
	mov   ebx,[hex_val]
	mov   esi,inbuff	     
loop_digit:
	rol   ebx,4
	mov   al,bl
	and   al,0Fh
	add   al,90h					
	daa						
	adc   al,40h
	daa		
	mov   byte[esi],al
	inc   esi
	dec   ecx
	jnz   loop_digit
;====================================================;
; terminator                                         ;
;====================================================;
	xor   eax,eax
	mov   byte [esi],al
	ret

;====================================================;
; Data                                               ;
;====================================================;

hex_val:  dd 0
tempbuff: dd 0

inbuff:   times   34 db 0
numbuff:  times   34 db 0				  
numbuff2: times   40 db 0

divtab: dd 1000000000					
	dd  100000000
	dd   10000000
	dd    1000000
	dd     100000
	dd	10000		
	dd	 1000
	dd	  100
	dd	   10
	dd	    1

 ;====================================================;
 ; Data.                                              ;
 ;====================================================;
StatusOK   db 'Good',0
letter	   db 0
Progress   db 0

val:	   dw 0,0

hour:	   db 0
minute:    db 0
second:    db 0

century:   db 0
year:	   db 0
month:	   db 0
day:	   db 0

