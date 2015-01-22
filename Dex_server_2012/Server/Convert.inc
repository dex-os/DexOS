;=========================================================;
; Convert                                        05/05/08 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

 ;----------------------------------------------------;
 ; ConevertFileName.                                  ;
 ;----------------------------------------------------;
ConevertFileName:
	pushad
	mov   esi,[NameOfFileToLoad]
	mov   edi,NameOfFileToLoadBuffer
	mov   ecx,8
ConvertFileName:
	cmp   byte [ds:esi],'.'
	je    @f
	cld
	movsb 
	dec   ecx
	jnz   ConvertFileName
	jmp   FinishedConvertingFileName
@@:
	mov   al,0x20
	cld
	rep   stosb
FinishedConvertingFileName:
	cmp   byte [ds:esi],'.'
	jne   @f
	inc   esi
@@:
	mov   ecx,3
	cld
	rep   movsb
	mov   al,0
	stosb
	popad
	ret


 ;----------------------------------------------------;
 ; UpperCase.       ; covverts a string to uppercase. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ; es:edi = string buffer (0 termanated)              ;
 ;----------------------------------------------------;
UpperCaseNew:
	pushad
	push  es
	mov   ax,18h
	mov   es,ax    
UcaseNextChar:
	mov   al,byte[es:edi]
	cmp   al,0
	je    UcaseDone
	cmp   al,0x61
	jb    DontUcaseChar
	cmp   al,0x7a
	ja    DontUcaseChar
	sub   al,0x20
	mov   byte[es:edi],al  
DontUcaseChar:
	inc   edi
	jmp   UcaseNextChar
UcaseDone:
	pop   es
	popad
	ret


 ;----------------------------------------------------;
 ; FindFile                                           ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;                                                    ;
 ;----------------------------------------------------;
FindFile:
	pushad
	mov   edx,6			              ; add 1 or sub 1 from the number if you add a page image etc
	mov   edi,WebPagesBuffer
	mov   esi,NameOfFileToLoadBuffer
 ;----------------------------------------------------;
 ; FindName.                                          ;
 ;----------------------------------------------------;
FindName:
	mov   ecx,11
FindNameCycle:
	cmp   byte [es:edi],0
	je    FindNameFailed
	pushad					       
	repe  cmpsb
	popad					       
	je    FindNameFound
	add   edi,19 ; 32
	dec   edx
	jnz   FindNameCycle
FindNameFailed:
	jmp   ErrorFindFile
FindNameFound:
	mov   edx,dword[es:edi+15]
	mov   dword [FileSize],edx
	mov   edx,dword[es:edi+11]
	mov   dword[FileAddress],edx
FindFileOK:
	popad
	clc
	ret

ErrorFindFile:
	popad
	stc
	ret

 ;----------------------------------------------------;
 ; Print_Status                                       ;
 ;----------------------------------------------------;
Print_Status:
        pushad 
	mov   al,49
	mov   ah,17
	call  [SetCursorPos]
	mov   edx,[NumbersVar]
	mov   [hex_val],edx
	call  testhex
	mov   esi,numbuff2
	call  [PrintString_0]
	mov   al,49
	mov   ah,18
	call  [SetCursorPos]
	mov   edx,[NumbersVarB]
	mov   [hex_val],edx
	call  testhex
	mov   esi,numbuff2
	call  [PrintString_0]
	mov   al,49
	mov   ah,19
	call  [SetCursorPos]
	mov   edx,[NumbersVar4]
	mov   [hex_val],edx
	call  testhex
	mov   esi,numbuff2
	call  [PrintString_0]
        popad
        ret

; These two String/Integer converting functions aren't mine, I found them on
; the flatassembler forum, and they were a lot better than my old ones.
IntToStr:
; eax = number, ebx = base, edi = buffer
push	ecx edx
xor ecx,ecx
  .new:
xor edx,edx
div ebx
push	edx
inc ecx
test	eax,eax
jnz .new
 .loop:
pop eax
add al,30h
cmp al,'9'
jng .ok
add al,7    
 .ok:
stosb
loop	.loop
mov al,0
stosb
pop edx ecx
ret


 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
FileSize	  dd 0
FileAddress	  dd 0
NameOfFileToLoad  dd 0
NameOfFileToLoadBuffer	 rb   16
