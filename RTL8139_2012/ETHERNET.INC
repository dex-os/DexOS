;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                 ;;
;;  ETHERNET.INC                                                   ;;
;;                                                                 ;;
;;  Ethernet network layer for DexOS                               ;;
;;                                                                 ;;
;;  Version 0.4  22 September 2003                                 ;;
;;                                                                 ;;
;;  This file contains the following:                              ;;
;;      PCI bus scanning for valid devices                         ;;
;;      Table of supported ethernet drivers                        ;;
;;      Code to identify and activate a supported driver           ;;
;;      ARP handler                                                ;;
;;      Driver interface to the IP layer                           ;;
;;      Gateway support                                            ;;
;;                                                                 ;;
;;  Individual driver files are included here                      ;;
;;                                                                 ;;
;;  The PCI bus scanning code was ported from the etherboot        ;;
;;  5.0.6 project. The copyright statement for that code is        ;;
;;                                                                 ;;
;;          GNU GENERAL PUBLIC LICENSE                             ;;
;;             Version 2, June 1991                                ;;
;;                                                                 ;;
;;  remaining parts Copyright 2002 Mike Hibbett                    ;;
;;   mikeh@oceanfree.net                                           ;;
;;                                                                 ;;
;;  See file COPYING for details                                   ;;
;;                                                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;********************************************************************
;   Interface
;      ethernet_driver   called by stack_handler in stack.inc
;      eth_probe         called by app_stack_handler in stack.inc
;
;********************************************************************

; Some useful information on data structures

;     Ethernet Packet - ARP Request example
;
;   0                   1                   2                   3
;   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
;
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |       Dest   H/W Address                                      |
;   |                    ( 14 byte header )                         |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                               |     Source     H/W Address    |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                                                               |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |    Protocol - ARP 08  06      |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |  H/W Type  00           01    |  Protocol Type   08 00        |
;   |                   ( ARP Request packet )                      |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | HLen    0x06  | PLen    0x04  |    OpCode        00   01      |
;   |               ( 0001 for request, 0002 for reply )            |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | Source Hardware Address ( MAC Address )                       |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                               |  Source IP Address            |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                               | Destination Hardware Address  |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                                                               |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | Destination IP Address                                        |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

PCICARDS_ENTRY_SIZE	    equ     20	  ; Size of each PCICARDS entry
; PCI Bus defines
PCI_HEADER_TYPE 	    equ     0x0e  ;8 bit
PCI_BASE_ADDRESS_0	    equ     0x10  ;32 bit
PCI_BASE_ADDRESS_5	    equ     0x24  ;32 bits
PCI_BASE_ADDRESS_SPACE_IO   equ     0x01
PCI_VENDOR_ID		    equ     0x00  ;16 bit
PCI_BASE_ADDRESS_IO_MASK    equ     0xFFFFFFFC

ETHER_IP		    equ     0x0008	; Reversed from 0800 for intel
ETHER_ARP		    equ     0x0608	; Reversed from 0806 for intel
ETHER_RARP		    equ     0x3580
ARP_REQ_OPCODE		    equ     0x0100
ARP_REP_OPCODE		    equ     0x0200

node_addr:	    db	0,0,0,0,0,0
eth_rx_data_len:    dw	0
eth_status:	    dd	0

arp_rx_count:	    dd	0
ip_rx_count:	    dd	0
dumped_rx_count:    dd	0
ip_tx_count:	    dd	0


; These hold the destination Host identity for ARP responses
remote_ip_add:	    dd	0
remote_hw_add:	    db	0, 0, 0, 0, 0, 0
broadcast_add:	    db	0xff,0xff,0xff,0xff,0xff,0xff

; This is used by getMACfromIP
MACAddress:	    db	0,0,0,0,0,0
gateway_ip:	    db	0, 0, 0, 0
subnet_mask:	    dd	0x00ffffff
dns_ip: 	    dd	0

; The follow is the ARP Table.
; This table must be manually updated and the kernel recompilied if
; changes are made to it.
; ARP_TABLE_SIZE defines the size of the table
; ARP_TABLE_ENTRIES defines the number of entries in the table
; Each entry is 10 bytes: 4 Byte IP address, 6 byte MAC Address,
;                         2 bytes status, 2 bytes TTL ( in seconds )
; Empty entries are filled with zeros
; The TTL field is decremented every second, and is deleted when it
; reaches 0. It is refreshed every time a packet is received
; If the TTL field is 0xFFFF it is a permanent entry and is never deleted
; The status field can be the following values
; 0x0000  entry not used
; 0x0001  entry holds a valid mapping
; 0x0002  entry contains an IP address, awaiting ARP response
; 0x0003  No response received to ARP request.
; The last status value is provided to allow the network layer to delete
; a packet that is queued awaiting an ARP response

ARP_NO_ENTRY		    equ 0
ARP_VALID_MAPPING	    equ 1
ARP_AWAITING_RESPONSE	    equ 2
ARP_RESPONSE_TIMEOUT	    equ 3

ARP_ENTRY_SIZE		    equ     14		; Number of bytes per entry
ARP_TABLE_SIZE		    equ     20		; Size of table
ARP_TABLE_ENTRIES	    equ     0		; Inital, hardcoded entrie ;cc

ARPTable:
times ( ARP_TABLE_SIZE - ARP_TABLE_ENTRIES ) * ARP_ENTRY_SIZE  db 0

NumARP:        db    ARP_TABLE_ENTRIES

;***************************************************************************
;   Function
;      ethernet_driver
;
;   Description
;       The ethernet RX and TX handler
;       This is a kernel function, called by stack_handler
;
;***************************************************************************
ethernet_driver:
    ; Do nothing if the driver is inactive
    ;cmp     [ethernet_active], byte 0
    ;je      eth_exit

    call    eth_rx
    call    eth_tx

eth_exit:
    ret

;***************************************************************************
;   Function
;      eth_rx
;
;   Description
;      Polls the ethernet card for received data. Extracts if present
;       Depending on the Protocol within the packet:
;         ARP : Pass to ARP_handler. This may result in an ARP reply
;               being tx'ed
;         IP  : Store in an IP buffer
;
;***************************************************************************
eth_rx:
    xor     ax, ax
    mov     [eth_rx_data_len], ax
    call    poll       ; Call the drivers poll function

    mov     ax, [eth_rx_data_len]
    cmp     ax, 0
    je	    erx_exit



    ; Check the protocol. Call appropriate handler
    mov     eax, Ether_buffer
    add     eax, 12		     ; The address of the protocol word

    mov     ax, [eax]

    cmp     ax, ETHER_ARP
    je	    erx_001		     ; It is ARP

    cmp     ax, ETHER_IP
    je	    erx_002		     ; It's IP

;   inc     dword [dumped_rx_count]

    jmp     erx_exit		   ; If not IP or ARP, ignore

erx_001:


    mov     eax, [arp_rx_count]
    inc     eax
    mov     [arp_rx_count], eax

    ; At this point, the packet is still in the Ether_buffer
    call    arp_handler

    jmp     erx_exit

erx_002:


    mov     eax, [ip_rx_count]
    inc     eax
    mov     [ip_rx_count], eax

    ; Check to see if the MAC address is in our arp table
    ; refresh the arp ttl if so

    mov     esi, Ether_buffer
    add     esi, 6

    call    refreshARP

    call    ether_IP_handler

    jmp     erx_exit

erx_exit:
    ret

;***************************************************************************
;   Function
;      eth_tx
;
;   Description
;      Looks at the NET1OUT_QUEUE for data to send.
;      Stores that destination IP in a location used by the tx routine
;      Looks up the MAC address in the ARP table; stores that where
;      the tx routine can get it
;      Get the length of the data. Store that where the tx routine wants it
;      Call tx
;      Places buffer on empty queue when the tx routine finished
;
;***************************************************************************
eth_tx:
    ; Look for a buffer to tx

    mov     eax, NET1OUT_QUEUE
    call    dequeue

    cmp     ax, NO_BUFFER

    je	    eth_exit		; Exit if no buffer available


     push    eax

    ; convert buffer pointer eax to the absolute address
    mov     ecx, IPBUFFSIZE
    mul     ecx
    add     eax, IPbuffs

    ; Extract the destination IP
    ; find the destination IP in the ARP table, get MAC
    ; store this MAC in 'MACAddress'
    mov     ebx, eax		   ; Save buffer address
    mov     edx, [ebx + 16]	   ; get destination address


    ; If the destination address is 255.255.255.255,
    ; set the MACAddress to all ones ( broadcast )
    mov     [MACAddress], dword 0xffffffff
    mov     [MACAddress + 4], word 0xffff
    cmp     edx, 0xffffffff
    je	    etx_send		    ; If it is broadcast, just send


    call    getMACfromIP	   ; Get the MAC address.

    cmp     eax, ARP_VALID_MAPPING
    jz	    etx_send

    ; No valid entry. Are we waiting for a response?
    cmp     eax, ARP_AWAITING_RESPONSE

    jne     etx_001

    ; Re-queue the packet, and exit
    pop     ebx
    mov     eax, NET1OUT_QUEUE
    call    queue

    jmp     etx_exit

etx_001:
    ; HAs the request been sent, but timed out?
    cmp     eax, ARP_RESPONSE_TIMEOUT
    jne     etx_002

    pop     eax
    call    freeBuff

    jmp     etx_exit

etx_002:
    ; There is no entry. Re queue the request, and ask ARP to send a request

    ; IP address is in edx
    push    edx
    call    arp_request
    pop     ebx

    ; Add an entry in the ARP table, awaiting response

    cmp     byte [NumARP], ARP_TABLE_SIZE
    je	    etx_003	       ; We cannot add a new entry in the table

    inc     byte [NumARP]


    movzx   eax, byte [NumARP]
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx
    sub     eax, ARP_ENTRY_SIZE

    mov     [eax + ARPTable], ebx
    xor     ebx, ebx
    mov     [eax + ARPTable + 4], ebx
    mov     [eax + ARPTable + 8], bx

    ; set the status field up - awaiting response
    mov     cl, 0x00
    mov     [eax + ARPTable + 10], cl
    mov     cl, 0x02
    mov     [eax + ARPTable + 11], cl

    ; Initialise the time to live field - 10s
    mov     cx, 0x000A
    mov     [eax + ARPTable + 12], cx

etx_003:
    pop     ebx 		       ; Get the buffer back
    mov     eax, NET1OUT_QUEUE
    call    queue

    jmp     etx_exit

etx_send:
    xor     ecx, ecx
    mov     ch, [ebx+2]
    mov     cl, [ebx+3] 	 ; ; Size of IP packet to send

    mov     esi, ebx

    mov     edi, MACAddress



    mov     bx, ETHER_IP
    call    transmit		; Call the drivers transmit function

    ; OK, we have sent a packet, so increment the count
    inc     dword [ip_tx_count]

    ; And finally, return the buffer to the free queue
    pop     eax
    call    freeBuff

ret
etx_exit:


    ret

;***************************************************************************
;   Function
;      ether_IP_handler
;
;   Description
;      Called when an IP ethernet packet is received on the ethernet
;      Header + Data is in Ether_buffer[]
;      We just need to get a buffer from the 'free' queue, and
;      store the packet in it, then insert the packet number into the
;      IPRX queue.
;      If no queue entry is available, the packet is silently discarded
;      All registers may be destroyed
;
;***************************************************************************
ether_IP_handler:
    mov     eax, EMPTY_QUEUE
    call    dequeue
    cmp     ax, NO_BUFFER
    je	    eiph00x

    ; convert buffer pointer eax to the absolute address
    push    eax
    mov     ecx, IPBUFFSIZE
    mul     ecx
    add     eax, IPbuffs

    mov     edi, eax

    ; get a pointer to the start of the DATA
    mov     esi, Ether_buffer + 14

    ; Now store it all away
    mov     ecx, IPBUFFSIZE / 4     ; Copy all of the available
			      ; data across - worse case
    cld
    rep     movsd

    ; And finally, place the buffer in the IPRX queue
    pop     ebx
    mov     eax, IPIN_QUEUE
    call    queue

eiph00x:
    ret

;***************************************************************************
;
;  ARP CODE FOLLOWS
;
;  The ARP code is used by ethernet drivers to translate an destination
;  IP address into an ethernet hardware address. Functions to broadcast
;  requests and handle response are (or will be) here.
;  The IP layer has no knowledge of ARP, as this is a network interface
;  issue
;
;***************************************************************************

;***************************************************************************
;   Function
;      arp_timer
;
;   Description
;      Called every 1s
;      It is responsible for removing expired routes
;      All registers may be destroyed
;
;***************************************************************************
arp_timer:
    ; loop through all the ARP entries, decrementing each one
    ; that doesn't have a TTL of 0xFFFF
    movzx   eax, byte [NumARP]

arp_001:
    cmp     eax, 0
    je	    arp_003

    push    eax
    dec     eax
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx
    cmp     word [ eax + ARPTable + 12], 0xFFFF
    je	    arp_002

    cmp     word [ eax + ARPTable + 12], 0
    je	    arp_002

    dec     word [eax + ARPTable + 12]

arp_002:
    pop     eax
    dec     eax
    jmp     arp_001

    ; Now, look for entries with a TTL of 0
    ; Valid entries and response timeout entries get removed
    ; awaiting response gets converted into a response timeout, with a
    ; short life time - this allows queued packets to be flushed
arp_003:
    movzx   edx, byte [NumARP]
    cmp     edx, 0
    je	    arp_exit

    ; EDX holds the # of entries to search through
    mov     eax, 0

arp_005:
    cmp     word [ eax + ARPTable + 12], 0
    jne     arp_004

    ; If it's status code is 0001 or 0003, delete the entry
    cmp     word [eax + ARPTable + 10], 0x0100
    je	    arp_007
    cmp     word [eax + ARPTable + 10], 0x0300
    je	    arp_007

    ; The only other valid code is 0002 - indicating a
    ; timeout while waiting for a response. Change the
    ; entry to response timed out

    mov     [eax + ARPTable + 10], word 0x0300
    mov     [eax + ARPTable + 12], word 0x000A
    jmp     arp_004

arp_007:
    ; Delete this entry
    mov     edi, ARPTable
    add     edi, eax
    mov     esi, edi
    add     esi, ARP_ENTRY_SIZE

    mov     ecx, (ARP_TABLE_SIZE - 1) * ARP_ENTRY_SIZE
    sub     ecx, eax

    rep     movsb

    dec     byte [NumARP]
    jmp     arp_006

arp_004:
    add     eax, ARP_ENTRY_SIZE
arp_006:
    dec     edx
    cmp     edx, 0
    jne     arp_005

arp_exit:
    ret

;***************************************************************************
;   Function
;      arp_request
;
;   Description
;      Sends an ARP request on the ethernet
;        The requested IP address is in edx
;      All registers may be destroyed
;
;***************************************************************************
arp_request:
    mov     ebx, Ether_buffer
    mov     ax, 0x0100
    mov     [ebx], ax
    add     ebx, 2

    mov     ax, 0x0008
    mov     [ebx], ax
    add     ebx, 2

    mov     ax, 0x0406
    mov     [ebx], ax
    add     ebx, 2

    mov     ax, 0x0100
    mov     [ebx], ax
    add     ebx, 2

    mov     ecx, node_addr
    mov     eax, [ecx]
    mov     [ebx], eax
    add     ecx, 4
    add     ebx, 4
    mov     ax, [ecx]
    mov     [ebx], ax
    add     ebx, 2
    mov     eax, [stack_ip]
    mov     [ebx], eax
    add     ebx, 4

    xor     eax, eax
    mov     [ebx], eax
    add     ebx, 4
    mov     [ebx], ax

    add     ebx, 2
    mov     [ebx], edx

    ; Now, send it!

    ; Pointer to 48 bit destination address in edi
    ; Type of packet in bx
    ; size of packet in ecx
    ; pointer to packet data in esi
    mov      edi, broadcast_add

;if DEBUGGING_STATE = DEBUGGING_ENABLED
;    pusha
;    mov     eax, 1                    ; Indicate that this is a tx packet
;    mov     ecx, 28
;   mov      esi, Ether_buffer
;    call    eth_dump
;    popa
;end if

    mov     bx, ETHER_ARP
    mov     ecx, 28
    mov     esi, Ether_buffer
    call    transmit		       ; Call the drivers transmit function
    ret

;***************************************************************************
;   Function
;      arp_handler
;
;   Description
;      Called when an ARP packet is received on the ethernet
;      Header + Data is in Ether_buffer[]
;       It looks to see if the packet is a request to resolve this Hosts
;       IP address. If it is, send the ARP reply packet.
;      This Hosts IP address is in dword [stack_ip]  ( in network format )
;       This Hosts MAC address is in node_addr[6]
;      All registers may be destroyed
;
;***************************************************************************
arp_handler:
    ; Is this a REQUEST?
    ; Is this a request for My Host IP
    ; Yes - So construct a response message.
    ; Send this message to the ethernet card for transmission

    mov     ebx, Ether_buffer

    mov     edx, ebx
    add     edx, 20
    mov     ax, [edx]
    cmp     ax, ARP_REQ_OPCODE	    ; Is this a request packet?
    jne     arph_resp		 ; No - so test for response

    mov     edx, ebx
    add     edx, 38
    mov     eax, [edx]

    cmp     eax, [stack_ip]	    ; Is it looking for my IP address?
    jne     arph_exit		    ; No - so quit now

    ; OK, it is a request for my MAC address. Build the frame and send it

    ; Save the important data from the original packet
    ; remote MAC address first
    mov     ecx, remote_hw_add
    mov     edx, ebx
    add     edx, 22		  ; edx points to Source h/w address
    mov     eax, [edx]
    mov     [ecx], eax
    add     edx, 4
    add     ecx, 4
    mov     ax, [edx]
    mov     [ecx],ax

    ; and also the remote IP address
    add     edx, 2
    mov     eax,[edx]
    mov     [remote_ip_add], eax

    ; So now we can reuse the packet. ebx still holds the address of
    ; the header + packet
    ; We dont need the header ( first 14 bytes )

    mov     edx, ebx
    add     edx, 20
    mov     ax, ARP_REP_OPCODE
    mov     [edx], ax
    add     edx, 2

    mov     ecx, node_addr
    mov     eax, [ecx]
    mov     [edx], eax
    add     ecx, 4
    add     edx, 4
    mov     ax, [ecx]
    mov     [edx], ax
    add     edx, 2
    mov     eax, [stack_ip]
    mov     [edx], eax
    add     edx, 4
    mov     ecx, remote_hw_add
    mov     eax, [ecx]
    mov     [edx], eax
    add     ecx, 4
    add     edx, 4
    mov     ax, [ecx]
    mov     [edx], ax

    add     edx, 2
    mov     eax, [remote_ip_add]
    mov     [edx], eax

    ; Now, send it!

    ; Pointer to 48 bit destination address in edi
    ; Type of packet in bx
    ; size of packet in ecx
    ; pointer to packet data in esi
    mov     edi, remote_hw_add

;if DEBUGGING_STATE = DEBUGGING_ENABLED
;    pusha
;    mov     eax, 1                    ; Indicate that this is a tx packet
;    mov     ecx, 28
;   mov      esi, Ether_buffer + 14
 ;   call    eth_dump
;    popa
;end if

    mov     bx, ETHER_ARP
    mov     ecx, 28
    mov     esi, Ether_buffer + 14
    call    transmit		    ; Call the drivers transmit function
    jmp     arph_exit

arph_resp:
    cmp     ax, ARP_REP_OPCODE	    ; Is this a replypacket?
    jne     arph_exit  ;arph_resp               ; No - so quit ***** April 18 2007 mod by craig untested********

    ; This was a reply, probably directed at me.
    ; save the remotes MAC & IP
    mov     ecx, remote_hw_add
    mov     edx, ebx
    add     edx, 22		  ; edx points to Source h/w address
    mov     eax, [edx]
    mov     [ecx], eax
    add     edx, 4
    add     ecx, 4
    mov     ax, [edx]
    mov     [ecx],ax

    ; and also the remote IP address
    add     edx, 2
    mov     eax,[edx]
    mov     [remote_ip_add], eax

    ; Now, add an entry in the table for this IP address if it doesn't exist

    push    eax
    movzx   eax, byte [NumARP]
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx
    pop     edx
    movzx   ecx, byte [NumARP]
    cmp     ecx, 0
    je	    arph_002

arph_001:
    sub     eax, ARP_ENTRY_SIZE
    cmp     [eax + ARPTable], edx
    loopnz  arph_001			  ; Return back if non match

    jnz     arph_002		       ; None found, add to end

    mov     ecx, [remote_hw_add]
    mov     [eax + ARPTable + 4], ecx
    mov     cx, [remote_hw_add+4]
    mov     [eax + ARPTable + 8], cx

    ; specify the type - a valid entry
    mov     cl, 0x00
    mov     [eax + ARPTable + 10], cl
    mov     cl, 0x01
    mov     [eax + ARPTable + 11], cl

    ; Initialise the time to live field - 1 hour
    mov     cx, 0x0E10
    mov     [eax + ARPTable + 12], cx
    jmp     arph_exit

arph_002:

    cmp     byte [NumARP], ARP_TABLE_SIZE
    je	    arph_exit

    inc     byte [NumARP]

    movzx   eax, byte [NumARP]
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx
    sub     eax, ARP_ENTRY_SIZE

    mov     ecx, [remote_ip_add]
    mov     [eax + ARPTable], ecx
    mov     ecx, [remote_hw_add]
    mov     [eax + ARPTable + 4], ecx
    mov     cx, [remote_hw_add+4]
    mov     [eax + ARPTable + 8], cx

    mov     cl, 0x00
    mov     [eax + ARPTable + 10], cl
    mov     cl, 0x01
    mov     [eax + ARPTable + 11], cl

    ; Initialise the time to live field - 1 hour
    mov     cx, 0x0E10
    mov     [eax + ARPTable + 12], cx

arph_exit:
    ret

; pointer to MAC in esi
refreshARP:
    mov     ebx, [esi]
    mov     dx, [esi+4]
    push    edx
    movzx   eax, byte [NumARP]
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx
    pop     edx
    movzx   ecx, byte [NumARP]
    cmp     ecx, 0
    je	    rf_exit

rf_001:
    sub     eax, ARP_ENTRY_SIZE
    cmp     [eax + ARPTable+4], ebx

    je	    rf_002
    loop    rf_001
    jmp     rf_exit

rf_002:
    cmp     [eax + ARPTable+8], dx
    je	    rf_gotone
    loop    rf_001
    jmp     rf_exit

rf_gotone:
    ; Initialise the time to live field - 1 hour
    mov     cx, 0x0E10
    mov     [eax + ARPTable + 12], cx

rf_exit:
    ret

;***************************************************************************
;   Function
;      getMACfromIP
;
;   Description
;       Takes an IP address in edx and scans the ARP table for
;        a matching entry
;       If a match is found, it's MAC address is stored in MACAddress.
;      Otherwise the value 0 is writen to MACAddress
;      eax holds ARP table entry status code ( ARP_ )
;      ebx unchanged
;
;***************************************************************************
getMACfromIP:
    ; first, check destination IP to see if it is on 'this' network.
    ; The test is:
    ; if ( destIP & subnet_mask == stack_ip & subnet_mask )
    ;   desitnation is local
    ; else
    ;  destination is remote, so pass to gateway

    mov     eax, edx
    and     eax, [subnet_mask]
    mov     ecx, [stack_ip]
    and     ecx, [subnet_mask]
    cmp     eax, ecx
    je	    gm0

    mov     edx, [gateway_ip]
gm0:
    push    edx
    xor     eax, eax
    mov     [MACAddress], eax
    mov     [MACAddress + 4], ax

    movzx   eax, byte [NumARP]
    mov     ecx, ARP_ENTRY_SIZE
    mul     ecx

    pop     edx

    movzx   ecx, byte [NumARP]
    cmp     ecx, 0
    je	    gm_none
gm1:
    sub     eax, ARP_ENTRY_SIZE
    cmp     [eax + ARPTable], edx
    loopnz  gm1 		     ; Return back if non match
    jnz     gm_none		      ; Quit if none found

    ; eax holds index
    mov     ecx, [eax + ARPTable + 4]
    mov     [MACAddress], ecx
    mov     cx, [eax + ARPTable + 8]
    mov     [MACAddress+4], cx

    ; Return the entry status in eax
    mov     ch, [eax + ARPTable + 10]
    mov     cl, [eax + ARPTable + 11]
    movzx   eax, cx
    jmp     gm_exit

gm_none:
    mov     eax, ARP_NO_ENTRY

gm_exit:
    ret


;***************************************************************************
;   Function
;      delay_us
;
;   Description
;       delays for 30 to 60 us
;
;        I would prefer this routine to be able to delay for
;       a selectable number of microseconds, but this works for now.
;
;       If you know a better way to do 2us delay, pleae tell me!
;***************************************************************************
delay_us:
    push    eax
    push    ecx

    mov     ecx,2

    in	    al,0x61
    and     al,0x10
    mov     ah,al
    cld

dcnt1:
    in	    al,0x61
    and     al,0x10
    cmp     al,ah
    jz	    dcnt1

    mov     ah,al
    loop    dcnt1

    pop     ecx
    pop     eax

    ret




; This is used for translating hex to ASCII for display or output
hexchars    db '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'

;end if

