;=========================================================;
; KeyBoard                                     12/12/2010 ;
;---------------------------------------------------------;
; by Craig Bamford.                                       ;
;                                                         ;
; Thanks to smaffy for help with code.                    ;
; Pmode keyboard driver.                                  ;
;=========================================================;
align 4
;====================================================;
; Key Board                                          ;
;====================================================;
key_board:
	cld
	xor   eax,eax
	in    al,0x60
	test  al,10000000b
	jnz   @f
	mov   [WaitKPressed],1
;====================================================;
; ctrl down                                          ;
;====================================================;
@@:
	cmp   al,29				     
	jne   noctrldwn
	mov   byte[ctrl_key],0xff
;====================================================;
; ctrl up                                            ;
;====================================================;
noctrldwn:
	cmp   al,29+128 			     
	jne   noctrlup
	mov   byte[ctrl_key],0x00
;====================================================;
; alt down                                           ;
;====================================================;
noctrlup:
	cmp   al,56				     
	jne   noaltdwn
	mov   byte[alt_key],0xff
;====================================================;
; alt up                                             ;
;====================================================;
noaltdwn:
	cmp   al,56+128 			    
	jne   noaltup
	mov   byte[alt_key],0x00
;====================================================;
; left shift down                                    ;
;====================================================;
noaltup:
	cmp   al,42				    
	jne   nolshftdwn
	mov   byte[lshift_key],0xff
;====================================================;
; left shift up                                      ;
;====================================================;
nolshftdwn:
	cmp   al,42+128 			     
	jne   nolshftup
	mov   byte[lshift_key],0x00
;====================================================;
; right shift down                                   ;
;====================================================;
nolshftup:
	cmp   al,54				     
	jne   norshftdwn
	mov   byte[rshift_key],0xff
;====================================================;
; right shift up                                     ;
;====================================================;
norshftdwn:
	cmp   al,54+128 			     
	jne   norshftup
	mov   byte[rshift_key],0x00
norshftup:
;====================================================;
; cap lock down                                      ;
;====================================================;
	cmp   al,58				      
	jne   nocapdwn
	mov   al,130		
	cmp   byte[cap_key],0xff
	je    @f
	mov   byte [cap_key],0xff
	xor   [keyBoardStatus],4 ;0xb6
	call  SetKeyBoardLeds
	jmp   nocapdwn
@@:
	mov   byte [cap_key],0x00
       ;mov   [keyBoardStatus],0xb2
	and   [keyBoardStatus],0xfb
	call  SetKeyBoardLeds
nocapdwn:
	cmp   al,69
	jne   @f
	test  [keyBoardStatus],2
	jz    SwitchNledOn
	and   [keyBoardStatus],0xfd
	call  SetKeyBoardLeds
	jmp   @f
SwitchNledOn:
	xor   [keyBoardStatus],2
	call  SetKeyBoardLeds
;====================================================;
; Check if below 128, then there is a key.           ;
;====================================================; 
@@: 
	cmp   al,128				      
	jae   nokey
	mov   edi,eax
	mov   [ScanCode],al
	mov   [ScanCode2],al
	cmp   byte [rshift_key],0xff
	je    kshift
	cmp   byte [lshift_key],0xff
	je    kshift
	cmp   byte [ctrl_key],0xff
	je    kctrl
	cmp   byte [alt_key],0xff
	je    kalt
	cmp   byte [cap_key],0xff
	je    kcap
;====================================================;
; Convert to ASCII.                                  ;
;====================================================; 
; normal tangent:
	mov   al,[edi+normal_keymap]
	jmp   keydone
kshift:
	mov   al,[edi+shift_keymap]
	jmp   keydone
kctrl:
	mov   al,[edi+normal_keymap]
	sub   al,0x60
	jmp   keydone
kalt:
	mov   al,[edi+alt_keymap]
	jmp   keydone	   
kcap:			  
	mov   al,[edi+caplock_keymap]
keydone:
	mov   [keybuffer],al
nokey:
	ret

;====================================================;
; Set keyboard leds                                  ;
;====================================================;

SetKeyBoardLeds:
	push  eax
	mov   al,0xed				     
	out   60h,al				      
KeyBoardWait:
	in    al,64h
	test  al,10b				      
	jne   KeyBoardWait			      
	mov   al,byte [keyBoardStatus]
	and   al,111b
	out   60h,al				      
	pop   eax
	ret

 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;       
WaitKPressed	db 0
keyBoardStatus	db 0
rshift_key	db 0
lshift_key	db 0
ctrl_key	db 0
alt_key 	db 0		     
cap_key 	db 0
ScanCode	db 0
ScanCode2	db 0
