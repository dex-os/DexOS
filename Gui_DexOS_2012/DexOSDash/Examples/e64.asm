;
;   64 bit Menuet example
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

    mov   rax , 0xA         ; Wait here for event
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

    mov   rax , 0x2        ; Read the key and ignore
    int   0x60

    jmp   still

button_event:

    mov   rax , 0x11
    int   0x60

    ; rax = status
    ; rbx = button id

    cmp   rbx , 0x10000001
    jne   no_application_terminate_button
    mov   rax , 0x200
    int   0x60
  no_application_terminate_button:

    cmp   rbx , 0x106
    jne   no_application_terminate_menu
    mov   rax , 0x200
    int   0x60
  no_application_terminate_menu:

    jmp   still


draw_window:

    mov   rax , 0xC                          ; Beginning of window draw
    mov   rbx , 0x1
    int   0x60

    mov   rax , 0x0                          ; Draw window
    mov   rbx , 0x0000010000000100           ; x start & size
    mov   rcx , 0x00000080000000C0           ; y start & size
    mov   rdx , 0x0000000000FFFFFF           ; type    & border color  
    mov   r8  , 0x0000000000000001           ; draw flags
    mov   r9  , window_label                 ; 0 or label - asciiz
    mov   r10 , menu_struct                  ; 0 or pointer to menu struct
    int   0x60

    mov   rax , 0x4                          ; Display text
    mov   rbx , text
    mov   rcx , 0x20
    mov   rdx , 0x40
    mov   rsi , 0x0
    mov   r9  , 0x1
    mov   r8  , 0x3

  newline:

    int   0x60

    add   rbx , 0x1F
    add   rdx , 0x10
    dec   r8
    jnz   newline

    mov   rax , 0xC                          ; End of window draw
    mov   rbx , 0x2
    int   0x60

    ret


; Data area

window_label:

    db    'EXAMPLE',0     ; Window label

text:

    db    'HELLO WORLD FROM 64 BIT MENUET',0
    db    'Second line                   ',0
    db    'Third line                    ',0

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

