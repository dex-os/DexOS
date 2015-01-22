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
 ; PrintTimeGui.      ; prints the time in text mode  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          none.                                     ;
 ;  Output:                                           ;
 ;          prints the time in text mode.             ;
 ;                                             (100%) ;
 ;....................................................; 

PrintTimeGui:
	push  es
	pushad
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLine],ebx
        ;call  RestoreTimeBG
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	sub   eax,55 ;80
	mov   [vesa_fontsmall_x],eax
	xor   eax,eax
	mov   ax,[ModeInfo_YResolution]
	sub   ax,22
	mov   [vesa_fontsmall_y],eax
	call  [GetTime]
	mov   [minute],ah
	mov   [second],al
	shr   eax,16
	mov   [hour],al
	mov   al,[hour]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	mov   edx,SkinTextColor ;text color
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
	mov   al,":"
	call  PrintVesaChar16
	mov   al,[minute]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
       ; mov   al,":"
       ; call  PrintVesaChar16
       ; mov   al,[second]
       ; call  bcd_to_ASCII
       ; mov   bx,[val]
       ; mov   al,bl
       ; call  PrintVesaChar16
       ; mov   al,bh
       ; call  PrintVesaChar16
	popad
	pop   es
	ret

Checktime:
	pushad
	call  [GetTime]
	cmp   [minute],ah
	je    NoMinChange
	call  PrintTimeGui
	;call  FullVertWait
	call  BuffToScreen
NoMinChange:
	popad
	ret
 ;----------------------------------------------------;
 ; prints date        ; prints the date in text mode  ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;          none.                                     ;
 ;  Output:                                           ;
 ;          prints the date in text mode.             ;
 ;                                             (100%) ;
 ;....................................................; 

print_date:
	push  es
	pushad
	call  [GetDate]  
	mov   [month],ah
	mov   [day],al
	shr   eax,16 
	mov   [century],ah
	mov   [year],al     
	popad
	pop   es
PrintDateGui:
	pushad
	mov   [vesa_fontsmall_x],404
	mov   [vesa_fontsmall_y],160
	mov   al,[day]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	mov   edx,SkinTextColor           
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
	mov   al,":"
	call  PrintVesaChar16
	mov   al,[month]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
	mov   al,":"
	call  PrintVesaChar16
	mov   al,[century]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   bx,0x3032
	mov   al,bl
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
	mov   al,[year]
	call  bcd_to_ASCII
	mov   bx,[val]
	mov   al,bl
	call  PrintVesaChar16
	mov   al,bh
	call  PrintVesaChar16
	popad
	ret


 ;----------------------------------------------------;
 ; bcd_to_ASCII         ;converts bcd number to ASCII ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;   Input:                                           ;
 ;         Bcd number in al.                          ;
 ;  Output:                                           ;
 ;         ASCII number in val.                       ;
 ;                                             (100%) ;
 ;....................................................;
		
bcd_to_ASCII:
       pushad						;start of convert
       mov  ah,al					;copy AL to AH
       and  ax,0f00fh					;mask bits
       mov  cl,4					;CL=04 for shift
       shr  ah,cl					;shift right AH to get unpacked BCD
       or   ax, 3030h					;combine with 30 to get ASCII
       xchg ah,al					;swap for ASCII storage convention
       mov  [val],ax					;store the ASCII value in VAL
       popad 
       ret


 ;====================================================;
 ; Data.                                              ;
 ;====================================================;
val:	   dw 0,0

hour:	   db 0
minute:    db 0
second:    db 0

century:   db 0
year:	   db 0
month:	   db 0
day:	   db 0
