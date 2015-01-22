;=========================================================;
; pic                                          01/04/2011 ;
;---------------------------------------------------------;
; (c) Craig Bamford, All rights reserved.                 ;
;                                                         ;
; Remapping the pics.                                     ;                          
;=========================================================;
;=======================================================  ;
;  remap_pic                                              ;
;=======================================================  ; 
 remap_pic:                                               ;
	cli                                               ;
	mov     al,0x11					  ; put both 8259s in init mode
	out     0x20,al                                   ;
	out     0xA0,al                                   ;
	mov     al,0x60                                   ; remap pic irq0-irq7 -> int0x60-0x67  
	out     0x21,al                                   ;
	mov     al,0x68                                   ; remap pic irq8-irq15 -> int int0x68-0x6F   
	out     0xA1,al					  ; 
	mov     al,4					  ; icw3 pic1(master)
	out     0x21,al					  ; bit 2=1: irq2 is the slave
	mov     al,2					  ; icw3 pic2
	out     0xA1,al					  ; bit 1=1: slave id is 2
	mov     al,1					  ; icw4 to both controllers
	out     0x21,al					  ; bit 0=1: 8086 mode
	out     0xA1,al                                   ;
;=======================================================  ;
;  mask_irqs                                              ;
;=======================================================  ; 
  mask_irqs:	                                          ;
	cli                                               ;
	mov     al,255					  ; mask all irqs
	out     0xa1,al                                   ;
	out     0x21,al                                   ;
	ret                                               ;

;=======================================================  ;
; unmask_irqs                                             ;
;=======================================================  ; 
unmask_irqs:                                              ;
	mov     al,0					  ; unmask irq's 
	out     0xA1,al                                   ;
	out     0x21,al                                   ;
	sti                                               ;
	ret                                               ;

;=======================================================  ;
; remap_pic_realmode                                      ;
;=======================================================  ; 
 remap_pic_realmode:                                      ;
	cli                                               ;
	mov     al,11h                                    ;
	out     0x20,al                                   ;
	out     0xA0,al                                   ;
	mov     al,0x08                                   ;
	out     0x21,al                                   ;
	mov     al,0x70                                   ;
	out     0xA1,al                                   ;
	mov     al,4                                      ;
	out     0x21,al                                   ;
	mov     al,2                                      ;
	out     0xA1,al                                   ;
	mov     al,1                                      ;
	out     0x21,al                                   ;
	out     0xA1,al                                   ;
;=======================================================  ;
; mask all irqs                                           ;
;=======================================================  ; 
	mov     al,255					  ; mask all irqs
	out     0xa1,al                                   ;
	out     0x21,al                                   ;
	ret                                               ;

;=======================================================  ;
; SetIntVector                                            ;
;=======================================================  ; 
SetIntVector:                                             ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	push    ecx                                       ;
	mov     cx,18h                                    ;
	mov     ds,cx                                     ;
	pop     ecx                                       ;
	movzx   eax,al                                    ;
	shl     eax,3                                     ;
	add     eax,idt                                   ;
	cli					          ; if  selector is needed
       ;mov     word [ds:eax+2],cx                        ;
	mov     word [ds:eax],dx                          ;
	shr     edx,16                                    ;
	mov     word [ds:eax+6],dx                        ;
	sti                                               ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	clc                                               ;
	ret                                               ;

;=======================================================  ;
; GetIntVector                                            ;
;=======================================================  ; 
GetIntVector:                                             ;
	pushad                                            ;
	push    es                                        ;
	push    ds                                        ;
	mov     cx,18h                                    ;
	mov     ds,cx                                     ;
	movzx   eax,al                                    ;
	shl     eax,3                                     ;
	add     eax,idt                                   ;
	cli                                               ;
	mov     cx,word [ds:eax+2]                        ;
	mov     [CXbackup],cx                             ;
	mov     dx,word [ds:eax+6]                        ;
	shl     edx,16                                    ;
	mov     dx,word [ds:eax]                          ;
	mov     [EBXbackup],edx                           ;
	sti                                               ;
	pop     ds                                        ;
	pop     es                                        ;
	popad                                             ;
	mov	edx,dword[EBXbackup]                      ;	                 
        mov     cx,[CXbackup]                             ;
	clc                                               ;
	ret                                               ;

;=======================================================  ;
; data                                                    ;
;=======================================================  ; 
CXbackup	     dw 0                                 ;
EBXbackup	     dd 0                                 ;
