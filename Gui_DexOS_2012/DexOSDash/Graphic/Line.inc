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

; This must be called before anything else
SetVesaModeSys:
        pushad
        cmp   dword[ModeInfo_BitsPerPixel],24
        je    Vesa24NotGood
        mov   dword[GETPIXEL],Vesa20_getpixel32
        mov   dword[PUTPIXEL],Vesa20_putpixel32
        xor   eax,eax
        mov   ax,[ModeInfo_XResolution]
        mov   dword[Screen_Max_X],eax
        mov   ax,[ModeInfo_YResolution]
        mov   dword[Screen_Max_Y],eax
        mov   ax,word[ModeInfo_BytesPerScanLine]
        mov   dword[BytesPerScanLineMOS],eax
        popad
        ret

Vesa24NotGood:
        mov   dword[GETPIXEL],Vesa20_getpixel24
        mov   dword[PUTPIXEL],Vesa20_putpixel24
        xor   eax,eax
        mov   ax,[ModeInfo_XResolution]
        mov   dword[Screen_Max_X],eax
        mov   ax,[ModeInfo_YResolution]
        mov   dword[Screen_Max_Y],eax
        mov   ax,word[ModeInfo_BytesPerScanLine]
        mov   dword[BytesPerScanLineMOS],eax
        popad
        ret
;*************************************************
; DRAWLINE
;     inc   [mouse_pause]
;     call    [_display.disable_mouse]
;
; draw a line
; eax = HIWORD = x1
;       LOWORD = x2
; ebx = HIWORD = y1
;       LOWORD = y2
; ecx = color
; edi = force ?
dl_x1 equ esp+20
dl_y1 equ esp+16
dl_x2 equ esp+12
dl_y2 equ esp+8
dl_dx equ esp+4
dl_dy equ esp+0
align 4
draw_line:
       ; mov   [vesaMOUSEoff],1
        pushad
        mov   dword[MenColor],ecx
        xor   edx,edx                        ; clear edx
        xor   esi,esi                        ; unpack arguments
        xor   ebp,ebp
        mov   si,ax                          ; esi = x2
        mov   bp,bx                          ; ebp = y2
        shr   eax,16                         ; eax = x1
        shr   ebx,16                         ; ebx = y1
        push  eax                            ; save x1
        push  ebx                            ; save y1
        push  esi                            ; save x2
        push  ebp                            ; save y2
; checking x-axis...
        sub   esi,eax                        ; esi = x2-x1
        push  esi                            ; save y2-y1
        jl    .x2lx1                         ; is x2 less than x1 ?
        jg    .no_vline                      ; x1 > x2 ?
        mov   edx,ebp                        ; else (if x1=x2)
        call  vline
        push  edx                            ; necessary to rightly restore stack frame at .exit
        jmp   .exit
.x2lx1:
        neg   esi                            ; get esi absolute value
.no_vline:
; checking y-axis...
        sub   ebp,ebx                        ; ebp = y2-y1
        push  ebp                            ; save y2-y1
        jl    .y2ly1                         ; is y2 less than y1 ?
        jg    .no_hline                      ; y1 > y2 ?
        mov   edx,[dl_x2]                    ; else (if y1=y2)
        call  hline
        jmp   .exit
.y2ly1:
        neg   ebp                            ; get ebp absolute value
.no_hline:
        cmp   ebp, esi
        jle   .x_rules                       ; |y2-y1| < |x2-x1|  ?
        cmp   [dl_y2], ebx                   ; make sure y1 is at the begining
        jge   .no_reverse1
        neg   dword[dl_dx]
        mov   edx,[dl_x2]
        mov   [dl_x2],eax
        mov   [dl_x1],edx
        mov   edx,[dl_y2]
        mov   [dl_y2],ebx
        mov   [dl_y1],edx
.no_reverse1:
        mov   eax, [dl_dx]
        cdq                                   ; extend eax sing to edx
        shl   eax,16                          ; using 16bit fix-point maths
        idiv  ebp                             ; eax = ((x2-x1)*65536)/(y2-y1)
        mov   edx,ebp                         ; edx = counter (number of pixels to draw)
        mov   ebp,1 *65536                    ; <<16   ; ebp = dy = 1.0
        mov   esi,eax                         ; esi = dx
        jmp   .y_rules
.x_rules:
        cmp   [dl_x2], eax                    ; make sure x1 is at the begining
        jge   .no_reverse2
        neg   dword[dl_dy]
        mov   edx,[dl_x2]
        mov   [dl_x2],eax
        mov   [dl_x1],edx
        mov   edx,[dl_y2]
        mov   [dl_y2],ebx
        mov   [dl_y1],edx
.no_reverse2:
        xor   edx,edx
        mov   eax,[dl_dy]
        cdq                                    ; extend eax sing to edx
        shl   eax,16                           ; using 16bit fix-point maths
        idiv  esi                              ; eax = ((y2-y1)*65536)/(x2-x1)
        mov   edx,esi                          ; edx = counter (number of pixels to draw)
        mov   esi,1 *65536                     ; << 16   ; esi = dx = 1.0
        mov   ebp,eax                          ; ebp = dy
.y_rules:
        mov   eax,[dl_x1]
        mov   ebx,[dl_y1]
        shl   eax,16
        shl   ebx,16
align 4
.draw:
        push  eax ebx
        shr   eax,16
        shr   ebx,16
        call  [putpixel]
        pop   ebx eax
        add   ebx,ebp                             ; y = y+dy
        add   eax,esi                             ; x = x+dx
        dec   edx
        jnz   .draw
; force last drawn pixel to be at (x2,y2)
        mov   eax,[dl_x2]
        mov   ebx,[dl_y2]
        call  [putpixel]
.exit:
        add   esp,6*4
        popad
;       dec   [mouse_pause]
;       call  [draw_pointer]
        ;mov   [vesaMOUSEoff],0
        ret


hline:
; draw an horizontal line
; eax = x1
; edx = x2
; ebx = y
; ecx = color
; edi = force ?
     push    eax edx
     cmp     edx, eax                            ; make sure x2 is above x1
     jge     @f
     xchg    eax, edx
align   4
@@:
     call    [putpixel]
     inc     eax
     cmp     eax, edx
     jle     @b
     pop     edx eax
     ret


vline:
; draw a vertical line
; eax = x
; ebx = y1
; edx = y2
; ecx = color
; edi = force ?
     push    ebx edx
     cmp     edx, ebx                          ; make sure y2 is above y1
     jge     @f
     xchg    ebx, edx
align   4
@@:
     call    [putpixel]
     inc     ebx
     cmp     ebx, edx
     jle     @b
     pop     edx ebx
     ret


;*************************************************
; getpixel
;
; eax = x coordinate
; ebx = y coordinate
;
; ret:
; ecx = 00 RR GG BB
;*************************************************
getpixel:
        push  eax ebx edx edi
        call  dword [GETPIXEL]
        pop   edi edx ebx eax
        ret

; eax = x
; ebx = y
Vesa20_getpixel24:
	push  es
        push  eax
	mov   ax,8h
	mov   es,ax
        pop   eax
        imul  ebx, [BytesPerScanLineMOS]    ; ebx = y * y multiplier
        lea   edi, [eax+eax*2]              ; edi = x*3
        add   edi, ebx                      ; edi = x*3+(y*y multiplier)
        add   edi,[ModeInfo_PhysBasePtr]
        mov   ecx, [es:edi]
        and   ecx, 0xffffff
        pop   es
        ret

Vesa20_getpixel32:
	push  es
        push  eax
	mov   ax,8h
	mov   es,ax
        pop   eax
        imul  ebx,[BytesPerScanLineMOS]      ; ebx = y * y multiplier
        lea   edi,[ebx+eax*4]                ; edi = x*4+(y*y multiplier)
        add   edi,[ModeInfo_PhysBasePtr]
        mov   ecx,[es:edi]
        and   ecx,0xffffff
        pop   es
        ret

;*************************************************
; eax = x coordinate
; ebx = y coordinate
; ecx = ?? RR GG BB    ; 0x01000000 negation
; edi = 0x00000001 force
;*************************************************
align 4
__sys_putpixel:

;;;        mov  [novesachecksum], dword 0
         pushad
         cmp   [Screen_Max_X], eax
         jb    .exit
         cmp   [Screen_Max_Y], ebx
         jb    .exit
         cmp   [Screen_Min_X], eax
         ja    .exit
         cmp   [Screen_Min_Y], ebx
         ja    .exit
        ;test  edi,1                       ; force ?
        ;jnz   .forced
; not forced:
        ;push eax
        ;mov  edx,[_display.width]         ; screen x size
        ;imul edx, ebx
        ;add  eax, [_WinMapAddress]
        ;movzx  edx, byte [eax+edx]
        ;cmp edx, [CURRENT_TASK]
        ;pop eax
        ;jne .exit
.forced:
; check if negation
       ;test  ecx,0x01000000
       ;jz    .noneg
       ;call  getpixel
       ;not   ecx
       ;mov   [StoreColor],ecx
.noneg:
; OK to set pixel
        call  dword[PUTPIXEL]             ; call the real put_pixel function
.exit:
        popad
        ret

; eax = x
; ebx = y
align 4
Vesa20_putpixel24:
	push  es
        push  eax
	mov   ax,8h
	mov   es,ax
        pop   eax
        imul  ebx,[BytesPerScanLineMOS]    ; ebx = y * y multiplier
        lea   edi,[eax+eax*2]              ; edi = x*3
        mov   eax,[MenColor]
        add   edi,[ModeInfo_PhysBasePtr]
        mov   [es:edi+ebx], ax
        shr   eax, 16
        mov   [es:edi+ebx+2], al
        pop   es
        ret

; eax = x
; ebx = y
align 4
Vesa20_putpixel32:
	push  es
        push  ecx
        push  eax
	mov   ax,8h
	mov   es,ax
        pop   eax
        imul  ebx,[BytesPerScanLineMOS]     ; ebx = y * y multiplier
        lea   edi,[ebx+eax*4]               ; edi = x*4+(y*y multiplier)
        mov   eax,[MenColor]                ; eax = color
        mov   ecx,[ModeInfo_PhysBasePtr]
        mov   [es:edi+ecx], eax
        pop   ecx
        pop   es
        ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
combine_colors:

      ; in
      ; ecx - color ( 00 RR GG BB )
      ; edi - ref to new color byte
      ; esi - ref to alpha byte
      ;
      ; out
      ; ecx - new color ( roughly (ecx*[esi]>>8)+([edi]*[esi]>>8) )
push es
push  eax
mov  ax,18h
mov  es,ax
pop  eax
      push eax
      push ebx
      push edx
      push ecx
      xor ecx, ecx
         ; byte 2
      mov eax, 0xff
      sub al, [esi+0]
      mov ebx, [esp]
      shr ebx, 16
      and ebx, 0xff
      mul ebx
      shr eax, 8
      add ecx, eax
      xor eax, eax
      xor ebx, ebx
      mov al, [edi+0]
      mov bl, [esi+0]
      mul ebx
      shr eax, 8
      add ecx, eax
      shl ecx, 8
         ; byte 1
      mov eax, 0xff
      sub al, [esi+1]
      mov ebx, [esp]
      shr ebx, 8
      and ebx, 0xff
      mul ebx
      shr eax, 8
      add ecx, eax
      xor eax, eax
      xor ebx, ebx
      mov al, [edi+1]
      mov bl, [esi+1]
      mul ebx
      shr eax, 8
      add ecx, eax
      shl ecx, 8
         ; byte 2
      mov eax, 0xff
      sub al, [esi+2]
      mov ebx, [esp]
      and ebx, 0xff
      mul ebx
      shr eax, 8
      add ecx, eax
      xor eax, eax
      xor ebx, ebx
      mov al, [edi+2]
      mov bl, [esi+2]
      mul ebx
      shr eax, 8
      add ecx, eax

      pop eax
      pop edx
      pop ebx
      pop eax
pop es
      ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;data
putpixel              dd __sys_putpixel
Screen_Max_X          dd 0
Screen_Max_Y          dd 0
Screen_Min_X          dd 0
Screen_Min_Y          dd 0
StoreColor            dd 0
GETPIXEL              dd 0
PUTPIXEL              dd 0
MenColor              dd 0
BytesPerScanLineMOS     dd 0
