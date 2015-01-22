;======================================================;
;   ____                    ______          30/03/2011 ;
;  /\  _`\                 /\__  _\                    ;
;  \ \ \/\ \     __   __  _\/_/\ \/    __   __  _      ;
;   \ \ \ \ \  /'__`\/\ \/'\  \ \ \  /'__`\/\ \/'\     ;
;    \ \ \_\ \/\  __/\/>  </   \ \ \/\  __/\/>  </     ;
;     \ \____/\ \____\/\_/\_\   \ \_\ \____\/\_/\_\    ;
;      \/___/  \/____/\//\/_/    \/_/\/____/\//\/_/    ;
;                                                      ;
;                                                      ;
; Coded by Craig Bamford(c) (a.k.a Dex)                ;
; Text Editor for DexOS  V0.03                         ;
;======================================================;
format binary as 'dex'
use32
	ORG   0x1A00000

	jmp   start
	db    'DEX6'
 ;----------------------------------------------------;
 ; Set some equ.                                      ;
 ;----------------------------------------------------;
MaxFileSize  =	      1024*1024*2
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

win_max      equ      4
file_max     equ      6 ;9
Edit_max     equ      4 ;7
Search_max   equ      2 ;5
Help_max     equ      2 ;5

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
 ; Load fat module.                                   ;
 ;----------------------------------------------------;
	mov	esi,ID
	call	[ModuleFunction]
	jc	NoFatModuleLoaded
	mov	[Fatmod],eax
  NoFatModuleLoaded:
	pushad
	mov   edi,Filehandle
	mov   ecx,640
	mov   al,0
	rep   stosb
	popad
 ;----------------------------------------------------;
 ; Get default text color                             ;
 ;----------------------------------------------------;
	call  [GetTextColor]
	mov   byte[StandaredColor],al
 ;----------------------------------------------------;
 ; GET CURRENT DEFAULT DRIVE                          ;
 ;----------------------------------------------------;
	call  [GetBootDrive]
	add   bl,41h
	mov   [DriveVar],al
 ;----------------------------------------------------;
 ; Initialize New Values.                             ;
 ;----------------------------------------------------;
	mov   [OutPutMonitor],0xb8000
	call  New
 ;----------------------------------------------------;
 ; Set The Look Of The Editor.                        ;
 ;----------------------------------------------------;
	call  ClearBuffer
	call  [GetRmPalette]
	mov   [StandaredPalette],edx
	mov   ax,1003h
	xor   bx,bx
	call  [RealModeInt10h]
	call  SetPalette
	call  [Clstext]
	call  DrawScreen
	call  WinHeadSetColor
	call  info_win
	call  ShowFileName
	call  GoToXY
	call  SpeedUpKeyBoard
	call  InfoWinBackGround
	call  PrintPage

 ;----------------------------------------------------;
 ; Main loop.                                         ;
 ;----------------------------------------------------; 
GetAnotherChar:
	call  GetKey
CheckKey:
	cmp   [AsciiKey],0
	je    key_0
	cmp   [AsciiKey],224
	je    key_0
 ;===================================================;
 ; AsciiKeys.                                        ;
 ;===================================================;
 ;----------------------------------------------------;
 ; Esc pressed.                                       ;
 ;----------------------------------------------------;                           
	cmp   [AsciiKey],escape_a   
	je    Exit
 ;----------------------------------------------------;
 ; BackSpace pressed.                                 ;
 ;----------------------------------------------------; 
	cmp   [AsciiKey],backspace   
	jne   AscKey1
	cmp   [CursorX],1
	jne   SameLine	

	cmp   [NumberOfLinesInFile],2
	je    GetAnotherChar

	cmp   [CursorY],1
	jbe   GetAnotherChar
	mov   [EndOfLineAddOn],0
	call  EndOfLine
	mov   ecx,[EOLNumber]
	mov   [EndOfLineAddOn],256 
	call  EndOfLine
	mov   edx,[EOLNumber]
	add   edx,ecx
	cmp   edx,253 
	jbe   @f
	call  Beep
	jmp   GetAnotherChar
@@:
	call  BackOneLine
	call  DeleteLineInFile
	xor   edx,edx
	mov   edx,[EOLNumber]
	cmp   [PosY],2
	ja    NotBellow6
	sub   [TopOfScreen],1
	jmp   Bellow6
NotBellow6:
	dec   [PosY]
Bellow6:
	cmp   edx,80 
	jb    Bellow80
	mov   [ColStart],edx
	sub   [ColStart],79 
	mov   [PosX],79
	jmp   Above80
Bellow80:
	mov   [PosX],dl
Above80:
	call  GoToXY
	mov   [CursorX],dx
	inc   [CursorX]
	dec   [CursorY]
	call  info_win
	dec   [LineCount]
	call  PrintPage
	jmp   GetAnotherChar
SameLine:     
	dec   [CursorX]
	call  info_win
	call  BackSpaceLine
	cmp   [PosX],0
	je    MovScreenLBS 
	dec   [PosX]
	call  GoToXY
	jmp   @f
MovScreenLBS: 
	dec   [ColStart]
	call  PrintPage
	jmp   GetAnotherChar
@@:  
	call  PrintLine 
	jmp   GetAnotherChar
AscKey1:
 ;----------------------------------------------------;
 ; Ctrl+l/L pressed.                                  ;
 ;----------------------------------------------------;
	cmp   [AsciiKey],0x0c
	jne   AscKey2
	jmp   GetAnotherChar
AscKey2:
 ;----------------------------------------------------;
 ; Ctrl+d/D pressed.                                  ;
 ;----------------------------------------------------;
	cmp   [AsciiKey],0x04
	jne   AscKey3
	cmp   [NumberOfLinesInFile],2
	je    GetAnotherChar
	mov   edx,[LineCount]
	cmp   [NumberOfLinesInFile],edx
	jbe   GetAnotherChar
	call  DeleteLineInFile
	call  PrintPage
	call  info_win
       ; call  CurrantDir
	jmp   GetAnotherChar
AscKey3:
	cmp   [AsciiKey],enter_a
	jne   AscKey4
	cmp   [PosY],23
	jae   @f
	call  AddLineToFile 
	call  MoveLine 
	inc   [PosY]
	inc   [CursorY]
	inc   [LineCount]
JustTheX:
	mov   [PosX],0 
	mov   [CursorX],1
	call  GoToXY
	mov   [ColStart],0 
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
@@:
	call  AddLineToFile 
	call  MoveLine 
	add   [TopOfScreen],1
	inc   [CursorY]
	inc   [LineCount]
	mov   [CursorX],1
	mov   [ColStart],0 
	call  PrintPage
	call  info_win
	mov   [PosX],0 
	call  GoToXY
	jmp   GetAnotherChar
AscKey4:
	cmp   [AsciiKey],0x1f
	jbe   GetAnotherChar
	cmp   [AsciiKey],128
	jae    GetAnotherChar
	mov   al,[AsciiKey]
	call  WriteChar
AscKey5: 
	jmp   GetAnotherChar
 ;===================================================;
 ; ScanKeys.                                         ;
 ;===================================================;
key_0:	
 ;----------------------------------------------------;
 ; Alt pressed.                                       ;
 ;----------------------------------------------------;
	cmp   [ScanKey],Alt	    
	jne   key_1
	call  Cursor_0
	call  TopMenuBar
	call  Cursor_1
	cmp   [ExitEditor],1
	je    Exit
	cmp   [ShowText],0
	je    GetAnotherChar
	call  PrintPage 
	mov   [ShowText],0
	jmp   GetAnotherChar
key_1:	 
 ;----------------------------------------------------;
 ; Pgdn pressed.                                      ;
 ;----------------------------------------------------;   
	cmp   [ScanKey],pgdn	   
	jne   Key2
	mov   edx,[TopOfScreen]
	add   edx,43
	cmp   [NumberOfLinesInFile],edx
	jae    @f
	cmp   [NumberOfLinesInFile],22
	jbe   GetAnotherChar
	mov   edx,[NumberOfLinesInFile]
	sub   edx,21
	cmp   [TopOfScreen],edx
	je    GetAnotherChar
	mov   [TopOfScreen],edx
	mov   [PosY],23
	mov   edx,[NumberOfLinesInFile]
	mov   [CursorY],dx
	mov   [LineCount],edx
	call  PrintPage
	call  info_win
	;call  CurrantDir
	call  GoToXY
	jmp   GetAnotherChar
@@: 
	add   [TopOfScreen],22
	add   [CursorY],22
	add   [LineCount],22
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
Key2:
 ;----------------------------------------------------;
 ; Pgup pressed.                                      ;
 ;----------------------------------------------------; 
	cmp   [ScanKey],pgup	    
	jne   Key3
	cmp   [TopOfScreen],1
	je    GetAnotherChar
	cmp   [TopOfScreen],22
	ja    @f
	mov   [TopOfScreen],1
	xor   edx,edx
	add   dl,[PosY]
	sub   edx,1
	mov   [CursorY],dx
	mov   [LineCount],edx
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
@@:
	sub   [TopOfScreen],22
	sub   [CursorY],22
	sub   [LineCount],22
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
Key3:
 ;----------------------------------------------------;
 ; Up pressed.                                        ;
 ;----------------------------------------------------; 
	cmp   [ScanKey],up
	jne   Key4
	cmp   [PosY],2
	jbe   @f
	dec   [PosY]
	call  GoToXY
	dec   [CursorY]
	call  info_win
	dec   [LineCount]
	jmp   GetAnotherChar
@@:
	cmp   [LineCount],2
	jb    GetAnotherChar
	sub   [TopOfScreen],1
	dec   [CursorY]
	dec   [LineCount]
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
Key4:
 ;----------------------------------------------------;
 ; Down pressed.                                      ;
 ;----------------------------------------------------;
	cmp   [ScanKey],down
	jne   Key5
	cmp   [PosY],22
	ja    @f
	mov   edx,[NumberOfLinesInFile]
	cmp   edx,[LineCount]
	jbe   GetAnotherChar
	inc   [PosY]
	call  GoToXY
	inc   [CursorY]
	call  info_win
	inc   [LineCount]
	jmp   GetAnotherChar
@@:
	mov   edx,[LineCount]
	cmp   [NumberOfLinesInFile],edx
	je    GetAnotherChar
	add   [TopOfScreen],1
	inc   [CursorY]
	inc   [LineCount]
	call  PrintPage
	call  info_win
	jmp   GetAnotherChar
Key5: 
       
 ;----------------------------------------------------;
 ; Right  pressed.                                    ;
 ;----------------------------------------------------;
	cmp   [ScanKey],right  
	jne   Key6
	cmp   [CursorX],254 
	je    NoScreenMov
	inc   [CursorX]
	call  info_win
	cmp   [PosX],79
	je    MovScreenR 
	inc   [PosX]
	call  GoToXY  
	jmp   NoScreenMov 
MovScreenR: 
	inc   [ColStart]
	call  PrintPage
	call  info_win
       ;call  CurrantDir
NoScreenMov: 
	jmp   GetAnotherChar
Key6: 
 ;----------------------------------------------------;
 ; Left  pressed.                                     ;
 ;----------------------------------------------------;  
	cmp   [ScanKey],left  
	jne   Key7
	cmp   [CursorX],1
	je    NoScreenMovL 
	dec   [CursorX]
	call  info_win
	cmp   [PosX],0
	je    MovScreenL 
	dec   [PosX]
	call  GoToXY
	jmp   NoScreenMovL 
MovScreenL: 
	dec   [ColStart]
@@:  
	call  PrintPage
	call  info_win
	;call  CurrantDir
NoScreenMovL: 
	jmp   GetAnotherChar
Key7: 
	cmp   [ScanKey],homekey 
	jne   Key8
	mov   [CursorX],1
	mov   [ColStart],0 
	mov   [PosX],0 
	call  PrintPage
	call  info_win
	;call  CurrantDir
	call  GoToXY
	jmp   GetAnotherChar
Key8:
	cmp   [ScanKey],endkey
	jne   Key9
	call  GoToEndOfLine
	jmp   GetAnotherChar
Key9:
       
	jmp   GetAnotherChar
comm_:
Exit:
 ;----------------------------------------------------;
 ; Exit.                                              ;
 ;----------------------------------------------------;
	mov   al,byte[StandaredColor]
	call  [TextColor]

	mov   ah,00h
	mov   al,03h
	xor   bx,bx
	call  [RealModeInt10h]

	call  [Clstext]
	ret


 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;

include 'View.inc'
include 'File.inc'
include 'Terminal.inc'
include 'EditInc.inc'
include 'EditData.inc'
include 'TextMode25.inc'
include 'fasm\DexOS\FASM.inc'
;rb 1024*1024
include 'Dex.inc'
include 'MenuList.inc'
;------------------------------------------
FasmScreenBuffer	   rb	100*160
;------------------------------------------

Buffer			   rb	MaxFileSize
ScreenBuffer		   rb	256*100
PageUpPointerAray	   rb	256 
PageUpPointerAray1:	   rb	1000*1024*2
align 4
buffer rb 1000h



