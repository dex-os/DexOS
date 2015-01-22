;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   PAINT 0.02 MenuetOS (c) Ville Turjanmaa
;;
;;   Compile with FASM
;;
;;   Converted for DexOS, By Dex
;;   

AppPaint:
        mov   dword[active_color],0x00000000
        mov   word[MainWindowX],90 ;100                    
        mov   word[MainWindowY],90 ;100 
        mov   word[MainWindowXsize],400   
        mov   word[MainWindowYsize],300    
        mov   [workareacx],10 
        mov   [workareacy],45  
        call  draw_window_paint
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
        jmp   stillPaint1
stillPaint:            
        call  BuffToScreen
stillPaint1: 
        call  WAIT_FOR_EVENT_10                  
        cmp   eax,1   
        je    redPaint      
        cmp   eax,3   
        je    buttonPaint
        jmp   stillPaint1  
                  
redPaint:                  
        mov   eax,[workareacx]
        mov   ebx,[workareasx]
        add   ebx,8  
        mov   ecx,[workareacy]
        mov   edx,[workareasy]
        add   edx,44
        mov   esi,0 
        call  CHECK_BOUNDARIES_80
	jnc   mouse 
        mov   ax,0
        mov   bx,word[MainWindowXsize]
        sub   bx,20
        mov   cx,0
        mov   dx,24
        mov   esi,1
        call  CHECK_BOUNDARIES_80
        jc    stillPaint1
        mov  eax,[workareacx]
        add  ax,word[MainWindowX]
        mov  ebx,[workareacy]
        add  bx,word[MainWindowY]
        mov  edx,[workareasx]
        mov  ecx,[workareasy]
        call  SET_SAVE_BOX_SIZE
        call  MOVE_MAIN_WINDOW_81 
        call  draw_window_paint
        pushad
        xor   eax,eax
        mov   ebx,eax
        mov   edx,eax
        mov   ecx,eax
	mov   ax,word[MainWindowX] 
        add   eax,[workareacx]            
	mov   bx,word[MainWindowY] 
        add   ebx,[workareacy]            
	mov   edx,[workareasx]       
	mov   ecx,[workareasy]
        mov   esi,BufferTemp       
        call  ReStoreFilledBoxVesa  
        popad      
        jmp   stillPaint
buttonPaint:        
        xor   eax,eax
        call  GET_PRESSED_BUTTON_ID_17
        shr   eax,8   
        cmp   eax,1      
        jne  .noclosePaint
        ;Exit Here 
        ret                                     

.noclosePaint:
        cmp   eax,11
        je    ButtonReleasePaint
        cmp   eax,12
        je    ButtonNewPaint
        dec   eax
        dec   eax
        cmp   eax,9
        ja    stillPaint1
        mov   eax,[colors+eax*4]
        mov   [active_color],eax
        jmp   stillPaint1
ButtonReleasePaint:
        mov   [mouse_pressed],0 
        jmp    stillPaint1

ButtonNewPaint:
        call  draw_window_paint
        jmp    stillPaint
mouse:
        call  GetMousePosLib
        cmp   bl,4
        je   .leftbtn
.not_pressed:
        mov   [mouse_pressed],0 
        jmp   stillPaint1
.leftbtn:
        cmp   [active_color],0xffffff
        je    ShowMousePaint
        mov   [vesaMOUSEoff],1
        call  ReStoreMouseBAackGround
ShowMousePaint:
        xor   eax,eax
        xor   ebx,ebx
        call  GetMousePosLib
        mov   ax,cx  
        mov   bx,dx
        mov   ecx,eax
        mov   edx,ebx
        mov   [old_x],eax     
        mov   [old_y],ebx
        jmp   MouseNew
MouseDownOK:
        mov   ecx,[old_x]    
        mov   edx,[old_y]
        mov   [old_x],eax 
        mov   [old_y],ebx
MouseNew:
        sub   ecx,2
        sub   edx,2
        shl   ecx,16         
        shl   edx,16
        sub   eax,2
        sub   ebx,2
        add   eax,ecx         
        add   ebx,edx
        mov   ecx,ebx             
        mov   ebx,eax
        mov   edx,[active_color]  
        call  DRAW_LINE_38
        mov   edi,0x00010001      
        add   ebx,edi             
        call  DRAW_LINE_38
        add   ecx,edi
        call  DRAW_LINE_38
        sub   ebx,edi
        call  DRAW_LINE_38
        sub   ebx,edi             
        call  DRAW_LINE_38         
        sub   ecx,edi
        call  DRAW_LINE_38
        sub   ecx,edi
        call  DRAW_LINE_38
        add   ebx,edi
        call  DRAW_LINE_38
        add   ebx,edi
        call  DRAW_LINE_38
        hlt
        xor   eax,eax
        xor   ebx,ebx
        call  GetMousePosLib
        cmp   bl,4
        jne   MousepaintExit 
        mov   ax,cx  
        mov   bx,dx
        jmp   MouseDownOK
MousepaintExit:
        mov   [vesaMOUSEoff],0
        cmp   [active_color],0xffffff
        je    ShowMousePaint1   
       	mov   [First],0
        int   2Ch 
ShowMousePaint1:
        jmp   stillPaint1

draw_window_paint:
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
                                  
        xor   eax,eax
        mov   ax,word[MainWindowXsize]      
        add   eax,-20                   
        mov   [workareasx],eax 
        xor   eax,eax        
        mov   ax,word[MainWindowYsize]    
        add   eax,-60                   
        mov   [workareasy],eax         
        mov   eax,1  
        mov   bx,word[MainWindowXsize]
        shr   bx,1
        mov   ecx,header_len
        shr   ecx,1
        mov   edx,ecx
        shl   cx,2
        add   cx,dx
        add   cx,dx
        sub   bx,cx
        shl   ebx,16
        mov   bx,8        
        mov   ecx,SkinTextColor  
        mov   edx,header                
        mov   esi,header_len            
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
        cmp   word[MainWindowYsize],80
        jb   .finish
        mov   ebx,10*65536+30          
        mov   ecx,23*65536+14          
        mov   edx,12                     
        mov   esi,0x00414449    
        call  DEFINE_BUTTON_08  
        mov   edx,1           
        mov   ebx,44*65536+10           
        mov   ecx,27*65536+10          
 .new_button:
        inc   edx                      
        mov   esi,[btn_colors-8+edx*4]  
        call  DEFINE_BUTTON_08                      
        add   ebx,12*65536              
        cmp   edx,9                     
        jbe   .new_button               
        xor   ebx,ebx   
        mov   ebx,[workareacx]
        mov   ecx,[workareacy]
        shl   ebx,16
        shl   ecx,16
        add   ebx,[workareasx]
        add   ecx,[workareasy]
        mov   edx,11 
        or    edx,0x40000000                   
        mov   esi,0x00ffff00    
        call  DEFINE_BUTTON_08 

        mov   ebx,[workareacx]
        mov   ecx,[workareacy]
        shl   ebx,16
        shl   ecx,16
        add   ebx,[workareasx]
        add   ecx,[workareasy]
        mov   edx,0xffffff
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

        mov   eax,1               
        mov   ebx,16
        shl   ebx,16
        mov   bx,26
        mov   ecx,SkinTextColor 
        mov   edx,CharNEWpaint
        mov   esi,3
        call  WRITE_TEXT_TO_WINDOW_LEN_04 
   
        mov   ebx,2                   
        call  WINDOW_REDRAW_STATUS_12
        mov   eax,[workareacx]
        mov   ebx,[workareasx]
        add   ebx,8
        mov   ecx,[workareacy]
        mov   edx,[workareasy] 
        add   edx,44
        call  DRAW_LINE_SET_YX_38A
        ret

;data
CharNEWpaint: db 'New',0
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

CharXpaint      db 'X' 
active_color    dd  0x00000000      
old_x           dd  ?          
old_y           dd  ?
workareacx      dd  ?              
workareacy      dd  ?
workareasx      dd  ?              
workareasy      dd  ?
restflag        dd  ?
meos_app_end:
