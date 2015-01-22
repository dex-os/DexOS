;*********************************
;*                               *
;*    PAINT 0.02 для MenuetOS    *
;*                               *
;*     Компилировать FASM'ом    *
;*                               *
;*********************************


;include 'macros.inc'

;******************************************************************************


AppPaint:
        mov   word[MainWindowX],100                    
        mov   word[MainWindowY],100 
        mov   word[MainWindowXsize],400   
        mov   word[MainWindowYsize],300    
;;;;;;;;;

    mov  [workareacx],10 
    mov  [workareacy],45  
        call  SetVesaModeSys
        call draw_window_paint

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
     mov  eax,[workareacx]
     mov  ebx,[workareasx]
     mov  ecx,[workareacy]
     mov  edx,[workareasy] 
     add  edx,42
     call DRAW_LINE_SET_YX_38A
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        call  BuffToScreen
        call  SetUpMouseVesa
        int   2Ch  
        jmp   stillPaint1
stillPaint:            
        call  BuffToScreen
stillPaint1: 
      ; HLT
    ;mov  eax,10   а
    ;int  0x40     
    call  WAIT_FOR_EVENT_10                  

    cmp  eax,1   
    je   redPaint      
    cmp  eax,3   
    je   buttonPaint

    jmp  stillPaint1  ;ccccccccccccccccccccccccccc    
                  
redPaint:                  
        mov   eax,[workareacx]
        mov   ebx,[workareasx]
        add   ebx,6  
        mov   ecx,[workareacy]
        add   ecx,5
        mov   edx,[workareasy]
        add   edx,46
        mov   esi,0 ;1
        call  CHECK_BOUNDARIES_80
	jc    stillPaint1
        jmp   mouse            

;******************************************************************************

  buttonPaint:        
   ; mov  eax,17  
    ;int  0x40
    xor   eax,eax
    call  GET_PRESSED_BUTTON_ID_17

    shr  eax,8   

    cmp   eax,1      
    jne  .noclosePaint 
                  
    ;or   eax,-1  
    ;int  0x40
 
     ret

  .noclosePaint:
    cmp  eax,11
    je   ButtonReleasePaint

    dec  eax
    dec  eax
    cmp  eax,9
    ja   stillPaint1
    mov  eax,[colors+eax*4]
    mov  [active_color],eax
    jmp  stillPaint1
ButtonReleasePaint:
    mov  [mouse_pressed],0 
    jmp  stillPaint1
;******************************************************************************

  mouse:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

     call  GetMousePosLib
    cmp  bl,4
    je   .leftbtn

  .not_pressed:

    mov  [mouse_pressed],0 
    jmp stillPaint1

  .leftbtn:

   
       mov   [vesaMOUSEoff],1
       call  ReStoreMouseBAackGround
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
        xor eax,eax
        xor ebx,ebx
        call  GetMousePosLib
        mov ax,cx  
        mov bx,dx
    mov  ecx,eax
    mov  edx,ebx
    mov  [old_x],eax     
    mov  [old_y],ebx
    jmp  MouseNew
MouseDownOK:
    mov  ecx,[old_x]    
    mov  edx,[old_y]
    mov  [old_x],eax 
    mov  [old_y],ebx
MouseNew:
    sub ecx,2
    sub edx,2
    shl  ecx,16         
    shl  edx,16
    sub  eax,2
    sub  ebx,2
    add  eax,ecx         
    add  ebx,edx

    mov  ecx,ebx             
    mov  ebx,eax
    ;mov  eax,38              
    mov  edx,[active_color]  
    call  DRAW_LINE_38


    mov  edi,0x00010001      
    add  ebx,edi             
    call  DRAW_LINE_38
    add  ecx,edi
    call  DRAW_LINE_38
    sub  ebx,edi
    call  DRAW_LINE_38

    sub  ebx,edi             
    call  DRAW_LINE_38         
    sub  ecx,edi
    call  DRAW_LINE_38
    sub  ecx,edi
    call  DRAW_LINE_38
    add  ebx,edi
    call  DRAW_LINE_38
    add  ebx,edi
    call  DRAW_LINE_38
        hlt
        xor eax,eax
        xor ebx,ebx
        call  GetMousePosLib
        cmp bl,4
        jne  MousepaintExit 
        mov ax,cx  
        mov bx,dx
        jmp MouseDownOK
   
MousepaintExit:   
        
	mov [First],0
        mov   [vesaMOUSEoff],0
                int   2Ch 
  jmp stillPaint1

;******************************************************************************

save_canvas:
    mov  ax,word[MainWindowXsize] ;[process.x_size]
    add  eax,[workareacx]
    mov  bx,word[MainWindowYsize] ;[process.y_size]
    add  ebx,[workareacy]

    jmp stillPaint1

;******************************************************************************



draw_window_paint:

   ; mov  eax,48                   
   ; mov  ebx,3
   ; mov  ecx,sc
   ; mov  edx,sizeof.system_colors
   ; int  0x40

        mov   ebx,1                      
        call  WINDOW_REDRAW_STATUS_12

;;;;;;;;;;;
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
;;;;;;;;;;;                                  


    ;mov  eax,9                     
    ;mov  ebx,process
    ;mov  ecx,-1
    ;int  0x40
    xor  eax,eax
    mov  ax,word[MainWindowXsize] ;[process.x_size]      
    add  eax,-20                   
    mov  [workareasx],eax 
    xor  eax,eax        
    mov  ax,word[MainWindowYsize] ;[process.y_size]      
    add  eax,-60                   
    mov  [workareasy],eax         

                                   
    mov  eax,1                    
    mov  ebx,8*65536+8            
        mov   ecx,SkinTextColor  
    mov  edx,header                
    mov  esi,header_len            
    ;int  0x40
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
                                   
   ; mov  eax,8                     
;   mov  ebx,(300-19)*65536+12  
    xor  ebx,ebx   
    mov  bx,word[MainWindowXsize] ;[process.x_size]
    add  ebx,-19
    shl  ebx,16
    add  ebx,12
    mov  ecx,5*65536+12            
    mov  edx,1                    
    mov  esi,0x00414449     
    ;int  0x40
        call  DEFINE_BUTTON_08 

    cmp  word[MainWindowYsize],80
    jb   .finish


    mov  ebx,10*65536+10           
    mov  ecx,27*65536+10          
 .new_button:
    inc  edx                      
    mov  esi,[btn_colors-8+edx*4]  
   ; int  0x40 
        call  DEFINE_BUTTON_08                      
    add  ebx,12*65536              
    cmp  edx,9                     
    jbe  .new_button               
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;
    xor  ebx,ebx   
    mov  ebx,[workareacx]
    mov  ecx,[workareacy]
    shl  ebx,16
    shl  ecx,16
    add  ebx,[workareasx]
    add  ecx,[workareasy]
    mov  edx,11 
    or   edx,0x40000000                   
    mov  esi,0x00ffff00    
    ;int  0x40
        call  DEFINE_BUTTON_08 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;mov  eax,13                    
    mov  ebx,[workareacx]
    mov  ecx,[workareacy]
    shl  ebx,16
    shl  ecx,16
    add  ebx,[workareasx]
    add  ecx,[workareasy]
    mov  edx,0xffffff
   ; int  0x40
    call  DRAW_BAR_13

 .finish:
        mov   eax,1               
        mov   ebx,400-16
        shl   ebx,16
        mov   bx,8
        mov   ecx,SkinTextColor 
        mov   edx,CharXpaint
        mov   esi,1
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
   ; mov  eax,12      
        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12

    ret

;******************************************************************************

;data

header:                 
   db  'PAINT v0.1 for DexOS'
header_len = $ - header  

   mouse_pressed   db  0 


btn_colors:
   dd 0xdddddd ; white
   dd 0x444444 ; black
   dd 0x00dd00 ; green
   dd 0x0000dd ; blue
   dd 0xdd0000 ; red
   dd 0xdd00dd ; magenta
   dd 0xdddd00 ; yellow
   dd 0x00dddd ; cyan
   dd 0x559955 ; warm green


colors:
   dd 0xffffff ; 
   dd 0x000000 ; 
   dd 0x00ff00 ; 
   dd 0x0000ff ; 
   dd 0xff0000 ; 
   dd 0xff00ff ; 
   dd 0xffff00 ; 
   dd 0x00ffff ; 
   dd 0x77bb77 ; 

;******************************************************************************
;udata
CharXpaint              db 'X' 
   active_color    dd  0xdddd00        

   old_x           dd  ?          
   old_y           dd  ?

                   
   workareacx     dd  ?              
   workareacy     dd  ?
   workareasx     dd  ?              
   workareasy     dd  ?



   restflag    dd  ?
  ; canvas      rb  800*600*3

meos_app_end:
