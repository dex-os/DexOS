;
;   EXAMPLE APPLICATION
;
;   Compile with FASM for Menuet
;
AppCdPlayer:
        mov   word[MainWindowX],100                    
        mov   word[MainWindowY],100 
        mov   word[MainWindowXsize],260
        mov   word[MainWindowYsize],160  
        call  draw_window_CD
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
        int   2Ch  
        jmp   stillCDnobuffer
stillCDbuffer:
        call  BuffToScreen
stillCDnobuffer:
        call  WAIT_FOR_EVENT_10  

    cmp  eax,1                  ; redraw request ?
    ;je   redCD
    cmp  eax,2                  ; key in buffer ?
    je   keyCD
    cmp  eax,3                  ; button in buffer ?
    je   buttonCD

    jmp  stillCDnobuffer

  redCD:                          ; redraw
    call draw_window_CD
    jmp  stillCDbuffer

  keyCD:                          ; key
        call  GET_KEY_02      
    jmp  stillCDnobuffer

  buttonCD:                       ; button
        call  GET_PRESSED_BUTTON_ID_17 

    shr  eax , 8

    cmp  eax , 1                ; Button close
    jne  nocloseCD
     ; Exit
     ret
  nocloseCD:

   ; cmp  eax , 0x106            ; Menu close
    ;jne  nocCD

  nocCD:

    jmp  stillCDnobuffer



; WINDOW DRAW


draw_window_CD:
        mov   ebx,1                      
        call  WINDOW_REDRAW_STATUS_12
        pushad
        movzx eax,word[MainWindowX]                   ; start X 
        movzx ecx,word[MainWindowY]                   ; start Y
        mov  esi,BenCd_Buffer                         ; source image
        mov  edi,VesaBuffer                           ; dest image
        call PutImageAlpha32                          ; call function
        popad


        xor   ebx,ebx   
        mov   bx,word[MainWindowXsize] 
        add   ebx,-19
        shl   ebx,16
        add   ebx,12
        mov   ecx,5*65536+12            
        mov   edx,1                    
        mov   esi,0x00414449     
        call  DEFINE_BUTTON_08 
        JMP   VCR

        mov   ebx,10*65536+40          
        mov   ecx,150*65536+14          
        mov   edx,1                     
        mov   esi,0x00414449    
       ; call  DEFINE_BUTTON_08  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
new_buttonCD:
        inc   edx                      
        call  DEFINE_BUTTON_08                      
        add   ebx,42*65536              
        cmp   edx,3                    
        jbe   new_buttonCD        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
VCR:


        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12

    ret


; DATA AREA
CharPLAYcd: db 'PLAY',0
CharSTOPcd: db 'STOP',0
CharBACKcd: db 'BACK',0
CharNEXTcd: db 'NEXT',0
CharOPENcd: db 'OPEN',0
CharXcdplayer     db   'X' 
headerMess:                 
                  db  'CDPlayer'
headerMess_len = $ - headerMess
textcd:

    db   'SYSTEM FUNCTIONS ARE IN FILE SYSFUNCS.TXT',0




I_ENDcd:




