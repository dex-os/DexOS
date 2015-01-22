;=========================================================;
; Gif/Bmp                                        00/00/07 ;
;---------------------------------------------------------;
; BMP to hex converter.                                   ;
;                                                         ;
; Dex4u V0.01                                             ;
; (c) Craig Bamford, All rights reserved.                 ;                          
;=========================================================;
format binary as 'dex'
use32
	ORG   0x1A00000 				 ; where our program is loaded to
	jmp   start					; jump to the start of program.
	db    'DEX6'					; We check for this, to make sure it a valid DexOS file.
	COLOR_ORDER equ MENUETOS
include 'Gif.inc' 
include 'Bmp.inc' 
msg1:	db " Vesa mode not supported",10,13,0
	db " Press any key to exit. ",10,13,0
msg2:	db " Error!, file not found. ",10,13,0
	db " Press any key to exit. ",10,13,0
msg3:	db " Error!, No comandline, BMP or GIF file found. ",10,13,0
	db " Press any key to exit. ",10,13,0
msg4:	db " Error!, Not a valid BMP or GIF file. ",10,13,0
	db " Press any key to exit. ",10,13,0
msg8:	db " File Converted successfully, thankyou for using Bmp2X!.",10,13,0
	db " Press any key to exit. ",10,13,0
 ;----------------------------------------------------;
 ; Start of program.                                  ;
 ;----------------------------------------------------;
start:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
 ;----------------------------------------------------;
 ; Get calltable address.                             ;
 ;----------------------------------------------------;
	mov   edi,Functions			      ; this is the interrupt
	mov   al,0				      ; we use to load the DexFunction.inc
	mov   ah,0x0a				      ; with the address to dex4u functions.
	int   50h

 ;----------------------------------------------------;
 ; Display start message.                             ;
 ;----------------------------------------------------;   
	mov   esi,MsgLogo1	 
	call  [PrintString_0]
       ; call  [WaitForKeyPress]
	mov   esi,MsgLogo2	 
	call  [PrintString_0]
 ;----------------------------------------------------;
 ; Get Params.                                        ;
 ;----------------------------------------------------;
	call  [GetParams]
	;add   esi,13
	mov   dword[command_line],esi
	call  get_params   
	jc    information
 ;----------------------------------------------------;
 ; Load file.                                         ;
 ;----------------------------------------------------; 
	mov   edi,FileToLoad
	mov   edx,file_area
	call  [ReadFileFat]
	jc    LoadfileError
 ;----------------------------------------------------;
 ; Load vesa info.                                    ;
 ;----------------------------------------------------;
	call [LoadVesaInfo]
	mov   edi,VESA_Info
	mov   ecx,193
	cld
	cli
	rep   movsd
	sti

 ;----------------------------------------------------;
 ; uncompress.                                        ;
 ;----------------------------------------------------;        
uncompress:
	cmp   dword[file_area],'GIF8'
	jnz   ext2
	giftoimg file_area,header		      
	jmp   DrawImage
ext2:
	cmp   word[file_area],'BM'
	jnz   ImageError
	bmptoimg file_area,header		      
DrawImage:
 ;----------------------------------------------------;
 ; ConvertImageToHex.                                 ;
 ;----------------------------------------------------;   
	call  ConvertImageToHex
 ;----------------------------------------------------;
 ; Delete old Bmp2Hex.inc.                            ;
 ;----------------------------------------------------;
       ; mov   esi,FileB2Hname
       ; call  [DeleteFile]
 ;----------------------------------------------------;
 ; Make New Bmp2Hex.inc.                              ;
 ;----------------------------------------------------;
	mov    esi,FileB2Hname
	mov    edi,ScreenBuffer
	mov    eax,[FileSizeB2H]
	call   [WriteFileFat]
	jc     VesaError1
 ;----------------------------------------------------;
 ; Mouse Demo Exit                                    ;
 ;----------------------------------------------------; 
	mov   esi,msg8
	call  [PrintString_0]
DemoEnd:
@@:
	ret

 ;----------------------------------------------------;
 ; Display Vesa Error message.                        ;
 ;----------------------------------------------------;
VesaError1:
	mov   esi,msg1
	call  [PrintString_0]
	jmp   @b
 ;----------------------------------------------------;
 ; Load file error.                                   ;
 ;----------------------------------------------------;
LoadfileError:
	mov   esi,msg2
	call  [PrintString_0]
	jmp   @b
 ;----------------------------------------------------;
 ; information.                                       ;
 ;----------------------------------------------------; 
information: 
	mov   esi,msg3
	call  [PrintString_0]
	jmp   @b
 ;----------------------------------------------------;
 ; Display Vesa Error message.                        ;
 ;----------------------------------------------------;
ImageError:
	mov   esi,msg4
	call  [PrintString_0]
	call  [WaitForKeyPress]
	jmp   @b
jmp $

 ;----------------------------------------------------;
 ; ConvertImageToHex                                  ;
 ;----------------------------------------------------;
ConvertImageToHex:
	pushad
	push  es
	mov   ecx,Logo1
	sub   ecx,Logo
	mov   esi,Logo
	mov   edi,ScreenBuffer
	cld
	rep   movsb
	mov   esi,header
	lodsd
	mov   word[ImageX],ax
	mov   esi,header
	add   esi,4
	lodsd
	mov   word[ImageY],ax
	mov   esi,header       
	dec   edi 
	xor   eax,eax
	mov   ecx,eax 
	mov   ebx,eax 
	mov   ax,word[ImageX]
	mov   cx,word[ImageY]
	mul   ecx
	mov   ecx,eax	
	mov   ax,'db'
	stosw
	mov   ax,'  '
	stosw
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
 ;----------------------------------------------------;
 ; Start of Convert Image To Hex loop                 ;
 ;----------------------------------------------------;
AddedDB:
	dec   edi
	mov   ax,0x0a0d  
	stosw
AddedDB1:
	mov   [AddedDBString],0
	mov   ax,'db'
	stosw
	mov   ax,'  '
	stosw
ConvertImageToHexLoop2:
	cmp   [AddedDBString],4
	jae   AddedDB
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	mov   ax,'0x'
	stosw
	lodsb
	call  write_hex16
	stosw
	mov   al,','
	stosb
	inc   [AddedDBString]
	loop  ConvertImageToHexLoop2
	dec   edi
	mov   al,0x00
	stosb
	sub   edi,ScreenBuffer
	mov   [FileSizeB2H],edi
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; get_params.                                        ;
 ;----------------------------------------------------;
get_params:
	mov   esi,[command_line]
	mov   edi,FileToLoad
	lodsb
	cmp   al,' '
	je    bad_params
;------------------------
	mov   ecx,12
GetParamsLoop:
	lodsb
	cmp   al,' '
	je    ParamsFound
	loop  GetParamsLoop
	jmp   bad_params
;------------------------
ParamsFound:
	lodsb
	cmp   al,0x1f
	jbe   bad_params
	cmp   al,0x80
	jae   bad_params
	cmp   al,' '
	je    bad_params
	stosb
	mov   cx,7
Cliloop: 
	lodsb
	cmp   al,'.'
	je    DoExt1
	stosb
	loop  Cliloop
	lodsb
	cmp   al,'.'
	jne   bad_params
DoExt1:
	stosb
	mov   cx,3
	rep   movsb
	mov   al,0
	stosb
	mov   ax,word[es:edi-4]
	cmp   ax,'bm'
	je    get_params_ok
	cmp   ax,'BM'
	je    get_params_ok
	cmp   ax,'GI'
	je    get_params_ok
	cmp   ax,'gi'
	jne   bad_params
get_params_ok:
	clc
	ret
bad_params:
	stc
	ret
 ;----------------------------------------------------;
 ; write_hex16                                        ;
 ;----------------------------------------------------;
write_hex16:
	push  ebx
	mov   ebx,eax
	shr   al,4					   
	call  hexget
	xchg  eax,ebx					  
	call  hexget
	shl   ax,8
	mov   al,bl					   
	pop   ebx
	ret

 ;----------------------------------------------------;
 ; hexget         convers from ASCII to hexadecimal.  ;
 ;----------------------------------------------------;

hexget:
	and   eax,0x0000000f
	or    eax,0x00000030
	cmp   eax,0x39
	ja    add7
	ret
add7:	add   eax,7		
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
VERSION_STRING equ "0.01"
DeleteMsg: db ' There is already a file called "Bmp2Hex.inc" on floppy,',10,13
	   db ' This will be deleted, do you want to continue  Y/N ?',10,13,0
MsgLogo1: 
db  0xd2,0x09,';***********************************;',10,13
	   db ';  ____                 ____ __  __ ;',10,13
	   db '; | __ ) _ __ ___  _ __|___ \\ \/ / ;',10,13
	   db "; |  _ \| '_ ` _ \| '_ \ __) |\  /  ;",10,13
	   db '; | |_) | | | | | | |_) / __/ /  \  ;',10,13
	   db '; |____/|_| |_| |_| .__/_____/_/\_\ ;',10,13
	   db ';                 |_|               ;',10,13
	   db ';***********************************;',10,13,13
db 0xd2,0x07, ' Gif/Bmp to Hex converter, for DexOS, version ',VERSION_STRING,13,13,0
	   db ' Press any key to load & convert image . ',13,13,0
MsgLogo2:  
	   db ' Loading and converting image, please wait..... ',13,13,0

Logo:  db ';***************************************;',0Dh,0Ah
       db ';  ____                 ____ __  __     ;',0Dh,0Ah
       db '; | __ ) _ __ ___  _ __|___ \\ \/ /     ;',0Dh,0Ah
       db "; |  _ \| '_ ` _ \| '_ \ __) |\  /      ;",0Dh,0Ah
       db '; | |_) | | | | | | |_) / __/ /  \      ;',0Dh,0Ah
       db '; |____/|_| |_| |_| .__/_____/_/\_\     ;',0Dh,0Ah
       db ';                 |_|                   ;',0Dh,0Ah
       db ';                                       ;',0Dh,0Ah
       db '; First  dword  = X size.               ;',0Dh,0Ah
       db '; Second dword  = Y size.               ;',0Dh,0Ah
       db '; Rest of bytes = Converted image.      ;',0Dh,0Ah
       db '; Converted image = 3BPP.       (DexOS) ;',0Dh,0Ah
       db ';***************************************;',0Dh,0Ah,0Dh,0Ah
       db 'Image:',0Dh,0Ah,0
Logo1:
       
msg0x		   db '0x',0
FileB2Hname	   db 'Bmp2Hex.inc',0

count1		   dd	0
FileSizeB2H	   dd	0
AddedDBString	   db	0
ImageX		   dw	0
ImageY		   dw	0
ImageXaddOn	   dd	0
ImageYaddOn	   dd	0
ImageXaddOn24	   dd	0
command_line	   dd	0
FileToLoad:	   rb	80
IM_END:
align 4 
header:
rd  800*600
gif_hash_offset:
rd 4096+1						
align 4 
include 'Dex.inc'
align 4 			       
ScreenBuffer rb 640*480*8
align 4 
file_area:
