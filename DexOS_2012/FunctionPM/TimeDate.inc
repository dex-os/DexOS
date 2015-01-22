;=========================================================;
; TimeDate                                     01/04/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;=======================================================  ;
; date.                                                   ;
;-------------------------------------------------------  ;
;                                                         ;                                                    ;
;   Input:                                                ;
;          none.                                          ;
;  Output:                                                ;
;          Puts bcd date in vals:                         ;
;          century, year, month, day.                     ;
;                                                         ;
;=======================================================  ;
date:							  ;
	push	ds es					  ;
	pusha						  ;
	cli						  ;
	mov	al,32h					  ; RTC 32h
	out	70h,al					  ;
	in	al,71h					  ; read century
	mov	[century],al				  ;
	mov	al,9					  ; RTC 09h
	out	70h,al					  ;
	in	al,71h					  ; read year
	mov	[year],al				  ;
	mov	al,8					  ; RTC 08h
	out	70h,al					  ;
	in	al,71h					  ; read month
	mov	[month],al				  ;
	mov	al,7					  ; RTC 07h
	out	70h,al					  ;
	in	al,71h					  ; read day
	mov	[day],al				  ;
	sti						  ;
	popa						  ;
	pop	es ds					  ;
	ret						  ;

;=======================================================  ;
;  FloppyDate.                                            ;
;=======================================================  ; 
FloppyDate:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;                     
	mov	es,ax					  ;
	call	date					  ;
	xor	bx,bx					  ;
	mov	dx,bx					  ;
	mov	ax,bx					  ;
	mov	al,[year]				  ;
	call	Bcd2Bin 				  ;
	add	al,20					  ;
	mov	dx,ax					  ;
	shl	dx,4					  ;
	mov	al,[month]				  ;
	call	Bcd2Bin 				  ;
	and	al,00001111b				  ;
	add	dl,al					  ;
	shl	dx,5					  ;
	mov	al,[day]				  ;
	call	Bcd2Bin 				  ;
	and	ax,0000000000011111b			  ;
	add	dx,ax					  ;
	mov	[FddDate],dx				  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ax,[FddDate]				  ;
	ret						  ;

;=======================================================  ;
;  FloppyTime.                                            ;
;=======================================================  ; 
FloppyTime:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;                     
	mov	es,ax					  ;
	call	time					  ;
	xor	bx,bx					  ;
	mov	dx,bx					  ;
	mov	ax,bx					  ;
	mov	al,[hour]				  ;
	call	Bcd2Bin 				  ;
	mov	dx,ax					  ;
	shl	dx,6					  ;
	mov	al,[minute]				  ;
	call	Bcd2Bin 				  ;
	and	al,00111111b				  ;
	add	dl,al					  ;
	shl	dx,5					  ;
	mov	al,[second]				  ;
	shr	al,1					  ;
	call	Bcd2Bin 				  ;
	and	ax,0000000000011111b			  ;
	add	dx,ax					  ;
	mov	[FddTime],dx				  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ax,[FddTime]				  ;
	ret						  ;


;=======================================================  ;
; time                                                    ;
;-------------------------------------------------------  ;
;                                                         ;
;   Input:                                                ;
;          none.                                          ;
;  Output:                                                ;
;          Puts bcd time in vals:                         ;
;          hour, minute, second.                          ;
;                                                         ;
;=======================================================  ;
time:							  ;
	push	ds es					  ;
	pusha						  ;
	cli						  ;
	mov	al,4					  ; RTC  04h
	out	70h,al					  ;
	in	al,71h					  ; read hour
	mov	[hour],al				  ;
	mov	al,2					  ; RTC  02h
	out	70h,al					  ;
	in	al,71h					  ; read minute
	mov	[minute],al				  ;
	xor	al,al					  ; RTC  00h
	out	70h,al					  ;
	in	al,71h					  ; read second
	mov	[second],al				  ;
	sti						  ;
	popa						  ;
	pop	es ds					  ;
	ret						  ;


;=======================================================  ;
; set_time                                                ;
;-------------------------------------------------------  ;
;                                                         ;
;   Input:                                                ;
;         Bcd time in vals.                               ;
;  Output:                                                ;
;         Prints ERROR msg on error.                      ;
;                                                         ;
;  set's time from vals                                   ;
;=======================================================  ;       
set_time:						  ;                     
	push	ds es					  ;
	pusha						  ;
	mov	bl,59h					  ;
	mov	al,[second]				  ;
	call	check_bcd_time				  ; check second number
	jc	set_time_error				  ;
	mov	al,[minute]				  ;
	call	check_bcd_time				  ; check minute number
	jc	set_time_error				  ;
	mov	bl,23h					  ;
	mov	al,[hour]				  ;
	call	check_bcd_time				  ; check hour number
	jc	set_time_error				  ;
	cli						  ;
	xor	al,al					  ; RTC 00h
	out	70h,al					  ;
	mov	al,[second]				  ;
	out	71h,al					  ; write second
	mov	al,2					  ; RTC 02h
	out	70h,al					  ;
	mov	al,[minute]				  ;
	out	71h,al					  ; write minute
	mov	al,4					  ; RTC 04h
	out	70h,al					  ;
	mov	al,[hour]				  ;
	out	71h,al					  ; write hour
	jmp	leave1					  ;
  leave1:						  ;
	sti						  ;
	popa						  ;
	pop	es ds					  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
; set_time_error.                                         ;
;=======================================================  ; 
set_time_error: 					  ;
	popa						  ;
	pop	es ds					  ;
	stc						  ;
	ret						  ;

;=======================================================  ;
; set date                                                ;
;-------------------------------------------------------  ;
;                                                         ;
;   Input:                                                ;
;         Bcd date in vals.                               ;
;  Output:                                                ;
;         Prints ERROR msg on error.                      ;
;                                                         ;
;=======================================================  ;              
set_date:						  ;
	push	ds es					  ;
	pusha						  ;
	mov	al,[day]				  ;
	mov	bl,31h					  ;
	or	al,al					  ; day cannot be 0
	jz	Set_date_error				  ;
	call	check_bcd_date				  ; check day number
	jc	Set_date_error				  ;
	mov	bl,12h					  ;
	mov	al,[month]				  ;
	or	al,al					  ; month cannot be 0
	jz	date_error				  ;
	call	check_bcd_date				  ; check month number
	jc	Set_date_error				  ;
	mov	bl,99h					  ;
	mov	al,[year]				  ;
	call	check_bcd_date				  ; check year number
	jc	Set_date_error				  ;
	mov	al,[century]				  ;
	call	check_bcd_date				  ; check century number
	jc	Set_date_error				  ;
	cli						  ;
	mov	al,7					  ; RTC 07h
	out	70h,al					  ;
	mov	al,[day]				  ;
	out	71h,al					  ; write day
	mov	al,8					  ; RTC 08h
	out	70h,al					  ;
	mov	al,[month]				  ;
	out	71h,al					  ; write month
	mov	al,9					  ; RTC 09h
	out	70h,al					  ;
	mov	al,[year]				  ;
	out	71h,al					  ; write year
	mov	al,32h					  ; RTC 32h
	out	70h,al					  ;
	mov	al,[century]				  ;
	out	71h,al					  ; write century
	jmp	leave2					  ;
  leave2:						  ;
	sti						  ;
	popa						  ;
	pop	es ds					  ;
	clc						  ;
	ret
;=======================================================  ;
; set_date_error.                                         ;
;=======================================================  ; 
Set_date_error: 					  ;
	popa						  ;
	pop	es ds					  ;
	stc						  ;
	ret						  ;

;=======================================================  ;
; bcd_to_ASCII                                            ;
;-------------------------------------------------------  ;
;                                                         ;
;   Input:                                                ;
;         Bcd number in al.                               ;
;  Output:                                                ;
;         ASCII number in val.                            ;
;                                                         ;
;  converts bcd number to ASCII                           ;
;=======================================================  ;          
bcd_to_ASCII:						  ;
       pusha						  ; start of convert
       mov     ah,al					  ; copy AL to AH
       and     ax,0f00fh				  ; mask bits
       mov     cl,4					  ; CL=04 for shift
       shr     ah,cl					  ; shift right AH to get unpacked BCD
       or      ax, 3030h				  ; combine with 30 to get ASCII
       xchg    ah,al					  ; swap for ASCII storage convention
       mov     [val],ax 				  ; store the ASCII value in VAL
       popa						  ;
       ret						  ;

;=======================================================  ;
; checks it a bcd number date                             ;
;=======================================================  ; 
check_bcd_date: 					  ;
	cmp	al,bl					  ;
	ja	date_error				  ;
	and	al,0Fh					  ;
	cmp	al,9					  ;
	ja	date_error				  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
; date_error                                              ;
;=======================================================  ;
date_error:						  ;                   
	stc						  ;
	ret						  ;

;=======================================================  ;
; checks it a bcd number time                             ;
;=======================================================  ; 
check_bcd_time: 					  ;
	cmp	al,bl					  ;
	ja	time_error				  ;
	and	al,0Fh					  ;
	cmp	al,9					  ;
	ja	time_error				  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
; time error.                                             ;
;=======================================================  ; 
time_error:						  ;
	stc						  ;
	ret						  ;

;=======================================================  ;
; Bcd2Bin.                                                ;
;=======================================================  ; 
Bcd2Bin:						  ;
	push	bx					  ;
	mov	bl,0x0a 				  ;
	mov	bh,al					  ;
	shr	al,4					  ;
	mul	bl					  ;
	and	bh,0x0f 				  ;
	add	al,bh					  ;
	pop	bx					  ;
	ret						  ;

;=======================================================  ;
; Bcd2Bin.                                                ;
;=======================================================  ; 
print_time:                                               ;
	push    es                                        ;
	pusha                                             ;
	mov     esi,time_lable                            ;
	call    print_string                              ;
	call    time                                      ;
	mov     al,[hour]                                 ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	mov     al,":"                                    ;
	call    print_char                                ;
	mov     al,[minute]                               ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	mov     al,":"                                    ;
	call    print_char                                ;
	mov     al,[second]                               ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	popa                                              ;
	pop     es                                        ;
	ret                                               ;

;=======================================================  ;
; print_date                                              ;
;=======================================================  ; 
print_date:                                               ;
	push    es                                        ;
	pusha                                             ;
	mov     esi,date_lable                            ;
	call    print_string                              ;
	call    date                                      ;
	mov     al,[day]                                  ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	mov     al,":"                                    ;
	call    print_char                                ;
	mov     al,[month]                                ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	mov     al,":"                                    ;
	call    print_char                                ;
	mov     al,[century]                              ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	mov     al,[year]                                 ;
	call    bcd_to_ASCII                              ;
	mov     bx,[val]                                  ;
	mov     al,bl                                     ;
	call    print_char                                ;
	mov     al,bh                                     ;
	call    print_char                                ;
	popa                                              ;
	pop     es                                        ;
	ret                                               ;

;=======================================================  ;
; string.                                                 ;
;=======================================================  ; 
time_lable:	   db " Time ",'$'                        ;
date_lable:	   db " Date ",'$'                        ;
;=======================================================  ;
; data.                                                   ;
;=======================================================  ; 
val:	   dw 0,0					  ;
hour:	   db 0 					  ;
minute:    db 0 					  ;
second:    db 0 					  ;
century:   db 0 					  ;
year:	   db 0 					  ;
month:	   db 0 					  ;
day:	   db 0 					  ;
FddDate    dw 0 					  ;
FddTime    dw 0 					  ;
