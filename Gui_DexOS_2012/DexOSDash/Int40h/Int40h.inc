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

FunctionMax = 11 					  ;
;=======================================================  ;
;  int_40h                              ( int functions)  ;
;                                                         ;
;=======================================================  ;
int_40h:						  ;
	cmp	ebx,0					  ;
	jne	Not_Zero				  ;
	mov	esi,FunctionTable			  ;
	jmp	int40hOK				  ;
  Not_Zero:						  ;
	cmp	ebx,FunctionMax 			  ;
	ja	int40hError				  ;
	shl	ebx,2					  ;
	add	ebx,FunctionTable			  ;                     
	call	dword[ebx]				  ;
	jc	int40hError				  ;
  int40hOK:						  ;
	and	byte [esp+8],not 1			  ; 
	iret						  ;
							  ;
int40hError:						  ; 
	or	byte [esp+8],1				  ; 
	iret						  ;

OldIn40hAddress  dd  0					  ;
;=======================================================  ;
;  Hook_int40h                                            ;
;=======================================================  ;
Hook_int40h:						  ;
	pushad						  ;
	mov	eax,40h 				  ; put interrupt number in AL
	call	[GetIntVector]				  ; New function to get int vector
	mov	[OldIn40hAddress],edx			  ; The offset is returned in EDX, CX = selector (BUT WE DO NOT CHANGE selector)
							  ;
	mov	edx,int_40h				  ; EDX = interrupt handler address
	mov	eax,40h 				  ; put interrupt number in AL
	call	[SetIntVector]				  ; New function to set int vector
	popad						  ;
	ret						  ;

;=======================================================  ;
;  UnHook_int40h                                          ;
;=======================================================  ;
UnHook_int40h:						  ;
	pushad						  ;
	mov	edx,[OldIn40hAddress]			  ; EDX = old interrupt handler address
	mov	eax,40h 				  ; put interrupt number in AL
	call	[SetIntVector]				  ; New function to set int vector
	popad						  ;
	ret						  ;

;=======================================================  ;
;  Test function                                          ;
;=======================================================  ;
Test_function:						  ;
	ret						  ;

;=======================================================  ;
;  UnHook_int40h                                          ;
;=======================================================  ;
FunctionTable:						  ;
Function0   dd FunctionTable				  ; Address of this FunctionTable.
Function1   dd VesaBufferGet_Set		          ; Get/set buffer address
Function2   dd DeCodeJpeg				  ; deCode jpeg
Function3   dd DIFdecoder                                 ; decodes a dex image format
Function4   dd DIFdecoder                                 ; decodes a dex font format
Function5   dd PutImage32				  ; Draw image to screen xy offset
Function6   dd PutImageAlpha32                            ; Draw image to screen xy offset with alpha blending
Function7   dd WRITE_TEXT_dos2x                           ; print string to offscreen buffer
Function8   dd WRITE_TEXT_dos2x_Direct                    ; Print string dirct to screen
Function9   dd WRITE_TEXT_TO_WINDOW_04_BGColor            ; print string to offscreen buffer + BACKGRUND COLOR
Function10  dd WRITE_TEXT_TO_WINDOW_04_D_BGColor          ; Print string dirct to screen + BACKGRUND COLOR
Function11  dd DISPLAY_NUMBER_TO_WINDOW_47                ; Prints and converts numbers
FunctionT   dd Test_function				  ; spair function
