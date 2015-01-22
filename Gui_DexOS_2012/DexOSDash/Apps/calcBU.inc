;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   Calculator for MenuetOS (c) Ville Turjanmaa
;;
;;   Compile with FASM
;;
;;   Converted for DexOS, By Dex
;;   
testcolorabc = 0x00090909
align 4
TestAPP:
        mov   word[MainWindowX],100                    
        mov   word[MainWindowY],100 
        mov   word[MainWindowXsize],230   
        mov   word[MainWindowYsize],185     
        mov   [display_type],0
        mov   [entry_multiplier],10
        mov   [display_start_y],0
        mov   [dot],'.'
        mov   [calc],' '
        mov   [integer],0
        mov   [decimal],0
        mov   [kymppi],10
        mov   esi,muutaBU
        mov   edi,muuta1
        mov   ecx,18
        cld
        rep   movsb
        mov   esi,muutaBU
        mov   edi,muuta2
        mov   ecx,18
        cld
        rep   movsb
        mov   esi,muutaBU
        mov   edi,muuta0
        mov   ecx,18
        cld
        rep   movsb

        call  draw_window
        call  BuffToScreen
        call  SetUpMouseVesa
	xor   eax,eax
	mov   edx,eax
 ;----------------------------------------------------;
 ; Mouse set Min Max XY.                              ;
 ;----------------------------------------------------;  
	mov   ax,[ModeInfo_XResolution]
	mov   dx,[ModeInfo_YResolution]
	sub   dx,4
	sub   ax,10
	call  SetMouseMaxMinXYLib
        int   6Ch
        jmp   stilla
still:
        call  BuffToScreen  
stilla:         
        call  WAIT_FOR_EVENT_10      
        cmp   eax,1                   
        je    red1
        cmp   eax,2                  
        je    key
        cmp   eax,3                   
        je    button
        jmp   stilla

red1: 
        mov   ax,0
        mov   bx,word[MainWindowXsize]
        sub   bx,20
        mov   cx,0
        mov   dx,24
        mov   esi,1
        call  CHECK_BOUNDARIES_80
	jc    stilla
        mov  eax,59
        add  ax,word[MainWindowX]
        mov  ebx,24
        add  bx,word[MainWindowY]
        mov  edx,143
        mov  ecx,44
        call  SET_SAVE_BOX_SIZE
        call  MOVE_MAIN_WINDOW_81 
        call  draw_window    
        pushad
        mov  eax,59
        add  ax,word[MainWindowX]
        mov  ebx,24
        add  bx,word[MainWindowY]
        mov  edx,143
        mov  ecx,44
        mov   esi,BufferTemp       
        call  ReStoreFilledBoxVesa  
        popad                
        jmp   still
key:                          
        call  GET_KEY_02             
        shr   eax,8
        mov   edi,asci               
        mov   ecx,36 ;34               
        cld
        repne scasb
        jne   stilla
        sub   edi,asci
        dec   edi
        mov   esi,butid
        add   esi,edi
        lodsb
        jmp   testbut

button:                        
        call  GET_PRESSED_BUTTON_ID_17  
        shr   eax,8
testbut:
        cmp   eax,1                  
        jne   noclose1
        ret                          
noclose1:
        cmp   eax,2
        jne   no_reset
        call  clear_all
        jmp   still
no_reset:
        finit
        mov   ebx,muuta1            
        mov   esi,18
        call  atof
        fstp  [trans1]
        mov   ebx,muuta2
        mov   esi,18
        call  atof
        fstp  [trans2]
        fld   [trans2]
        cmp   eax,30
        jne   no_sign
        cmp   [dsign],byte '-'
        jne   no_m
        mov   [dsign],byte '+'
        call  print_display
        jmp   still
no_m:
        mov   [dsign],byte '-'
        call  print_display
        jmp   still
no_sign:
        cmp   eax,3
        jne   no_display_change
        inc   [display_type]
        cmp   [display_type],2
        jbe   display_continue
        mov   [display_type],0
display_continue:
        mov   eax,[display_type]
        mov   eax,[multipl+eax*4]
        mov   [entry_multiplier],eax
        call  print_display
        jmp   still
  multipl:  dd  10,16,2

no_display_change:
        cmp   eax,6
        jb    no_10_15
        cmp   eax,11
        jg    no_10_15
        add   eax,4
        call  number_entry
        jmp   still
no_10_15:
        cmp   eax,12
        jb    no_13
        cmp   eax,14
        jg    no_13
        sub   eax,11
        call  number_entry
        jmp   still
no_13:
        cmp   eax,12+6
        jb    no_46
        cmp   eax,14+6
        jg    no_46
        sub   eax,11+3
        call  number_entry
        jmp   still
no_46:
        cmp   eax,12+12
        jb    no_79
        cmp   eax,14+12
        jg    no_79
        sub   eax,11+6
        call  number_entry
        jmp   still
no_79:
        cmp   eax,13+18
        jne   no_0
        mov   eax,0
        call  number_entry
        jmp   still
no_0:
        cmp   eax,32
        jne   no_id
        inc   [id]
        and   [id],1
        mov   [new_dec],100000
        jmp   still
no_id:
        cmp   eax,16
        jne   no_sin
        fld   [trans1]
        fsin
        jmp   show_result
no_sin:
        cmp   eax,17
        jne   no_int
        fld   [trans1]
        frndint
        jmp   show_result
no_int:
        cmp   eax,22
        jne   no_cos
        fld   [trans1]
        fcos
        jmp   show_result
no_cos:
        cmp   eax,23
        jne   no_lg2
        fldlg2
        jmp  show_result
no_lg2:
        cmp  eax,28
        jne  no_tan
        fld  [trans1]
        fcos
        fstp [tmp2]
        fld  [trans1]
        fsin
        fdiv [tmp2]
        jmp  show_result
no_tan:
        cmp   eax,29
        jne   no_pi
        fldpi
        jmp   show_result
no_pi:
        cmp   eax,34
        jne   no_sqrt
        fld   [trans1]
        fsqrt
        jmp   show_result
no_sqrt:
        cmp   eax,15
        jne   no_add
        call  calculate
        call  print_display
        call  new_entry
        mov   [calc],'+'
        jmp   still
no_add:
        cmp   eax,21
        jne   no_sub
        call  calculate
        call  print_display
        call  new_entry
        mov   [calc],'-'
        jmp   still
no_sub:
        cmp   eax,27
        jne   no_div
        call  calculate
        call  print_display
        call  new_entry
        mov   [calc],'/'
        jmp   still
no_div:
        cmp   eax,33
        jne   no_mul
        call  calculate
        call  print_display
        mov   [calc],'*'
        call  new_entry
        jmp   still
no_mul:
        cmp   eax,35
        jne   no_calc
        call  calculate
        jmp   still
no_calc:
        jmp   still
show_result:
        call  ftoa
        call  print_display
        jmp   still
error:
        jmp   stilla


calculate:
        pushad
        cmp  [calc],' '
        je   no_calculation
        cmp  [calc],'/'
        jne  no_cdiv
        fdiv [trans1]
no_cdiv:
        cmp   [calc],'*'
        jne   no_cmul
        fmul  [trans1]
no_cmul:
        cmp   [calc],'+'
        jne   no_cadd
        fadd  [trans1]
no_cadd:
        cmp   [calc],'-'
        jne   no_cdec
        fsub  [trans1]
no_cdec:
        call  ftoa
        call  print_display
no_calculation:
        popad
        ret

number_entry:
        pushad
        cmp   eax,[entry_multiplier]
        jge   no_entry
        cmp   [id],1
        je    decimal_entry
        mov   ebx,[integer]
        test  ebx,0xF0000000
        jnz   no_entry
        mov   ebx,eax
        mov   eax,[integer]
        mov   ecx,[entry_multiplier]
        mul   ecx
        add   eax,ebx
        mov   [integer],eax
        call  print_display
        call  to_muuta
        popad
        ret

decimal_entry:
        imul  eax,[new_dec]
        add   [decimal],eax
        mov   eax,[new_dec]
        xor   edx,edx
        mov   ebx,[entry_multiplier]
        div   ebx
        mov   [new_dec],eax
        call  print_display
        call  to_muuta
        popad
        ret

no_entry:
        call  print_display
        call  to_muuta
        popad
        ret

to_muuta:
        pushad
        mov   al,[dsign]
        mov   esi,muuta0
        mov   edi,muuta1
        mov   ecx,18
        cld
        rep   movsb
        mov   [muuta1],al
        mov   edi,muuta1+10     
        mov   eax,[integer]
new_to_muuta1:
        mov   ebx,10
        xor   edx,edx
        div   ebx
        mov   [edi],dl
        add   [edi],byte 48
        dec   edi
        cmp   edi,muuta1+1
        jge   new_to_muuta1
        mov   edi,muuta1+17      
        mov   eax,[decimal]
new_to_muuta2:
        mov   ebx,10
        xor   edx,edx
        div   ebx
        mov   [edi],dl
        add   [edi],byte 48
        dec   edi
        cmp   edi,muuta1+12
        jge   new_to_muuta2
        popad
        ret

print_muuta:
        pushad
        mov   ebx,25*65536+125
        mov   ecx,200*65536+22
        mov   edx,0xffffff
        call  DRAW_BAR_13        
        mov   eax,1               
        mov   ebx,25*65536+200
        mov   ecx,0x0
        mov   edx,muuta1
        mov   esi,18
        call  WRITE_TEXT_TO_WINDOW_LEN_04  
        mov   eax,1                
        mov   ebx,25*65536+210
        mov   ecx,0x0
        mov   edx,muuta2
        mov   esi,18
        call  WRITE_TEXT_TO_WINDOW_LEN_04  
        popad
        ret

new_entry:
        pushad
        mov   esi,muuta1
        mov   edi,muuta2
        mov   ecx,18
        cld
        rep   movsb
        mov   esi,muuta0
        mov   edi,muuta1
        mov   ecx,18
        cld
        rep   movsb
        mov   [integer],0
        mov   [decimal],0
        mov   [id],0
        mov   [new_dec],100000
        mov   [sign],byte '+'
        popad
        ret

ten        dd    10.0,0
tmp        dw       1,0
sign       db       1,0
tmp2       dq     0x0,0
exp        dd     0x0,0
new_dec    dd  100000,0
id         db     0x0,0

k8         dd 10000000
k8r        dq 0

res     dd   0

trans1  dq   0
trans2  dq   0

controlWord dw 1

ftoa:    
        pushad
        fst   [tmp2]
        fstcw [controlWord]      
        mov   ax,[controlWord]
        mov   [tmp],ax
        or    [tmp],word 0x0C00
        fldcw [tmp]
        ftst                      
        fstsw ax
        and   ax, 4500h
        mov   [sign], 0
        cmp   ax,0100h
        jne   no_neg
        mov   [sign],1
no_neg:
        fld   [tmp2]
        fistp [integer]
        fld   [tmp2]
        fisub [integer]
        fldcw [controlWord]
        cmp   byte[sign], 0     
        je    no_neg2
        fchs
no_neg2:
        mov   [res],0         
        mov   edi,6
newd:
        fimul [kymppi]
        fist  [decimal]
        mov   ebx,[res]
        imul  ebx,10
        mov   [res],ebx
        mov   eax,[decimal]
        add   [res],eax
        fisub [decimal]
        fst   [tmp2]
        ftst
        fstsw ax
        test  ax,1
        jnz   real_done
        fld   [tmp2]
        dec   edi
        jz    real_done
        jmp   newd

real_done:
        mov    eax,[res]
        mov   [decimal],eax
        cmp   [integer],0x80000000      
        jne   no_error
        mov   [integer],0
        mov   [decimal],0
        call  clear_all
        mov   [calc],'E'
no_error:
        mov   [dsign],byte '+'
        cmp   [sign],byte 0             
        je    no_negative
        mov   eax,[integer]
        not   eax
        inc   eax
        mov   [integer],eax
        mov   [dsign],byte '-'
no_negative:
        call  to_muuta
        popad
        ret



atof:
        push  eax
        push  edi
        fldz
        mov   edi,0
        cmp   esi,0
        je    .error                
        mov   byte[sign],0
        cmp   byte[ebx],'+'       
        jne   .noPlus
        inc   edi
        jmp   .noMinus
.noPlus:
        cmp   byte[ebx],'-'
        jne   .noMinus
        mov   byte[sign],1      
        inc   edi
.noMinus:
        cmp   esi,edi
        je    .error
        call  atof_convertWholePart
        jc   .error
        call  atof_convertFractionalPart
        jc   .error
        cmp   byte[sign],0
        je    .dontNegate
        fchs                    
.dontNegate:
        mov   bh, 0               
        jmp   .exit
.error:
        mov   bh,1               
        fstp  st0                
.exit:
        pop   edi
        pop   eax
        ret

atof_convertWholePart:
.mainLoop:
        mov   al,[ebx + edi]
        cmp   al,'.'
        je   .exit
        cmp   al,'0'             
        jb   .error
        cmp   al,'9'
        ja   .error
        sub   al,'0'
        mov   ah,0
        mov   [tmp], ax
        fmul  dword[ten]
        fiadd word[tmp]
        inc   edi
        cmp   esi,edi             
        je    .exit
        jmp   .mainLoop
.error:
        stc                      
        ret

.exit:
        clc                     
        ret


atof_convertFractionalPart:
        fld1                     
.mainLoop:
        cmp   esi,edi             
        je    .exit
        inc   edi                  
        cmp   esi,edi             
        je    .exit
        mov   al,[ebx + edi]
        cmp   al,'0'             
        jb    .error
        cmp   al,'9'
        ja    .error
        fdiv  dword[ten]          
        sub   al,'0'
        mov   ah,0
        mov   [tmp],ax
        fild  word[tmp]
        fmul  st0,st1
        faddp st2,st0
        jmp   .mainLoop
.error:
        stc                    
        fstp  st0                
        ret

.exit:
        clc                     
        fstp  st0                
        ret

;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************
draw_window:
        mov   ebx,1                      
        call  WINDOW_REDRAW_STATUS_12
        mov   bx,word[MainWindowX]
        shl   ebx,16  
        mov   bx,word[MainWindowXsize]
        mov   cx,word[MainWindowY]
        shl   ecx,16 
        mov   cx,word[MainWindowYsize]     
        mov   edx,testcolorabc            
        mov   esi,0x00414449 
        mov   edi,0                    
        call  DEFINE_AND_DRAW_WINDOW_00  
        mov   ebx,24*65536+25+3
        mov   ecx,70*65536+17
        mov   edx,6
        mov   esi,0x00414449 
        mov   edi,7
newbutton:
        dec   edi
        jnz   no_new_row
        mov   edi,6
        mov   ebx,24*65536+25+3
        add   ecx,20*65536
no_new_row:
        push  edi
        mov   edi,0
        call  DEFINE_BUTTON_08            
        pop   edi
        add   ebx,30*65536
        inc   edx
        cmp   edx,11+24
        jbe   newbutton
        mov   bx,230 -18
        shl   ebx,16
        mov   bx,12                                    
        mov   cx,4                         
        shl   ecx,16
        mov   cx,12                                   
        mov   edx,1                                  
        mov   esi,0x00414449                      
        call  DEFINE_BUTTON_08  
        mov   ebx,25*65536+27
        mov   ecx,41*65536+15
        mov   edx,2
        mov   esi,0x00414449 
        mov   edi,0
        call  DEFINE_BUTTON_08            
        mov   ebx,203*65536+7
        mov   ecx,28*65536+7
        mov   edx,3
        mov   esi,0x00414449 
        mov   edi,0
        call  DEFINE_BUTTON_08             
        mov   ebx,25*65536+35           
        mov   ecx,SkinTextColor 
        mov   edx,text
        mov   esi,30
newline:
        mov   eax,1             
        call  WRITE_TEXT_TO_WINDOW_LEN_04  
        add   ebx,10
        add   edx,30
        cmp   [edx],byte 'x'
        jne   newline
        call  print_display
        mov   eax,1               
        mov   ebx,230-14
        shl   ebx,16
        mov   bx,7
        mov   ecx,SkinTextColor 
        mov   edx,CharX
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12
        ret



print_display:
        pushad
        mov   ebx,59*65536+143
        mov   ecx,24*65536+44
        mov   edx,0x00000000  
        call  DRAW_BAR_13            
        mov   eax,1               
        mov   ebx,120*65536+28
        mov   ecx,SkinTextColor 
        mov   edx,calc
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1  
        mov   ebx,175*65536+28
        mov   ecx,SkinTextColor 
        mov   edx,[display_type]
        shl   edx,2
        add   edx,display_type_text
        mov   esi,3
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        cmp   [display_type],0   
        jne   no_display_decimal
        mov   ebx,11*65536   
        mov   ecx,[integer]
        mov   edx,84*65536+44
        mov   esi,SkinTextColor 
        call  DISPLAY_NUMBER_TO_WINDOW_47  
        mov   ebx,6*65536
        mov   ecx,[decimal]
        mov   edx,156*65536+44
        mov   esi,SkinTextColor ;0x0
        call  DISPLAY_NUMBER_TO_WINDOW_47   
        mov   eax,1   
        mov   ebx,150*65536+44
        mov   ecx,SkinTextColor 
        mov   edx,dot
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1  
        mov   ebx,77*65536+44
        mov   ecx,SkinTextColor 
        mov   edx,dsign
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
no_display_decimal:
        cmp   [display_type],1
        jne   no_display_hexadecimal
        mov   eax,1              
        mov   ebx,138*65536+44
        mov   ecx,SkinTextColor 
        mov   edx,dsign
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   ebx,1*256+8*65536   
        mov   ecx,[integer]
        mov   edx,144*65536+44
        mov   esi,SkinTextColor 
        call  DISPLAY_NUMBER_TO_WINDOW_47   
no_display_hexadecimal:
        cmp   [display_type],2
        jne   no_display_binary
        mov   eax,1                       
        mov   ebx,96*65536+44
        mov   ecx,SkinTextColor 
        mov   edx,dsign
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   ebx,2*256+15*65536   
        mov   ecx,[integer]
        mov   edx,102*65536+44
        mov   esi,SkinTextColor 
        call  DISPLAY_NUMBER_TO_WINDOW_47  
no_display_binary:
        popad
        ret

clear_all:
        pushad
        mov   [calc],' '
        mov   [integer],0
        mov   [decimal],0
        mov   [id],0
        mov   [dsign],byte '+'
        mov   esi,muuta0
        mov   edi,muuta1
        mov   ecx,18
        cld
        rep   movsb
        mov   esi,muuta0
        mov   edi,muuta2
        mov   ecx,18
        cld
        rep   movsb
        call  print_display
        popad
        ret



; DATA AREA

CharX              db 'X'  
muutaBU            db   '+0000000000.000000'

display_type       dd   0   ; 0 = decimal, 1 = hexadecimal, 2= binary
entry_multiplier   dd  10

display_start_y    dd  0x0
display_type_text  db 'dec hex bin'

dot     db  '.'

calc    db  ' '

integer dd    0
decimal dd    0
NN DB 'CRAIG'

kymppi  dd   10

dsign:
muuta1  db   '+0000000000.000000'
muuta2  db   '+0000000000.000000'
muuta0  db   '+0000000000.000000'


text:
    db '                              '
    db '  C                           '
    db '                              '
    db '                              '
    db '  A    B    C    D    E    F  '
    db '                              '
    db '  1    2    3    +   SIN  INT '
    db '                              '
    db '  4    5    6    -   COS  LG2 '
    db '                              '
    db '  7    8    9    /   TAN   P  '
    db '                              '
    db ' -/+   0    .    *   SQR   =  '
    db '                              '
    db 'x <- END MARKER, DONT DELETE  '


asci:  db 49,50,51,52,53,54,55,56,57,48,43,61,45,42,47,44,46,52,13,97,98,99,100,101,102,65,66,67,68,69,70,112,80,27,115,83
butid: db 12,13,14,18,19,20,24,25,26,31,15,35,21,33,27,32,32,33,35, 6, 7, 8,  9, 10, 11, 6, 7, 8, 9,10,11, 29,29, 1, 16,16


I_END: 
