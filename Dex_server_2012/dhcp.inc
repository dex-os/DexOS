;******************************************;
; AutoDHCP 1.1 By hidnplayr                ;
;******************************************;
TestForDHCP:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   [DhcpErrorCode],0
	; Turn cursor off
	mov   ch,0x20 
	mov   cl,0x20 
	mov   ah,1
	call  [RealModeInt10h]

	mov   eax,52		; first, enable the stack
	mov   ebx,2		; Note: for dex this is not
	mov   ecx,0x00000383	; as enable, is done on loading
	int   0x52		; Driver.

	mov   eax,53		; then, read in the status
	mov   ebx,255
	mov   ecx,6
	int   0x53

	cmp   eax,53 ;0             ; if eax is zero, no driver was found
	jne    @f			;jne cc
	mov   [DhcpErrorCode],1
	jmp   CloseErrorNoStackLoaded
@@:

	; Read the configuartion word, is the normal function
	; for this int, but in Dex4u it also gets the pointer
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

	; now that the stack is running, lets start the dhcp request
	; First, open socket
	mov   eax,53
	mov   ebx,0
	mov   ecx,68		    ; local port dhcp client
	mov   edx, 67		    ; remote port - dhcp server
	mov   esi,0xffffffff	    ; broadcast
	int   0x53

	mov   [socketNum],eax

	; Setup the first msg we will send
	mov   byte[dhcpMsgType],0x01 ; DHCP discover
	mov   dword [dhcpLease], 0xffffffff  
	mov   [dhcpClientIP],dword 0
	mov   [dhcpServerIP],dword 0

	; this is were we end up to send Request
SendRequest:
	call  buildRequest
ctr000:
	; write to socket ( send broadcast request )
	mov   eax,53
	mov   ebx,4
	mov   ecx,[socketNum]
	mov   edx,[dhcpMsgLen]
	mov   esi,dhcpMsg
	int   0x53

	; Setup the DHCP buffer to receive response

	mov   eax, dhcpMsg
	mov   [dhcpMsgLen],eax	 ; Used as a pointer to the data

	; now, we wait for data from remote
	mov   [ClockTicks],41 ;54
	mov   [BlockAddOn],0
wait_for_data:			 ; wait a sec
	call  dword[stack_handler]
	call  PrintBlock
	call  dword[stack_handler]

	; Any data in the UDP receive buffer?
	mov   eax,53
	mov   ebx,2
	mov   ecx,[socketNum]
	int   0x53

	cmp   eax,0
	jne   letsgo		; wait_for_data
	dec   [ClockTicks]
	cmp   [ClockTicks],1
	jbe   NoDHCPserverFound
	mov   ax,18
	call  [SetDelay]
	jmp   wait_for_data
letsgo:
	; we have data - this will be the response
ctr002:
	call  dword[stack_handler]
	mov   eax,53
	mov   ebx,3
	mov   ecx,[socketNum]
	int   0x53		; read byte - block (high byte)

	; Store the data in the response buffer
	mov   eax,[dhcpMsgLen]
	mov   [eax],bl
	inc   dword[dhcpMsgLen]
	mov   eax,53
	mov   ebx,2
	mov   ecx,[socketNum]
	int   0x53		; any more data?

	cmp   eax, 0
	jne   ctr002		; yes, so get it

	; depending on which msg we sent, handle the response
	; accordingly.
	; If the response is to a dhcp discover, then:
	;  1) If response is DHCP OFFER then
	;  1.1) record server IP, lease time & IP address.
	;  1.2) send a request packet
	;  2) else exit ( display error )
	; If the response is to a dhcp request, then:
	;  1) If the response is DHCP ACK then
	;  1.1) extract the DNS & subnet fields. Set them in the stack
	;  2) else exit ( display error )


	cmp   [dhcpMsgType],byte 0x01	; did we send a discover?
	je    discover
	cmp   [dhcpMsgType],byte 0x03	; did we send a request?
	je    request

	; should never get here - we only send discover or request
	jmp   CloseError

discover:
	mov   esi,msg8a
	call  parseResponse

	; Was the response an offer? It should be
	cmp   [dhcpMsgType],byte 0x02
	jne   IpError			; NO - so quit
	; send request
	mov   [dhcpMsgType],byte 0x03	; DHCP request
	jmp   SendRequest		; buildRequest

request:
	mov   esi,msg8b
	call  parseResponse

	; Was the response an ACK? It should be
	cmp   [dhcpMsgType],byte 0x05
	jne   NoDHCPserverFound 	; NO - so quit
 ;----------------------------------------------------;
 ; Set DHCP settings in stack                         ;
 ;----------------------------------------------------;
SetDHCPsettingsInStack:
 ;----------------------------------------------------;
 ; Close OK.                                          ;
 ;----------------------------------------------------;
close:
	; close socket
	mov   eax,53
	mov   ebx,1
	mov   ecx,[socketNum]
	int   0x53

	pop   es
	pop   ds
	popad
	clc
	ret

CloseErrorNoStackLoaded:
	pop   es
	pop   ds
	popad
	stc
	ret
 ;----------------------------------------------------;
 ; CloseError.                                        ;
 ;----------------------------------------------------;
CloseError:
	; close socket
	mov   eax,53
	mov   ebx,1
	mov   ecx,[socketNum]
	int   0x53

	; Turn cursor on
	mov   ch,0x0b 
	mov   cl,0x0c 
	mov   ah,1
	call  [RealModeInt10h]
	jmp   CloseErrorNoStackLoaded
	ret

 ;----------------------------------------------------;
 ; No DHCP server Found.                              ;
 ;----------------------------------------------------;
NoDHCPserverFound:
NoDHCPserverFounda:
IpError:
jmp CloseError

					
;***************************************************************************
;   Function
;      buildRequest
;
;   Description
;      Creates a DHCP request packet.
;
;***************************************************************************
buildRequest:
	; Clear dhcpMsg to all zeros
	xor   eax,eax
	mov   edi,dhcpMsg
	mov   ecx,512
	cld
	rep   stosb

	mov   edx, dhcpMsg
	mov   [edx],byte 0x01		     ; Boot request
	mov   [edx+1],byte 0x01 	     ; Ethernet
	mov   [edx+2],byte 0x06 	     ; Ethernet h/w len
	mov   [edx+4],dword 0x11223344	     ; xid
	mov   [edx+10],byte 0x80	     ; broadcast flag set
	push  eax
	mov   eax,[Mac1]
	mov   [edx+28],eax		     ; dword 0x72a0b5bd
	mov   eax,[Mac2]
	mov   [edx+32],eax		     ; dword 0x30000000
	pop   eax
	mov   [edx+236],dword 0x63538263     ; magic number

	; option DHCP msg type
	mov   [edx+240],word 0x0135
	mov   al, [dhcpMsgType]
	mov   [edx+240+2],al

	; option Lease time = infinity
	mov    [edx+240+3], word 0x0433
	mov    eax, [dhcpLease]
	mov    [edx+240+5], eax

	; option requested IP address
	mov   [edx+240+9],word 0x0432
	mov   eax,[dhcpClientIP]
	mov   [edx+240+11],eax

	; option request list
	mov   [edx+240+15], word 0x0437
	mov   [edx+240+17],dword 0x0f060301

	; Check which msg we are sending
	cmp   [dhcpMsgType],byte 0x01
	jne   br001

	; "Discover" options
	; end of options marker
	mov   [edx+240+21],byte 0xff

	mov   [dhcpMsgLen], dword 262
	jmp   br_exit

br001:
	; "Request" options

	; server IP
	mov   [edx+240+21], word 0x0436
	mov   eax, [dhcpServerIP]
	mov   [edx+240+23], eax

	; option Return MAC
	mov   [edx+240+27],word 0x073d
	mov   eax,dword[Mac1]
	mov   [edx+240+29],eax
	mov   eax,dword[Mac2]
	mov   [edx+240+33],eax

	; option end
	mov   [edx+240+35],byte 0xff
	mov   [dhcpMsgLen], dword 276
br_exit:
	ret

 ;----------------------------------------------------;
 ; ParseResponse.                                     ;
 ;----------------------------------------------------;
parseResponse:
	; Check for valid ip address
	mov   edx,dhcpMsg
	cmp   dword[edx+16],0
	je    pr_exit
	mov   edx,dhcpMsg

	pusha
	add   edx,16
       ; Set Client IP
	mov   eax,[edx]
	mov   dword[dhcpClientIP],eax
	popa

	; Scan options
	add   edx,240	    ; Point to first option
pr001:
	; Get option id
	mov   al,[edx]
	cmp   al,0xff	    ; End of options?
	je    pr_exit

	cmp   al,53	    ; Msg type is a single byte option
	jne   pr002

	mov   al,[edx+2]
	mov   [dhcpMsgType],al
	add   edx,3
	jmp   pr001	    ; Get next option

pr002:
	; All other (accepted) options are 4 bytes in length
	inc   edx
	movzx ecx,byte [edx]
	inc   edx	    ; point to data

	cmp   al,54	    ; server id
	jne   pr0021
	mov   eax,[edx]     ; All options are 4 bytes, so get it
	mov   [dhcpServerIP], eax
	jmp   pr003

pr0021:
	cmp   al,51	    ; lease
	jne   pr0022

	pusha
	cmp   dword[edx],-1
	jne   no_lease_forever
	mov   [YesForEver],1
	jmp   @f
no_lease_forever:
	mov   [YesForEver],0
	mov   eax,[edx]
	call  Mirror_eax
	mov   [LeaseTime],eax
PrintHex:   
    ;    call  [WriteHex32]
   ;     mov   esi,msg16
   ;     call  [PrintString]
@@:
	popa
	jmp   pr003
pr0022:
	cmp   al,1	    ; subnet mask
	jne   pr0023
	pusha
	mov   ecx,[edx]
	mov   dword[dhcpClientSubNet],ecx 
	popa
	jmp   pr003

pr0023:
	cmp   al,6	    ; dns ip
	jne   pr0024

	pusha
	mov   ecx,[edx]
	mov   dword[dhcpClientDnsIp],ecx 
	popa
pr0024:
	cmp   al,3	    ; gateway ip
	jne   pr003

	pusha
	mov   ecx,[edx]
	mov   dword[dhcpClientGatewayIp],ecx 
	popa
pr003:
	add   edx,ecx
	jmp   pr001
pr_exit:
	ret


 ;----------------------------------------------------;
 ; printDEC.                                          ;
 ;----------------------------------------------------;
printDEC:
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
 ; printIP.                                           ;
 ;----------------------------------------------------;
printIP:
	xor   eax,eax

	mov   al,byte[edx]
	call  printDEC

	mov   al,'.'
	call  [PrintCharCursor]

	mov   al,byte[edx+1]
	call  printDEC

	mov   al,'.'
	call  [PrintCharCursor]

	mov   al,byte[edx+2]
	call  printDEC

	mov   al,'.'
	call  [PrintCharCursor]

	mov   al,byte[edx+3]
	call  printDEC
	ret

 ;----------------------------------------------------;
 ; Mirror_eax.                                        ;
 ;----------------------------------------------------;  
Mirror_eax:
	push  edi
	push  es
	mov   ax,0x18				    
	mov   es,ax					
	mov   edi,Eax_buffer
	mov   byte[es:edi+3],al
	shr   eax,8
	mov   byte[es:edi+2],al
	shr   eax,8
	mov   byte[es:edi+1],al
	shr   eax,8
	mov   byte[es:edi+0],al
	mov   eax,dword[Eax_buffer]
	pop   es
	pop   edi
	ret

 ;----------------------------------------------------;
 ; printIP.                                           ;
 ;----------------------------------------------------;
printLease:
	pushad
	xor   eax,eax
	mov   al,byte[edx]
	call  printDEC 
	mov   al,byte[edx+1]
	call  printDEC
	mov   al,byte[edx+2]
	call  printDEC
	mov   al,byte[edx+3]
	call  printDEC
	mov   esi,msg21
	call  [PrintString_0]
	popad
	ret

PrintBlock:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ebx,20
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,48
	add   edi,[BlockAddOn]
	add   [BlockAddOn],2
	mov   ah,0xe7
	mov   al,' '
	stosw
	pop   ds
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; Set DHCP settings in stack                         ;
 ;----------------------------------------------------;
SetDhcpSettings:
	; Set Client IP
	mov   eax,52	    
	mov   ebx,3
	mov   ecx,dword[dhcpClientIP]
	int   0x52

	; subnet mask
	mov   eax,52
	mov   ebx,12
	mov   ecx,dword[dhcpClientSubNet]
	int   0x52

	; dns ip
	mov   eax,52
	mov   ebx,14
	mov   ecx,dword[dhcpClientDnsIp]
	int   0x52

	; gateway ip
	mov   eax,52
	mov   ebx,11
	mov   ecx,dword[dhcpClientGatewayIp]
	int   0x52

	ret



 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
DhcpErrorCode	 dw  0
YesForEver	 dd  0
BlockAddOn	 dd  0
Eax_buffer:	 dd  0
LeaseTime	 dd  0
LeaseHours	 dd  0
ClockTicks	 db  0
stack_handler	 dd  0
dhcpMsgType:	 db  0
dhcpLease:	 dd  0
dhcpServerIP:	 dd  0
dhcpClientIP	 dd  0
dhcpMsgLen:	 dd  0
socketNum:	 dd  0xFFFF
dhcpClientupDate dd 0
dhcpClientSubNet dd 0
dhcpClientDnsIp  dd 0
dhcpClientGatewayIp    dd 0
dhcpClientIPDefault    dd 0x2685fea9
Mac1:		 db 0,0,0,0
Mac2:		 db 0,0,0,0
dhcpMsg:	 rb  512

msg1:		db  'AutoDHCP 1.1',13,'By hidnplayr',13,13,0
msg3:		db  'No Card detected',13,0
msg6a:		db  'Looking for dhcp server:',0
msg7:		db  'Press any key to exit.',13,0
msg8a:		db  13,'[OFFER?]',0
msg8b:		db  13,'[ACK?]',0
msg8:		db  ' Data received, parcing response.',13,0
msg9:		db  ' Lease.............: ',0
msg10:		db  'forever',13,0
msg11:		db  ' IP-address........: ',0
msg12:		db  ' Subnetmask........: ',0
msg13:		db  ' StandardGateway...: ',0
msg14:		db  ' DNS-server........: ',0
msg16:		db  13,0
msg17:		db  13,13,'Timeout Error!, No DHCP Server Found',13 
msg17a: 	db  'Do you want to use default settings, Y/N ?',13,0

msg18:		db  'Do you want to use these dhcp severs settings, Y/N ?',13,13,0
msg19:		db  'IP has been set.',13,0

msg20:		db  13,'IP-address error!, not a valid value.',13 
		db  'Do you want to use default settings, Y/N ?',13,0

msg21:		db  ' Hours',13,0
