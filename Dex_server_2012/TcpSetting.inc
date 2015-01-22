;====================================================;
; Set Address.                                       ;
;====================================================;
SetAddress:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax

	;  read in the status
	mov   eax,53
	mov   ebx,255
	mov   ecx,6
	int   0x53

	; if eax is zero, no TCP/IP stack loaded
	cmp   eax,0
	je    SetAddressError

	; Set Client IP
	mov   eax,52	    
	mov   ebx,3
	mov   ecx,dword[IpAddress]
	mov   [dhcpClientIP],ecx
	int   0x52

	; subnet mask
	mov   eax,52
	mov   ebx,12
	mov   ecx,dword[IpSubnetMaskAddress]
	mov   [dhcpClientSubNet],ecx
	int   0x52

	; dns ip
	mov   eax,52
	mov   ebx,14
	mov   ecx,dword[IpDNSAddress]
	mov   [dhcpClientDnsIp],ecx
	int   0x52

	; gateway ip
	mov   eax,52
	mov   ebx,11
	mov   ecx,dword[IpGatewayAddress]
	mov   [dhcpClientGatewayIp],ecx
	int   0x52

	pop   es
	pop   es
	popad
	clc
	ret

SetAddressError:
	pop   es
	pop   es
	popad
	stc
	ret

;====================================================;
; GetStackSettings.                                  ;
;====================================================;
GetStackSettings:
	; Set Client IP
	mov  eax,52
	mov  ebx,1
	int   0x52
	mov  dword [ip_address],eax
	; gateway ip
	mov  eax,52
	mov  ebx,9
	int   0x52
	mov  dword [gateway_ip],eax
	; subnet mask
	mov  eax,52
	mov  ebx,10
	int   0x52
	mov  dword [subnet_mask],eax
	; dns ip
	mov  eax,52
	mov  ebx,13
	int   0x52
	mov   dword [dns_ip],eax
	ret

;====================================================;
; IP Address.                                        ;
;====================================================;
IPAddress:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   [IPbuffer1],edi
	mov   [LetterCount],cx
	mov   esi,[IPbuffer1]
	sub   esi,1
	mov   edi,IpAddress
	xor   eax,eax
	push  edi
	stosd
	pop   edi
 ;----------------------------------------------------;
 ; Convert it.                                        ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	xor   edx,edx
ip1:
	inc   esi
	cmp   [ds:esi],byte '0'
	jb    ip2
	cmp   [ds:esi],byte '9'
	jg    ip2
	imul  eax,10
	movzx ebx,byte [ds:esi]
	sub   ebx,48
	add   eax,ebx
	jmp   ip1
ip2:
	mov   [es:edi],al
	xor   eax,eax
	inc   edi
	cmp   edi,IpAddress+3
	jbe   ip1
	pop   es
	pop   es
	popad
	ret

;====================================================;
; Gateway IP.                                        ;
;====================================================;
GatewayIP:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   [IPbuffer2],edi
	mov   [LetterCount],cx
	mov   esi,[IPbuffer2]
	sub   esi,1
	mov   edi,IpGatewayAddress
	xor   eax,eax
	push  edi
	stosd
	pop   edi
 ;----------------------------------------------------;
 ; Convert it.                                        ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	xor   edx,edx
ip1a:
	inc   esi
	cmp   [ds:esi],byte '0'
	jb    ip2a
	cmp   [ds:esi],byte '9'
	jg    ip2a
	imul  eax,10
	movzx ebx,byte [ds:esi]
	sub   ebx,48
	add   eax,ebx
	jmp   ip1a
ip2a:
	mov   [es:edi],al
	xor   eax,eax
	inc   edi
	cmp   edi,IpGatewayAddress+3
	jbe   ip1a
	pop   es
	pop   es
	popad
	ret

;====================================================;
; Subnet Mask.                                       ;
;====================================================;
SubnetMask:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   [IPbuffer3],edi
	mov   [LetterCount],cx
	mov   esi,[IPbuffer3]
	sub   esi,1
	mov   edi,IpSubnetMaskAddress
	xor   eax,eax
	push  edi
	stosd
	pop   edi
 ;----------------------------------------------------;
 ; Convert it.                                        ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	xor   edx,edx
ip1b:
	inc   esi
	cmp   [ds:esi],byte '0'
	jb    ip2b
	cmp   [ds:esi],byte '9'
	jg    ip2b
	imul  eax,10
	movzx ebx,byte [ds:esi]
	sub   ebx,48
	add   eax,ebx
	jmp   ip1b
ip2b:
	mov   [es:edi],al
	xor   eax,eax
	inc   edi
	cmp   edi,IpSubnetMaskAddress+3
	jbe   ip1b
	pop   es
	pop   es
	popad
	ret

;====================================================;
; DNS IP.                                            ;
;====================================================;
DNSIP:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   [IPbuffer4],edi
	mov   [LetterCount],cx
	mov   esi,[IPbuffer4]
	sub   esi,1
	mov   edi,IpDNSAddress
	xor   eax,eax
	push  edi
	stosd
	pop   edi
 ;----------------------------------------------------;
 ; Convert it.                                        ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	xor   edx,edx
ip1c:
	inc   esi
	cmp   [ds:esi],byte '0'
	jb    ip2c
	cmp   [ds:esi],byte '9'
	jg    ip2c
	imul  eax,10
	movzx ebx,byte [ds:esi]
	sub   ebx,48
	add   eax,ebx
	jmp   ip1c
ip2c:
	mov   [es:edi],al
	xor   eax,eax
	inc   edi
	cmp   edi,IpDNSAddress+3
	jbe   ip1c
	pop   es
	pop   es
	popad
	ret

;====================================================;
; Data.                                              ;
;====================================================;
ip_address	     dd  0
gateway_ip	     dd  0
subnet_mask	     dd  0
dns_ip		     dd  0

;IPAddress
IPbuffer1	     dd  0
LetterCount	     dw  0
IpAddress	     db  0,0,0,0
JustIncase1	     rb  128
;IPGatewayAddress
IPbuffer2	     dd  0
IpGatewayAddress     db  0,0,0,0
JustIncase2	     rb  128
;IPSubnetMaskAddress
IPbuffer3	     dd  0
IpSubnetMaskAddress  db  0,0,0,0
JustIncase3	     rb  128
;IPDNSAddress
IPbuffer4	     dd  0
IpDNSAddress	     db  0,0,0,0
JustIncase4	     rb  128