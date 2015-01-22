;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.05                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2011                                                ;;
;; Filename         :   Gui.inc                                                     ;;
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

;====================================================;
; DisplayNumber.                                     ;
;====================================================;
;
; eax = print type, al=0 -> ebx is number
;                   al=1 -> ebx is pointer
;                   ah=0 -> display decimal
;                   ah=1 -> display hexadecimal
;                   ah=2 -> display binary
;                   eax bits 16-21 = number of digits to display (0-32)
;                   eax bits 22-31 = reserved
;
; ebx = number or pointer
; ecx = x shl 16 + y
; edx = color
DisplayNumber:
    mov   byte[DisplayNumberBGvar],0
    cmp   edi,0
    je    @f
    mov   byte[DisplayNumberBGvar],1
    mov   dword[DisplayNumberBGcolor],edi
@@:
    xor   edi,edi
    push  eax
    and   eax,0x3fffffff
    cmp   eax,0xffff            ; length > 0 ?
    pop   eax
    jge   cont_displ
    ret

   cont_displ:
     push  eax
     and   eax,0x3fffffff
     cmp   eax,61*0x10000        ; length <= 60 ?
     pop   eax
     jb    cont_displ2
     ret

   cont_displ2:
     pushad
     mov   ecx,32
     mov   esi,LetterBuffer1
     mov   edi,LetterBuffer
     cld
     rep   movsb
     popad
     pushad
     mov   dword[DisplayNumberColor],edx
     mov   dword[XYsave],ecx 

     cmp   al,1                  ; ecx is a pointer ?
     jne   displnl1
     mov   ebx,[ebx]
   displnl1:

     cmp   ah,0                  ; DESIMAL
     jne   no_display_desnum
     shr   eax,16
     and   eax,0xC03f
     ;and   eax,0x2f
     push  eax
     and   eax,0x3f
     mov   edi,LetterBuffer
     add   edi,31                 
     mov   ecx,eax
     mov   eax,ebx
     mov   ebx,10
   d_desnum:
     xor   edx,edx
     div   ebx
     add   dl,48
     mov   [edi],dl
     dec   edi
     loop  d_desnum
     pop   eax
     mov   esi,LetterBuffer
     add   esi,32    
     sub   esi,eax
     mov   edi,eax
     mov   ecx,2
     mov   edx,dword[DisplayNumberColor] 
     mov   eax,dword[XYsave]
     shr   eax,16
     add   ax,word[MainWindowX] 
     mov   [Xsave],ax
     mov   eax,dword[XYsave]
     add   ax,word[MainWindowY] 
     mov   [Ysave],ax
     mov   ax,[Xsave]
     shl   eax,16
     mov   ax,[Ysave]
     cmp   byte[DisplayNumberBGvar],0
     je    NoBG1
     mov   ebx,dword[DisplayNumberBGcolor]
     call  PrintBitMapFontsLen_BGColor
     jmp   NoBG1end
NoBG1:
     call  PrintBitMapFontsLen 
NoBG1end:
     popad
     ret

   no_display_desnum:

     cmp   ah,0x01               ; HEXADECIMAL
     jne   no_display_hexnum
     shr   eax,16
     and   eax,0xC03f
     ;and   eax,0x2f
     push  eax
     and   eax,0x3f
     mov   edi,LetterBuffer
     add   edi,31                  
     mov   ecx,eax
     mov   eax,ebx
     mov   ebx,16
   d_hexnum:
     xor   edx,edx
     div   ebx
     add   edx,hexletters
     mov   dl,[edx]
     mov   [edi],dl
     dec   edi
     loop  d_hexnum
     pop   eax
     mov   esi,LetterBuffer
     add   esi,32
     sub   esi,eax
     mov   edi,eax
     mov   ecx,2
     mov   edx,dword[DisplayNumberColor] 
     mov   eax,dword[XYsave]
     shr   eax,16
     add   ax,word[MainWindowX] 
     mov   [Xsave],ax
     mov   eax,dword[XYsave]
     add   ax,word[MainWindowY] 
     mov   [Ysave],ax
     mov   ax,[Xsave]
     shl   eax,16
     mov   ax,[Ysave]
     cmp   byte[DisplayNumberBGvar],0
     je    NoBG2
     mov   ebx,dword[DisplayNumberBGcolor]
     call  PrintBitMapFontsLen_BGColor
     jmp   NoBG2end
NoBG2:
     call  PrintBitMapFontsLen 
NoBG2end:
     popad
     ret

   no_display_hexnum:

     cmp   ah,0x02               ; BINARY
     jne   no_display_binnum
     shr   eax,16
     and   eax,0xC03f
     ;and   eax,0x2f
     push  eax
     and   eax,0x3f
     mov   edi,LetterBuffer
     add   edi,31                
     mov   ecx,eax
     mov   eax,ebx
     mov   ebx,2
   d_binnum:
     xor   edx,edx
     div   ebx
     add   dl,48
     mov   [edi],dl
     dec   edi
     loop  d_binnum
     pop   eax
     mov   esi,LetterBuffer
     add   esi,32
     sub   esi,eax
     mov   edi,eax
     mov   ecx,2                                            ;change this for big fonts to 2
     mov   edx,dword[DisplayNumberColor] 
     mov   eax,dword[XYsave]
     shr   eax,16
     add   ax,word[MainWindowX] 
     mov   [Xsave],ax
     mov   eax,dword[XYsave]
     add   ax,word[MainWindowY] 
     mov   [Ysave],ax
     mov   ax,[Xsave]
     shl   eax,16
     mov   ax,[Ysave]
     cmp   byte[DisplayNumberBGvar],0
     je    NoBG3
     mov   ebx,dword[DisplayNumberBGcolor]
     call  PrintBitMapFontsLen_BGColor
     jmp   NoBG3end
NoBG3:
     call  PrintBitMapFontsLen 
NoBG3end:
     popad
     ret
   no_display_binnum:
     popad
     ret

draw_num_text:
     ret
DisplayNumberBGvar db 0
DisplayNumberBGcolor dd 0
DisplayNumberColor dd 0
XYsave dd 0
Xsave  dw 0
Ysave  dw 0

hexletters  db '0123456789ABCDEF'

LetterBuffer:   db '                                '
LetterBuffer1:  db '                                '
