;=========================================================;
; AutoDex                                      01/04/2012 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
ConfigFileBuff = 0x400000-0x10000
LoadConfigFile:
	call  [RamSize]
	cmp   eax,8
	jb    NoRamDetected
	cmp   dword[Fatmod],0
	je    NoFmod
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Open file.                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   edx,FileName1
	mov   eax,9
	call  dword[Fatmod]
	jc    nofile
	xor   ebx,ebx
	mov   bx,ax
	mov   dword[tempHandler],ebx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Read file.                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   edx,ConfigFileBuff
	mov   eax,11
	call  dword[Fatmod]
	jc    nofile22
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Close file.                                                                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ebx,dword[tempHandler]
	mov   eax,10
	call  [Fatmod]
	jmp   LoadingFinished
;;;
NoFmod:
	mov   edi,FileName1
	mov   edx,ConfigFileBuff
	call  [ReadFileFat]
	jc    nofile
LoadingFinished:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Main loop.                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   [BFPointer],ConfigFileBuff
CommandsLoop:
	mov   esi,[BFPointer]
	cmp   byte[esi],13
	jne   No_Ret
	inc   esi
	mov   [BFPointer],esi
	jmp   CommandsLoop
No_Ret:
	cmp   byte[esi],10
	jne   Commands
	inc   esi
	mov   [BFPointer],esi
	jmp   CommandsLoop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Commands.                                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Commands:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Test for 'END'                                                                   ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,[BFPointer]
	mov   ecx,3
	mov   edi,cEND1
	repe  cmpsb
	je    ExitHere
	mov   esi,[BFPointer]
	mov   ecx,3
	mov   edi,cEND
	repe  cmpsb
	je    ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Test for 'LOAD'                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,[BFPointer]
	mov   ecx,4
	mov   edi,cLOAD
	repe  cmpsb
	je    ItIsLOAD
	mov   esi,[BFPointer]
	mov   ecx,4
	mov   edi,cLOAD1
	repe  cmpsb
	je    ItIsLOAD
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Print Bad Command                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
not_Command:
	mov   esi,BadCommand666
	call	[PrintString_0]
	jmp   ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load file error                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile:
load_error:
	mov   esi,LError
	call	[PrintString_0]
	jmp   ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load file error                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile22:
	mov   ebx,dword[tempHandler]
	mov   eax,10
	call  [Fatmod]

	mov   esi,LError
	call	[PrintString_0]
	jmp   ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load file error                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile1:
	mov   esi,LError1
	call	[PrintString_0]
	mov   [Exit?],1
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load file error                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile11:
	mov   ebx,dword[tempHandler]
	mov   eax,10
	call  [Fatmod]

	mov   esi,LError1
	call	[PrintString_0]
	mov   [Exit?],1
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; No Ram Detected                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NoRamDetected:
	mov   esi,NoRamDetectErr
	call	[PrintString_0]
	jmp   ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Exit..                                                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ExitHere:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; LOAD..                                                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ItIsLOAD:
	pushad
	call  Loader
	popad
	cmp   [Exit?],0
	je    CommandsLoop
	jmp   ExitHere
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; LOAD..                                                                           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Loader:
	mov   esi,[BFPointer]
	mov   edi,CommandBuffer2NEW
	mov   ecx,127
	cld
	rep   movsb
	mov   al,0
	stosb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load main files.                                                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp   dword[Fatmod],0
	je    NoFmod2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Open file.                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   edx,FileName2
	mov   eax,9
	call  dword[Fatmod]
	jc    nofile1
	xor   ebx,ebx
	mov   bx,ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Read file.                                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   dword[tempHandler],ebx
	mov   edx,ImageLoadAddrVar
	mov   eax,11
	call  dword[Fatmod]
	jc    nofile11
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Close file.                                                                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ebx,dword[tempHandler]
	mov   eax,10
	call  [Fatmod]
	jmp   LoadingFileFinished
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Just in case no fat module loaded                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NoFmod2:
	mov   edi,FileName2
	mov   edx,ImageLoadAddrVar
	call  [ReadFileFat]
	jc    nofile1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Run file.                                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LoadingFileFinished:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   edi,CommandBuffer2NEW
	xor   eax,eax
	mov   ebx,eax
	mov   ecx,eax
	mov   edx,eax
	call  ImageLoadAddrVar
	mov   ax,18h					
	mov   ds,ax
	mov   es,ax
	xor   eax,eax
	mov   edi,eax
	mov   ebx,eax
	mov   ecx,eax
	mov   edx,eax
	mov   esi,[BFPointer]
	add   esi,4
	cmp   byte[ds:esi],' '
	jne   SetPointerLoop
	inc   esi
SetPointerLoop:
	mov   ecx,24
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; check for a ret                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SetPointerLoop1:
	cmp   byte[ds:esi],13
	je    TryNextChar
	cmp   byte[ds:esi],10
	je    TryNextChar
	inc   esi
	loop  SetPointerLoop1
	mov   [Exit?],1
	ret
TryNextChar:
	inc   esi
	mov   [BFPointer],esi
	mov   [Exit?],0
	ret
 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
tempHandler	dd	  0
GuiStartup	db	  0
Exit?		db	  0
ModuleDir	db	  "DRIVERS",0
BadCommand666	db	  "Error, unknown command in startup.bat",13,13,0
LError		db	  "Error, No startup.bat file found",13,13,0
LError1 	db	  "Error, Loading file",13,13,0
NoRamDetectErr	db	  "Error, Can not detect ram size or bellow 8MB ",13,13,0
FileName1	db	  'STARTUP.BAT',0
FileName2	db	  'LOADMOD.DEX',0
BFPointer	dd	  0
Hz		dw	  0
CommandBuffer2NEW: times   128	db 0
 ;---------------;
 ; Commands.     ;
 ;---------------;
cLOAD		db 'LOAD ',0
cLOAD1		db 'load ',0
;cGUI            db 'GUI ',0
;cGUI1           db 'gui ',0
cEND		db 'END ',0
cEND1		db 'end ',0
;ConfigFileBuff:   times 8192 db ?  ;8kb Basic language file buffer

