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

 ;----------------------------------------------------;
 ; DoMouse.                                           ;
 ;----------------------------------------------------;
DoMouse:
	pushad
	call  MouseMovmentClick
	mov   cx,[current_x]
	mov   dx,[current_y]
	mov   [VesaStartXx],cx
	mov   [VesaStartYy],dx
	cmp   [vesaMOUSEoff],1
	je    NoDrawLoop
	xor   eax,eax
	mov   ebx,eax
	mov   ax,cx
	mov   bx,dx
	call  ReStoreMouseBAackGround
	call  DrawMouse
        mov   al,[buttons]
        cmp   al,[ButtonOld]
        je    NotChanged
        inc   [ButtonReleased]
        cmp   [ButtonReleased],1
        ja    @f
        call  TestButtonXY
        jmp   NotChanged
@@:
        mov   eax,dword[MouseClickIDTest]
        mov   dword[MouseClickIDTest],0x00ffffff
        call  TestButtonXY
        mov   dword[MouseClickIDYesNo],0
        cmp   eax,dword[MouseClickIDTest]
        jne   NotTheSame
        mov   dword[MouseClickIDYesNo],1
NotTheSame:
        mov   dword[MouseClickIDTest],0
        mov   [ButtonReleased],0
NotChanged:
        mov   al,[buttons]
        mov   [ButtonOld],al
NoDrawLoop:
	mov   al,0x20
	out   0xa0,al
	out   0x20,al
	popad
	iret

 ;----------------------------------------------------;
 ; TestButtonXY.                                      ;
 ;----------------------------------------------------;
TestButtonXY:
        pushad
        push  es
        push  ds
        mov   esi,ButtonStruct
        cmp   dword[esi],0
        je    TestButtonXYExit
        mov   ecx,dword[esi]
        ;mov ecx,2
        add   esi,4
align 4
TestButtonXYLoop:
        add   esi,16
        pushad
        mov   edx,esi
        lodsw
        mov   bx,ax
        cmp   [VesaStartXx],bx
        jb    NotInRange
        lodsw
        add   ax,bx
        cmp   [VesaStartXx],ax
        ja    NotInRange
        lodsw
        mov   bx,ax
        cmp   [VesaStartYy],bx
        jb    NotInRange
        lodsw
        add   ax,bx
        cmp   [VesaStartYy],ax
        ja    NotInRange
        sub   edx,4
        mov   esi,edx
        mov   edi,MouseClickID
        movsd
        movsw
        movsw
        movsw
        movsw
        ;mov   eax,1
        ;stosd
        mov   edi,MouseClickID
        mov   eax,[edi]
        mov   dword[MouseClickIDTest],eax
        popad
        jmp   TestButtonXYExit
NotInRange:
        popad 
        loop  TestButtonXYLoop
TestButtonXYExit:
        pop   ds
        pop   es
        popad
        ret


 ;----------------------------------------------------;
 ; DoMouseTest.                                       ;
 ;----------------------------------------------------;
DoMouseTest:
	pushad
	call  MouseMovmentClick
	mov   cx,[current_x]
	mov   dx,[current_y]
	mov   [VesaStartXx],cx
	mov   [VesaStartYy],dx

	cmp   [vesaMOUSEoff],1
	je    NoDrawLoopTest
	xor   eax,eax
	mov   ebx,eax
	mov   ax,cx
	mov   bx,dx
	call  ReStoreMouseBAackGround
	call  DrawMouse
NoDrawLoopTest:
	popad
	ret

 ;----------------------------------------------------;
 ; DrawMouse                                          ;
 ;----------------------------------------------------;
DrawMouse:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,8h
	mov   es,ax
	pop   eax
	call  [VesaRes]
	pop  ds
	pop  es
	popad
	ret

; eax = x
; ebx = y
DrawMouse24:
	mov   [VarEax],eax
	mov   [VarEbx],ebx
	xor   edx,edx
	mov   dx,[ModeInfo_BytesPerScanLine]
	imul  ebx,edx				; ebx = y * y multiplier MouseBack
	lea   edi,[eax+eax*2]			; edi = x*3
	add   edi,[ModeInfo_PhysBasePtr]
	add   edi,ebx				; ebx = where to put pixel in memory
	mov   [SaveEdi],edi
	mov   esi,MouseBack
	mov   ecx,20
DrawMouse24Loop1S:
	push  ecx
	mov   ecx,16
	xor   eax,eax
DrawMouse24Loop2S:
	mov   al,byte[es:edi+2]
	shl   eax, 16
	mov   ax,word[es:edi]
	mov   dword[ds:esi],eax
	inc   edi
	inc   edi
	inc   edi
	inc   esi
	inc   esi
	inc   esi
	inc   esi
	loop  DrawMouse24Loop2S
	pop   ecx
	sub   edi,16*3
	add   edi,edx
	loop  DrawMouse24Loop1S
	mov   edi,[SaveEdi]
	mov   esi,mouse3_bmp32
	mov   ecx,20 ;32
DrawMouse24Loop1:
	push  ecx
	mov   ecx,16
DrawMouse24Loop2:
	lodsd
	cmp   eax,0
	je    NoPixel
	mov   [es:edi], ax
	shr   eax, 16
	mov   [es:edi+2], al
NoPixel:
	inc   edi
	inc   edi
	inc   edi
	loop  DrawMouse24Loop2
	pop   ecx
	sub   edi,16*3
	add   edi,edx
	loop  DrawMouse24Loop1
	ret


   
; eax = x
; ebx = y
DrawMouse32:
	mov   [VarEax],eax
	mov   [VarEbx],ebx
	xor   edx,edx
	mov   dx,[ModeInfo_BytesPerScanLine]
	imul  ebx,edx  
	lea   edi,[ebx+eax*4]
	add   edi,[ModeInfo_PhysBasePtr]
	mov   [SaveEdi],edi
	mov   esi,MouseBack
	mov   ecx,20
DrawMouse32Loop1S:
	push  ecx
	mov   ecx,16
DrawMouse32Loop2S:
	mov   eax,dword[es:edi]
	mov   dword[ds:esi],eax
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	inc   esi
	inc   esi
	inc   esi
	inc   esi
	loop  DrawMouse32Loop2S
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  DrawMouse32Loop1S
	mov   esi,mouse3_bmp32
	mov   edi,[SaveEdi]
	mov   ecx,20
DrawMouse32Loop1:
	push  ecx
	mov   ecx,16
DrawMouse32Loop2:
	lodsd
	cmp   eax,0
	je    NoPixel32
	mov   [es:edi],eax
NoPixel32:
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	loop  DrawMouse32Loop2
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  DrawMouse32Loop1
	ret

ReStoreMouseBAackGround:
	cmp   [First],0
	je    ItsTheFirst
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,8h
	mov   es,ax
	pop   eax
	call  [VesaResRMBG]
	pop  ds
	pop  es
	popad
ItsTheFirst:
	mov   [First],1
	ret

ReStoreMouseBAackGround24:
	mov   eax,[VarEax]
	mov   ebx,[VarEbx]
	xor   edx,edx
	mov   dx,[ModeInfo_BytesPerScanLine]
	imul  ebx,edx				; ebx = y * y multiplier MouseBack
	lea   edi,[eax+eax*2]			; edi = x*3
	add   edi,[ModeInfo_PhysBasePtr]
	add   edi,ebx				; ebx = where to put pixel in memory
	mov   esi,MouseBack
	mov   ecx,20
RsMouseBg24Loop1:
	push  ecx
	mov   ecx,16
RsMouseBg24Loop2:
	lodsd
	mov   [es:edi],ax
	shr   eax, 16
	mov   [es:edi+2],al
	inc   edi
	inc   edi
	inc   edi
	loop  RsMouseBg24Loop2
	pop   ecx
	sub   edi,16*3
	add   edi,edx
	loop  RsMouseBg24Loop1
	ret

ReStoreMouseBAackGround32:
	mov   eax,[VarEax]
	mov   ebx,[VarEbx]
	xor   edx,edx
	mov   dx,[ModeInfo_BytesPerScanLine]
	imul  ebx,edx  
	lea   edi,[ebx+eax*4]  
	add   edi,[ModeInfo_PhysBasePtr]
	xor   edx,edx
	mov   dx,[ModeInfo_BytesPerScanLine]
	mov   esi,MouseBack
	mov   ecx,20
RsMouseBg32Loop1:
	push  ecx
	mov   ecx,16
RsMouseBg32Loop2: 
	lodsd
	mov   [es:edi],eax
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	loop  RsMouseBg32Loop2
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  RsMouseBg32Loop1
	ret

 ;----------------------------------------------------;
 ; Mouse setup Vesa.                                  ;
 ;----------------------------------------------------;
SetUpMouseVesa:
	pushad
        mov   edi,MouseClickIDTest
        mov   ecx,1024
        mov   al,0
        cld
        rep   stosb
	cmp   [ModeInfo_BitsPerPixel],24
	jne   Not24 
	mov   edx,DrawMouse24
	mov   [VesaRes],edx
	mov   edx,ReStoreMouseBAackGround24
	mov   [VesaResRMBG],edx  
	jmp   @f 
Not24:
	mov   edx,DrawMouse32
	mov   [VesaRes],edx 
	mov   edx,ReStoreMouseBAackGround32
	mov   [VesaResRMBG],edx  
@@:
	call  ResetMouseLib
	cmp   ax,0xffff
	jne   MouseError
	xor   eax,eax
	mov   edx,eax
 ;----------------------------------------------------;
 ; Mouse set Min Max XY.                              ;
 ;----------------------------------------------------;  
	mov   ax,[ModeInfo_XResolution]
	mov   Dx,[ModeInfo_YResolution]
	sub   Dx,4
	sub   ax,10
	call  SetMouseMaxMinXYLib
 ;----------------------------------------------------;
 ; Mouse set start XY.                                ;
 ;----------------------------------------------------; 
	mov   ax,[ModeInfo_XResolution]
	mov   Dx,[ModeInfo_YResolution]
	shr   Dx,1
	shr   ax,1
	call  SetMouseXYLib
	xor   ecx,ecx
	xor   edx,edx
	call  GetMousePosLib
	mov   [VesaStartXx],cx
	mov   [VesaStartYy],dx
	mov   [VesaStartXold],cx
	mov   [VesaStartYold],dx
 ;----------------------------------------------------;
 ; Hook in mouse IRQ.                                 ;
 ;----------------------------------------------------;
	mov   eax,6Ch
	call  [GetIntVector]
	mov   [OldMouseInt],edx
	mov   edx,DoMouse
	mov   eax,6Ch
	call  [SetIntVector]
	popad
	clc
	ret

MouseError:
	popad
	stc
	ret



DrawMouse32B:
	pushad
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLineM],ebx
	mov   edx,[BytesPerScanLineM]
	xor   eax,eax 
	xor   ebx,ebx
	mov   ax,[current_x]
	mov   bx,[current_y]
	mov   [VarEax2],eax
	mov   [VarEbx2],ebx
	imul  ebx,edx  
	lea   edi,[ebx+eax*4]  
	add   edi,VesaBuffer
	mov   [SaveEdi2],edi
	mov   esi,MouseBack2
	mov   ecx,20
DrawMouse32Loop1SB:
	push  ecx
	mov   ecx,16
DrawMouse32Loop2SB:
	mov   eax,dword[es:edi]
	mov   dword[ds:esi],eax
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	inc   esi
	inc   esi
	inc   esi
	inc   esi
	loop  DrawMouse32Loop2SB
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  DrawMouse32Loop1SB
	mov   esi,mouse3_bmp32
	mov   edi,[SaveEdi2]
	mov   ecx,20
DrawMouse32Loop1B:
	push  ecx
	mov   ecx,16
DrawMouse32Loop2B:
	lodsd
	cmp   eax,0
	je    NoPixel32B
	mov   [es:edi],eax
NoPixel32B:
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	loop  DrawMouse32Loop2B
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  DrawMouse32Loop1B
	popad
	ret

 ;----------------------------------------------------;
 ; ReStoreMouseBAackGround32B                         ;
 ;----------------------------------------------------;
ReStoreMouseBAackGround32B:
	cmp    [FirstB],0
	je     ItsTheFirstB
	pushad
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLineM],ebx
	mov   edx,[BytesPerScanLineM]
	mov   eax,[VarEax2]
	mov   ebx,[VarEbx2]
	imul  ebx,edx  
	lea   edi,[ebx+eax*4]  
	add   edi,VesaBuffer
	mov   esi,MouseBack2
	mov   ecx,20
RsMouseBg32Loop1B:
	push  ecx
	mov   ecx,16
RsMouseBg32Loop2B:
	lodsd
	mov   [es:edi],eax
	inc   edi
	inc   edi
	inc   edi
	inc   edi
	loop  RsMouseBg32Loop2B
	pop   ecx
	sub   edi,16*4
	add   edi,edx
	loop  RsMouseBg32Loop1B
	popad
ItsTheFirstB:
	mov    [FirstB],1
	ret



