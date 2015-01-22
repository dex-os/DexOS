;
;   EXAMPLE APPLICATION
;
;   Compile with FASM for Menuet
;
AppCdPlayer:
        mov   word[MainWindowX],100                    
        mov   word[MainWindowY],100 
        mov   word[MainWindowXsize],227 
        mov   word[MainWindowYsize],183 
        mov   [SaveTrack],0 
        call  ConvertYtoDec 
        call  draw_window_CD
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        pushad
	xor   eax,eax                              
        mov   ax,word[MainWindowX]
        add   ax,227 -50
	mov   ebx,52 
        add   bx,word[MainWindowY]                       
        mov   edi,track_Buffer
        mov   ecx,16                      
        mov   edx,30                      
        call  SaveFilledBoxVesa
        popad
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
        hlt
        call  CHECK_FOR_EVENT_NO_WAIT_11  ;WAIT_FOR_EVENT_10  

    cmp  eax,1                  ; redraw request ?
    ;je   redCD
    cmp  eax,2                  ; key in buffer ?
    je   keyCD
    cmp  eax,3                  ; button in buffer ?
    je   buttonCD
    call checkTest
    jmp  stillCDnobuffer

  redCD:                          ; redraw
    call draw_window_CD
    jmp  stillCDbuffer

  keyCD:                          ; key
        call  GET_KEY_02   
        cmp   ah,27
        je    AppCdPlayerExit

        cmp   ah,0x70
        je    CdPlay
        cmp   ah,0x50
        je    CdPlay

        cmp   ah,0x73
        je    CdStop
        cmp   ah,0x53
        je    CdStop

        cmp   ah,0x62
        je    CdBack
        cmp   ah,0x42
        je    CdBack

        cmp   ah,0x6e
        je    CdNext
        cmp   ah,0x4e
        je    CdNext

        cmp   ah,0x6f
        je    CdOpen
        cmp   ah,0x4f
        je    CdOpen

    jmp  stillCDnobuffer

  buttonCD:                       ; button
        call  GET_PRESSED_BUTTON_ID_17 

    shr  eax , 8

    cmp  eax , 1                ; Button close
    jne  nocloseCD
AppCdPlayerExit:
     ; Exit
     ret

  nocloseCD:

    cmp  eax,2
    je   CdPlay

    cmp  eax,3
    je   CdStop

    cmp  eax,4
    je   CdBack

    cmp  eax,5
    je   CdNext

    cmp  eax,6
    je   CdOpen

    jmp  stillCDnobuffer

CdPlay:
    call  [PlayAllTracks]
    mov   [TrackNUMBER],1
    mov   [Visable1],1
    mov   [BusyCounter],0
    jmp  stillCDnobuffer
CdStop:
     call  [Stop_cd]
    mov   [Visable1],0
    mov   [BusyCounter],0
    mov   [SaveTrack],0 
    mov   [TrackNUMBER],1
        call  ConvertYtoDec
        call  RestoreTrackBack
        call  WriteTrackNumber
    jmp  stillCDnobuffer
CdBack:
    cmp   [TrackNUMBER],1
    jbe   stillCDnobuffer
    dec   [TrackNUMBER]
    mov   al,[TrackNUMBER]
    call  [PlayTrack]
    mov   [NunmberOfTracks],al
    mov   [Visable1],1
    mov   [BusyCounter],0
    jmp  stillCDnobuffer
CdNext:
    call [TrackInFo]
    mov  [NunmberOfTracks],al
    cmp  [TrackNUMBER],al
    jae  stillCDnobuffer
    inc   [TrackNUMBER]
    mov   al,[TrackNUMBER]
    call  [PlayTrack]
    mov   [NunmberOfTracks],al
    mov   [Visable1],1
    mov   [BusyCounter],0
    jmp  stillCDnobuffer
CdOpen:
    call  [Eject_cd]
    mov   [BusyCounter],0
    mov   [TrackNUMBER],1
        mov   [SaveTrack],0 
        call  ConvertYtoDec
        call  RestoreTrackBack
        call  WriteTrackNumber
    jmp  stillCDnobuffer


checkTest:
        pushad
        mov   al,[SaveTrack]
        call  CdBusy
        cmp   al,[SaveTrack]
        je    checkTestExit
        call  ConvertYtoDec
        call  RestoreTrackBack
        call  WriteTrackNumber
checkTestExit:
        popad
        ret

RestoreTrackBack:
        pushad
	xor   eax,eax                              
        mov   ax,word[MainWindowX]
        add   ax,227-50
	mov   ebx,52 
        add   bx,word[MainWindowY]                       
        mov   esi,track_Buffer
        mov   ecx,16                      
        mov   edx,30                      
        call  ReStoreFilledBoxVesaDirect
        popad
        ret

WriteTrackNumber:
        pushad
        mov   eax,1               
        mov   ebx,227-85
        shl   ebx,16
        mov   bx,54
        mov   ecx,SkinTextColor  ;TexTextColor
        mov   edx,MpMsg
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_04_D
        popad
        ret

; WINDOW DRAW
draw_window_CD:
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        pushad
        movzx  eax,word[MainWindowX]                  ;  X 
        add    eax,10
        movzx  ecx,word[MainWindowY]                  ;  Y 
        add    ecx,26     
        mov   esi,BackCd_Buffer                       ; source image
        mov   edi,VesaBuffer                          ; dest image
        call  PutImage32                              ; call function
        popad
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
        mov   eax,1
        mov   bx,word[MainWindowXsize]
        shr   bx,1
        mov   ecx,headerMess_len
        shr   ecx,1
        mov   edx,ecx
        shl   cx,2
        add   cx,dx
        add   cx,dx
        sub   bx,cx
        shl   ebx,16
        mov   bx,8        
        mov   ecx,SkinTextColor  
        mov   edx,headerMess                
        mov   esi,headerMess_len            
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        xor   ebx,ebx   
        mov   bx,word[MainWindowXsize] 
        add   ebx,-19
        shl   ebx,16
        add   ebx,12
        mov   ecx,5*65536+12            
        mov   edx,1                    
        mov   esi,0x00414449     
        call  DEFINE_BUTTON_08 

        mov   ebx,10*65536+40          
        mov   ecx,160*65536+14          
        mov   edx,1                     
        mov   esi,0x00414449    
 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
new_buttonCD:
        inc   edx                      
        call  DEFINE_BUTTON_08                      
        add   ebx,42*65536              
        cmp   edx,5                    
        jbe   new_buttonCD        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        mov   eax,1               
        mov   ebx,19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,SkinTextColor 
        mov   edx,CharPLAYcd
        mov   esi,4
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1               
        mov   ebx,19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,TexTextColor
        mov   edx,CharPLAYcd_P
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1               
        mov   ebx,61 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,SkinTextColor 
        mov   edx,CharSTOPcd
        mov   esi,4
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1               
        mov   ebx,61 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,TexTextColor 
        mov   edx,CharSTOPcd_S
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1               
        mov   ebx,103 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,SkinTextColor 
        mov   edx,CharBACKcd
        mov   esi,4
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1               
        mov   ebx,103 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,TexTextColor
        mov   edx,CharBACKcd_B
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1               
        mov   ebx,145 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,SkinTextColor 
        mov   edx,CharNEXTcd
        mov   esi,4
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1               
        mov   ebx,145 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,TexTextColor
        mov   edx,CharNEXTcd_N
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1               
        mov   ebx,187 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,SkinTextColor 
        mov   edx,CharOPENcd
        mov   esi,4
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
        mov   eax,1               
        mov   ebx,187 ;19 
        shl   ebx,16
        mov   bx,163
        mov   ecx,TexTextColor
        mov   edx,CharOPENcd_O
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   eax,1               
        mov   ebx,227 -16
        shl   ebx,16
        mov   bx,8
        mov   ecx,SkinTextColor 
        mov   edx,CharXcdplayer
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 

        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12

    ret

 ;----------------------------------------------------;
 ; CdBusy.                                            ;
 ;----------------------------------------------------;
CdBusy:
	pushad
	cmp   [Visable1],0
	je    CdNotBusy
        cmp   [BusyCounter],0
        jne   @f
        call  [TimerSecondCount ]
	mov   [BusyCounter],eax
        add   [BusyCounter],18*2
@@:
        call  [TimerSecondCount ]
	cmp   eax,[BusyCounter]
	jb    CdNotBusy
	mov   [BusyCounter],eax
        add   [BusyCounter],18*4   
	call  [CheckCdStateus]			 
	jc    CdNotBusy
	mov   [SaveTrack],ah
        mov   [TrackNUMBER],ah
	cmp   al,0x11
	je    CdNotBusy
	cmp   al,0x13
	jne   CdNotBusy
	;mov   [Visable],0
	mov   [SaveTrack],0x00
	mov   [Visable1],0
CdNotBusy:
	popad
	ret

;====================================================;
; ConvertXtoDec.                                     ;
;====================================================;
ConvertYtoDec:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   esi,str_x
	mov   byte[str_x],' '
	mov   byte[str_x+1],' '
	mov   [str_x+2],' '
	xor   ax,ax
	mov   al,[SaveTrack]
	mov   bx,10
loopd3: 
	mov   dx,0
	div   bx
	add   dl,30h
	mov   cl,byte[ds:esi+1]
	mov   byte[ds:esi+2],cl
	mov   cl,byte[ds:esi]
	mov   byte[ds:esi+1],cl
	mov   byte [ds:esi],dl
	cmp   ax,0
	jne   loopd3
	pop   ds
	pop   es
	popad
	ret

; DATA AREA
MpMsg:			 db 'Track '
str_x			 db '    ', 0
TrackNUMBER     db 0
NunmberOfTracks db 0
Visable1        db 0
SaveTrack       db 0
BusyCounter     dd 0

CharPLAYcd_P: db 'P',0
CharPLAYcd:   db 'PLAY',0
CharSTOPcd_S: db 'S',0
CharSTOPcd:   db 'STOP',0
CharBACKcd_B: db 'B',0
CharBACKcd:   db 'BACK',0
CharNEXTcd_N: db 'N',0
CharNEXTcd:   db 'NEXT',0
CharOPENcd_O: db 'O',0
CharOPENcd:   db 'OPEN',0
CharXcdplayer db 'X',0 
headerMess:                 
                  db  'CdPlayer'
headerMess_len = $ - headerMess
textcd:

    db   'SYSTEM FUNCTIONS ARE IN FILE SYSFUNCS.TXT',0
I_ENDcd:




