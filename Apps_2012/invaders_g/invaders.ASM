; INVADERS v1.0
;
; by the hidden player
;
;
format binary as 'gex'
use32
	ORG   0x1A00000 			 ; where our program is loaded to
	jmp   start				       ; jump to the start of program.
	db    'GEX6'				       ; NOTE: ON THE NEW VER, THIS WILL NEED CHANGING TO 'DEX2'


SCREEN_X equ 640
SCREEN_Y equ 480

SHIP_X equ 32
SHIP_Y equ 32

SHIP_X_POS equ (SCREEN_X-SHIP_X)/2
SHIP_Y_POS equ SCREEN_Y-SHIP_Y-15

ENEMY_X equ 32
ENEMY_Y equ 32

ALIEN_X equ 48
ALIEN_Y equ 38
ALIEN_Y_POS equ 1

BOUNDARY equ 10
MOVEMENT equ 7

TRANSPARENCY equ 0x00000000

WINDOW_X equ 100
WINDOW_Y equ 100

BULLET_X equ 10
BULLET_Y equ 10

STARS_	 equ 226
STARLEVELS equ 3

ENEMY_STARTING_X equ 25
ENEMY_STARTING_Y equ 50

BULLETSPEED equ 12

gif_hash_offset = gif_hash_area

include 'ascgl.inc'

start:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax

	mov   eax,[return]	       ;****************
	mov   [eax],esp 	       ;****************
	mov   esp,0x1f0000	       ;**************** 0x200000 -64k

;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  SpeedUpKeyBoard
;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;=======================================================  ;
; Get call table address.                                 ;
;=======================================================  ;
	mov	edi,Functions				  ; fill the function table
	mov	al,0					  ; so we have some usefull functions
	mov	ah,0x0a 				  ;
	int	50h					  ;
;=======================================================  ;
; Load vesa info.                                         ;
;=======================================================  ;
	call	[LoadVesaInfo]				  ;
	mov	edi,VESA_Info				  ;
	mov	ecx,193 				  ;
	cld						  ;
	cli						  ;
	rep	movsd					  ;
	sti						  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;=======================================================  ;
; Decode Dif                                              ;
;=======================================================  ;
	mov	esi,file_area_Menu			  ;
	mov	edi,Menu_Buffer 			  ;
	mov	ecx,VesaBuffer				  ;
	mov	ebx,3					  ;
	int	40h					  ;
	jc	VesaError1				  ;
;=======================================================  ;
; get menu buffer address                                 ;
;=======================================================  ;
	mov	ebx,1					  ;
	mov	ecx,0					  ; get background buffer address 
	int	40h					  ;                                       
	mov	dword[MenuBuffer],esi			  ;
;=======================================================  ;
; Move the background image to our buffer                 ;
;=======================================================  ;
	call	MoveBuffer				  ;
;=======================================================  ;
; Draw the main box alpha image                           ;
;=======================================================  ;
	mov	edi,VesaBuffer				  ;
	mov	esi,Menu_Buffer 			  ;
	mov	eax,48					  ;
	mov	ecx,2					  ;3
	mov	ebx,6					  ; 
	int	40h					  ;
;=======================================================  ;
; Write buffer to screen                                  ;
;=======================================================  ;
	call	BuffToScreenScr 			  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;=======================================================  ;
; Set some vars up                                        ;
;=======================================================  ;
	xor	eax,eax 				  ;
	mov	ax,[ModeInfo_XResolution]		  ;
	shr	ax,1					  ;
	sub	ax,320					  ;
	mov	[WinStartX],eax 			  ;
	xor	eax,eax 				  ;
	mov	ax,[ModeInfo_YResolution]		  ;
	shr	ax,1					  ;
	sub	ax,285					  ;
	mov	[WinStartY],eax 			  ;
	call	GetWinBufferPointer			  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov   cx,100 ;
	call  [InterruptTimer]

    giftoimg gif_bullet,bullet
    giftoimg gif_ship,ship
    giftoimg gif_enemy1,enemy1
    giftoimg gif_enemy2,enemy2
    giftoimg gif_enemy3,enemy3
    giftoimg gif_enemy4,enemy4
    giftoimg gif_enemy5,enemy5
    giftoimg gif_alien,alien
    giftoimg gif_menu1,menu1
    giftoimg gif_menu2,menu2
    giftoimg gif_menu3,menu3
    giftoimg gif_menu4,menu4
    giftoimg gif_logo,logo
    giftoimg gif_pause,pause_
    giftoimg gif_highscore,highscore
    giftoimg gif_smallfont,smallfont
;    giftoimg gif_planet,planet

    call createstars
    call drawit

still:

    cmp  byte[status],3
    je	 waitfordraw
    call drawit

    call checkbullet


   waitfordraw:

    mov  ax,1
    call [SetDelay]
    call [KeyPressedScanNW]  ; ah=scancode al=ascii
    test ah,ah
    jz	 still

    cmp  byte[status],1
    jne  nogame

	 cmp  ah,77
	 jnz  noright
	 cmp  dword[ship_x],SCREEN_X-SHIP_X-BOUNDARY
	 jge  noright
	 add  dword[ship_x],MOVEMENT

	 jmp  still

       noright:
	 cmp  ah,75
	 jnz  noleft
	 cmp  dword[ship_x],BOUNDARY
	 jle  noleft
	 sub  dword[ship_x],MOVEMENT

	 jmp  still

       noleft:
	 cmp  ah,72
	 jnz  noup

	 cmp  dword[bullet_y],1
	 jg  noup

	 mov  eax,dword[ship_x]
	 add  eax,(SHIP_X-BULLET_X)/2
	 mov  dword[bullet_x],eax
	 mov  dword[bullet_y],SHIP_Y_POS;-BULLET_Y

	 jmp  still

       noup:
	 cmp  ah,25
	 jnz  no_pause1

	 mov  byte[status],3

	 call drawit

	 jmp  still

       no_pause1:

nogame:
    cmp  byte[status],0
    jne  nomenu

	 cmp  ah,80
	 jnz  no_down

	 cmp  byte[menu],3
	 jne  no_menu1

	 mov  byte[menu],0
	 jmp  no_menu3

       no_menu1:
	 inc  byte[menu]

       no_menu3:
       no_down:
	 cmp  ah,72
	 jnz  no_up

	 cmp  byte[menu],0
	 jne  no_menu0

	 mov  byte[menu],3
	 jmp  no_menu2

       no_menu0:
	 dec  byte[menu]

       no_menu2:
       no_up:

	 cmp  ah,1
	 je   exit

       no_esc:
	 cmp  ah,28
	 jnz  no_enter

	 call exemenu

      no_enter:
nomenu:
    cmp  byte[status],3
    jne  nopause

    cmp  ah,25
    jnz  nopause

    mov  byte[status],1

  nopause:
    cmp  byte[status],0
    je	 still

    cmp  ah,1
    jnz  still

    mov  byte[status],0

    jmp  still






    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   ;;
   ;;  Drawing routine
   ;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

drawit:
    mov  eax,0x00000000
    call fillscreen
;    imgtoimg planet,0,0,vscreen                                            ; Draw background
    call drawstars

    cmp  byte[status],1
    jne  @f

    call drawenemys						  ; Draw the enemy's to buffer
    call drawbullet
    aimgtoimg ship,dword[ship_x],SHIP_Y_POS,vscreen,TRANSPARENCY,0  ; Draw the ship to buffer

    jmp  .done

@@:
    cmp  byte[status],2
    jne  @f

    jmp  .done

@@:
    cmp  byte[status],4 ; about screen
    jne  @f


    mov  esi,msgAbout
    mov  ebx,20
    mov  ecx,20

.charloop:
    movzx  eax,byte[esi]

    test eax,eax
    jz	 .done
    cmp  al,13
    je	 .nextline
    cmp  al,'a'
    jl	 .nextchar
    cmp  al,'z'
    jg	 .nextchar

    mov  edx,eax
    sub  edx,'a'
    mov  eax,20
    mul  edx
    mov  edx,eax

    pusha
    getimg smallfont,0,edx,24,20,smallchar
    popa

    pusha
    aimgtoimg smallchar,ebx,ecx,vscreen,TRANSPARENCY,0
    popa

.nextchar:
    add  ebx,25
    inc  esi

    jmp  .charloop

.nextline:
    mov  ebx,20
    add  ecx,21
    inc  esi

    jmp  .charloop

@@:
    cmp  byte[status],3
    jne  @f

    aimgtoimg pause_,150,80,vscreen,TRANSPARENCY,0

    jmp  .done
@@:
    cmp  byte[status],0
    jne  @f

    aimgtoimg logo,50,80,vscreen,TRANSPARENCY,0


    cmp  byte[menu],0
    jne  .menu_0
    aimgtoimg menu1,30,200,vscreen,TRANSPARENCY,1
    jmp  .menu_1
.menu_0:
    aimgtoimg menu1,30,200,vscreen,TRANSPARENCY,0
.menu_1:
    cmp  byte[menu],1
    jne  .menu_2
    aimgtoimg menu2,80,250,vscreen,TRANSPARENCY,1
    jmp  .menu_3
.menu_2:
    aimgtoimg menu2,80,250,vscreen,TRANSPARENCY,0
.menu_3:
    cmp  byte[menu],2
    jne  .menu_4
    aimgtoimg menu3,120,300,vscreen,TRANSPARENCY,1
    jmp  .menu_5
.menu_4:
    aimgtoimg menu3,120,300,vscreen,TRANSPARENCY,0
.menu_5:
    cmp  byte[menu],3
    jne  .menu_6
    aimgtoimg menu4,150,350,vscreen,TRANSPARENCY,1
    jmp  .menu_7
.menu_6:
    aimgtoimg menu4,150,350,vscreen,TRANSPARENCY,0
.menu_7:
    jmp  .done
@@:
    cmp  byte[status],5
    jne  @f

    aimgtoimg highscore,60,40,vscreen,TRANSPARENCY,0
@@:
.done:
    call  BuffToScreen
    ret


drawenemys:

	mov  dword[alldeadb],1

	cmp  byte[enemy_d],0
	jne  other_dir

	cmp  dword[enemy_x],2
	jg   no_change_dir
	mov  byte[enemy_d],1
	add  dword[enemy_y],1
      no_change_dir:
	sub  dword[enemy_x],2
	jmp  no_other_dir

      other_dir:
	cmp  dword[enemy_x],SCREEN_X-5*(BOUNDARY+ENEMY_X)
	jl   no_change_dir2
	mov  byte[enemy_d],0
	add  dword[enemy_y],1
      no_change_dir2:
	add  dword[enemy_x],2
      no_other_dir:

	mov  edi,enemy_table
	mov  eax,dword[enemy_x]
	mov  dword[current_enemy_x],eax
	mov  eax,dword[enemy_y]
	mov  dword[current_enemy_y],eax

      loopit:
	cmp  byte[edi],1
	je   drawenemy1

	cmp  byte[edi],2
	je   drawenemy2

	cmp  byte[edi],3
	je   drawenemy3

	cmp  byte[edi],4
	je   drawenemy4

	cmp  byte[edi],5
	je   drawenemy5

	cmp  edi,enemy_table_end
	je   alldead
	call nextenemy
	jmp  loopit

      drawenemy1:
	mov  dword[alldeadb],0
	pusha
	aimgtoimg enemy1,dword[current_enemy_x],dword[current_enemy_y],vscreen,TRANSPARENCY,0
	popa

	jmp  checknext

      drawenemy2:
	mov  dword[alldeadb],0
	pusha
	aimgtoimg enemy2,dword[current_enemy_x],dword[current_enemy_y],vscreen,TRANSPARENCY,0
	popa

	jmp  checknext

      drawenemy3:
	mov  dword[alldeadb],0
	pusha
	aimgtoimg enemy3,dword[current_enemy_x],dword[current_enemy_y],vscreen,TRANSPARENCY,0
	popa

	jmp  checknext

      drawenemy4:
	mov  dword[alldeadb],0
	pusha
	aimgtoimg enemy4,dword[current_enemy_x],dword[current_enemy_y],vscreen,TRANSPARENCY,0
	popa

	jmp  checknext

      drawenemy5:
	mov  dword[alldeadb],0
	pusha
	aimgtoimg enemy5,dword[current_enemy_x],dword[current_enemy_y],vscreen,TRANSPARENCY,0
	popa

      checknext:
	cmp  edi,enemy_table_end
	je   alldead
	call nextenemy
	jmp  loopit

      alldead:
	cmp  dword[alldeadb],0
	je   enemy_end

	call new_game

      enemy_end:
;        cmp  dword[alien_x],0
;        je   alien_end
;
;        cmp  dword[alien_x],SCREEN_X-ALIEN_X
;        jl   @f
;
;        mov  edx, 0
;        mov  eax, [alien_x]
;        sub  eax, SCREEN_X
;        mov  ebx, ALIEN_X
;        sub  ebx, eax
;
;        jmp  drawalien
;      @@:
;
;      drawalien:
;        getimg alien,ALIEN_X-1,ALIEN_Y,2,1,alienpiece
;        aimgtoimg alienpiece,dword[alien_x],ALIEN_Y_POS,vscreen,TRANSPARENCY,0
;        dec  dword[alien_x]
;
;      alien_end:
	ret

nextenemy:
	inc  edi
	add  dword[current_enemy_x],ENEMY_X+BOUNDARY
	mov  eax,dword[current_enemy_x]
	sub  eax,dword[enemy_x]
	cmp  eax,5*(ENEMY_X+BOUNDARY)
	jl   no_newline

	sub  dword[current_enemy_x],5*(ENEMY_X+BOUNDARY)
	add  dword[current_enemy_y],ENEMY_Y+BOUNDARY
      no_newline:
	ret

drawbullet:
	cmp  dword[bullet_y],BULLETSPEED
	jl   nobullet
	sub  dword[bullet_y],BULLETSPEED

	aimgtoimg bullet,dword[bullet_x],dword[bullet_y],vscreen,TRANSPARENCY,0

      nobullet:
	ret

checkbullet:
	cmp  dword[bullet_y],BULLETSPEED				   ; does the bullet hit top of the screen?
	jl   nohit						 ; yes, hide bullet

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       ; check if bullet is inside the enemy field (you can disble this if you want)
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       mov  eax,dword[enemy_y]
       cmp  dword[bullet_y],eax
       jl   nohit

       ;mov  eax,dword[enemy_y]
       add  eax,4*(ENEMY_Y+BOUNDARY)
       cmp  dword[bullet_y],eax
       jg   nohit

       mov  eax,dword[enemy_x]
       cmp  dword[bullet_x],eax
       jl   nohit

       ;mov  eax,dword[enemy_x]
       add  eax,5*(ENEMY_Y+BOUNDARY)
       cmp  dword[bullet_x],eax
       jg   nohit

       ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       mov  edi,enemy_table
       mov  eax,dword[enemy_x]
       mov  dword[current_enemy_x],eax
       mov  eax,dword[enemy_y]
       mov  dword[current_enemy_y],eax

      check:
	cmp byte[edi],0 		; is the enemy at this position alive?
	je  nextcheck			; no, try next enemy

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       ; check if bullet hits current enemy
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

       mov  eax,dword[current_enemy_y]	; move the enemy y position into eax
       cmp  dword[bullet_y],eax 	; is the bullet's y position less than eax (enemy y pos)
       jl   nextcheck			; yes, bullet can't be colliding, check next enemy

       ;mov  eax,dword[current_enemy_y]  ; move the enemy y position into eax
       add  eax,ENEMY_Y 		; add the width of the enemy to the enemy's y position (wich is still stored in eax)
       cmp  dword[bullet_y],eax 	; is the bullet's y position greater than eax (the end of the enemy)
       jg   nextcheck			; yes, bullet can't be colliding, check next enemy



       mov  eax,dword[current_enemy_x]	; now do the same but for the x positions
       cmp  dword[bullet_x],eax 	;
       jl   nextcheck			;
					;
       ;mov  eax,dword[current_enemy_x]  ;
       add  eax,ENEMY_Y 		;
       cmp  dword[bullet_x],eax 	;
       jg   nextcheck			;

       jmp  hit

       ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      nextcheck:
	call nextenemy			; calculate the next enemy x pos etc
	cmp  edi,enemy_table_end	; is this the last enemy?
	je   nohit			; yes, none of them was hit
	jmp  check			; no, check if enemy is alive and draw it

      hit:
	movsx ebx,byte[edi]		; mov the enemy number onto ebx
	add  dword[score],ebx		; add this number to the score

	mov  byte[edi],0		; hit? hide the enemy
	mov  dword[bullet_y],1		; mov the bullet to top of screen (hide it)

     nohit:
ret


fillscreen: ; eax - screen color ( 0x00RRGGBB )
    mov  edi,vscreen+8
    cld
    mov  ecx,SCREEN_X*SCREEN_Y
  .lab1:
    mov  [edi],eax
    add  edi,3
    loop .lab1
    ret

gameover:
    mov  byte[status],2
    ret



exemenu:
    cmp  byte[menu],0  ;start
    jne  exemenu1
    jmp  new_game

exemenu1:
    cmp  byte[menu],1  ;about
    jne  exemenu2
    mov  byte[status],4
    jmp  drawit

exemenu2:
    cmp  byte[menu],2  ;about
    jne  exemenu3
    mov  byte[status],5
    jmp  drawit

exemenu3:
    cmp  byte[menu],3  ;exit
    je	 menuexit

    ret

menuexit:
       ; pop   eax ; we dont want to return to 'call exemenu' bu we want to return to CLI!
       ; pop   eax
exit:
	mov   cx,0
	call  [InterruptTimer]

	mov   eax,[return]			      ;********************
	mov   esp,[eax] 			      ;********************
	ret


VesaError1:					     ;we end up here, if vesa mode not supported.
	mov   eax,[return]			      ;********************
	mov   esp,[eax] 			      ;********************
	ret



new_game:
     mov  dword[enemy_x],ENEMY_STARTING_X
     mov  dword[enemy_y],ENEMY_STARTING_Y

     mov  eax,level1
     mov  ebx,enemy_table

   next:
     mov  ecx,[eax]
     mov  [ebx],ecx

     cmp  ebx,enemy_table_end
     je   done

     inc  eax
     inc  ebx

     jmp  next

    done:
     mov byte[status],1
     jmp  still

     ret

drawstars:
    mov   esi, STARS

.loop:
    cmp   esi, STARS+(STARS_*5)
    jge   .done

    movzx eax, byte[esi]    ; z (speed, brightness)
    movzx ebx, word[esi+1]  ; x
    movzx ecx, word[esi+3]  ; y
    add   bx , ax
    cmp   bx , SCREEN_X
    jl	  .moveit

    xor   ebx,ebx
    inc   ebx

    call  random_generator
    mov   ecx, [generator]
    and   ecx, SCREEN_Y-1
    inc   ecx
    mov   word[esi+3],cx

    call  random_generator
    and   al, STARLEVELS
    test  al,al
    jnz   @f
    inc   al
   @@:
    mov   [esi],al

.moveit:
    mov   word [esi+1],bx

    movzx eax, byte[esi]
    inc   eax
    mov   edx, 0xff/(STARLEVELS+1)
    mul   edx

    mov   ah,al
    shl   eax,8
    mov   al,ah
    mov   ebp, eax

    mov   eax, SCREEN_X
    mul   ecx
    add   eax, ebx
    mov   edx, 3
    mul   edx

    cmp   eax, SCREEN_X*SCREEN_Y*3
    jg	  @f
    add   eax, vscreen+8
    and   dword[eax],0xff000000
    or	  dword[eax],ebp
   @@:

    add   esi, 5
    jmp   .loop
.done:

    ret


;====================================================;
; SpeedUpKeyBoard.                                   ;
;====================================================;

SpeedUpKeyBoard:
	push  eax
	mov   al,0xf3				     
	out   0x60,al				      
KeyBoardWait:
	in    al,0x64
	test  al,10b				      
	jne   KeyBoardWait			      
	mov   al,1 
	and   al,111b
	out   0x60,al				      
	pop   eax
	ret


createstars:
    mov   ebx, STARS
.loop:
    cmp   ebx, STARS+(STARS_*5)
    jge   .done

    call  random_generator
    and   al, STARLEVELS
    test  al,al
    jnz   @f
    inc   al
   @@:
    mov   byte[ebx],al

    call  random_generator
    and   eax, SCREEN_X-1
    inc   eax
    mov   word[ebx+1],ax

    call  random_generator
    and   eax, SCREEN_Y-1
    inc   eax
    mov   word[ebx+3],ax

    add   ebx, 5
    jmp   .loop
.done:

    ret

 ;----------------------------------------------------;
 ; BuffToScreen.  ;Puts whats in the buffer to screen ;
 ;----------------------------------------------------;
BuffToScreen:
	cmp   [ModeInfo_BitsPerPixel],24
	jne   Try32
	call  BuffToScreen24
	jmp   wehavedone24
Try32:
	cmp   [ModeInfo_BitsPerPixel],32
	jne   wehavedone24
	call  BuffToScreen32 
wehavedone24:
	ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               32bpp ;
 ;----------------------------------------------------;
BuffToScreen24:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[VesaScreen] ;[ModeInfo_PhysBasePtr]
	 mov   esi,vscreen + 8
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax
	 mov   bx,[ModeInfo_XResolution]
	 sub   ebx,640
	 mov   [ImageXaddOn24],ebx
	 shl   ebx,2
	 sub  ebx,[ImageXaddOn24]
	 mov  [ImageXaddOn],ebx
	 mov   ecx,480

testloop24:
	 push  ecx
	 mov   ecx,640
	 cld
@@:
	 lodsd
	 dec   esi
	 stosd
	 dec   edi
	 loop @b
	 add   edi,[ImageXaddOn]
	 pop   ecx
	 loop  testloop24
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               24bpp ;
 ;----------------------------------------------------;
BuffToScreen32:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[VesaScreen] ;[ModeInfo_PhysBasePtr]
	 mov   esi,vscreen + 8
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax
	 mov   bx,[ModeInfo_XResolution]
	 sub   ebx,640
	 shl   ebx,2
	 mov   ecx,480
testloop32:
	 push  ecx
	 mov   ecx,640
	 cld
@@:	 lodsd

	 stosd
	 dec   esi
	 loop @b
	 add   edi,ebx
	 pop   ecx
	 loop  testloop32
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; GetWinBufferPointer.                               ;
 ;----------------------------------------------------;
GetWinBufferPointer:					 
	push  eax					      
	push  ebx
	mov   eax,[WinStartX]
	mov   [WinBuffX],eax
	mov   ebx,[WinStartY]
	mov   [WinBuffY],ebx
	mov   edi,[ModeInfo_PhysBasePtr]				 
	xor   ebx,ebx
	mov   ebx,[WinBuffX]
	shl   ebx,2
	cmp   [ModeInfo_BitsPerPixel],24   
	jne   @F
	sub   ebx,[WinBuffX] 
@@:	  
	add   edi, ebx
	mov   ebx,[WinBuffY]
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution] 
	shl   eax,2 
	     
	cmp   [ModeInfo_BitsPerPixel],24    
	jne   @f
	sub   ax,[ModeInfo_XResolution] 
@@:
	mul   ebx
	add   edi,eax
	mov   dword[VesaScreen],edi
	pop   ebx
	pop   eax
	ret

;;;;;;;;;;;;;;;;;;;;;;;;
;=======================================================  ;
; BuffToScreen.                                           ;
;=======================================================  ;
BuffToScreenScr:					  ; test for 24bit or 32bit vesa
	cmp	[ModeInfo_BitsPerPixel],24		  ;
	jne	Try32Scr				  ;
	call	BuffToScreen24Scr			  ;
	jmp	wehavedone24Scr 			  ;
Try32Scr:						  ;
	cmp	[ModeInfo_BitsPerPixel],32		  ;
	jne	wehavedone24Scr 			  ;
	call	BuffToScreen32Scr			  ;
wehavedone24Scr:					  ;
@@:							  ;
	ret						  ;

;=======================================================  ;
; BuffToScreen32 (32bpp)                                  ;
;=======================================================  ;
BuffToScreen32Scr:					  ;
	 pushad 					  ;
	 push	 es					  ;
	 mov	 ax,8h					  ;
	 mov	 es,ax					  ;
	 mov	 edi,[ModeInfo_PhysBasePtr]		  ;
	 mov	 esi,VesaBuffer 			  ;
	 xor	 eax,eax				  ;
	 mov	 ecx,eax				  ;
	 mov	 ax,[ModeInfo_XResolution]		  ;
	 mov	 cx,[ModeInfo_YResolution]		  ;
	 mul	 ecx					  ;
	 mov	 ecx,eax				  ;
	 cld						  ;
	 cli						  ;
	 rep	 movsd					  ;
	 sti						  ;
	 pop	 es					  ;
	 popad						  ;
	 ret						  ;

;=======================================================  ;
; BuffToScreen24 (24bpp)                                  ;
;=======================================================  ;
BuffToScreen24Scr:					  ;
	 pushad 					  ;
	 push	 es					  ;
	 mov	 ax,8h					  ;
	 mov	 es,ax					  ;
	 xor	 eax,eax				  ;
	 mov	 ecx,eax				  ;
	 mov	 ebx,eax				  ;
	 mov	 ax,[ModeInfo_YResolution]		  ;
	 mov	 ebp,eax				  ;
	 lea	 eax,[ebp*2+ebp]			  ;
	 mov	 edi,[ModeInfo_PhysBasePtr]		  ;
	 mov	 esi,VesaBuffer 			  ;
	 cld						  ;
.l1:							  ;
	 mov	 cx,[ModeInfo_XResolution]		  ;
	 shr	 ecx,2					  ;
.l2:							  ;
	 mov	 eax,[esi]				  ;
	 mov	 ebx,[esi+4]				  ;
	 shl	 eax,8					  ;
	 shrd	 eax,ebx,8				  ;
	 stosd						  ;
							  ;
	 mov	 ax,[esi+8]				  ;
	 shr	 ebx,8					  ;
	 shl	 eax,16 				  ;
	 or	 eax,ebx				  ;
	 stosd						  ;
							  ;
	 mov	 bl,[esi+10]				  ;
	 mov	 eax,[esi+12]				  ;
	 shl	 eax,8					  ;
	 mov	 al,bl					  ;
	 stosd						  ;
							  ;
	 add	 esi,16 				  ;
	 loop	 .l2					  ;
							  ;
	 sub	 ebp,1					  ;
	 ja	.l1					  ;
							  ;
	 pop	 es					  ;
	 popad						  ;
	 ret						  ;

;=======================================================  ;
; Move buffer                                             ;
;=======================================================  ;
MoveBuffer:						  ;
	 pushad 					  ;
	 mov	esi,[MenuBuffer]			  ;
	 mov	edi,VesaBuffer				  ;
	 mov	ecx,800*600				  ;
	 rep	movsd					  ;
	 popad						  ;
	 ret						  ;

;;;;;;;;;;;;;;;;;;;;;;;;
VesaScreen    dd  0
ImageXaddOn24 dd  0
ImageXaddOn   dd  0
WinBuffY      dd  0
WinBuffX      dd  0
WinStartX     dd  0
WinStartY     dd  0
MenuBuffer    dd  0

random_generator:
xor eax,[generator]
imul eax,214013
xor eax,0xdeadbeef
rol eax,9
mov [generator],eax
ror eax,16
and eax,0x7fff

    ret

level1:
db 4,5,2,5,4
db 2,3,2,3,2
db 4,2,4,2,1
db 1,5,2,5,1
rb 10

enemy_table:
db 0,0,0,0,0
db 0,0,0,0,0
db 0,0,0,0,0
db 0,0,0,0,0
enemy_table_end:
rb 10


msg1:	db " Vesa mode not supported",13
	db " Press any key to exit. ",13,0
msgAbout	db 'about text should',13,'be placed here',13,13,'lots of work to do',0
ship_x		dd SHIP_X_POS
enemy_x 	dd 0
enemy_y 	dd 0
enemy_d 	dd 0
current_enemy_x dd 0
current_enemy_y dd 0
bullet_x	dd 0
bullet_y	dd 1
score		dd 0
alldeadb	dd 0
status		dd 0	    ; status: 0=menu 1=game 2=gameover 3=paused 4=about 5=highscores...
menu		db 0	    ; menu: 0=start 1=about 2=highscores 3=exit...
sound1		db 0x85,0x25,0x85,0x40,0
generator	dd 0x45dd4d15
alien_x 	dd SCREEN_X-1
numberofpixels	dd 0
return		dd 0

align 4 						  ;
file_area_Menu: 					  ;
file   'ScrMenu\ScrMenu.dif'				  ;
file_area_Menu_FileEnd: 				  ;
rd 1							  ;

gif_bullet:   file 'bullet.gif'
rb 4

gif_ship:     file 'ship.gif'
rb 4

gif_enemy1:   file 'enemy1.gif'
rb 4

gif_enemy2:   file 'enemy2.gif'
rb 4

gif_enemy3:   file 'enemy3.gif'
rb 4

gif_enemy4:   file 'enemy4.gif'
rb 4

gif_enemy5:   file 'enemy5.gif'
rb 4

gif_alien:    file 'alien.gif'
rb 4

gif_menu1:    file 'menu1.gif'
rb 4

gif_menu2:    file 'menu2.gif'
rb 4

gif_menu3:    file 'menu3.gif'
rb 4

gif_menu4:    file 'menu4.gif'
rb 4

gif_logo:     file 'logo.gif'
rb 4

gif_pause:    file 'pause.gif'
rb 4

gif_highscore file 'highscores.gif'
rb 4

gif_smallfont file 'font_small.gif'
rb 4

;gif_planet    file 'planet.gif'
;rb 4

;xm            file 'space-patrol.xm'
;xm_length = $ - xm

vscreen:
dd SCREEN_X
dd SCREEN_Y
rb SCREEN_X*SCREEN_Y*3+10

IM_END:

STARS:
rb STARS_*5

bullet:
rb BULLET_X*BULLET_Y*3+8+10

ship:
rb SHIP_X*SHIP_Y*3+10

enemy1:
rb ENEMY_X*ENEMY_Y*3+10

enemy2:
rb ENEMY_X*ENEMY_Y*3+10

enemy3:
rb ENEMY_X*ENEMY_Y*3+10

enemy4:
rb ENEMY_X*ENEMY_Y*3+10

enemy5:
rb ENEMY_X*ENEMY_Y*3+10

alien:
rb ALIEN_X*ALIEN_Y+3*10

menu1:
rb 220*15*3+10

menu2:
rb 120*15*3+10

menu3:
rb 235*15*3+10

menu4:
rb 100*15*3+10

logo:
rb 40*540*3+10

pause_:
rb 40*320*3+10

highscore:
rb 40*520*3+10

smallfont:
rb 24*520*3+10

smallchar:
rb 24*20*3+20

alienpiece:
rb ALIEN_X*ALIEN_Y+3*20

;planet:
;rb 640*480*3+10

gif_hash_area:
rd 4096+1			   ;hash area size for unpacking GIF


I_END:

include 'dex.inc'

align 4 						  ;
Menu_Buffer: rd  704*512+2				  ;
VesaBuffer:  rd  800*600				  ;
