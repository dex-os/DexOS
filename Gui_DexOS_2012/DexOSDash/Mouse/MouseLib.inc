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
 ; PutMousePointer                                    ;
 ;----------------------------------------------------;
MouseMovmentClick:
	mov   bl,0xad				
	call  keyboard_cmd
	xor   edi,edi				
	mov   ecx,PS2_PACKETSIZE
PS2MouseISR_ReadPacket: 
	push  cx
	call  keyboard_read	
	pop   cx		
	or    ah,ah
	jnz   PS2MouseISR_ExitReadPacket
	mov   byte[recvbuffer+edi],al
	inc   edi
	loop  PS2MouseISR_ReadPacket
PS2MouseISR_ExitReadPacket:
	mov   ax,di
	mov   [datasize],al
	mov   al,[recvbuffer]	
	and   al,7h
	mov   ah,al
	and   ah,1
	shl   ah,2
	mov   bl,al
	and   bl,6
	shr   bl,1
	or    ah,bl
	mov   al,[buttons]
	mov   [buttons],ah
	xor   ah,al
	shl   ah,1
	or    [eventmsk],ah	
	mov   al,[recvbuffer+1] 		
	mov   ah,[recvbuffer]			
	and   ah,10h
	shr   ah,4
	neg   ah
	add   [current_x],ax	
	mov   [delta_x],ax
	or    ax,ax
	setnz bl
	or    [eventmsk],bl			
	mov   al,[recvbuffer+2] 		
	mov   ah,[recvbuffer]			
	and   ah,0x20
	shr   ah,5
	neg   ah
	neg   ax
	add   [current_y], ax	
	mov   [delta_y],ax
	or    ax,ax
	setnz bl
	or    [eventmsk],bl
 ;----------------------------------------------------;
 ; check boundaries                                   ;
 ;----------------------------------------------------;
	mov   dx,[current_x]
	cmp   dx,[min_x]			
	jnl   PS2MouseISR_CheckMaxX
	mov   dx,[min_x]
	jmp   PS2MouseISR_StoreNewX
PS2MouseISR_CheckMaxX:
	cmp   dx,[max_x]
	jl    PS2MouseISR_StoreNewX
	mov   dx,[max_x]
PS2MouseISR_StoreNewX:
	mov   [current_x],dx
	mov   dx,[current_y]
	cmp   dx,[min_y]			
	jnl   PS2MouseISR_CheckMaxY
	mov   dx,[min_y]
	jmp   PS2MouseISR_StoreNewY
PS2MouseISR_CheckMaxY:
	cmp   dx,[max_y]
	jl    PS2MouseISR_StoreNewY
	mov   dx,[max_y]
PS2MouseISR_StoreNewY:
	mov   [current_y],dx
@@:
	mov	bl,0aeh 			
	call	keyboard_cmd
	ret

 ;----------------------------------------------------;
 ; Gets mouse pointer                                 ;
 ;----------------------------------------------------;

get_mouse_pointer2:					
	push  eax					   
	push  ebx
	xor   ebx,ebx
	mov   bx,[current_x ]
	shl   ebx,1
	add   edi, ebx
	xor   ebx,ebx
	mov   bx,[current_y]
	mov   eax,0xa0
	mul   bx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

get_mouse_pointer1:					
	push  eax					   
	push  ebx
	xor   ebx,ebx
	mov   bx,[current_x1]
	shl   ebx,1
	add   edi, ebx
	xor   ebx,ebx
	mov   bx,[current_y1]
	mov   eax,0xa0
	mul   bx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; ResetMouse                                         ;
 ;----------------------------------------------------;
ResetMouseLib:
       ; cmp   [PS2Mouse],1
       ; jne   NoPs2MouseExit
	cli
	pushad
	mov   bl,0xa8				
	call  keyboard_cmd
	mov   [min_x],0 
	mov   [max_x],799
	mov   [min_y],0
	mov   [max_y],599
	mov   [buttons],0
	mov   [current_x],400
	mov   [current_y],300
	mov   [current_x1],400
	mov   [current_y1],300
	mov   [MouseShow],0
	popad
	mov   ax,0xffff
	sti
	ret
NoPs2MouseExit:
	xor  ax,ax
	ret
 ;----------------------------------------------------;
 ; ShowMouse                                          ;
 ;----------------------------------------------------;
ShowMouseLib:
	cli
	mov   [MouseShow],1
	sti
	ret
 ;----------------------------------------------------;
 ; HideMouse                                          ;
 ;----------------------------------------------------;   
HideMouseLib:
	cli
	mov   [MouseShow],0
	sti
	ret
 ;----------------------------------------------------;
 ; GetMousePos                                        ;
 ;----------------------------------------------------;
GetMousePosLib:
	cli
	mov   bl,[buttons]
	mov   cx,[current_x]
	mov   dx,[current_y]
	sti
	ret
 ;----------------------------------------------------;
 ; SetMouseMaxMinXY                                   ;
 ;----------------------------------------------------;
SetMouseMaxMinXYLib:
	cli
	mov   [max_x],ax
	shr   eax,16
	mov   [min_x],ax
	mov   [max_y],dx
	shr   edx,16
	mov   [min_y],dx
	sti
	ret
 ;----------------------------------------------------;
 ; SetMouseXY                                         ;
 ;----------------------------------------------------;
SetMouseXYLib:
	cli
	mov   [current_x],ax
	mov   [current_y],dx
	sti
	ret
 ;----------------------------------------------------;
 ; SetMouseXY                                         ;
 ;----------------------------------------------------;
SetMouseOffLib:
	cli
	pushad
	mov	bl,0a7h 			
	call	keyboard_cmd
	popad
	sti
	ret

;====================================================;
; keyboardRead.                                      ;
;====================================================;
keyboard_read:
	push  ecx
	push  edx
	mov   ecx,0xffff
key_read_loop:
	in    al,0x64
	test  al,1
	jnz   key_read_ready
	loop  key_read_loop
	mov   ah,1
	jmp   key_read_exit
key_read_ready:
	push  ecx
	mov   ecx,32
key_read_delay:
	loop  key_read_delay
	pop   ecx
	in    al,0x60
	xor   ah,ah
key_read_exit:
	pop   edx
	pop   ecx
	ret

;====================================================;
; keyboardWrite.                                     ;
;====================================================;
keyboard_write:
	push  ecx
	push  edx
	mov   dl,al
	mov   ecx,0ffffh
kbd_wrt_loop1:
	in    al,0x64
	test  al,0x20
	jz    kbd_wrt_ok1
	loop  kbd_wrt_loop1
	mov   ah,1
	jmp   kbd_wrt_exit
kbd_wrt_ok1:
	in    al,0x60
	mov   ecx,0xffff
kbd_wrt_loop:
	in    al,0x64
	test  al,2
	jz    kbd_wrt_ok
	loop  kbd_wrt_loop
	mov   ah,1
	jmp   kbd_wrt_exit
kbd_wrt_ok:
	mov   al,dl
	out   0x60,al
	mov   ecx,0ffffh
kbd_wrt_loop3:
	in    al,64h
	test  al,2
	jz    kbd_wrt_ok3
	loop  kbd_wrt_loop3
	mov   ah,1
	jmp   kbd_wrt_exit
kbd_wrt_ok3:
	mov   ah,8
kbd_wrt_loop4:
	mov   ecx,0xffff
kbd_wrt_loop5:
	in    al,64h
	test  al,1
	jnz   kbd_wrt_ok4
	loop  kbd_wrt_loop5
	dec   ah
	jnz   kbd_wrt_loop4
kbd_wrt_ok4:
	xor   ah,ah
kbd_wrt_exit:
	pop   edx
	pop   ecx
	ret

;====================================================;
; keyboard_cmd.                                      ;
;====================================================;
keyboard_cmd:
	xor   eax,eax
	mov   ecx,0xffff
cmd_wait:
	in    al,64h
	test  al,2
	jz    cmd_send
	loop  cmd_wait

	jmp   cmd_error
cmd_send:
	mov   al,bl
	out   64h,al
	mov   ecx,0xffff
cmd_accept:
	in    al,0x64
	test  al,2
	jz    cmd_ok
	loop  cmd_accept
cmd_error:
	mov   ah,1
	jmp   cmd_exit
cmd_ok:
	xor   ah,ah
cmd_exit:
	ret

;====================================================;
; Data.                                              ;
;====================================================;
align 4
MouseClickIDTest:
           dd   0
align 4
MouseClickID: 
           dd   0
           dw   0
           dw   0
           dw   0
           dw   0
MouseClickIDYesNo:
           dd   0

PS2_PACKETSIZE	=	3
align 4
MouseShow	db	0
align 4
SaveChar	dw	0
min_x		dw	0
max_x		dw	79
min_y		dw	0 
align 4
max_y		dw	49
eventmsk	db	0
align 4
recvbuffer	db	0,0,0,0,0,0,0,0
align 4
datasize	db	0
buttons 	db	0
current_x	dw	40 
current_y	dw	25
current_x1	dw	40  
current_y1	dw	25
delta_x 	dw	0
delta_y 	dw	0
align 4
 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
ButtonReleased     db 0
ButtonOld          db 0
MenuHighLightShow  db 0
StartMenuShow	   db 0
HighLightOld	   dd 1
BytesPerScanLineM  dd 0
SaveEdi2	   dd 0
vesaMOUSEoff	   db 0
OldMouseInt	   dd 0
VarEax		   dd 0
VarEbx		   dd 0
VarEax2 	   dd 0
VarEbx2 	   dd 0
First		   db 0
FirstB		   db 0
StartMenuMin_x	   dw 0
StartMenuMax_x	   dw 0
StartMenuMin_y	   dw 0
StartMenuMax_y	   dw 0justtest       rb  1024
