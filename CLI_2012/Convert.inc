;=========================================================;
; Convert                                      12/12/2010 ;
;---------------------------------------------------------;
; Converts hex, bin, dec.                                 ;
; Based on code by                                        ;
; Ronald Nordberg.                                        ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
;====================================================;
; example test function                              ;
;====================================================;
testhex:
	pushad
	push  es
	push  ds
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
	call  strip1
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
	;mov   al,0
	 mov   al,'$' ;craig                                    
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
nozero1: dec   esi		;si craig                        
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
numbuff2end db '  $'


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
