;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.05                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2011                                                ;;
;; Filename         :   Gui.inc                                                    ;;
;; Copy Right Owner :   Craig Bamford                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Team DexOS       :   0x4e71, bubach, Dex, hidnplayr, jas2o, steve, Cloud         ;;
;;                  :   smiddy, viki.                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Disclaimer       :                                                               ;;
;; This software is provided "AS IS" without warranty of any kind, either           ;;
;; expressed or implied, including, but not limited to, the implied                 ;;
;; warranties of merchantability and fitness for a particular purpose. The          ;;
;; entire risk as to the quality and performance of this software is with           ;;
;; you.                                                                             ;;
;; In no event will the author's, distributor or any other party be liable to       ;;
;; you for damages, including any general, special, incidental or                   ;;
;; consequential damages arising out of the use, misuse or inability to use         ;;
;; this software (including but not limited to loss of data or losses               ;;
;; sustained by you or third parties or a failure of this software to operate       ;;
;; with any other software), even if such party has been advised of the             ;;
;; possibility of such damages.                                                     ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Thanks to Alexei Frounze, Tomasz Grysztar, Jim Hall, Pat Villani, Pype.Clicker.  ;;
;;                                                                                  ;;
;; Also a thanks to  the many forums i am a member of, i would like to thank anyone ;;
;; who  has helped me, by answering my ? (too many too list).                       ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;=========================================================;
; GraphicFunctions                               28/11/09 ;
;---------------------------------------------------------;
; dos2x Lib's                                             ;
; by Craig Bamford.                                       ;
;                                                         ;
; GraphicFunctionsList:                                   ;
; ********************                                    ;
;  DrawStandWindow                                        ;
;  SetDrawBoxVesaColor                                    ;
;  VesaBufferSetM2D                                       ;
;  GetXYPointer                                           ;
;  Fade_screen                                            ;
;  DrawFilledBoxVesa                                      ;
;  SaveFilledBoxVesa                                      ;
;  ReStoreFilledBoxVesa                                   ;
;  SaveFilledBoxVesaDirect                                ;
;  ReStoreFilledBoxVesaDirect                             ;
;  DrawBoxVesa                                            ;
;  DrawButtonBoxVesa (outline set color)                  ;
;  DrawBoxVesaDirect                                      ;
;  SaveBoxVesa (outline)                                  ;
;  ReStoreBoxVesa (outline)                               ;
;  DrawFilledBoxTranVesa                                  ;
;  DrawFilledBoxTranLightVesa                             ;
;  DrawLineTrans32BPP                                     ;
;  GetVesaScreenPointer                                   ;
;  GetVesaScreenPointerDirect                             ;
;  BuffToScreen                                           ;
;  PutImageFill32                                         ;
;  PutImage32                                             ;
;  PutImageAlpha32                                        ;
;  FullVertWait                                           ;
;                                                         ;
;=========================================================; 

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Draw Stand Window                                  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;       EDI = WORK AREA COLOR                        ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawStandWindow:
	pushad
	mov   [WinColorWorkArea],edi			
	mov   [WinColor],esi
	mov   [WinX],eax
	mov   [WinY],ebx
	mov   [WinL],ecx
	mov   [WinW],edx
	mov   eax,[WinX] 
	add   eax,10 
	mov   ebx,[WinY] 
	add   ebx,10
	mov   edx,[WinW] 
	mov   ecx,[WinL] 
	call  DrawFilledBoxTranVesa
	mov   esi,[WinColor] 
	mov   eax,[WinX] 
	mov   ebx,[WinY] 
	mov   edx,[WinW] 
	mov   ecx,[WinL] 
	call  DrawFilledBoxVesa
	mov   esi,[WinColor] 
	mov   eax,[WinX] 
	add   eax,4
	mov   ebx,[WinY] 
	add   ebx,8
	mov   edx,[WinW] 
	sub   edx,8 
	mov   ecx,12
	call  DrawFilledBoxFadeVesa
	mov   esi,[WinColorWorkArea]			    
	mov   eax,[WinX] 
	add   eax,4
	mov   ebx,[WinY] 
	add   ebx,20
	mov   edx,[WinW] 
	sub   edx,8
	mov   ecx,[WinL] 
	sub   ecx,24
	call  DrawFilledBoxVesa
	mov   esi,dword[SetBoxlineColorRB]		   
	mov   edi,dword[SetBoxlineColorLT]		   
	mov   eax,[WinX] 
	mov   ebx,[WinY] 
	mov   edx,[WinW] 
	mov   ecx,[WinL] 
	call  DrawBoxVesa				   
	mov   esi,0x00060606 ;dword[SetBoxlineColorLT]     
	mov   edi,0x00060606 ;dword[SetBoxlineColorRB]     
	mov   eax,[WinX] 
	add   eax,4
	mov   ebx,[WinY] 
	add   ebx,20
	mov   edx,[WinW] 
	sub   edx,8					  
	mov   ecx,[WinL] 
	sub   ecx,24
	call  DrawBoxVesa
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; SetDrawBoxVesaColor                                ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EDI = Left & Top line color                  ;
 ;       ESI = Right & Bottom line color              ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;

SetBoxlineColorLT   dd 0x00404040
SetBoxlineColorRB   dd 0x00000000

SetDrawBoxVesaColor:
	mov   dword[SetBoxlineColorLT],edi
	mov   dword[SetBoxlineColorRB],esi
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; VesaBufferGet_Set                                  ;
 ;----------------------------------------------------;
VesaBufferGet_Set:
	cmp	ecx,0
	je	VesaBufferGetM2D
	cmp	ecx,1
	je	VesaBufferSetM2D
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; VesaBufferSetM2D                                   ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       ESI = VESA buffer address                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
VesaBufferSetM2D:
	mov   dword[SetVesaBuffer],esi
	clc 
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; VesaBufferGetM2D                                   ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       None                                         ;
 ; Output:                                            ;
 ;       ESI = VESA buffer address                    ;
 ;....................................................;
VesaBufferGetM2D:
	mov   esi,dword[SetVesaBuffer]
	clc 
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; GetXYPointer                ;get pointer from x y. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       ECX = START Y                                ;
 ;                                                    ;
 ; Output:                                            ;
 ;      EDI = XYPointer                               ;
 ;....................................................;
GetXYPointer:
	pushad
	push  es
	push  ds
	mov   [VesaStartX],eax
	mov   [VesaStartY],ecx
	call  GetVesaScreenPointer
	sub   edi,dword[SetVesaBuffer]
	mov   dword[X_Y_Pointer],edi
GetXYPointerExit:
	pop   ds
	pop   es
	popad
	mov   edi,dword[X_Y_Pointer]
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; GetXYPointerBuffer          ;get pointer from x y. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       ECX = START Y                                ;
 ;                                                    ;
 ; Output:                                            ;
 ;      EDI = XYPointer                               ;
 ;                                            buffer  :
 ;....................................................;
GetXYPointerBuffer:
	pushad
	push  es
	push  ds
	mov   [VesaStartX],eax
	mov   [VesaStartY],ecx
	call  GetVesaScreenPointer
	mov   dword[X_Y_Pointer],edi
GetXYPointerExitBuffer:
	pop   ds
	pop   es
	popad
	mov   edi,dword[X_Y_Pointer]
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; GetXYPointerBufferDirect    ;get pointer from x y. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       ECX = START Y                                ;
 ;                                                    ;
 ; Output:                                            ;
 ;      EDI = XYPointer                               ;
 ;                                            screen  ;
 ;....................................................;
GetXYPointerBufferDirect:
	pushad
	push  es
	push  ds
	mov   [VesaStartX],eax
	mov   [VesaStartY],ecx
	call  GetVesaScreenPointerDirect
	mov   dword[X_Y_Pointer],edi
GetXYPointerExitBufferDirect:
	pop   ds
	pop   es
	popad
	mov   edi,dword[X_Y_Pointer]
	ret



 ;----------------------------------------------------;
 ; Fade screen                                        ;
 ;----------------------------------------------------;
Fade_screen:
	pushad
	cmp   [ModeInfo_YResolution],480
	jne   Not_480
	call  fade_screen32_640
	jmp   wehavedone1_SF
Not_480:
	cmp   [ModeInfo_YResolution],600
	jne   Not_600
	call  fade_screen32_800
	jmp   wehavedone1_SF
Not_600:
	cmp   [ModeInfo_YResolution],768
	jne   Not_768
	call  fade_screen32_1024
	jmp   wehavedone1_SF
Not_768:
	cmp   [ModeInfo_YResolution],1024
	jne   wehavedone1_SF
	call  fade_screen32_1280
	jmp   wehavedone1_SF
wehavedone1_SF:
	popad
	ret 
 
 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Fade screen 32bit              ; fade_screen32_640 ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
fade_screen32_640:
	mov  edi,VesaBuffer  
	;mov  ebx,10                                     
	mov  ebx,130					 
	mov  byte[count],80 
	xor  ecx,ecx
dolop1_600:
	;inc  bl                                        
	dec  bl 					 
	mov  eax,ebx
	shl  eax,24
	shr  eax,8
	mov  al,bl
	shl  ax,8
	mov  al,bl
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	sub  byte[count],1
	cmp  byte[count],0
	jne  dolop1_600
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Fade screen 32bit               ;fade_screen32_800 ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ; NOTE: 6 Lines per loop are written                 ;
 ;....................................................;
fade_screen32_800:
	mov  edi,VesaBuffer  
	;mov  ebx,10                                    
	mov  ebx,140					 
	mov  byte[count],100 
	xor  ecx,ecx
dolop1_800:
	;inc  bl                                        
	dec  bl 					 
	mov  eax,ebx
	shl  eax,24
	shr  eax,8
	mov  al,bl
	shl  ax,8
	mov  al,bl
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	sub  byte[count],1
	cmp  byte[count],0
	jne  dolop1_800
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Fade screen 32bit             ; fade_screen32_1024 ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
fade_screen32_1024:
	mov  edi,VesaBuffer  
	;mov  ebx,10                                     
	mov  ebx,140					
	mov  byte[count],128
	xor  ecx,ecx
dolop1_1024:
	;inc  bl                                         
	dec  bl 					
	mov  eax,ebx
	shl  eax,24
	shr  eax,8
	mov  al,bl
	shl  ax,8
	mov  al,bl
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	sub  byte[count],1
	cmp  byte[count],0
	jne  dolop1_1024
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Fade screen 32bit             ; fade_screen32_1280 ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
fade_screen32_1280:
	mov  edi,VesaBuffer  
	;mov  ebx,10                                     
	mov  ebx,180					 
	mov  byte[count],170 
	xor  ecx,ecx
dolop1_1280:
	;inc  bl                                         
	dec  bl 					 
	mov  eax,ebx
	shl  eax,24
	shr  eax,8
	mov  al,bl
	shl  ax,8
	mov  al,bl
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,1
	cli
	rep  stosd
	sti
	sub  byte[count],1
	cmp  byte[count],0
	jne  dolop1_1280
;''''''''''''''''''''''''''''''''''''''''''''''''''
; Add 4 more lines for 1024
;--------------------------------------------------
	dec  bl 				       
	mov  eax,ebx
	shl  eax,24
	shr  eax,8
	mov  al,bl
	shl  ax,8
	mov  al,bl
	mov  cx,[ModeInfo_XResolution]
	shl  ecx,2
	cli
	rep  stosd
	sti
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Fade screen 24bit             ; fades screen 24bit ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
fade_screen24:
	mov  edi,VesaBuffer 
	mov  eax,0x00000000 
	mov  byte[count],120  
dolop2:    
	mov  ecx,640*2
	inc  ah
	inc  ah
	cli
dolop2a:
	stosd
	dec  edi
	loop dolop2a
	sti
	sub byte[count],1
	cmp  byte[count],0
	jne  dolop2

	mov  byte[count],120
dolop22:    
	mov  ecx,640*2
	dec  ah
	dec  ah
	cli
dolop22a:
	stosd
	dec  edi
	loop dolop22a
	sti
	sub byte[count],1
	cmp  byte[count],0
	jne  dolop22
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawFilledBoxVesa                                  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawFilledBoxVesa:
	pushad
	mov   [VesaColor],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
Line32:
	mov   eax,[VesaColor]
	call  DrawLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	loop  Line32 
wehavedoneDFBV:
	popad
	ret 

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; SaveFilledBoxVesa                                  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       EDI = Dest buffer                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;

SaveFBoxDestBuff  dd  0
VesaLineX4	  dd  0

SaveFilledBoxVesa:
	pushad
	mov   [SaveFBoxDestBuff],edi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	shl   edx,2
	mov   [VesaLineX4],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   esi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   edi,[SaveFBoxDestBuff] 
SaveFBoxLine32:
	call  SaveLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   esi,[VesaLineStartTotal]
	add   edi,[VesaLineX4]
	loop  SaveFBoxLine32 
wehavedoneSaveFBox:
	popad
	ret 

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; SaveFilledBoxVesa_dos2x                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       ESI = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       EDI = Dest buffer                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;

SaveFilledBoxVesa_dos2x:
	pushad
	mov   [SaveFBoxDestBuff],edi
	mov   [VesaStartX],eax
	mov   [VesaStartY],esi
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	shl   edx,2
	mov   [VesaLineX4],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   esi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   edi,[SaveFBoxDestBuff] 
SaveFBoxLine32_dos2x:
	call  SaveLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   esi,[VesaLineStartTotal]
	add   edi,[VesaLineX4]
	loop  SaveFBoxLine32_dos2x 
wehavedoneSaveFBox_dos2x:
	popad
	ret 


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; SaveFilledBoxVesaDirect                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       EDI = Dest buffer                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
SaveFilledBoxVesaDirect:
	pushad
	mov   [SaveFBoxDestBuff],edi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	shl   edx,2
	mov   [VesaLineX4],edx
	mov   ecx,[VesaLineY]
align 4
SaveFilledBoxVesaDirectLoop1:
	push  ecx
	mov   ecx,[VesaLineX]
align 4
SaveFilledBoxVesaDirectLoop2:
	push  ecx
	push  eax
	call  getpixel
	mov   eax,ecx
	stosd
	pop   eax
	inc   eax
	pop   ecx
	loop  SaveFilledBoxVesaDirectLoop2
	mov   eax,[VesaStartX]
	inc   ebx
	pop   ecx
	loop  SaveFilledBoxVesaDirectLoop1
wehavedoneSaveFBoxDirect:
	popad
	ret 




 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; ReStoreFilledBoxVesa                               ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = Source buffer                          ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;

ReStoreFBoxDestBuff  dd  0

ReStoreFilledBoxVesa:
	pushad
	mov   [ReStoreFBoxDestBuff],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	shl   edx,2
	mov   [VesaLineX4],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   esi,[ReStoreFBoxDestBuff] 
ReStoreFBoxLine32:
	call  ReStoreLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	add   esi,[VesaLineX4]
	loop  ReStoreFBoxLine32 
wehavedoneReStoreFBox:
	popad
	ret 



 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; ReStoreFilledBoxVesaDirect                         ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = Source buffer                          ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;

ReStoreFilledBoxVesaDirect:
	push  es
	pushad
	mov   [ReStoreFBoxDestBuff],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	shl   edx,2
	mov   [VesaLineX4],edx
	push  eax
	mov   ax,8h
	mov   es,ax
	pop   eax
	call  GetVesaScreenPointerDirect
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   esi,[ReStoreFBoxDestBuff] 
ReStoreFBoxLine32Direct:
	call  ReStoreLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	add   esi,[VesaLineX4]
	loop  ReStoreFBoxLine32Direct 
wehavedoneReStoreFBoxDirect:
	popad
	pop  es
	ret 


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawLineVesaM                         ; Drawsline. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = LINE Y SIZE                            ;
 ;       EDX = LINE X SIZE                            ;
 ;       EDI = COLOR                                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawLineVesaM:
	pushad
	mov   [VesaColor],edi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   ecx,[VesaLineY]
	mov   eax,[VesaColor]
DrawLineVesaMloop:
	call  DrawLine32BPP
	loop  DrawLineVesaMloop
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawBoxVesa                ; Draws a box out line. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       EDI = COLOR1                                 ;
 ;       ESI = COLOR2                                 ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawBoxVesa:
	pushad
	mov   [VesaColor],edi
	mov   [VesaColor2],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor]
	call  DrawLine32BPP
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
LineV32:
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor]
	call  DrawVline32
	loop  LineV32 
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor2]
	call  DrawLine32BPP
wehavedoneDBV:
	popad
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawButtonBoxVesa  ; Draws a box outline set color.;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;  Note:                                             ;
 ;       This draw a box outline using set            ;
 ;       colors 0x00313131 for all sides              ;
 ;       0x000b0b0b for corner pixels                 ;
 ;                                                    ;
 ;....................................................;
DrawButtonBoxVesa:
	pushad
	mov   [VesaColor],edi
	mov   [VesaColor2],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   eax,0x00313131 ;[VesaColor]
	call  DrawLine32BPP
	mov   [VesaColor],0x000b0b0b
	mov   [VesaColor2],0x000b0b0b
	call  DrawVline32
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
DrawButtonBoxVesaLoop:
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   [VesaColor],0x00313131
	mov   [VesaColor2],0x00313131
	call  DrawVline32
	loop  DrawButtonBoxVesaLoop
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   eax,0x00313131 ;[VesaColor2]
	call  DrawLine32BPP
	mov   [VesaColor],0x000b0b0b
	mov   [VesaColor2],0x000b0b0b
	call  DrawVline32
DrawButtonBoxVesaExit:
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawBoxVesaDirect             ;Draws a line direct ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;       EDI = COLOR                                  ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawBoxVesaDirect:
	pushad
	mov   [VesaColor],edi
	mov   [VesaColor2],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	push  es
	mov   ax,8h
	mov   es,ax
	call  GetVesaScreenPointerDirect
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	cmp   [ModeInfo_BitsPerPixel],24
	jne   @f
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor]
	call  DrawLine24BPP
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	sub   bx,[ModeInfo_XResolution]
LineV24Direct:
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	call  DrawVline24
	loop  LineV24Direct
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor2]
	mov   [VesaColor],eax
	call  DrawLine24BPP
	jmp   wehavedoneDirect
@@:
	cmp   [ModeInfo_BitsPerPixel],32
	jne   wehavedoneDirect
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor]
	call  DrawLine32BPP
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
LineV32Direct:
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor]
	call  DrawVline32
	loop  LineV32Direct
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	mov   eax,[VesaColor2]
	call  DrawLine32BPP
wehavedoneDirect:
	pop   es
	popad
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; SaveBoxVesa               ;Save a box outline line ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI =                                        ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
SaveBoxVesa:
	pushad
	mov   [VesaColor],edi
	mov   [VesaColor2],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   esi,[VesaLineStartTotal]
	mov   edi,BackGroundSaveHLine1
	call  SaveLine32BPP
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   edi,BackGroundSaveVLine
LineV32Save:
	add   [VesaLineStartTotal],edx 
	mov   esi,[VesaLineStartTotal]
	call  SaveVline32
	loop  LineV32Save
	add   [VesaLineStartTotal],edx 
	mov   esi,[VesaLineStartTotal]
	mov   edi,BackGroundSaveHLine2
	call  SaveLine32BPP
wehavedoneSave:
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; ReStoreBoxVesa            ;Save a box outline line ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI =                                        ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
ReStoreBoxVesa:
	pushad
	mov   [VesaColor],edi
	mov   [VesaColor2],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	push  es
	mov   ax,8h
	mov   es,ax
	call  GetVesaScreenPointerDirect
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	cmp   [ModeInfo_BitsPerPixel],32
	jne   @f
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   esi,BackGroundSaveHLine1
	call  ReStoreLine32BPP
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2
	mov   esi,BackGroundSaveVLine
	mov   dword[SavedESI],esi
LineV32ReStore:
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	call  ReStoreVline32
	loop  LineV32ReStore
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   esi,BackGroundSaveHLine2
	call  ReStoreLine32BPP
	jmp   wehavedoneReStore
@@:
	cmp   [ModeInfo_BitsPerPixel],24
	jne   wehavedoneReStore
	mov   ecx,[VesaLineY]
	sub   ecx,2
	mov   edi,[VesaLineStartTotal]
	mov   esi,BackGroundSaveHLine1
	call  ReStoreLine24BPP
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	sub   bx,[ModeInfo_XResolution]
	mov   esi,BackGroundSaveVLine
	mov   dword[SavedESI],esi
LineV24ReStore:
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	call  ReStoreVline24
	loop  LineV24ReStore
	add   [VesaLineStartTotal],ebx
	mov   edi,[VesaLineStartTotal]
	mov   esi,BackGroundSaveHLine2
	call  ReStoreLine24BPP
wehavedoneReStore:
	pop   es
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawFilledBoxTranVesa                              ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawFilledBoxTranVesa:
	pushad
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
	mov   esi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
LineT32:
	call  DrawLineTrans32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   esi,[VesaLineStartTotal]
	loop  LineT32 
wehavedoneDFBTV:
	popad
	ret 

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawFilledBoxTranLightVesa                         ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawFilledBoxTranLightVesa:
	pushad
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
	mov   esi,[VesaLineStartTotal]
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
LineTL32:
	call  DrawLineTransL32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	mov   esi,[VesaLineStartTotal]
	loop  LineTL32 
wehavedoneDFBTLV:
	popad
	ret 


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; DrawFilledBoxFadeVesa                ;Draws a line ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX = START X                                ;
 ;       EBX = START Y                                ;
 ;       ECX = BOX Y SIZE                             ;
 ;       EDX = BOX X SIZE                             ;
 ;       ESI = COLOR                                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
DrawFilledBoxFadeVesa:
	pushad
	mov   [VesaColor],esi
	mov   [VesaStartX],eax
	mov   [VesaStartY],ebx
	mov   [VesaLineY],ecx
	mov   [VesaLineX],edx
 ;----------------------------------------------------;
 ; Split the color into RGB                           ;
 ;----------------------------------------------------;
	mov   ebx,[VesaColor]
	mov   [Blue],bl
	mov   [Green],bh
	shr   ebx,8
	mov   [Red],bh
	call  GetVesaScreenPointer
	mov   [VesaLineStart],edi
	mov   [VesaLineStartTotal],edi
	xor   edx,edx
	mov   dx,[ModeInfo_XResolution] 
	shl   edx,2 
 ;----------------------------------------------------;
 ; Fade loop 32bit                                    ;
 ;----------------------------------------------------;
L1:
	mov   ecx,[VesaLineY]
	mov   edi,[VesaLineStartTotal]
LineF32:
	mov   eax,[VesaColor]
	call  DrawLine32BPP
	add   [VesaLineStartTotal],edx 
	mov   edi,[VesaLineStartTotal]
	cmp   [Red],3
	jbe   @f
	sub   [Red],4
@@:
	mov   al,[Red]
	shl   eax,8
	cmp   [Green],3
	jbe   @f
	sub   [Green],4
@@:
	mov   al,[Green]
	shl   eax,8
	cmp   [Blue],3
	jbe   @f
	sub   [Blue],4
@@:
	mov   al,[Blue]
	mov   [VesaColor],eax
	loop  LineF32 
wehavedoneDFBFV:
	popad
	ret 

 ;----------------------------------------------------;
 ; DrawLineTrans32BPP.                                ;
 ;----------------------------------------------------;
DrawLineTrans32BPP:
	pushad
	mov   ecx,[VesaLineX]
@@:
	lodsd
	shr   eax,1 
	and   eax,0x7f7f7f
	stosd
	loop  @b 
	popad
	ret

DrawLineTrans24BPP:
	pushad
	mov   ecx,[VesaLineX]
@@:
	lodsb
	shr   al,1 
	and   al,0x7f
	stosb
	lodsw
	shr   ax,1
	and   ax,0x7f7f
	stosw
	loop  @b 
	popad
	ret

 ;----------------------------------------------------;
 ; DrawLineTransL32BPP.                               ;
 ;----------------------------------------------------;
DrawLineTransL32BPP:
	pushad
	mov   ecx,[VesaLineX]
@@:
	lodsd
	shr   eax,1 
	or   eax,0x00000ff ;and
	and  eax,0x007f7fff
	stosd
	loop  @b 
	popad
	ret

DrawLineTransL24BPP:
	pushad
	mov   ecx,[VesaLineX]
@@:
	lodsw
	shr   ax,1
	or   ax,0xff00
	and   ax,0xff7f
	stosw
	lodsb
	shr   al,1 
	and   al,0x7f
	stosb
	loop  @b 
	popad
	ret

 ;----------------------------------------------------;
 ; DrawLine24BPP.                                     ;
 ;----------------------------------------------------;
DrawLine24BPP:
	pushad
	mov   ecx,[VesaLineX]
LineDraw24:
	mov   eax,[VesaColor]
	stosb
	shr   eax,8
	stosw
	loop  LineDraw24
	popad
	ret

 ;----------------------------------------------------;
 ; DrawLine32BPP.                                     ;
 ;----------------------------------------------------;
DrawLine32BPP:
	pushad
	mov   ecx,[VesaLineX]
	rep   stosd
	popad
	ret

 ;----------------------------------------------------;
 ; SaveLine32BPP.                                     ;
 ;----------------------------------------------------;
SaveLine32BPP:
	pushad
	mov   ecx,[VesaLineX]
	rep   movsd
	popad
	ret

 ;----------------------------------------------------;
 ; SaveVline32.                                       ;
 ;----------------------------------------------------;
SaveVline32:
	pushad
	mov   ecx,[VesaLineX]
	sub   ecx,2
	movsd
@@:
	add   esi,4
	loop  @b
	movsd
	mov   dword[SavedEdI],edi
	popad
	mov   edi,dword[SavedEdI]
	ret

 ;----------------------------------------------------;
 ; ReStoreLine32BPP.                                  ;
 ;----------------------------------------------------;
ReStoreLine32BPP:
	pushad
	mov   ecx,[VesaLineX]
	rep   movsd
	popad
	ret

 ;----------------------------------------------------;
 ; ReStoreVline32.                                    ;
 ;----------------------------------------------------;
ReStoreVline32:
	pushad
	mov   ecx,[VesaLineX]
	sub   ecx,2
	mov   esi,dword[SavedESI]
	movsd
@@:
	add   edi,4
	loop  @b
	movsd
	mov  dword[SavedESI],esi
	popad
	ret

 ;----------------------------------------------------;
 ; DrawVline24.                                       ;
 ;----------------------------------------------------;
DrawVline24:
	pushad
	mov   eax,[VesaColor]
	mov   ecx,[VesaLineX]
	sub   ecx,2
	stosb
	shr   eax,8 
	stosw
@@:
	add   edi,3
	loop  @b
	mov   eax,[VesaColor2]
	stosb
	shr   eax,8 
	stosw
	popad
	ret

 ;----------------------------------------------------;
 ; DrawVline32.                                       ;
 ;----------------------------------------------------;
DrawVline32:
	pushad
	mov   eax,[VesaColor]
	mov   ecx,[VesaLineX]
	sub   ecx,2
	stosd
@@:
	add   edi,4
	loop  @b
	mov   eax,[VesaColor2]
	stosd
	popad
	ret
;
 ;----------------------------------------------------;
 ; SaveVline24.                                       ;
 ;----------------------------------------------------;
ReStoreVline24:
	pushad
	mov   ecx,[VesaLineX]
	sub   ecx,2
	mov   esi,dword[SavedESI]
	lodsd
	stosb
	shr   eax,8
	stosw
@@:
	add   edi,3
	loop  @b

	lodsd
	stosb
	shr   eax,8
	stosw

	mov  dword[SavedESI],esi
	popad
	ret

 ;----------------------------------------------------;
 ; DrawLine24BPP.                                     ;
 ;----------------------------------------------------;
ReStoreLine24BPP:
	pushad
	mov   ecx,[VesaLineX]
LineDraw24ReStore:
	lodsd
	stosb
	shr   eax,8
	stosw
	loop  LineDraw24ReStore
	popad
	ret
;
 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
GetVesaScreenPointer:
	push  eax
	push  ebx
	mov   edi,dword[SetVesaBuffer]	 ;VesaBuffer
	xor   ebx,ebx
	mov   ebx,[VesaStartX]
	shl   ebx,2
@@:	  
	add   edi, ebx
	mov   ebx,[VesaStartY]
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution] 
	shl   eax,2
@@:
	mul   ebx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; GetVesaScreenPointerDirect.                        ;
 ;----------------------------------------------------;
GetVesaScreenPointerDirect:
	push  eax
	push  ebx
	mov   edi,[ModeInfo_PhysBasePtr]
	xor   ebx,ebx
	mov   ebx,[VesaStartX]
	shl   ebx,2
	cmp   [ModeInfo_BitsPerPixel],24
	jne   @f
	sub   ebx,[VesaStartX]
@@:	  
	add   edi, ebx
	mov   ebx,[VesaStartY]
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution] 
	shl   eax,2
	cmp   [ModeInfo_BitsPerPixel],24
	jne   @f
	sub   ax,[ModeInfo_XResolution]
@@:
	mul   ebx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; BuffToScreen.  ;Puts whats in the buffer to screen ;
 ;----------------------------------------------------;
BuffToScreen:
	mov   [vesaMOUSEoff],1
	cmp   [ModeInfo_BitsPerPixel],24
	jne   Try32
	call  BuffToScreen24
	jmp   wehavedone24
Try32:
	cmp   [ModeInfo_BitsPerPixel],32
	jne   wehavedone24
	call  BuffToScreen32 
wehavedone24:
	cmp   [NoMouseInt],1
	je    @f
	mov   [First],0
	mov   [vesaMOUSEoff],0
	int   2Ch
@@:
	ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               32bpp ;
 ;----------------------------------------------------;
BuffToScreen32:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[ModeInfo_PhysBasePtr]
	 mov   esi,dword[SetVesaBuffer] ;VesaBuffer 
	 xor   eax,eax
	 mov   ecx,eax	
	 mov   ax,[ModeInfo_XResolution]
	 mov   cx,[ModeInfo_YResolution]
	 mul   ecx
	 mov   ecx,eax	
	 cld
	 cli
	 rep   movsd
	 sti
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               24bpp ;
 ;----------------------------------------------------;
BuffToScreen24:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax ;ccc
	 mov   ax,[ModeInfo_YResolution]
	 mov   ebp,eax
	 lea   eax,[ebp*2+ebp]
	 mov   edi,[ModeInfo_PhysBasePtr]
	 mov   esi,dword[SetVesaBuffer] ;VesaBuffer  
	 cld
.l1:
	 mov   cx,[ModeInfo_XResolution]
	 shr   ecx,2	     
.l2:
	 mov   eax,[esi]	 ;eax = -- R1 G1 B1
	 mov   ebx,[esi+4]	 ;ebx = -- R2 G2 B2
	 shl   eax,8		 ;eax = R1 G1 B1 --
	 shrd  eax,ebx,8	 ;eax = B2 R1 G1 B1
	 stosd

	 mov   ax,[esi+8]	 ;eax = -- -- G3 B3
	 shr   ebx,8		 ;ebx = -- -- R2 G2
	 shl   eax,16		 ;eax = G3 B3 -- --
	 or    eax,ebx		 ;eax = G3 B3 R2 G2
	 stosd

	 mov   bl,[esi+10]	 ;ebx = -- -- -- R3
	 mov   eax,[esi+12]	 ;eax = -- R4 G4 B4
	 shl   eax,8		 ;eax = R4 G4 B4 --
	 mov   al,bl		 ;eax = R4 G4 B4 R3
	 stosd

	 add esi,16
	 loop  .l2

	 sub ebp,1
	 ja .l1

	 pop   es
	 popad
	 ret


 ;----------------------------------------------------;
 ; FillScreenBuffer32                           32bpp ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EDI = Dest buffer                            ;
 ;       ECX = Fill color                             ;
 ; Output:                                            ;
 ;       Dest buffer filled with                      ;
 ;       color                                        ;
 ;----------------------------------------------------;
FillScreenBuffer32:
	 pushad
	 push  es
	 mov   dword[PutImage32Color],ecx
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ax,[ModeInfo_XResolution]
	 mov   cx,[ModeInfo_YResolution]
	 mul   ecx
	 mov   ecx,eax
	 mov   eax,dword[PutImage32Color]
	 cld
	 cli
	 rep   stosd
	 sti
	 pop   es
	 popad
	 ret


 ;----------------------------------------------------;
 ; PutImageFill32                               32bpp ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EDI = Dest buffer                            ;
 ;       ESI = Source buffer                          ;
 ;       ECX = Fill color                             ;
 ; Source buffer:                                     ;
 ;       Buffer     = X of image size   (1 dword)     ;
 ;       Buffer + 4 = Y of image size   (1 dword)     ;
 ;       Buffer + 8 = image start (32bpp)             ;
 ; Output:                                            ;
 ;       Dest buffer filled with                      ;
 ;       color plus source buffer                     ;
 ;       image.                                       ;
 ;                                                    ;
 ;----------------------------------------------------;
align 4
ImageX		   dw	0
ImageY		   dw	0
ImageXaddOn	   dd	0
ImageYaddOn	   dd	0
ImageXaddOn24	   dd	0
HeaderAddOn	   dd	0
BufferEDI	   dd	0
BufferESI	   dd	0
PutImage32Color    dd	0
HeaderAddOnEDI	   dd	0
 ;----------------------------------------------------;
 ; PutBmp32                                           ;
 ;----------------------------------------------------;
PutImageFill32:
	 pushad
	 push  es
	 mov   dword[BufferEDI],edi
	 mov   dword[BufferESI],esi
	 mov   dword[PutImage32Color],ecx
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ax,[ModeInfo_XResolution]
	 mov   cx,[ModeInfo_YResolution]
	 mul   ecx
	 mov   ecx,eax
	 mov   eax,dword[PutImage32Color]
	 cld
	 cli
	 rep   stosd
	 sti
	 mov   esi,dword[BufferESI]
	 lodsd
	 mov   word[ImageX],ax
	 mov   ax,[ModeInfo_XResolution]
	 sub   ax,word[ImageX]
	 shl   eax,2
	 mov   dword[ImageXaddOn],eax
	 mov   esi,dword[BufferESI]
	 add   esi,4
	 lodsd
	 mov   word[ImageY],ax
	 mov   ax,[ModeInfo_YResolution]
	 sub   ax,word[ImageY]
	 mov   dword[ImageYaddOn],eax
	 mov   esi,dword[BufferESI]
	 add   esi,8
	 add   esi,[HeaderAddOn]
	 mov   edi,dword[BufferEDI]
	 xor   ecx,ecx
	 mov   cx,word[ImageY]
align 4
PutImageFillLoop1:
	 push  ecx
	 mov   cx,word[ImageX]
@@:
	 rep   movsd
	 add   edi,[ImageXaddOn]
	 pop   ecx
	 loop  PutImageFillLoop1
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; PutImage32                                XXRRGGBB ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EDI = Dest buffer                            ;
 ;       ESI = Source buffer                          ;
 ;       EAX = Start X                                ;
 ;       ECX = Start Y
 ; Source buffer:                                     ;
 ;       Buffer     = X of image size   (1 dword)     ;
 ;       Buffer + 4 = Y of image size   (1 dword)     ;
 ;       Buffer + 8 = image start (32bpp)             ;
 ; Output:                                            ;
 ;       image.                                       ;
 ;                                                    ;
 ;----------------------------------------------------;
PutImage32:
	pushad
	push  es
	mov   dword[WinX],eax
	mov   dword[WinY],ecx
	mov   dword[BufferEDI],edi
	mov   dword[BufferESI],esi
	movzx ebx,[ModeInfo_XResolution]				; put the x of screen and fill top half of ebx with zeros
	shl   ebx,2							; mul by 4 as we have 4 bytes to a pixel.
	mov   [BytesPerScanLine],ebx					; store it.
	mov   esi,dword[BufferESI] 
	add   esi,8
	mov   eax,[WinX] 
	mov   ebx,[WinY] 
	imul  ebx,[BytesPerScanLine]	
	lea   edi,[ebx+eax*4]		
	mov   [HeaderAddOnEDI],edi
	mov   esi,dword[BufferESI]
	lodsd
	mov   word[ImageX],ax
	mov   ax,[ModeInfo_XResolution]
	sub   ax,word[ImageX]
	shl   eax,2
	mov   dword[ImageXaddOn],eax
	mov   esi,dword[BufferESI]
	add   esi,4
	lodsd
	mov   word[ImageY],ax
	mov   ax,[ModeInfo_YResolution]
	sub   ax,word[ImageY]
	mov   dword[ImageYaddOn],eax
	mov   esi,dword[BufferESI]
	add   esi,8
	add   esi,[HeaderAddOn]
	mov   edi,dword[BufferEDI]
	add   edi,dword[HeaderAddOnEDI]
	xor   ecx,ecx
	mov   cx,word[ImageY]
align 4
PutImageLoop1:
	push  ecx
	mov   cx,word[ImageX]
@@:
	rep   movsd
	add   edi,[ImageXaddOn]
	pop   ecx
	loop  PutImageLoop1
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; PutImageAlpha32                          AARRGGBB  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EDI = Dest buffer                            ;
 ;       ESI = Source buffer                          ;
 ;       EAX = Start X                                ;
 ;       ECX = Start Y                                ;
 ; Source buffer:                                     ;
 ;       Buffer     = X of image size   (1 dword)     ;
 ;       Buffer + 4 = Y of image size   (1 dword)     ;
 ;       Buffer + 8 = image start (32bpp)             ;
 ; Output:                                            ;
 ;       image.                                       ;
 ;                                                    ;
 ; NOTES: Same as above function but supports         ;
 ;        images with Alpha channal.                  ;
 ;----------------------------------------------------;
PutImageAlpha32:
	pushad
	push  es
	mov   dword[WinX],eax
	mov   dword[WinY],ecx
	mov   dword[BufferEDI],edi
	mov   dword[BufferESI],esi
	movzx ebx,[ModeInfo_XResolution]
	shl   ebx,2
	mov   [BytesPerScanLine],ebx
	mov   esi,dword[BufferESI] 
	add   esi,8
	mov   eax,[WinX] 
	mov   ebx,[WinY] 
	imul  ebx,[BytesPerScanLine]	
	lea   edi,[ebx+eax*4]		
	mov   [HeaderAddOnEDI],edi
	mov   esi,dword[BufferESI]
	lodsd
	mov   word[ImageX],ax
	shl   eax,2
	mov   dword[ImageXaddOn],eax
	mov   esi,dword[BufferESI]
	add   esi,4
	lodsd
	mov   word[ImageY],ax
	mov   ax,[ModeInfo_YResolution]
	sub   ax,word[ImageY]
	mov   dword[ImageYaddOn],eax
	mov   esi,dword[BufferESI]
	add   esi,8
	add   esi,[HeaderAddOn]
	mov   edi,dword[BufferEDI]
	add   edi,dword[HeaderAddOnEDI]
	xor   ecx,ecx
	mov   cx,word[ImageY]
align 4
PutImageAlpha32Loop:
	push  edi
	push  esi
	push  ecx
	mov   cx,word[ImageX]
align 4
@@:
	push  ecx
	mov   eax,[esi]
	push  eax
	pop   ecx
	shr   ecx,24
	mov   ebx,dword[edi] ;background
	call  AlphaBlend1
	add   esi,4  
	mov   [edi],eax
	add   edi,4  
	pop   ecx
	loop  @b
	pop   ecx
	pop   esi
	pop   edi
	add   esi,dword[ImageXaddOn]
	add   edi,[BytesPerScanLine]
	loop  PutImageAlpha32Loop
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; FullVertWait                                       ;
 ;----------------------------------------------------; 
FullVertWait:
	pushad
	mov   dx,0x03da
Vr:
	in    al,dx
	test  al,8
	jnz   Vr					      
Nvr:
	in    al,dx
	test  al,8
	jz    Nvr					      
	popad
	ret
;Temp
;*****************************************************************************
;  AlphaBlend :
;
;  BlendColour = ((AlphaValue * (ForeColour - BackColour)) / 256) + BackColour
;
;  has to be done for all 3 channels R/G/B
;
;  input:
;       eax = ForeColour  0x00RRGGBB
;       ebx = BackColour  0x00RRGGBB
;       ecx = AlphaValue  0 - 255
;
;  output:
;       eax = BlendColour 0x00RRGGBB
;
;  note: this function needs the color in eax,ebx not the address
;    eg: mov eax,dword[edi]
;        mov ebx,dword[esi]
;
;******************************************************************************
align 4
AlphaBlend1:

	push  esi			    ; save esi
	push  edi			    ; save edi
	push  edx			    ; save edx

	mov   esi,eax			    ; save foreground colour
	mov   edi,ebx			    ; save background colour

; start with blue

	and   eax,0x000000ff		    ; seperate blue
	and   ebx,0x000000ff		    ; seperate blue
	push  ebx			    ; save Background Blue
	sub   eax,ebx			    ; first part of equation
	imul  eax,ecx			    ; our multiply
	shr   eax,8			    ; our divide
	pop   ebx			    ; restore Background Blue
	add   eax,ebx			    ; add together
	push  eax			    ; we now have 0x000000BB

; next do green

	mov   eax,esi			    ; get our ForegroundColour
	shr   eax,8			    ; move to low byte
	and   eax,0x000000ff		    ; seperate green
	mov   ebx,edi			    ; get our BackgroundColour
	shr   ebx,8			    ; move to low byte
	and   ebx,0x000000ff		    ; seperate green
	push  ebx			    ; save Background Green
	sub   eax,ebx			    ; first part of equation
	imul  eax,ecx			    ; our multiply
	shr   eax,8			    ; our divide
	pop   ebx			    ; restore Background Green
	add   eax,ebx			    ; add together
	shl   eax,8			    ; put back in position
	pop   edx			    ; restore our result
	add   eax,edx			    ; add green back in
	push  eax			    ; we now have 0x0000GGBB

; next do red

	mov   eax,esi			    ; get our ForegroundColour
	shr   eax,16			    ; move to low byte
	and   eax,0x000000ff		    ; seperate Red
	mov   ebx,edi			    ; get our BackgroundColour
	shr   ebx,16			    ; move to low byte
	and   ebx,0x000000ff		    ; seperate Red
	push  ebx			    ; save Background Red
	sub   eax,ebx			    ; first part of equation
	imul  eax,ecx			    ; our multiply
	shr   eax,8			    ; our divide
	pop   ebx			    ; restore Background Red
	add   eax,ebx			    ; add together
	shl   eax,16			    ; put back in position
	pop   edx			    ; restore our result
	add   eax,edx			    ; add Red back in

	pop   edx			    ; restore edx
	pop   edi			    ; restore edi
	pop   esi			    ; restore esi

	ret				    ; we now have 0x00RRGGBB return


 ;----------------------------------------------------;
 ; DATA                                               ;
 ;----------------------------------------------------;
align 4
WinColorWorkArea	dd 0
SetVesaBuffer		dd 0
X_Y_Pointer		dd 0
SavedESI		dd 0
SavedEdI		dd 0
WinColor		dd 0
WinX			dd 0
WinY			dd 0
WinL			dd 0
WinW			dd 0
count			db 0 
VesaLineY		dd 0
VesaLineX		dd 0
VesaLineStartTotal	dd 0
VesaLineStart		dd 0
VesaStartX		dd 0
VesaStartY		dd 0 
VesaColor		dd 0
VesaColor2		dd 0
NoMouseInt		db 0
Red			db 0
Green			db 0
Blue			db 0 
BytesPerScanLine	dd 0

