;*****************************************************************************
;  DIF Decoder:
;
; input:
;           esi = DIF file (Source buffer)
;           edi = DIF file uncompressed (Dest buffer)
;           ecx = Temp buffer, the same size as the (Dest buffer)
;
; output:
;           dest buffer + 0 = X
;           dest buffer + 4 = Y
;           dest buffer + 8 = Uncompressed image (32bpp format 0xAARRGGBB)
;
; error:    sets CF to 1   (ax = error code, 0 = OK, 1 = not a dif file, 2 = jpeg error)
;
; regs:
;           all regs saved (unless error, then ax = error code)
;
;******************************************************************************
DIFdecoder:
	pushad						       ; save regs
	mov   word[DifErrorCode],0			       ; zero error code var
	mov   dword[DifOutPutBuffer],edi		       ; save address of Image dest (uncompressed)
	mov   dword[DifinPutBuffer],esi 		       ; save address of compressed  (Source buffer)
	mov   dword[DifTempOutPutBuffer],ecx		       ; save address of temp buffer
	mov   esi,dword[DifinPutBuffer] 		       ; get what at offset 0 of the Dif file
	mov   word[DifErrorCode],1			       ; put the error code 1 in error var
	lodsd						       ;
	cmp   eax, 'DIF1'				       ; check for the 'DIF1' dword
	je    FormatOK 					       ; if SO than jump to OK label
        cmp   eax, 'DSF1'                                      ; check for the 'DSF1' dword
	je    FormatOK 					       ; if SO than jump to OK label
	cmp   eax, 'DFF1'				       ; check for the 'DFF1' dword
        jne   DifError                                         ; if not than jump to error
FormatOK:                                                      ; the file format is OK
	lodsd						       ; get the first jpeg address offset
	add   eax,dword[DifinPutBuffer] 		       ; add it to the load address
	mov   dword[Alpha1Address],eax			       ; save it
	lodsd						       ; load the address of second jpeg
	mov   dword[Alpha2Address],eax			       ; save it
	sub   eax,4					       ; then take size of the 'DIF1' of it
	mov   dword[Alpha1Size],eax			       ; and save it as size of first jpeg
	mov   eax,dword[DifinPutBuffer] 		       ; get the dif load address
	add   dword[Alpha2Address],eax			       ; add it to the second jpeg address var
	lodsd						       ; load the next dword from dif header
	mov   dword[Alpha2Size],eax			       ; store it as size of second jpeg
	mov   edi,dword[DifOutPutBuffer]		       ; restore the image dest address
	mov   esi,dword[Alpha1Address]			       ; restore the image source address
	mov   ecx,dword[Alpha1Size]			       ; restore size of first jpeg
	mov   word[DifErrorCode],2			       ; set error code to 2
	call  DeCodeJpeg				       ; call jpeg decoding function
	jc    DifError					       ; CF flag set on error
	mov   edi,dword[DifTempOutPutBuffer]		       ; restore the temp buffer address
	mov   esi,dword[Alpha2Address]			       ; restore the second jpeg address
	mov   ecx,dword[Alpha2Size]			       ; plus size (compressed)
	call  DeCodeJpeg				       ; call jpeg decoding function
	jc    DifError					       ; CF flag set on error
	mov   word[DifErrorCode],0			       ; set the error var to 0
	pushad						       ; save the regs
	mov   esi,dword[DifTempOutPutBuffer]		       ; load the temp buffer address
	lodsd						       ; load the X size
	mov   word[ImageXdif],ax			       ; store it
	mov   esi,dword[DifTempOutPutBuffer]		       ;
	add   esi,4					       ;
	lodsd						       ; get the Y size
	mov   word[ImageYdif],ax			       ; store it
	xor   eax,eax					       ; zero eax
	mov   ecx,eax					       ; zero ecx
	mov   ax, word[ImageXdif]			       ; get the X size
	mov   cx,word[ImageYdif]			       ; get the y size
	mul   ecx					       ; multy ply them
	mov   ecx,3					       ;
	div   ecx	 ;cccccccccccc                         ; div eax, by 3
	mov   ecx,eax					       ; keep it for the loop number
	mov   esi,dword[DifTempOutPutBuffer]		       ; restore the temp buffer
	add   esi,8					       ; jump over the XY dwords
	mov   edi,dword[DifOutPutBuffer]		       ; restore the out put buffers address
	add   edi,8					       ; jump over the XY dwords
align 4 						       ; align the loop address
AlphaLoop:						       ;
	add   edi,3					       ; go to the top byte
	lodsb						       ; load a byte from the alph data
	stosb						       ; store it in the alpha channal
	add   edi,3					       ; move on to the next top byte
	add   esi,3					       ; move on to the next alpha data
	lodsb						       ; load a byte
	stosb						       ; store it
	add   edi,3					       ; move on to the next top byte
	add   esi,3					       ; move on to the next alpha data
	lodsb						       ; load a byte
	stosb						       ; store it
	add   esi,3					       ; move on to the next alpha data
	loop  AlphaLoop 				       ; loop ifecx not zero
	popad						       ; store regs
	popad						       ; store regs
	;mov   ax,dword[DifErrorCode]                          ; no errors
	clc						       ; return
	ret

DifError:						       ;
	popad						       ; store regs
	mov   ax,word[DifErrorCode]			       ; store error code
	stc						       ; set carry flag
	ret						       ; return
align 4
 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;
DifErrorCode	      dw 0
ImageXdif	      dw 0
ImageYdif	      dw 0
Alpha1Address	      dd 0
Alpha1Size	      dd 0
Alpha2Address	      dd 0
Alpha2Size	      dd 0
DifOutPutBuffer       dd 0
DifinPutBuffer	      dd 0
DifTempOutPutBuffer   dd 0
