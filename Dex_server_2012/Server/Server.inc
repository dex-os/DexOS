;=========================================================;
; Server and Stat's                            05/05/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
;====================================================;
; Win_Netstat.                                       ;
;====================================================;
Win_ServerStats:
	pushad
	push  es
	push  ds
	mov   [NumbersVar],0
	mov   [NumbersVarB],0
	mov   [NumbersVar4],0
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Shadow.                                       ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,8 ;12
	mov   ecx,20
loopabShadowServerSt:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_abShadowStatsSt:
	mov   [es:edi],al
	add   edi,2
	loop  rev_abShadowStatsSt
	pop   ecx
	loop  loopabShadowServerSt
	cld
 ;---------------------;
 ; Draw stat Window.   ;
 ;---------------------;
DrawWinStat:
	mov   esi,ServerStats+1
	xor   ebx,ebx
	mov   bx,4 ;8
	mov   ecx,20
loopServerStats2:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[ServerStats]
loopServerStats1:
	movsb
	stosb
	loop  loopServerStats1
	pop   ecx
	loop  loopServerStats2
	call  ServerStatus
	call  StartTime
	mov   ax,18h
	mov   es,ax
 ;----------------------------------------------------;
 ; Status.                                            ;
 ;----------------------------------------------------;
	call  Print_Status
	mov   eax,[return]
	mov   [eax],esp
	mov   esp,0x900000

	; Read the configuartion word, is the normal function
	; for this int, but in DexOS it also gets the pointer
	; for the main stack loop,which is returned in EAX.
	xor   ebx,ebx
	int   52h
	mov   dword[stack_handler],ebx

	; get mac address
	mov   ebx,15
	int   52h
    
	mov   esi,eax
	mov   edi,Mac1
	mov   ecx,6
	cld
	rep   movsb

	; Now that we know we have a ethernet driver and stack, we can settup our server
	; First, open socket
ops:
	mov   eax,53
	mov   ebx,5
	mov   ecx,80		; local port # - http
	mov   edx,0		; no remote port specified
	mov   esi,0		; no remote ip specified
	mov   edi,0		; PASSIVE open
	int   0x53
	cmp   eax,0xFFFFFFFF
	je    ExitError
	mov   [socketNum], eax

 ;----------------------------------------------------;
 ; Get 1 second timer.                                ;
 ;----------------------------------------------------;
	mov   al,0x0
	out   0x70,al
	in    al,0x71
	mov   [last1SecondTick],al
	jmp   StatMainLoop2
 ;----------------------------------------------------;
 ; Fast loops.                                        ;
 ;----------------------------------------------------;
StatMainLoop1:
	call  TimeOutTest
	jc    Not4b
	call  [stack_handler]
	call  check_status
	cmp   [status],4
	je    Its4a
	cmp   [status],11
	je    Not4b
	cmp   [status],0
	jne   Not4or11or0a
Not4b:
	call  close_socket
	jc    ExitError
	mov   [TimerVar],0x3bffc
	jmp   Not4or11or0a
Its4a:
	call  start_transmission
	jc    ExitError
 ;----------------------------------------------------;
 ; Status.                                            ;
 ;----------------------------------------------------;
	call  Print_Status
	mov   [TimerVar],0x3bffc
Not4or11or0a:
@@:
	call  [KeyPressedNoWait]
	cmp   al,1
	je    letsgoS
	cmp   [TimerVar],1
	jbe   StatMainLoop2
	dec   [TimerVar]
	jmp   StatMainLoop1
 ;----------------------------------------------------;
 ; Slow loops.                                        ;
 ;----------------------------------------------------;
StatMainLoop2:
	call  TimeOutTest
	jc    Its11
	push  eax
 ;----------------------------------------------------;
 ; Current Time                                       ;
 ;----------------------------------------------------;
	call  ServerStatus
	call  CurrentTime
	pop   eax
	hlt
	call  [stack_handler]
	call  check_status
	cmp   [status],4
	je    Its4
	cmp   [status],11
	je    Its11
	cmp   [status],0
	jne   Not4
Its11:
	call  close_socket
	jc    ExitError
	mov   [TimerVar],0x3bffc
	jmp   @b
Its4:
	call  start_transmission
	jc    ExitError

 ;----------------------------------------------------;
 ; Status.                                            ;
 ;----------------------------------------------------;
	call  Print_Status
	mov   [TimerVar],0x3bffc
	jmp   @b
Not4:
@@:
	call  [KeyPressedNoWait]
	cmp   al,1
	je    letsgoS
	jmp   StatMainLoop2
 ;----------------------------------------------------;
 ; Exits.                                             ;
 ;----------------------------------------------------;
letsgoS:
	call  [WaitForKeyPress]
	cmp   al,27 
	mov   [TimerVar],0 
	jne   StatMainLoop2
CloseE:
	; close socket
	mov   eax,53
	mov   ebx,8
	mov   ecx,[socketNum]
	int   0x53
	mov   ax,4
	call  [SetDelay]
	mov   eax,53
	mov   ebx,8
	mov   ecx,[socketNum]
	int   0x53
@@:
	call  [stack_handler]
CloseNoDriver:
 ;----------------------------------------------------;
 ;  Close no driver.                                  ;
 ;----------------------------------------------------;
	mov   eax,[return]
	mov   esp,[eax]
	pop   ds
	pop   es
	popad
	ret

letsgoServerStats:
	pop   ds
	pop   es
	popad
	ret

ExitError:
	mov  byte [es:0xB809C], "E"
ExitErrorLoop:
	call  [stack_handler]
	mov   eax,53
	mov   ecx,[socketNum]
	mov   ebx,8
	int   0x53
	call  [stack_handler]
	mov   eax,53
	mov   ebx,5
	mov   ecx,80		; local port # - http
	mov   edx,0		; no remote port specified
	mov   esi,0		; no remote ip specified
	mov   edi,0		; PASSIVE open
	int   0x53
	cmp   eax,0xFFFFFFFF
	je    ExitErrorLoop
	mov   [socketNum], eax
	mov   byte [es:0xB809C], " "
	jmp   StatMainLoop2

 ;----------------------------------------------------;
 ; start_transmission                                 ;
 ;----------------------------------------------------;
start_transmission:
	pushad
;trysomemore:
	mov   [retries],18
wait_for_dataS:
	call  [stack_handler]
	call  check_for_incoming_data
	mov   eax,Header
	cmp   [eax],dword 'GET '
	je    data_received
	;cmp  [eax],dword 'POST'
	;je   data_received
	hlt
	dec   [retries]
	jnz   wait_for_dataS
	jmp   no_http_request
data_received:
	call  [stack_handler]
	call  ReadFile
        jc    WeHave404
	call  [stack_handler]
	call  wait_for_empty_slot
	call  send_header
  WeHave404:
	call  [stack_handler]
	mov   esi,dword[FileAddress]
	mov   [filepos],esi
	mov   [fileadd],700
  newblock:
	call  [stack_handler]
	call  wait_for_empty_slot
	mov   edx,[fileadd]
	cmp   edx,[file_left]
	jbe   file_size_ok
	mov   edx,[file_left]
file_size_ok:
	sub  [file_left],edx
	mov  dword[Bytes2Write],edx
 ;----------------------------------------------------;
 ; write to socket.                                   ;
 ;----------------------------------------------------;
WriteSocketTryAgain:
	mov   eax,53
	mov   ebx,7
	mov   ecx,[socketNum]
	mov   esi,[filepos]
	int   0x53
 ;----------------------------------------------------;
 ; Any write to socket errors ?.                      ;
 ;----------------------------------------------------;
	cmp   eax,0xFFFFFFFF
	jne   WriteSocketOK
	call  [stack_handler]
	mov   edx,dword[Bytes2Write]
	jmp   WriteSocketTryAgain
 ;----------------------------------------------------;
 ; Write to socket OK.                                ;
 ;----------------------------------------------------;
WriteSocketOK:
	call  [stack_handler]
	mov   edx,[fileadd]
	add   [filepos],edx
	cmp   [file_left],0
	jg    newblock
 ;----------------------------------------------------;
 ; Exit.                                              ;
 ;----------------------------------------------------;
no_http_request:
	call  close_socket
	jc    start_transmissionError
	popad
	clc
	ret
 ;----------------------------------------------------;
 ; Exit Error.                                        ;
 ;----------------------------------------------------;
start_transmissionError:
	popad
	stc
	ret

 ;----------------------------------------------------;
 ; wait_for_empty_slot                                ;
 ;----------------------------------------------------;
wait_for_empty_slot:
	pushad
wait_more:
	call  [stack_handler]
	mov  eax,53
	mov  ebx,255
	mov  ecx,103
	int  0x53
	cmp  eax,0
	je   no_wait_more
	mov  byte [es:0xB809C], "S"
	jmp  wait_more
no_wait_more:
	mov   byte [es:0xB809C], " "
	popad
	ret

 ;----------------------------------------------------;
 ; check_for_incoming_data                            ;
 ;----------------------------------------------------;
check_for_incoming_data:
	pushad
	call  [stack_handler]
	mov   edi,Header
	xor   eax,eax
	mov   ecx,2048
	cld
	rep   stosd
	mov   [pos],0
	check:
	mov   eax,53
	mov   ebx,2
	mov   ecx, [socketNum]
	int   0x53

	cmp   eax,0
	je    _ret_now1

	mov   [testme],1
new_data:
	call  [stack_handler]
	mov   eax,53
	mov   ebx,2
	mov   ecx,[socketNum]
	int   0x53

	cmp   eax,0
	je   _ret

	mov   eax,53
	mov   ebx,3
	mov   ecx,[socketNum]
	int   0x53
	cmp   bl,10
	jne   no_lf
	jmp   new_data
no_lf:
	cmp   bl,20
	jb    new_data
yok:
	cld
	push  edi
	mov   edi,Header
	add   edi,[pos]
	inc   [pos]
	mov   al,bl
	stosb
	pop   edi
	jmp   new_data
_ret:
	 jmp  check
_ret_now:
	popad
	ret

_ret_now1:
	popad
	ret

PacketSize dd 0
HeaderFile dd 0
 ;----------------------------------------------------;
 ; send_header                                        ;
 ;----------------------------------------------------;
send_header:
	pushad
mov eax, [filesize]
mov ebx, 10
mov edi, inuse
call IntToStr
sub edi, inuse
mov dword [inuse+edi-1], 0x0a0d0a0d
add edi, html_header.len+3
mov dword[PacketSize],edi
mov edi,html_header
mov dword[HeaderFile],edi
send_headerTryAgain1:
	mov   eax,53
	mov   ebx,7
	mov   ecx,[socketNum]
	mov   edx,dword[PacketSize]
	mov   esi,dword[HeaderFile]
	int   0x53
	cmp   eax,0xFFFFFFFF
	jne   send_headerOK1
	call  [stack_handler]
	jmp   send_headerTryAgain1
send_headerOK1:
	call  [stack_handler]
	popad
	ret

 ;----------------------------------------------------;
 ; send_header                                        ;
 ;----------------------------------------------------;
close_socket:
	pushad
	mov   byte [es:0xB809C], "C"
	mov   eax,53
	mov   ebx,8
	mov   ecx,[socketNum]
	int   0x53
	mov   eax,53
	mov   ebx,5
	mov   ecx,80		 ; local port # - http
	mov   edx,0		 ; no remote port specified
	mov   esi,0		 ; no remote ip specified
	mov   edi,0		 ; PASSIVE open
	int   0x53
	cmp   eax,0xFFFFFFFF
	je    close_socketError
	mov   [socketNum],eax
	call  [stack_handler]
	popad
	clc
	ret

close_socketError:
	popad
	stc
	ret
 ;----------------------------------------------------;
 ; printDEC.                                          ;
 ;----------------------------------------------------;
printDEC1:
	pushad
	push  10
	pop   ecx
	push  -'0'
.l0:
	xor   edx,edx
	div   ecx
	push  edx
	test  eax,eax
	jnz   .l0
.l1:
	pop   eax
	add   al,'0'
	call  [PrintCharCursor]
	test  al,al
	jnz   .l1
	popad
	ret

 ;----------------------------------------------------;
 ; printIPits.                                        ;
 ;----------------------------------------------------;
printIPits:
	xor   eax,eax
	mov   al,byte[edx+3]
	call  printDEC1
	mov   al,byte[edx+2]
	call  printDEC1
	mov   al,byte[edx+1]
	call  printDEC1
	mov   al,byte[edx]
	call  printDEC1
	ret

 ;----------------------------------------------------;
 ; check_status.                                      ;
 ;----------------------------------------------------;
check_status:
	pushad
	mov   eax,53
	mov   ebx,6
	mov   ecx,[socketNum]
	int   0x53

	mov   [OldStatus],eax
	cmp   eax,[status]
	je    c_ret
	mov   [status],eax
	add   al,48
	mov   byte [es:0xB809C],al
c_ret:
	popad
	ret

 ;----------------------------------------------------;
 ; TimeOutTest.                                       ;
 ;----------------------------------------------------;
TimeOutTest:
	pushad
	mov   al,0x0   ;second
	out   0x70,al
	in    al,0x71
	cmp   al,[last1SecondTick]
	je    TimeOutTestCount
	mov   [last1SecondTick],al
	mov   eax,[OldStatus]
	cmp   [status],1
	je    TimeOutTestOK
	cmp   [status],eax
	jne   TimeOutTestOK
	inc   [TimeOutTestCounter]
	cmp   [TimeOutTestCounter],10
	jae   TimeOutTestError
	jmp   TimeOutTestCount
TimeOutTestOK:
	mov   [TimeOutTestCounter],0
TimeOutTestCount:
	popad
	clc
	ret

TimeOutTestError:
	mov   [TimeOutTestCounter],0
	popad
	stc
	ret

DumpStatus:
	pushad
	mov   [socketNumTest],0
	mov   [countstatus],156
	mov   ecx,16
DumpStatusLoop:
	push  ecx
	inc   [socketNumTest]
	mov   eax,53
	mov   ebx,6
	mov   ecx,[socketNumTest]
	int   0x53

	add   [countstatus],160
	add   al,48
	mov   edx, [countstatus]
	mov   byte [es:0xB8000+edx],al
	pop   ecx
	loop  DumpStatusLoop
	popad
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;

socketNumbers	      dd 0
countstatus1	      db 0
countstatus	      dd 0
socketNumTest	      dd 0
wanted_file2	      db 'dexlogo.png',0
OldInClockTickAddress dd 0
return		      dd 0
TimeOutTestCounter    db 0
last1SecondTick       db 0
OldStatus	      dd 0
filepos 	      dd 0
fileadd 	      dd 0
pos		      dd 0
retries 	      dd 0
status		      dd 0
NumbersVar	      dd 0
NumbersVarB	      dd 0
NumbersVar4	      dd 0
TimerVar	      dd 0
testme		      db 0
posy		      dd 1
posx		      dd 0
file_type	      dd 0
type_len	      dd 0

CloseSockTimerVar     dd 0
Bytes2Write	      dd 0

msg6:		 db  'Socket Error! :(',13,0
;include 'Clocktic.inc'
include 'Convert.inc'
include 'TimeDate.inc'
include 'Read.inc'
include 'Pages\WebPages.inc'
