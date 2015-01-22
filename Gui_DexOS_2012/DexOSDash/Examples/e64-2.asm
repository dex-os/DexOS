;            
;   64 bit Menuet Example 2
;
;   Compile with FASM 1.60 or above
;

use64

    org   0x0

    db    'MENUET64'              ; Header identifier
    dq    0x01                    ; Version
    dq    START                   ; Start of code
    dq    image_end               ; Size of image
    dq    0x100000                ; Memory for app
    dq    0xffff0                 ; Esp
    dq    0x00                    ; Prm 
    dq    0x00                    ; Icon


START:

    call  draw_window       ; At first, draw the window

still:

    mov   rax , 10          ; Wait here for event
    int   0x60

    test  rax , 0x1         ; Window redraw
    jnz   window_event
    test  rax , 0x2         ; Keyboard press
    jnz   key_event
    test  rax , 0x4         ; Button press
    jnz   button_event

    jmp   still

window_event:

    call  draw_window
    jmp   still

key_event:

    mov   rax , 0x2         ; Read the key and ignore
    int   0x60

    jmp   still

button_event:

    mov   rax , 0x11                      ; Get data
    int   0x60

    ; rax = status
    ; rbx = button id

    cmp   rbx , 300                       ;  Vertical scroll 300-319
    jb    no_vertical_scroll
    cmp   rbx , 319
    ja    no_vertical_scroll
    mov  [vscroll_value], rbx
    call  draw_vertical_scroll
    jmp   still
  no_vertical_scroll:
                                          
    cmp   rbx , 0x10000001                ; Terminate button
    jne   no_application_terminate_button
    mov   rax , 512
    int   0x60
  no_application_terminate_button:

    cmp   rbx , 0x106                     ; Menu
    jne   no_application_terminate_menu
    mov   rax , 0x200
    int   0x60
  no_application_terminate_menu:

    cmp   rbx , 20                        ; Clickme button  
    jne   no_clickme
    mov   rax , 111 
    mov   rbx , 1
    int   0x60
    mov   rcx , rax 
    mov   rax , 9  
    mov   rbx , 2
    mov   rdx , image_end
    mov   r8  , 1024
    int   0x60
    mov   rax , 256
    mov   rbx , image_end+408 ; path/name
    mov   rcx , 0
    int   0x60
    jmp   still
  no_clickme:

    jmp   still


draw_window:

    mov   rax , 0xC                       ; Beginning of window draw
    mov   rbx , 0x1
    int   0x60

    ; Window position

    mov   rax , 3
    mov   rbx , 1
    int   0x60
    shr   rax , 16
    imul  rax , 4
    shl   rax , 32
    mov   rbx , rax
    mov   rcx , rax

    mov   rax , 0x0                       ; Draw window
    add   rbx , 0x116                     
    add   rcx , 0xDB
    mov   rdx , 0xffffff        
    mov   r8  , 0x1        
    mov   r9  , window_label              
    mov   r10 , menu_struct               
    int   0x60

    ; Define button

    mov   rax , 8 
    mov   rbx ,  20 * 0x100000000 + 65
    mov   rcx , 110 * 0x100000000 + 20
    mov   rdx , 20  
    mov   r8  , 0
    mov   r9  , button_text
    int   0x60

    ; Vertical scroll

    call  draw_vertical_scroll

    mov   rax , 0xc
    mov   rbx , 2
    int   0x60

    ret


draw_vertical_scroll:

    ; Vertical scroll

    mov   rax , 113
    mov   rbx , 1
    mov   rcx , 300
    mov   rdx , 20
    mov   r8  ,[vscroll_value]
    mov   r9  , 250
    mov   r10 , 50
    mov   r11 , 150
    int   0x60

    ret


; Data area

window_label:              ; Window label

    db    'EXAMPLE',0     

button_text:               ; Button text

    db    'CLICK ME',0

vscroll_value:             ; Scroll value

    dq    305        

menu_struct:               ; Menu Struct

    dq   0                 ; Version

    dq   0x100             ; Start value of ID to return ( ID + Line )

                           ; Returned when menu closes and
                           ; user made no selections.

    db   0,'FILE',0        ; ID = 0x100 + 1
    db   1,'New',0         ; ID = 0x100 + 2
    db   1,'Open..',0      ; ID = 0x100 + 3
    db   1,'Save..',0      ; ID = 0x100 + 4
    db   1,'-',0           ; ID = 0x100 + 5
    db   1,'Quit',0        ; ID = 0x100 + 6

    db   0,'HELP',0        ; ID = 0x100 + 7
    db   1,'Contents..',0  ; ID = 0x100 + 8
    db   1,'About..',0     ; ID = 0x100 + 9

    db   255               ; End of Menu Struct

image_end:






