;=========================================================;
; NetStat                                        05/01/08 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
;====================================================;
; Win_Netstat.                                       ;
;====================================================;
Win_Netstat:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,16
loopabShadowNetst:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_abShadowNetst:
	mov   [es:edi],al
	add   edi,2
	loop  rev_abShadowNetst
	pop   ecx
	loop  loopabShadowNetst
	cld
 ;---------------------;
 ; Draw Netstat Window.;
 ;---------------------;
DrawWinNetStat:
	mov   esi,NetstatAbout+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,16
loopNetst2:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[NetstatAbout]
loopNetst1:
	movsb
	stosb
	loop  loopNetst1
	pop   ecx
	loop  loopNetst2

	xor   ebx,ebx
	int   52h
	mov   dword[stack_handler],ebx			   ; save stack_handler pointer
 ;----------------------------------------------------;
 ;  Main loop.                                        ;
 ;----------------------------------------------------;  
StatMainLoop:
	call  [stack_handler] 
	;hlt  
	call  [CountTicks]
	jnc   StatMainLoop
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------;
	mov   al,49
	mov   ah,7
	call  [SetCursorPos]
 ;----------------------------------------------------;
 ;  Ethernet card status.                             ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,6
	int   0x53
	call  [WriteHex32]
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------; 
	mov   al,49
	mov   ah,9
	call  [SetCursorPos] 
 ;----------------------------------------------------;
 ;  IP packets received.                              ;
 ;----------------------------------------------------;   
	mov   eax,53
	mov   ebx,255
	mov   ecx,2
	int   53h  
	call  [WriteHex32]
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------; 
	mov   al,49
	mov   ah,10
	call  [SetCursorPos]
 ;----------------------------------------------------;
 ;  ARP packets received.                             ;
 ;----------------------------------------------------; 
	mov   eax,53
	mov   ebx,255
	mov   ecx,5
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------;  
	mov   al,49
	mov   ah,11
	call  [SetCursorPos]  
 ;----------------------------------------------------;
 ;  Dumped received packets.                          ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,4
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------;     
	mov   al,49
	mov   ah,13
	call  [SetCursorPos] 
 ;----------------------------------------------------;
 ;  EMPTY QUEUE.                                      ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,100
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------;  
	mov   al,49
	mov   ah,14
	call  [SetCursorPos]  
 ;----------------------------------------------------;
 ;  IPOUT QUEUE.                                      ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,101
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------; 
	mov   al,49
	mov   ah,15
	call  [SetCursorPos]   
 ;----------------------------------------------------;
 ;  IPIN  QUEUE.                                      ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,102
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Set the cursor.                                    ;
 ;----------------------------------------------------; 
	mov   al,49
	mov   ah,16
	call  [SetCursorPos]   
 ;----------------------------------------------------;
 ;  NET1OUT QUEUE.                                    ;
 ;----------------------------------------------------;  
	mov   eax,53
	mov   ebx,255
	mov   ecx,103
	int   53h  
	call  [WriteHex32] 
 ;----------------------------------------------------;
 ; Anykey pressed ?.                                  ;
 ;----------------------------------------------------;
	call  [KeyPressedNoWait]
	cmp   al,1
	je    letsgoNetstat
	jmp   StatMainLoop
@@:
letsgoNetstat:
	pop   ds
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;