;=========================================================;
; RTL8139 Driver + stack                         04/30/08 ;
;---------------------------------------------------------;
; Ported from MenuetOS, by Dex.                           ;
;                                                         ;
; To assemble use fasm as follows                         ;
; c:\fasm RTL8139.asm RTL8139.obj                         ;
;                                                         ;
; Then use Coff2Dex to convert to driver files, eg:       ;
; Coff2Dex rtl8139.obj rtl8139.dri rtl8139.rel  <enter>   ;
;                                                         ;
;=========================================================;
format COFF
	jmp   ModRun			; jump to the start of program.
    Signature	  db	'MOD1'		; We check it's a valid Module file.
    ModID	  db	'DEXSTACK'	; This the ID of the module.
    SizeOfModule  dd	 ModEnd 	; This put at end of module.
    ModLoadPtr	  dd	 ModLoad	; This is to run code on first load of module.
    ModUnLoadPtr  dd	 ModUnLoad	; This is to run code on unload of module.
    ModNumberPtr  dd	 2		; This points to the function list.

    include 'Stack.inc'
    include 'RTL8139con.inc'
ModRun:
	pushad				; ************ [STARTUP CODE HERE] ************
	push  ds
	push  es
	mov   ax,18h
	mov   ds,ax
	mov   es,ax

	mov   edi,Functions				  
	mov   al,0					  
	mov   ah,0x0a				  
	int   50h		
 ;----------------------------------------------------;
 ; Get Base offset.                                   ;
 ;----------------------------------------------------;        
	call  [GetBaseAddOn]
	mov   [BaseAddOn],eax
 ;----------------------------------------------------;
 ; stack_init.                                        ;
 ;----------------------------------------------------;
	call  stack_init 
 ;----------------------------------------------------;
 ; test for ethernet card.                            ;
 ;----------------------------------------------------;
	mov   esi,DeviceVendorID
	mov   ecx,19			; Put the number of ID's to try here
TryNextID:
	lodsd
	call  [PciFindDevice]		; Scan PCI bus for RTL8139 card
	jnc   CardFound 		; IF no card print error and exit.
	loop  TryNextID
	jmp   CardNotFound		; IF no card print error and exit.
CardFound:
	mov   dword[PciEthAddress],eax
 ;----------------------------------------------------;
 ; setup ethernet card.                               ;
 ;----------------------------------------------------;
	call  Probe
	call  GetHwAddress		; Get the hareware address of card

	; Set int 52h

	mov   eax,52h			; put interrupt number in AL
	call  [GetIntVector]		; New function to get int vector
	mov   [OldIn52hAddress],edx	; The offset is returned in EDX, CX = selector (BUT WE DO NOT CHANGE selector)

	mov   edx,int_52h		; EDX = interrupt handler address
	mov   eax,52h			; put interrupt number in AL
	call  [SetIntVector]		; New function to set int vector

	; Set int 53h

	mov   eax,53h			; put interrupt number in AL
	call  [GetIntVector]		; New function to get int vector
	mov   [OldIn53hAddress],edx	; The offset is returned in EDX, CX = selector (BUT WE DO NOT CHANGE selector)

	mov   edx,int_53h		; EDX = interrupt handler address
	mov   eax,53h			; put interrupt number in AL
	call  [SetIntVector]		; New function to set int vector

 ;----------------------------------------------------;
 ; Print card found and exit.                         ;
 ;----------------------------------------------------; 
	mov   esi,Msg2			; this point's to our string.
	call  [PrintString_0]		  ; this call the print function.
	pop   es
	pop   ds
	popad
	ret				; This returns to the CLI/GUI

 ;----------------------------------------------------;
 ; Print card not found.                              ;
 ;----------------------------------------------------;
CardNotFound:
	mov   esi,MsgError		; this point's to our string.
	call  [PrintString_0]		  ; this call the print function.
	pop   es
	pop   ds
	popad
	ret				; This returns to the CLI/GUI


ModLoad:				; ************ [HERE IS CODE TO CALL FUNCTIONS] ************
	push  ds
	push  es
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	cmp   ebx,[ModNumberPtr]
	ja    ModError
	shl   ebx,2
	add   ebx,ModFunctions
	call  dword[ebx]
	jc    ModError
ModOK:
	pop   es
	pop   ds
	clc
	ret


ModError:
	pop   es
	pop   ds
	stc
	ret


ModUnLoad:				; ************ [UNLOAD MOD CODE HERE] ************
	ret

ServiceUnUsed:				; Unused function.
	ret

Service1:				; ************ [FIST FUNCTON CODE HERE] ************
	pushad
	mov   esi,msgService1
	;call  [PrintString]
	popad
	ret
Service2:				; ************ [SECOND FUNCTON CODE HERE] ************
	pushad
	mov   esi,msgService2
	;call  [PrintString]
	popad
	ret
 ;----------------------------------------------------;
 ; Start of code in module.                           ;
 ;----------------------------------------------------;

ModFunctions:
		  dd	 ServiceUnUsed	; Reserve the first one.
		  dd	 Service1	; Points to first function.
		  dd	 Service2	; Points to second function.

 ;----------------------------------------------------;
 ; String Data.                                       ;
 ;----------------------------------------------------;
MsgModLoadOK	  db 'Test module loaded',10,13,13,0
msgService1	  db 'Hello from Service1!',10,13,0
msgService2	  db 'Hello from Service2!',10,13,0

Msg2:		  db 'RTL8139 ethernet card, found and setup :)',10,13,0

MsgGood:	  db 'Got packet ;) ',10,13,0
Msg3:		  db 13,13,'HWaddr ',0

MsgError:	  db 'Error!, RTL8139 ethernet card not found :(.',10,13,0

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
BaseAddOn	  dd 0
TestInt 	  dd 0
io_addr 	  dd 0
HwVerID 	  db 0
PciEthAddress	  dd 0 
eth_rx_data_len1  dw 0
OldIn52hAddress   dd 0
OldIn53hAddress   dd 0
 ;----------------------------------------------------;
 ; Include files.                                     ;
 ;----------------------------------------------------;
include 'RTL8139.inc'
include 'RTL8139ID.inc'
 ;----------------------------------------------------;
 ; BSS goes here.                                     ;
 ;----------------------------------------------------;

align 4 				;-----+
Cut  db '2CUT'				; These must be here
					;-----+
include 'Dex.inc'			; Dex inc file
PackerBuffer	       rb   0x19000
align 4
eth_data_startDex:
stack_data_startDex:   rb   0x4000
align 4
stack_dataDex:	       rb   0x1ffff
align 4
stack_data_endDex:     rb   0x50001
align 4
resendQDex:	       rb   0x32000
REQTEST 	       RB   0x32000
ModEnd:


