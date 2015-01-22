; 
;   EXAMPLE APPLICATION 
; 
;   Compile with FASM for Menuet 
; 
 
use32 
 
               org    0x0 
 
               db     'MENUET01'              ; 8 byte id 
               dd     0x01                    ; header version 
               dd     START                   ; start of code 
               dd     I_END                   ; size of image 
               dd     0x100000                ; memory for app 
               dd     0x7fff0                 ; esp 
               dd     0x0 , 0x0               ; I_Param , I_Icon 
 
START:                          ; start of execution 
 
     call draw_window 
 
still: 
 
    mov  eax,10                 ; wait here for event 
    int  0x40 
 
    cmp  eax,1                  ; redraw request ? 
    je   red 
    cmp  eax,2                  ; key in buffer ? 
    je   key 
    cmp  eax,3                  ; button in buffer ? 
    je   button 
 
    jmp  still 
 
  red:                          ; redraw 
    call draw_window 
    jmp  still 
 
  key:                          ; key 
    mov  eax,2                  ; just read it and ignore 
    int  0x40 
    jmp  still 
 
  button:                       ; button 
    mov  eax,17                 ; get id 
    int  0x40 
 
    cmp  ah,1                   ; button id=1 ? 
    jne  noclose 
 
    mov  eax,-1                 ; close this program 
    int  0x40 
  noclose: 
 
    jmp  still 
 
 
 
 
;   ********************************************* 
;   *******  WINDOW DEFINITIONS AND DRAW ******** 
;   ********************************************* 
 
 
draw_window: 
 
 
    mov  eax,12                    ; function 12:tell os about windowdraw 
    mov  ebx,1                     ; 1, start of draw 
    int  0x40 
 
                                   ; DRAW WINDOW 
    mov  eax,0                     ; function 0 : define and draw window 
    mov  ebx,100*65536+300         ; [x start] *65536 + [x size] 
    mov  ecx,100*65536+120         ; [y start] *65536 + [y size] 
    mov  edx,0x02ffffff            ; color of work area RRGGBB,8->color gl 
    mov  esi,0x805080d0            ; color of grab bar  RRGGBB,8->color gl 
    mov  edi,0x005080d0            ; color of frames    RRGGBB 
    int  0x40 
 
                                   ; WINDOW LABEL 
    mov  eax,4                     ; function 4 : write text to window 
    mov  ebx,8*65536+8             ; [x start] *65536 + [y start] 
    mov  ecx,0x10ddeeff            ; font 1 & color ( 0xF0RRGGBB ) 
    mov  edx,labelt                ; pointer to text beginning 
    mov  esi,labellen-labelt       ; text length 
    int  0x40 
 
                                   ; CLOSE BUTTON 
    mov  eax,8                     ; function 8 : define and draw button 
    mov  ebx,(300-19)*65536+12     ; [x start] *65536 + [x size] 
    mov  ecx,5*65536+12            ; [y start] *65536 + [y size] 
    mov  edx,1                     ; button id 
    mov  esi,0x6688dd              ; button color RRGGBB 
    int  0x40 
 
    mov  ebx,20*65536+35           ; draw info text with function 4 
    mov  ecx,0x224466 
    mov  edx,text 
    mov  esi,40 
  newline: 
    mov  eax,4 
    int  0x40 
    add  ebx,10 
    add  edx,40 
    cmp  [edx],byte 'x' 
    jne  newline 
 
    mov  eax,12                    ; function 12:tell os about windowdraw 
    mov  ebx,2                     ; 2, end of draw 
    int  0x40 
 
    ret 
 
 
; DATA AREA 
 
 
text: 
    db 'THIS IS AN EXAMPLE PROGRAM YOU COULD    ' 
    db 'USE, A:\EXAMPLE.ASM  CODE IS COMMENTED  ' 
    db 'AND CLEAR. SYSTEM FUNCTIONS ARE IN FILE ' 
    db 'SYSFUNCS.TXT                            ' 
 
    db 'x <- END MARKER, DONT DELETE            ' 
 
 
labelt: 
     db   'EXAMPLE APPLICATION' 
labellen: 
 
I_END: 