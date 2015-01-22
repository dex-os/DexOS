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
; MenuetOS to DexOS functions                    28/11/09 ;
;---------------------------------------------------------;
;                                                         ;
;=========================================================;

 ;=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 ;                                         Emulated MenuetOS Sysfunctions
 ;=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 ;  CLOSE_THIS_PROGRAM_X1           X1
 ;  DEFINE_AND_DRAW_WINDOW_00       00
 ;  GET_KEY_02                      02
 ;  GET_SYSTEM_CLOCK_03             03
 ;  WRITE_TEXT_TO_WINDOW_04         04
 ;  WRITE_TEXT_TO_WINDOW_04_D       04
 ;  WRITE_TEXT_TO_WINDOW_LEN_04     04
 ;  WRITE_TEXT_TO_WINDOW_LEN_04_D   04
 ;  DELAY_X_100_SECS_05             05
 ;  DEFINE_BUTTON_08                08
 ;  WAIT_FOR_EVENT_10               10
 ;  CHECK_FOR_EVENT_NO_WAIT_11      11
 ;  WINDOW_REDRAW_STATUS_12         12
 ;  DRAW_BAR_13                     13
 ;  GET_PRESSED_BUTTON_ID_17        17
 ;  WAIT_FOR_EVENT_WITH_TIMEOUT_23  23
 ;  DISPLAY_NUMBER_TO_WINDOW_47     47
 ;  CHANGE_SIZE_OR_COORDINATES_     ''
 ;  WINDOW_OF_APPLICATION_67        67
 ;  CHECK_BOUNDARIES_80             80
 ;  MOVE_MAIN_WINDOW_81             81

 ;---------------------------------------------------
 ; Function-1    CLOSE THIS PROGRAM
 ;---------------------------------------------------
CLOSE_THIS_PROGRAM_x1:
	;mov   edx,[OldMouseInt]
	;mov   eax,6Ch
	;call  [SetIntVector]
	;call  [SetMouseOff]
 ;----------------------------------------------------;
 ; return to text mode                                ;
 ;----------------------------------------------------;
	mov	ax,03h				      ;  move the number of the mode to ax
	call	[RealModeInt10h]		      ;  and enter the mode using int 10h
	xor	eax,eax
	call	[SetCursorPos]
	call	UnHook_int40h
	ret

;-----------------------------------------------------
; Function00   DEFINE AND DRAW WINDOW
;-----------------------------------------------------
;     ebx [x start]*65536 + [x size]
;     ecx [y start]*65536 + [y size]
;     edx body color     0xXYRRGGBB  if X=8 -> color glide
;                                    if Y=0 -> window type  I
;                                    if Y=1 -> reserve area, no draw
;                                    if Y=2 -> window type  II
;                                    if Y=3 -> skinned window,
;                                              closebutton id=1 added
;     esi grab bar color 0xX0RRGGBB  if X=8 -> negative color glide
;                                    if X=4 -> positive color glide
;     edi frame color    0x00RRGGBB
;     ret: nothing changed
;------------------------------------------------------
DEFINE_AND_DRAW_WINDOW_00:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EbxM2D]
	shr   eax,16				     
	mov   edx,dword[EbxM2D]
	shl   edx,16
	shr   edx,16				     
	mov   ebx,dword[EcxM2D]
	shr   ebx,16				     
	mov   ecx,dword[EcxM2D]
	shl   ecx,16
	shr   ecx,16				     
	mov   esi,dword[EsiM2D] 		     
	mov   edi,dword[EdxM2D] 		     
	call  DrawStandWindow			     
DEFINE_AND_DRAW_WINDOW_EXIT:
	pop   ds
	pop   es
	popad
	clc
	ret

;-----------------------------------------------------
; Function02   GET KEY
;-----------------------------------------------------
;     ret: al 0 successful -> ah = key
;          al 1 no key in buffer
;-----------------------------------------------------
GET_KEY_02:
	cmp   byte[KeyBuffer],0
	jne   GotKey
	cmp   byte[KeyBufferScan],0
	je    NO_KEY_IN_BUFFER
GotKey:
	mov   ah,byte[KeyBuffer]
	mov   bl,byte[KeyBufferScan]
	mov   al,0
	mov   byte[KeyBuffer],0
	mov   byte[KeyBufferScan],0
	ret
NO_KEY_IN_BUFFER:
	mov   al,1
	ret

;-----------------------------------------------------
; Function03   GET SYSTEM CLOCK
;-----------------------------------------------------
;     ret: eax 0x00SSMMHH sec,min,hour
;-----------------------------------------------------
GET_SYSTEM_CLOCK_03:
	pushad
	call   [GetTime]
	mov    ebx,eax
	mov    edx,eax
	shl    eax,24
	shr    eax,8
	shr    ebx,16
	mov    ah,dh
	mov    al,bl
	mov    dword[EaxM2D],eax
	popad
	mov    eax,dword[EaxM2D]
	ret

;-----------------------------------------------------
;    WRITE TEXT TO WINDOW _OLD 
;-----------------------------------------------------
;     ebx [x start]*65536 + [y start], the window X Y is also added
;     ecx color         - 0x00RRGGBB
;     eax font size     - small = 1 
;                       - large = 2
;     edx pointer to text beginning
;     esi text length (NOT used)
;     ret: nothing changed
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_04_OLD:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EbxM2D] 		    
	shr   eax,16
	add   ax,word[MainWindowX]		    
	shl   eax,16
	mov   ecx,dword[EbxM2D] 		    
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		     
	mov   ecx,dword[EaxM2D] 		     
	mov   esi,dword[EdxM2D] 		     
	call  PrintBitMapFonts
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_OLD	     
WRITE_TEXT_TO_WINDOW_EXIT_OLD:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_OLD:		     
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW
;-----------------------------------------------------
WRITE_TEXT_dos2x:
       cmp     edi,0
       je      WRITE_TEXT_TO_WINDOW_04
       jmp     WRITE_TEXT_TO_WINDOW_LEN_04

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx color         - 0x00RRGGBB
;     eax font size     - small = 1 
;                       - large = 2
;     esi pointer to text beginning
;     edi must zero
;     ret: nothing changed
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_04:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		    
	shr   eax,16
	add   ax,word[MainWindowX]		    
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		    
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		     
	mov   ecx,dword[EaxM2D] 		     
	mov   esi,dword[EsiM2D] 		     
	call  PrintBitMapFonts
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR	     
WRITE_TEXT_TO_WINDOW_EXIT:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR:		     
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW BACKGROUND COLOR
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx font color       - 0x00RRGGBB
;     edi background color - 0x00RRGGBB
;     eax font size        - small = 1 
;                          - large = 2
;     esi pointer to text beginning
;     ret: nothing changed
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_04_BGColor:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		    
	shr   eax,16
	add   ax,word[MainWindowX]		    
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		    
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		     
	mov   ecx,dword[EaxM2D] 		     
	mov   esi,dword[EsiM2D] 
	mov   edi,dword[EdiM2D] 	     
	call  PrintBitMapFonts_BGColor
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_BGColor	     
WRITE_TEXT_TO_WINDOW_EXIT_BGColor:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_BGColor:		     
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW LEN
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx color         - 0x00RRGGBB
;     eax font size     - small = 1 
;                       - large = 2
;     esi pointer to text beginning
;     edi text length 
;     ret: nothing changed
;
;    This same as above but users length, not zero
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_LEN_04: 
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		     
	shr   eax,16
	add   ax,word[MainWindowX]		     
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		     
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		    
	mov   ecx,dword[EaxM2D] 		    
	mov   esi,dword[EsiM2D] 		     
	mov   edi,dword[EdiM2D] 
	call  PrintBitMapFontsLen
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_LEN    
WRITE_TEXT_TO_WINDOW_EXIT_LEN:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_LEN:		     
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW Direct
;-----------------------------------------------------
WRITE_TEXT_dos2x_Direct:
       cmp     edi,0
       je      WRITE_TEXT_TO_WINDOW_04_D
       jmp     WRITE_TEXT_TO_WINDOW_LEN_04_D

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW Direct
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx color         - 0x00RRGGBB
;     eax font size     - small = 1 
;                       - large = 2
;     esi pointer to text beginning
;     edi must be zero
;     ret: nothing changed
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_04_D:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		    
	shr   eax,16
	add   ax,word[MainWindowX]		    
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		    
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		     
	mov   ecx,dword[EaxM2D] 		     
	mov   esi,dword[EsiM2D] 		     
	call  PrintBitMapFonts_Direct
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_D        
WRITE_TEXT_TO_WINDOW_EXIT_D:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_D:		       
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW Direct BACKGROUND COLOR
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx font color       - 0x00RRGGBB
;     edi background color - 0x00RRGGBB
;     eax font size        - small = 1 
;                          - large = 2
;     esi pointer to text beginning
;     ret: nothing changed
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_04_D_BGColor:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		    
	shr   eax,16
	add   ax,word[MainWindowX]		    
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		    
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		     
	mov   ecx,dword[EaxM2D] 		     
	mov   esi,dword[EsiM2D]
	mov   edi,dword[EdiM2D] 		     
	call  PrintBitMapFonts_Direct_BGColor
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_D_BGColor        
WRITE_TEXT_TO_WINDOW_EXIT_D_BGColor:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_D_BGColor:		       
	pop   ds
	pop   es
	popad
	stc
	ret

;-----------------------------------------------------
; Function04   WRITE TEXT TO WINDOW LEN Direct
;-----------------------------------------------------
;     edx [x start]*65536 + [y start], the window X Y is also added
;     ecx color         - 0x00RRGGBB
;     eax font size     - small = 1 
;                       - large = 2
;     esi pointer to text beginning
;     edi text length 
;     ret: nothing changed
;
;    This same as above but users length, not zero
;-----------------------------------------------------
WRITE_TEXT_TO_WINDOW_LEN_04_D: 
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EdxM2D] 		     
	shr   eax,16
	add   ax,word[MainWindowX]		     
	shl   eax,16
	mov   ecx,dword[EdxM2D] 		     
	shl   ecx,16
	shr   ecx,16
	add   cx,word[MainWindowY]		     
	mov   ax,cx
	mov   edx,dword[EcxM2D] 		    
	mov   ecx,dword[EaxM2D] 		    
	mov   esi,dword[EsiM2D] 		     
	mov   edi,dword[EdiM2D] 
	call  PrintBitMapFontsLen_Direct
	jc    WRITE_TEXT_TO_WINDOW_EXIT_ERROR_LEN_D    
WRITE_TEXT_TO_WINDOW_EXIT_LEN_D:
	pop   ds
	pop   es
	popad
	clc
	ret

WRITE_TEXT_TO_WINDOW_EXIT_ERROR_LEN_D:		       
	pop   ds
	pop   es
	popad
	stc
	ret



;-----------------------------------------------------
; Function05   DELAY X/100 SECS
;-----------------------------------------------------
;     ebx delay in 1/100 secs
;     ret: nothing changed
;-----------------------------------------------------
DELAY_X_100_SECS_05:
	pushad
	mov    ecx,ebx
DELAY_X_100_SECS_loop:
	call   FullVertWait
	loop   DELAY_X_100_SECS_loop
	popad
	ret
;-----------------------------------------------------
; Function08   DEFINE BUTTON
;-----------------------------------------------------
;     ebx [x start]*65536 + [x size] the window X is also added
;     ecx [y start]*65536 + [y size] the window Y is also added
;     edx button id number (24 bits)
;     esi button color 0x 00 RR GG BB
;     edi button image (edx bit 29)
;     ret: nothing changed
;
;     if bit 31 is set in edx then function removes a button matching the id
;     note that function doesn't remove the button image to avoid
;     unnecessary flickering - when for example changing the colour
;
;     if bit 30 is set in edx then button image is not drawn
;
;     if bit 29 is set in edx then button users AARRGGBB image 
;     first dword of image X, second dword Y, third
;     start of button image
;-----------------------------------------------------
DEFINE_BUTTON_08:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	test  edx,0x80000000
	jnz   remove_button
	test  edx,0x40000000
	jnz   button_no_draw
	test  edx,0x20000000
	jnz   button_use_image
	call  SetButtonList
use_default_button:
	mov   eax,dword[EbxM2D] 		     
	shr   eax,16
	add   ax,word[MainWindowX]		     
	mov   edx,dword[EbxM2D]
	shl   edx,16
	shr   edx,16				     
	mov   ebx,dword[EcxM2D]
	shr   ebx,16				     
	add   bx,word[MainWindowY]		     
	mov   ecx,dword[EcxM2D]
	shl   ecx,16
	shr   ecx,16				     
	mov   esi,dword[EsiM2D]
	shl   esi,8
	shr   esi,8				     
	call  DrawFilledBoxFadeVesa		     
	mov   edi,0x00bbbbbb			     
	mov   esi,0x00222222			     
	call  DrawButtonBoxVesa 		     
	jmp   DEFINE_BUTTON_EXIT
remove_button:
	pushad
	mov   esi,ButtonStruct			     
	mov   ecx,dword[esi]			     
	mov   ebx,dword[EdxM2D] 		     
	mov   edi,ButtonStruct
remove_button_loop:
	add   edi,16				     
	cmp   dword[edi],ebx			     
	je    FoundButton
	loop  remove_button_loop
	popad
	jmp   DEFINE_BUTTON_EXIT		     
FoundButton:
	mov  dword[edi],0			     
	popad
	jmp   DEFINE_BUTTON_EXIT
button_no_draw:
	call  SetButtonList
	jmp   DEFINE_BUTTON_EXIT
button_use_image:
	call  SetButtonList
DEFINE_BUTTON_EXIT:
	pop   ds
	pop   es
	popad
	clc
	ret

DEFINE_BUTTON_EXIT_ERROR:			    
	pop   ds
	pop   es
	popad
	stc
	ret

SetButtonList:
	pushad
	mov   esi,ButtonStruct
	cmp   dword[esi],255
	jae   SetButtonListExit
	inc   dword[esi]
	mov   edi,dword[esi]
	shl   edi,4
	add   edi,ButtonStruct
	mov   eax,dword[EdxM2D] 		     
	stosd
	mov   eax,dword[EbxM2D] 		     
	shr   eax,16
	add   ax,word[MainWindowX]		     
	stosw
	mov   eax,dword[EbxM2D] 		     
	stosw
	mov   eax,dword[EcxM2D]
	shr   eax,16				     
	add   ax,word[MainWindowY]		    
	stosw
	mov   eax,dword[EcxM2D] 		     
	stosw
SetButtonListExit:
	popad
	ret


;-----------------------------------------------------
; Function08   DEFINE BUTTON GUI
;-----------------------------------------------------
;     ebx [x start]*65536 + [x size] the window X is also added
;     ecx [y start]*65536 + [y size] the window Y is also added
;     edx button id number (24 bits)
;     esi button color 0x 00 RR GG BB
;     edi button image (edx bit 29)
;     ret: nothing changed
;
;     if bit 31 is set in edx then function removes a button matching the id
;     note that function doesn't remove the button image to avoid
;     unnecessary flickering - when for example changing the colour
;
;     if bit 30 is set in edx then button image is not drawn
;
;     if bit 29 is set in edx then button users AARRGGBB image 
;     first dword of image X, second dword Y, third
;     start of button image
;
;     NOTE: This is to be used by GUI, not 
;           user program.
;-----------------------------------------------------
DEFINE_BUTTON_GUI_08:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	test  edx,0x80000000
	jnz   remove_button_GUI
	test  edx,0x40000000
	jnz   button_no_draw_GUI
	test  edx,0x20000000
	jnz   button_use_image_GUI
	call  SetButtonList_GUI
use_default_button_GUI:
	mov   eax,dword[EbxM2D] 		    
	shr   eax,16
	;add   ax,word[MainWindowX]                   
	mov   edx,dword[EbxM2D]
	shl   edx,16
	shr   edx,16				     
	mov   ebx,dword[EcxM2D]
	shr   ebx,16				     
	;add   bx,word[MainWindowY]                   
	mov   ecx,dword[EcxM2D]
	shl   ecx,16
	shr   ecx,16				     
	mov   esi,dword[EsiM2D]
	shl   esi,8
	shr   esi,8				     
	call  DrawFilledBoxVesa 
	mov   edi,0x00bbbbbb			     
	mov   esi,0x00222222			     
	call  DrawButtonBoxVesa 		     
	jmp   DEFINE_BUTTON_EXIT_GUI
remove_button_GUI:
	pushad
	mov   esi,ButtonStruct			     
	mov   ecx,dword[esi]			     
	mov   ebx,dword[EdxM2D] 		     
	mov   edi,ButtonStruct
remove_button_loop_GUI:
	add   edi,16				     
	cmp   dword[edi],ebx			     
	je    FoundButton_GUI
	loop  remove_button_loop_GUI
	popad
	jmp   DEFINE_BUTTON_EXIT_GUI			 
FoundButton_GUI:
	mov  dword[edi],0			     
	popad
	jmp   DEFINE_BUTTON_EXIT_GUI
button_no_draw_GUI:
	call  SetButtonList_GUI
	jmp   DEFINE_BUTTON_EXIT_GUI
button_use_image_GUI:
	call  SetButtonList_GUI
DEFINE_BUTTON_EXIT_GUI:
	pop   ds
	pop   es
	popad
	clc
	ret

DEFINE_BUTTON_EXIT_ERROR_GUI:			       
	pop   ds
	pop   es
	popad
	stc
	ret

SetButtonList_GUI:
	pushad
	mov   esi,ButtonStruct
	cmp   dword[esi],255
	jae   SetButtonListExit_GUI
	inc   dword[esi]
	mov   edi,dword[esi]
	shl   edi,4
	add   edi,ButtonStruct
	mov   eax,dword[EdxM2D] 		     
	stosd
	mov   eax,dword[EbxM2D] 		     
	shr   eax,16
	;add   ax,word[MainWindowX]                   
	stosw
	mov   eax,dword[EbxM2D] 		    
	stosw
	mov   eax,dword[EcxM2D]
	shr   eax,16				    
	;add   ax,word[MainWindowY]                   
	stosw
	mov   eax,dword[EcxM2D] 		     
	stosw
SetButtonListExit_GUI:
	popad
	ret

       
;-----------------------------------------------------
; Function10   WAIT FOR EVENT
;-----------------------------------------------------
;      ret: eax event type, 1 window redraw,
;           2 key in buffer, 3 button pressed
;      note:
;           Need to add window redraw code
;-----------------------------------------------------
WAIT_FOR_EVENT_10:
	call  [KeyPressedScanNW]
	cmp   al,0
	jne   WAIT_FOR_EVENT_KEY
	cmp   ah,0
	jne   WAIT_FOR_EVENT_KEY
	cmp   dword[MouseClickIDYesNo],0
	jne   WAIT_FOR_EVENT_BUTTON
	call  GetMousePosLib
	cmp   bl,0
	jne   WAIT_FOR_EVENT_RED
	hlt
	jmp  WAIT_FOR_EVENT_10

WAIT_FOR_EVENT_RED:				     
	mov   eax,1
	ret
WAIT_FOR_EVENT_KEY:				     
	mov   byte[KeyBuffer],al		     
	mov   byte[KeyBufferScan],ah
	mov   eax,2 
	ret
WAIT_FOR_EVENT_BUTTON:				     
	mov   eax,3
	ret

;-----------------------------------------------------
; Function11   CHECK FOR EVENT, NO WAIT
;-----------------------------------------------------
;      ret: eax 0 no event, 1 window redraw,
;           2 key in buffer, 3 button pressed
;      note:
;           Need to add window redraw code
;-----------------------------------------------------
CHECK_FOR_EVENT_NO_WAIT_11:
	call  [KeyPressedScanNW]
	cmp   al,0
	jne   CHECK_FOR_EVENT_NO_WAIT_KEY
	cmp   ah,0
	jne   CHECK_FOR_EVENT_NO_WAIT_KEY
	cmp   dword[MouseClickIDYesNo],0
	jne   CHECK_FOR_EVENT_NO_WAIT_BUTTON
	call  GetMousePosLib
	cmp   bl,0
	jne   CHECK_FOR_EVENT_NO_WAIT_RED
CHECK_FOR_EVENT_NO_WAIT_EXIT:			     
	xor   eax,eax
	ret

CHECK_FOR_EVENT_NO_WAIT_RED:			    
	mov   eax,1
	ret
CHECK_FOR_EVENT_NO_WAIT_KEY:			     
	mov   byte[KeyBuffer],al		     
	mov   byte[KeyBufferScan],ah
	mov   eax,2 
	ret
CHECK_FOR_EVENT_NO_WAIT_BUTTON: 		     
	mov   eax,3
	ret

;-----------------------------------------------------
; Function12   WINDOW REDRAW STATUS
;-----------------------------------------------------
;     ebx  1 start of redraw, 2 end of redraw
;     ret: nothing changed
;-----------------------------------------------------
WINDOW_REDRAW_STATUS_12:
	pushad
	push  es
	push  ds
	cmp   ebx,1
	je    WINDOW_REDRAW_STATUS_EXIT2
	cmp   ebx,2
	jne   WINDOW_REDRAW_STATUS_EXIT1
	;call  BuffToScreen
WINDOW_REDRAW_STATUS_EXIT1:
	pop   ds
	pop   es
	popad
	ret

WINDOW_REDRAW_STATUS_EXIT2:
	mov   edi,ButtonStruct
	mov   al,0
	mov   ecx,256*16
	cld
	rep   stosb
	pop   ds
	pop   es
	popad
	ret


;-----------------------------------------------------
; Function13   DRAW BAR
;-----------------------------------------------------
;
;     ebx [x start]*65536 + [x size]
;     ecx [y start]*65536 + [y size]
;     edx color 0x00RRGGBB
;     ret: nothing changed
;-----------------------------------------------------
DRAW_BAR_13:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EbxM2D] 		     
	shr   eax,16
	add   ax,word[MainWindowX]		     
	mov   edx,dword[EbxM2D]
	shl   edx,16
	shr   edx,16				     
	mov   ebx,dword[EcxM2D]
	shr   ebx,16				    
	add   bx,word[MainWindowY]		     
	mov   ecx,dword[EcxM2D]
	shl   ecx,16
	shr   ecx,16				     
	mov   esi,dword[EdxM2D]
	shl   esi,8
	shr   esi,8				     
	call  DrawFilledBoxVesa 		     
	pop   ds
	pop   es
	popad
	ret


;-----------------------------------------------------
; Function17   GET PRESSED BUTTON ID
;-----------------------------------------------------
;     ret: al 0 successful -> ah  = id number (8 bits)
;               shr eax,8  -> eax = id number (24 bits)
;          al 1 no key in buffer
;-----------------------------------------------------
GET_PRESSED_BUTTON_ID_17:
	mov   eax,1
	cmp   dword[MouseClickIDYesNo],1
	jne   GET_PRESSED_BUTTON_ID_EXIT
	mov   eax,dword[MouseClickID]
	shl   eax,8
	mov   dword[MouseClickIDYesNo],0
	mov   dword[MouseClickID],0		 
GET_PRESSED_BUTTON_ID_EXIT:
	ret


;-----------------------------------------------------
; Function23   WAIT FOR EVENT WITH TIMEOUT
;-----------------------------------------------------
;     ebx time to delay in hs
;     ret: eax event type: 0 no event, 1 window redraw,
;                          2 key in buffer, 3 button
; NOTE: 
;      You will need to set clock ticks to
;      100 ticks per second to be 100% compatable
;-----------------------------------------------------
WAIT_FOR_EVENT_WITH_TIMEOUT_23:
	cmp   ebx,0
	je    WAIT_FOR_EVENT_WITH_TIMEOUT_EXIT
	mov   eax,ebx
	call  [TimerNoWait]
WAIT_FOR_EVENT_WITH_TIMEOUT_Loop:
	call  [KeyPressedScanNW]
	cmp   al,0
	jne   WAIT_FOR_EVENT_WITH_TIMEOUT_KEY
	cmp   ah,0
	jne   WAIT_FOR_EVENT_WITH_TIMEOUT_KEY
	;call  GetMousePosLib
	cmp   dword[MouseClickIDYesNo],0
	jne   WAIT_FOR_EVENT_WITH_TIMEOUT_BUTTON
	call  GetMousePosLib
	cmp   bl,0
	jne   WAIT_FOR_EVENT_WITH_TIMEOUT_RED
	mov   al,0
	call  [TimerNoWait]
	cmp   al,1
	je    WAIT_FOR_EVENT_WITH_TIMEOUT_Loop
WAIT_FOR_EVENT_WITH_TIMEOUT_EXIT:		     
	xor   eax,eax
	ret

WAIT_FOR_EVENT_WITH_TIMEOUT_RED:		     
	mov   eax,1
	ret
WAIT_FOR_EVENT_WITH_TIMEOUT_KEY:		     
	mov   byte[KeyBuffer],al		     
	mov   byte[KeyBufferScan],ah
	mov   eax,2 
	ret
WAIT_FOR_EVENT_WITH_TIMEOUT_BUTTON:		    
	mov   eax,3
	ret

;-----------------------------------------------------
; Function37   READ MOUSE POSITION
;-----------------------------------------------------
;
;     ebx=0 screen relative
;     ebx=1 window relative
;     ebx=2 buttons pressed
;     return in eax
;
;-----------------------------------------------------
READ_MOUSE_POSITION_37:
	cmp   ebx,0
	jne   TryWinRelative
	call  GetMousePosLib
	mov   ax,cx
	shl   eax,16
	mov   ax,dx
	jmp   READ_MOUSE_POSITION_EXIT
TryWinRelative:
	cmp   ebx,1
	jne   TryButtonPressed
	call  GetMousePosLib
	cmp   cx,word[MainWindowX]
	jb    READ_MOUSE_POSITION_EXIT_BADXY
	sub   cx,word[MainWindowX]
	mov   ax,cx
	shl   eax,16
	cmp   dx,word[MainWindowY]
	jb    READ_MOUSE_POSITION_EXIT_BADXY
	sub   dx,word[MainWindowY]
	mov   ax,dx
	jmp   READ_MOUSE_POSITION_EXIT
TryButtonPressed:
	cmp   ebx,2
	jne   READ_MOUSE_POSITION_EXIT
	call  GetMousePosLib
	xor   eax,eax
	mov   al,bl
READ_MOUSE_POSITION_EXIT:
	ret

READ_MOUSE_POSITION_EXIT_BADXY:
	mov   eax,-1
	shl   eax,16
	mov   ax,-1
	ret

;-----------------------------------------------------
; Function38   DRAW LINE
;-----------------------------------------------------
;
;     ebx  [x start] shl 16 + [x end]
;     ecx  [y start] shl 16 + [y end]
;     edx  colour 0x00RRGGBB
;
;     return : nothing changed
;-----------------------------------------------------
DRAW_LINE_38:
	pushad
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EbxM2D]
	mov   ebx,dword[EcxM2D]
	mov   ecx,dword[EdxM2D]
	call  draw_line
	popad	     
	ret

;-----------------------------------------------------
; Function38A   DRAW LINE
;-----------------------------------------------------
;
;     eax = Screen_Min_X
;     ebx = Screen_Max_X
;     ecx = Screen_Min_Y
;     edx = Screen_Max_Y
;
;     return : nothing changed
;-----------------------------------------------------
DRAW_LINE_SET_YX_38A:
	pushad
	add   ax,word[MainWindowX] 
	mov   dword[Screen_Min_X],eax
	add   bx,word[MainWindowX] 
	mov   dword[Screen_Max_X],ebx
	add   cx,word[MainWindowY]
	mov   dword[Screen_Min_Y],ecx
	add   dx,word[MainWindowY]
	mov   dword[Screen_Max_Y],edx
	popad
	ret

;-----------------------------------------------------
; Function47   DISPLAY NUMBER TO WINDOW
;-----------------------------------------------------
;
;     eax = print type, al=0 -> ecx is number
;                       al=1 -> ecx is pointer
;                       ah=0 -> display decimal
;                       ah=1 -> display hexadecimal
;                       ah=2 -> display binary
;           bits 16-21 = number of digits to display (0-32)
;           bits 22-31 = reserved
;     ecx = number or pointer
;     edx = x shl 16 + y
;     esi = color
;     edi = background color or zero for no background color
;-----------------------------------------------------
DISPLAY_NUMBER_TO_WINDOW_47:
	pushad
	push  es
	push  ds
	mov   dword[EdiM2D],edi
	mov   dword[EsiM2D],esi
	mov   dword[EaxM2D],eax
	mov   dword[EbxM2D],ebx
	mov   dword[EdxM2D],edx
	mov   dword[EcxM2D],ecx
	mov   eax,dword[EaxM2D]
	mov   ebx,dword[EcxM2D]
	mov   edx,dword[EsiM2D]
	mov   ecx,dword[EdxM2D]
	mov   edi,dword[EdiM2D]
	call  DisplayNumber
	pop   ds
	pop   es
	popad 
	ret


;-----------------------------------------------------------------
; Function67  CHANGE SIZE OR COORDINATES - WINDOW OF APPLICATION 
;-----------------------------------------------------------------
;     ebx = a new position on an axis x
;     ecx = a new position on an axis y
;     edx = new width (not implemented yet)
;     esi = new height (not implemented yet)
;
;     The parameters, which value is equal -1, are ignored.
;     For example, specify ebx = -1 and ecx = -1, if you want
;     to change a size of the window, but to not move it.
;-----------------------------------------------------------------
CHANGE_SIZE_OR_COORDINATES_WINDOW_OF_APPLICATION_67: 
	pushad
	push  es
	push  ds
	cmp   bx,-1
	je    @f
	mov   word[MainWindowX],bx
@@:
	cmp   cx,-1
	je    @f
	mov   word[MainWindowY],cx
@@: 
	cmp   dx,-1
	je    @f
	mov   word[MainWindowXsize],dx
@@:
	cmp   di,-1
	je    @f
	mov   word[MainWindowXsize],di
@@:
	pop   ds
	pop   es
	popad 
	ret

;-----------------------------------------------------------------
; Function80  CHECK BOUNDARIES
;-----------------------------------------------------------------
;     eax = Min X
;     ebx = Max X
;     ecx = Min Y
;     edx = Max Y
;     esi = 0   Do not test for mouse button press
;
;-----------------------------------------------------------------

TestMin_x      dw 0
TestMax_x      dw 0
TestMin_y      dw 0
TestMax_y      dw 0   ;24

CHECK_BOUNDARIES_80:
	pushad	
	mov   word[TestMin_x],ax
	mov   word[TestMax_x],bx
	mov   word[TestMin_y],cx
	mov   word[TestMax_y],dx
	cmp   esi,0
	jne   NotZero
	call  GetMousePosLib
	jmp   @f
NotZero:
	call  GetMousePosLib		       
	cmp   bl,0			       
	je    CheckTopBarExitNoKeyPress
@@:				       
	xor   ebx,ebx						       
	mov   bx,[TestMin_x]		       
	add   bx,word[MainWindowX] 
	cmp   cx,bx			       
	ja    Check_MaxX		       
	jmp   Not_In_Zone		       
Check_MaxX:
	mov   bx,word[TestMax_x]				       
	add   bx,word[MainWindowX]	       
	cmp   cx,bx			       
	jb    Store_NewX		       
	jmp   Not_In_Zone		       
Store_NewX:					       
	mov   bx,[TestMin_y]		       
	add   bx,word[MainWindowY]	       
	cmp   dx,bx			       
	ja    Check_MaxY		       
	jmp   Not_In_Zone		       
	jmp   Store_NewY		       
Check_MaxY:				       
	mov   bx,[TestMax_y]		       
	add   bx,word[MainWindowY]	       
	cmp   dx,bx			       
	jb    Store_NewY		       
	jmp   Not_In_Zone		       
Store_NewY:				       
	popad				       
	clc				       
	ret				       

CheckTopBarExitNoKeyPress:					       
Not_In_Zone:				       
	popad				       
	stc				       
	ret

;-----------------------------------------------------------------
; Function81A  SET_SAVE_BOX_SIZE
;-----------------------------------------------------------------
;     eax = START X
;     ebx = START Y
;     edx = SIZE  X
;     ecx = SIZE  Y
;
; Call this before MOVE_MAIN_WINDOW_81
;-----------------------------------------------------------------
MainWindowXSaveBox	     dd 0	 
MainWindowYSaveBox	     dd 0      
MainWindowXsizeSaveBox	     dd 0   
MainWindowYsizeSaveBox	     dd 0

SET_SAVE_BOX_SIZE:
	mov   [MainWindowXSaveBox],eax
	mov   [MainWindowYSaveBox],ebx 
	mov   [MainWindowXsizeSaveBox],edx
	mov   [MainWindowYsizeSaveBox],ecx
	ret

;-----------------------------------------------------------------
; Function81  MOVE MAIN WINDOW
;-----------------------------------------------------------------
;
; Change  MOVE_MAIN_WINDOW_COLOR to change box color
;
;-----------------------------------------------------------------

MOVE_MAIN_WINDOW_COLOR =  0x0052ba0d
align 4 				       
VesaStartXxOld dw 0			       
VesaStartYyOld dw 0			       
align 4 				       
CdPlayerFirst  db 0			       
align 4 				       
CdPlayerStartX dd 0			       
CdPlayerStartY dd 0			       
CdPlayerMouseX dd 0			       
CdPlayerMouseY dd 0	

MOVE_MAIN_WINDOW_81:
	pushad
 ;----------------------------------------------------;
 ; Move the window code                               ;
 ;----------------------------------------------------;
	mov   edi,MOVE_MAIN_WINDOW_COLOR		      
	mov   esi,MOVE_MAIN_WINDOW_COLOR	       
	xor   eax,eax
	mov   ebx,eax
	mov   edx,eax
	mov   ecx,eax
	mov   ax,word[MainWindowX]	       
	mov   bx,word[MainWindowY]	       
	mov   dx,word[MainWindowXsize]	       
	mov   cx,word[MainWindowYsize]	       
	call  SaveBoxVesa		       
	call  DrawBoxVesaDirect 	      
	mov   [vesaMOUSEoff],1
	call  ReStoreMouseBAackGround
	pushad
	mov   eax,dword[MainWindowXSaveBox]		
	mov   ebx,dword[MainWindowYSaveBox]		
	mov   edx,dword[MainWindowXsizeSaveBox] 	
	mov   ecx,dword[MainWindowYsizeSaveBox] 
	mov   edi,BufferTemp	   
	call  SaveFilledBoxVesaDirect
	popad	     
align 4        
MoveWinLoop:				      
	hlt			       
	call  GetMousePosLib		       
	cmp   bl,0			       
	je    MoveCdPlayer
	mov   word[current_x_abc],cx	
	mov   word[current_y_abc],dx		       
	cmp   byte[CdPlayerFirst],0	       
	jne   NotFirst			      
	pushad
	mov   eax,dword[MainWindowXSaveBox]		
	mov   ebx,dword[MainWindowYSaveBox]		
	mov   edx,dword[MainWindowXsizeSaveBox] 	
	mov   ecx,dword[MainWindowYsizeSaveBox] 
	mov   esi,BufferTemp	   
	call  ReStoreFilledBoxVesa
	popad
	mov   [NoMouseInt],1		      
	call  BuffToScreen
	mov   [NoMouseInt],0		       
	xor   edx,edx			      
	mov   dx,word[current_x_abc]		      
	mov   [VesaStartXxOld],dx	       
	sub   dx,word[MainWindowX]	       
	mov   dword[CdPlayerMouseX],edx       
	mov   dx,word[current_y_abc]	      
	mov   [VesaStartYyOld],dx	       
	sub   dx,word[MainWindowY]	       
	mov   dword[CdPlayerMouseY],edx        
	mov   byte[CdPlayerFirst],1	       
 ;----------------------------------------------------;
 ; Not the first time                                 ;
 ;----------------------------------------------------;
NotFirst:				       
	mov   ax,word[MainWindowX]	       
	mov   bx,word[MainWindowY]	       
	mov   dx,word[MainWindowXsize]	       
	mov   cx,word[MainWindowYsize]	       
	call  ReStoreBoxVesa		       
	xor   edx,edx			       
	xor   ebx,ebx			       
	mov   dx,word[current_x_abc]		       
	mov   [VesaStartXxOld],dx	       
	cmp   edx,dword[CdPlayerMouseX]        
	jb    MaxXok1			       
	sub   edx, dword[CdPlayerMouseX]       
	mov   word[MainWindowX],dx	  
 ;----------------------------------------------------;
 ; Check the XY of box is not off the screen          ;
 ;----------------------------------------------------;
MinXok1:				       
	mov   bx,[ModeInfo_XResolution]        
	sub   bx,word[MainWindowXsize]		
	cmp   edx,ebx			       
	jbe   MaxXok1			       
	mov   word[MainWindowX],bx	  
MaxXok1:				      
	mov   dx,word[current_y_abc]	       
	mov   [VesaStartYyOld],dx  
	cmp   edx,dword[CdPlayerMouseY]  
	jae   MinYok1
	mov   word[MainWindowY],1	
	mov   dword[CdPlayerMouseY],1
	jmp   MaxYok1			       
MinYok1:				       
	sub   edx,dword[CdPlayerMouseY]        
	mov   word[MainWindowY],dx	  
	mov   bx,[ModeInfo_YResolution]        
	sub   bx,word[MainWindowYsize] 
	cmp   edx,ebx			       
	jbe   MaxYok1			       
	mov   word[MainWindowY],bx	  
MaxYok1:
 ;----------------------------------------------------;
 ; Draw the wire frammed box                          ;
 ;----------------------------------------------------;
	mov   edi,MOVE_MAIN_WINDOW_COLOR	       
	mov   esi,MOVE_MAIN_WINDOW_COLOR       
	xor   eax,eax
	mov   ebx,eax
	mov   edx,eax
	mov   ecx,eax
	mov   ax,word[MainWindowX]	       
	mov   bx,word[MainWindowY]	       
	mov   dx,word[MainWindowXsize]	       
	mov   cx,word[MainWindowYsize]	       
	call  SaveBoxVesa		       
	call  DrawBoxVesaDirect 	       
	mov   [vesaMOUSEoff],1	
	jmp   MoveWinLoop		       
 ;----------------------------------------------------;
 ; Draw the window at new address                     ;
 ;----------------------------------------------------;
align 4
MoveCdPlayer:				       
	mov   [vesaMOUSEoff],0		       
	mov   byte[CdPlayerFirst],0	       
	call  DrawBackGround 
	popad
	ret


current_x_abc dw 0
current_y_abc dw 0

;Data
TeamDexOSMes:  db  'Men2Dex, Code by Craig Bamford 2011(c) ',0

align 4
KeyBuffer:	      db    0
KeyBufferScan	      db    0
align 4
MainWindowX	      dw    0
MainWindowY	      dw    0
MainWindowXsize       dw    0
MainWindowYsize       dw    0
EdiM2D		      dd    0
EsiM2D		      dd    0
EaxM2D		      dd    0
EbxM2D		      dd    0
EdxM2D		      dd    0
EcxM2D		      dd    0

