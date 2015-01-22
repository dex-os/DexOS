;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;    EXAMPLE APPLICATION
;
;    Compile with FASM for DexOS
;

HexTextColor  =  0x079317     
TexTextColor  =  0x00fe00  
begin dd 0

AppHexView:                         ; start of execution
        mov   word[MainWindowX],100                    
        mov   word[MainWindowY],100 
        mov   word[MainWindowXsize],400   
        mov   word[MainWindowYsize],270   
        ;call  open_file
        call  draw_window_HexView
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
        jmp   StillHexNoBuffer
StillHeXBuffer:
        call  BuffToScreen
StillHexNoBuffer:
        call  WAIT_FOR_EVENT_10  
        dec   eax                    
       ;je    redHexView
        dec   eax                    
        je    keyHexView
        dec   eax                    
        je    buttonHexView

        jmp   StillHexNoBuffer
redHexView:                          
        call draw_window_HexView
        jmp  StillHeXBuffer
keyHexView:                          
        call  GET_KEY_02  
        jmp  StillHexNoBuffer
buttonHexView:                      
        xor   eax,eax
        call  GET_PRESSED_BUTTON_ID_17
        cmp   ah,3
        jne   no_up
        cmp   [begin],16
        jb    no_up
        add   [begin],-16
        jmp   redHexView
no_up:
        cmp   ah,4
        jne   no_down
        add   [begin],16
        jmp   redHexView
no_down:
        dec   ah                    
        jne   StillHexNoBuffer
        ;EXIT
        ret

;   *********************************************
;   *******  WINDOW DEFINITIONS AND DRAW ********
;   *********************************************
draw_window_HexView:
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

        xor   ebx,ebx   
        mov   bx,word[MainWindowXsize] 
        add   ebx,-19
        shl   ebx,16
        add   ebx,12
        mov   ecx,5*65536+12            
        mov   edx,1                    
        mov   esi,0x00414449     
        call  DEFINE_BUTTON_08 

        mov   eax,1               
        mov   ebx,400-16
        shl   ebx,16
        mov   bx,8
        mov   ecx,SkinTextColor 
        mov   edx,CharXpaint
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1                     
        mov   ebx,8*65536+8             
        mov   ecx,SkinTextColor           
        mov   edx,labelt                
        mov   esi,labellen-labelt       
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   ebx,280*65536+16*6
        mov   ecx,240*65536+14
        mov   edx,2
        mov   esi,0x00414449 ;0x5599cc
        call  DEFINE_BUTTON_08 

        mov   ebx,15*65536+125
        inc   edx
        call  DEFINE_BUTTON_08 

        add   ebx,127*65536
        inc   edx
        call  DEFINE_BUTTON_08 

        mov   eax,1
        mov   ebx,15*65536+243
        mov   ecx,SkinTextColor 
        mov   edx,buttons_H
        mov   esi,blen-buttons_H
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   ebx,280*65536+35           
        mov   ecx,0xffffff
        mov   edx,text_H
        add   edx,[begin]
        mov   esi,16
        mov   edi,esi
newlineH:
        push  ebx                          ; hex
        push  edx
        push  edi

        mov   edi,16
        mov   ecx,edx
        mov   edx,ebx
        add   edx,-265*65536

        mov   ebx,0x00020101
        mov   esi,HexTextColor  
newhex:
        call  DISPLAY_NUMBER_TO_WINDOW_47
        add   edx,16*65536
        inc   ecx
        dec   edi
        jne   newhex

        pop   edi
        pop   edx
        pop   ebx

        mov   eax,1                       ; text
        mov   esi,16
        mov   ecx,TexTextColor ;0xffffff
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        add   ebx,12
        add   edx,16
        dec   edi
        jnz   newlineH
        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12
        ret


;DATA
file_name  db  'EXAMPLE.ASM   '
      ;    db  'EXAMPLE       '

open_file:

        pusha

        mov  eax,6
        mov  ebx,file_name
        xor  ecx,ecx
        mov  edx,-1
        mov  esi,text_H
       ;int  0x40

        popa

        ret



; DATA AREA

labelt:  db  'HEXVIEW'
labellen:
buttons_H  db  '          UP                  DOWN'
           db  '               EXAMPLE    '
blen:

align 4
I_ENDH:
text_H:
file   'HexTest.exe'
file_hexFile_FileEnd:
