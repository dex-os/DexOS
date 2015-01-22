;=========================================================;
; NetSetUp                                       05/01/08 ;
;---------------------------------------------------------;
; Net set up and server program.                          ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
format binary as 'dex'
use32
	ORG  0x1A00000

	jmp   start
	db    'DEX6'
 ;----------------------------------------------------;
 ; Set some equ.                                      ;
 ;----------------------------------------------------;
space	     equ      0x20
color1	     equ      0x07
color2	     equ      0x87 
color2a      equ      0x78
color3	     equ      0x07
reverse1     equ      0x0f
reverse2     equ      0x70
reverse3     equ      0x74
reverseMark  equ      0x87
Shadow1      equ      0x08
error1	     equ      0xcf
escape	     equ      1 		   ;-----+
enter_s      equ      28		   ;     |
f10	     equ      68		   ;     |
f1	     equ      59		   ;     |
f2	     equ      60		   ;     |
f3	     equ      61		   ;     |
Alt	     equ      56		   ;     |
pgup	     equ      73		   ;     |
pgdn	     equ      81		   ;     |
ctrl_pgup    equ      132		   ;     |
ctrl_pgdn    equ      118		   ;     |
up	     equ      72		   ;     |
left	     equ      75		   ;     |
right	     equ      77		   ; Scan Code
down	     equ      80		   ;     |
inskey	     equ      82		   ;     |
delkey	     equ      83		   ;     |
homekey      equ      71		   ;     |
endkey	     equ      79		   ;     |
d_key	     equ      32		   ;     |
o_key	     equ      24		   ;     |
m_key	     equ      50		   ;     |
b_key	     equ      48		   ;     |
k_key	     equ      37		   ;     |
h_key	     equ      35		   ;     |
p_key	     equ      25		   ;     |
n_key	     equ      49		   ;     |
y_key	     equ      21		   ;-----+

backspace    equ      8 		   ;-----+
escape_a     equ      27		   ; ASCII Code
enter_a      equ      13		   ;-----+
win_maxC     equ      3
win_max      equ      2
file_max     equ      6 
Edit_max     equ      4 
Search_max   equ      2 
Help_max     equ      2 

 ;----------------------------------------------------;
 ; Program start.                                     ;
 ;----------------------------------------------------;
start:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
 ;----------------------------------------------------;
 ; Get calltable address.                             ;
 ;----------------------------------------------------;
	mov   edi,Functions				  
	mov   al,0					  
	mov   ah,0x0a				 
	int   50h					  
 ;----------------------------------------------------;
 ; Test if we have a ethernet driver + stack loaded.  ;
 ;----------------------------------------------------;
	mov   [StackNotLoaded],0

	mov   eax,53				; then, read in the status
	mov   ebx,255
	mov   ecx,6
	int   0x53

	cmp   eax,53				; if eax is 53, no driver was found
	jne   @f
	mov   [StackNotLoaded],1
@@:
 ;----------------------------------------------------;
 ; Initialize New Values.                             ;
 ;----------------------------------------------------;
	mov   [OutPutMonitor],0xb8000
	call  [GetRmPalette]
	mov   [StandaredPalette],edx
	xor   eax,eax 
	mov   ah,00h
	mov   al,03h
	xor   bx,bx
	call  [RealModeInt10h]
	mov   ah,10h
	mov   al,02h
	xor   bx,bx
	call  [RealModeInt10h]
	mov   ax,1003h
	mov   edx,[StandaredPalette]
	xor   bx,bx
	call  [RealModeInt10h]
	mov   [PosX],0
	mov   [PosY],2
	call  GoToXY
	call  TopGreenBar
	call  Cls
	call  BottomGreenBar
	call  WinHeadSetColor
 ;===================================================;
 ; Main loop.                                        ;
 ;===================================================;   
GetAnotherChar:
	call  GetKey
CheckKey:    
	cmp   [AsciiKey],0
	je    key_0
	cmp   [AsciiKey],224
	je    key_0
 ;===================================================;
 ; Esc pressed.                                      ;
 ;===================================================;                         
	cmp   [AsciiKey],escape_a   
	je    Exit
	jmp   GetAnotherChar
 ;===================================================;
 ; ScanKeys.                                         ;
 ;===================================================;
key_0:	
	cmp   [ScanKey],Alt	    
	jne   key_1
	call  Cursor_0
	call  TopMenuBar
	call  Cursor_1
	cmp   [ExitEditor],1
	je    Exit
	cmp   [ShowText],0
	je    GetAnotherChar
	jmp   GetAnotherChar
key_1:
	jmp   GetAnotherChar
Exit:
 ;----------------------------------------------------;
 ; Exit.                                              ;
 ;----------------------------------------------------;
	call  [Clstext]
	ret

 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;
include 'Function.inc'
include 'TcpSetting.inc'
include 'dhcp.inc'
include 'Netstat.inc'
include 'Server\Server.inc'
include 'Data.inc'
include 'Dex.inc'
Header	  rb	 500*6
Stop	  db	 0,0,0
Header1   rb	 1024 * 10
wanted_file_buffer:
