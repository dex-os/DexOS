;=========================================================;
; Isr                                          01/04/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
align 4
;=======================================================  ;
;  unhandled int..                                        ;
;=======================================================  ; 
unhandled_int:                                            ;
        pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809E], "U"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  unhandled_irq1                                         ;
;=======================================================  ; 
unhandled_irq1:                                           ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "&"                    ;
	mov     al,0x20                                   ;
	out     0xa0,al                                   ;
	out     0x20,al                                   ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  nmi interrupt.                                         ;
;=======================================================  ; 
nmi_interrupt:                                            ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809E], "N"                    ;
  reboot1:                                                ;
	jmp     reboot1                                   ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  clock tick.                                            ;
;=======================================================  ; 
clock_tick:                                               ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
  FloppyTimer:                                            ;
	test    [FddTimerOn],-1				  ; See if timer on
	jz      TimerCode				  ; no jump lable
	dec     [FddTimer]				  ; Yes, decrement
	jnz     TimerCode			          ; Not done
	and     [FddTimerOn],0				  ; finished, inform task
        call    Fdd_motor_off                             ;
  TimerCode:                                              ;
	test    [TimerOn],-1				  ; See if timer on
	jz      NoTimer					  ; no jump lable
	dec     [Timer]					  ; Yes, decrement
	jnz     NoTimer					  ; Not done
	and     [TimerOn],0				  ; finished, inform task
  NoTimer:                                                ;
	test    [TimerOnNw],-1				  ; See if floppy motor timer is on.
	jz      NoFddTimer				  ; no jump lable
	dec     [TimerNw] 				  ; Yes, decrement
	jnz     NoFddTimer				  ; Not done
	and     [TimerOnNw],0				  ; mov floppy time switch off
  NoFddTimer:                                             ;
	test    [TimerProgOn],-1			  ; See if program timer is on.
	jz      NoProgTimer				  ; no jump lable
	inc     [TimerProg]				  ; Yes, increas
	cmp     [TimerProg],0x00ffffff                    ;
	jb      NoProgTimer				  ; Not done
	and     [TimerProgOn],0				  ; Prog time switch off
  NoProgTimer:                                            ;
	call    irq_clear                                 ;
	mov     [Tick18],1                                ;
	inc     [NumberOfTicks]                           ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  keyboard irq.                                          ;
;=======================================================  ; 
keyboard_irq:                                             ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
        ;mov    byte [fs:0xB809C], "K"                    ;
	cmp     [GameKeyBoardOn],1                        ;
	jne     NoGameKeyBoard                            ;
	call    GamesKeyBoard                             ;
	jmp     ClearIRQgo                                ;
NoGameKeyBoard:                                           ;
	call    key_board                                 ;
ClearIRQgo:                                               ;
	mov     ah,[ScanCode2]                            ;
	mov     [ScanCode2],0                             ;
	call    irq_clear                                 ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  Mouse irq.                                             ;
;=======================================================  ; 
Mouse_irq:                                                ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	call    MouseMovmentClick                         ;
	mov     al,0x20                                   ;
	out     0xa0,al                                   ;
	out     0x20,al                                   ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  floppy irq.                                            ;
;=======================================================  ; 
floppy_irq:                                               ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	call    irq_clear                                 ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  page_fault.                                            ;
;=======================================================  ; 
page_fault:                                               ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "P"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  div_error.                                             ;
;=======================================================  ; 
div_error:                                                ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "D"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  debug_exception.                                       ;
;=======================================================  ; 
debug_exception:                                          ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "d"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  int3_trap.                                             ;
;=======================================================  ; 
int3_trap:                                                ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "3"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  into_trap.                                             ;
;=======================================================  ; 
into_trap:                                                ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "O"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  bound_trap.                                            ;
;=======================================================  ; 
bound_trap:                                               ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "B"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  invalid_instruction.                                   ;
;=======================================================  ; 
invalid_instruction:                                      ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "I"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  no_coprocessor.                                        ;
;=======================================================  ; 
no_coprocessor:                                           ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "n"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  double_fault.                                          ;
;=======================================================  ; 
double_fault:                                             ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "8"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  coprocessor_segment_overrun.                           ;
;=======================================================  ; 
coprocessor_segment_overrun:                              ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "9"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  invalid_tss.                                           ;
;=======================================================  ; 
invalid_tss:                                              ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "A"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  segment_not_present.                                   ;
;=======================================================  ; 
segment_not_present:                                      ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "B"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  stack_fault.                                           ;
;=======================================================  ; 
stack_fault:                                              ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "S"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  gpf.                                                   ;
;=======================================================  ; 
gpf:                                                      ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "D"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  coprocessor_error.                                     ;
;=======================================================  ; 
coprocessor_error:                                        ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "e"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  alignment_check.                                       ;
;=======================================================  ; 
alignment_check:                                          ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "F"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  machine_check.                                         ;
;=======================================================  ; 
machine_check:                                            ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "G"                    ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  Dos int 21h functions                                  ;
;=======================================================  ; 
Dos_Int_21h:                                              ;
	pushad                                            ;
	push	es                                        ;
	push	ds                                        ;

	pop	ds                                        ;
	pop	es	                                  ;
	popad                                             ;
	iret                                              ;
align 4

;=======================================================  ;
;  int_50h                         ( int functions)       ;
;=======================================================  ; 
int_50h:                                                  ;   
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	cmp     ah,0ah					  ; Test for 0ah = "Load CallTable Address"
	jne     ErrorInt50h                               ;
  LoadCallTableAddress:                                   ;
	mov     esi,CallTable                             ;
	mov     ecx,256*2                                 ;
	cmp     al,0                                      ;
	je      FillTable                                 ;
	mov     ecx,1                                     ;
  FillTable:                                              ;
	cld                                               ;
	rep     movsd                                     ;
  sucsess40h:                                             ;
	pop     ds				    	  ; universal return procedure
	pop     es                                        ;
	popad                                             ;
	and	byte [esp+8],not 1			  ; clear CF
	iret                                              ;
;=======================================================  ;
;  int_50h                         ( int functions)       ;
;=======================================================  ; 
ErrorInt50h:                                              ;
	pop     ds					  ; universal return error procedure
	pop     es                                        ;
	popad                                             ;
	or	byte [esp+8],1				  ; set CF
	iret                                              ;
align 4
;=======================================================  ;
;  TestInt                                                ;
;=======================================================  ; 
TestInt:                                                  ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
 ;-----------------                                       ;
 ; some code here                                         ;
 ;-----------------                                       ;   
	pop     ds                                        ;
	pop     es                                        ; 
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  unhandled_irq                                          ;
;=======================================================  ; 
unhandled_irq:                                            ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     byte [fs:0xB809C], "Q"                    ;
	mov     al,0x20                                   ;
	out     0xa0,al                                   ;
	out     0x20,al                                   ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	iret                                              ;
align 4
;=======================================================  ;
;  irq_clear                                              ;
;=======================================================  ; 
irq_clear:                                                ;
	push    eax                                       ;
	mov     al,0x20                                   ;
	out     0xa0,al                                   ;
	out     0x20,al				          ; quiet screaming irq chip.
	pop     eax                                       ;
	ret                                               ;
align 4
;=======================================================  ;
;  data                                                   ;
;=======================================================  ;
NumberOfTicks dd 0                                        ;
Tick18	      db 0                                        ;                                    
testthis      dd 0                                        ;
EsiPointer    dd 0                                        ;
TimerProg     dd 0                                        ;
TimerProgOn   db 0                                        ;
