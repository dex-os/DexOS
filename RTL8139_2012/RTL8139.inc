
 ;****************************************************;
 ; get MAC (hardware address)                         ;
 ;----------------------------------------------------;
GetHwAddress:
	mov   edx,[io_addr]
	add   edx,0x00		  ;IDR0
	mov   edi,node_addr
	mov   ecx,6
loop_mac:
	in    al,dx
	stosb
	inc   edx
	dec   ecx
	jnz   loop_mac
	ret


 ;****************************************************;
 ;  SetIntrtl.                                        ;
 ;----------------------------------------------------;
SetIntrtl:
	cli
	mov   edx,[io_addr]
	add   edx,0x3c		   ;Interrupt mask register
	mov   eax,0xffffffff	   ;RT8139_INTERRUPT_MASK
	out   dx,ax
	sti
	ret

;***************************************************************************
;   Function
;      probe
;   Description
;      Searches for an ethernet card, enables it and clears the rx buffer
;      If a card was found, it enables the ethernet -> TCPIP link
;   Destroyed registers
;      eax, ebx, ecx, edx
;
;***************************************************************************
Probe:
; enable the device
	mov   eax,[PciEthAddress] ;Put pci bus + dev into EAX
	add   eax,PciRegCommand    ;Add the function number
	call  [PciRegRead32]	  ;Call the function Pci RegRead 32bit
	or    dl, (1 shl PciBitMaster) or (1 shl PciBitPio)
	and   dl, not (1 shl PciBitMmio)
	mov   eax,[PciEthAddress] ;Put pci bus + dev into EAX
	add   eax,PciRegCommand   ;Add the function number
	call  [PciRegWrite32]	  ;Call the function Pci RegWrite 32bit
; get chip version
	mov   eax,[PciEthAddress]
	add   al,PCI_BASE_ADDRESS_0 
	call  [PciRegRead32]
	and   edx, PCI_BASE_ADDRESS_IO_MASK 
	mov   dword[io_addr],edx
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_TXCONFIG_2
	in    ax,dx
	shr   ah,2
	shr   ax,6
	and   al,01111111b
	;call  [WriteHex16]
	mov   ecx,HwVerArraySize-1
ChipVerLoop:
	cmp   al,[HwVerArray+ecx]
	je    ChipVerFound
	dec   ecx
	jns   ChipVerLoop
	xor   cl,cl					       ; default RTL8139
ChipVerFound:
	mov   [HwVerID],cl
; wake up the chip
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_HLTCLK
	mov   al,'R'					       ; run the clock
	out   dx,al
; unlock config and BMCR registers
	add   edx,RTL8139_REG_9346CR - RTL8139_REG_HLTCLK
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) or (1 shl RTL8139_BIT_93C46_EEM0)
	out   dx,al
; enable power management
	add   edx,RTL8139_REG_CONFIG1 - RTL8139_REG_9346CR
	in    al,dx
	cmp   byte [HwVerID],IDX_RTL8139B
	jl    .old_chip
; set LWAKE pin to active high (default value).
; it is for Wake-On-LAN functionality of some motherboards.
; this signal is used to inform the motherboard to execute a wake-up process.
; only at newer chips.
	or    al,(1 shl RTL8139_BIT_PMEn)
	and   al,not (1 shl RTL8139_BIT_LWACT)
	out   dx,al
	add   edx,RTL8139_REG_CONFIG4 - RTL8139_REG_CONFIG1
	in    al,dx
	and   al,not (1 shl RTL8139_BIT_LWPTN)
	out   dx,al
	jmp   .finish_wake_up
.old_chip:
; wake up older chips
	and   al,not ((1 shl RTL8139_BIT_SLEEP) or (1 shl RTL8139_BIT_PWRDWN))
	out   dx,al
.finish_wake_up:
; lock config and BMCR registers
	xor   al,al
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_9346CR
	out   dx,al
;***************************************************************************
;   Function
;      reset
;   Description
;      Place the chip (ie, the ethernet card) into a virgin state
;   Destroyed registers
;      eax, ebx, ecx, edx
;
;***************************************************************************
reset:
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_COMMAND
	mov   al,1 shl RTL8139_BIT_RST
	out   dx,al
	mov   cx,10000 ; wait no longer for the reset ;ccccc0
.wait_for_reset:
	in    al,dx
	test  al,1 shl RTL8139_BIT_RST
	jz    .reset_completed			    ; RST remains 1 during reset
	dec   cx
	jns   .wait_for_reset
.reset_completed:
; get MAC (hardware address)
	mov   ecx,2
.mac_read_loop:
	lea   eax,[EE_93C46_REG_ETH_ID+ecx]
	push  ecx
	call  rtl8139_read_eeprom
	pop   ecx
	mov   [node_addr+ecx*2],ax
	dec   ecx
	jns   .mac_read_loop
; unlock config and BMCR registers
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_9346CR
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) or (1 shl RTL8139_BIT_93C46_EEM0)
	out   dx,al
; initialize multicast registers (no filtering)
	mov   eax,0xffffffff
	add   edx,RTL8139_REG_MAR0 - RTL8139_REG_9346CR
	out   dx,eax
	add   edx,RTL8139_REG_MAR4 - RTL8139_REG_MAR0
	out   dx,eax
; enable Rx/Tx
	mov   al,(1 shl RTL8139_BIT_RE) or (1 shl RTL8139_BIT_TE)
	add   edx,RTL8139_REG_COMMAND - RTL8139_REG_MAR4
	out   dx,al
; 32k Rxbuffer, unlimited dma burst, no wrapping, no rx threshold
; accept broadcast packets, accept physical match packets
	mov   ax,RTL8139_RX_CONFIG		     ;ccccc
	;or    ax,1                                              ;ccccccccccccc
	add   edx,RTL8139_REG_RXCONFIG - RTL8139_REG_COMMAND
	out   dx,ax
; 1024 bytes DMA burst, total retries = 16 + 8 * 16 = 144
	mov   ax,(RTL8139_TX_MXDMA shl RTL8139_BIT_TX_MXDMA) \
		  or (RTL8139_TXRR shl RTL8139_BIT_TXRR)
	add   edx,RTL8139_REG_TXCONFIG - RTL8139_REG_RXCONFIG
	out   dx,ax
; enable auto negotiation
	add   edx,RTL8139_REG_BMCR - RTL8139_REG_TXCONFIG
	in    ax,dx
	or    ax,(1 shl RTL8139_BIT_ANE)
	out   dx,ax
; set auto negotiation advertisement
	add   edx,RTL8139_REG_ANAR - RTL8139_REG_BMCR
	in    ax,dx
	or    ax,(1 shl RTL8139_BIT_SELECTOR) or (1 shl RTL8139_BIT_10) \
		   or (1 shl RTL8139_BIT_10FD) or (1 shl RTL8139_BIT_TX) \
		   or (1 shl RTL8139_BIT_TXFD)
	out   dx,ax
; lock config and BMCR registers
	xor   eax,eax
	add   edx,RTL8139_REG_9346CR - RTL8139_REG_ANAR
	out   dx,al
; init RX/TX pointers
	xor   eax,eax
	mov   [rtl8139_rx_buff_offset],eax
	mov   [curr_tx_desc],eax
; clear missing packet counter
	xor   eax,eax
	add   edx,RTL8139_REG_MPC - RTL8139_REG_9346CR
	out   dx,eax
; disable all interrupts
	xor   eax,eax
	add   edx,RTL8139_REG_IMR - RTL8139_REG_MPC
	out   dx,ax
; set RxBuffer address, init RX buffer offset, init TX ring
	mov   eax,rtl8139_rx_buff
	add   eax,[BaseAddOn]
	add   edx,RTL8139_REG_RBSTART - RTL8139_REG_IMR
	out   dx,eax
; enable Rx/Tx
	mov   al,(1 shl RTL8139_BIT_RE) or (1 shl RTL8139_BIT_TE)
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_COMMAND
	out   dx,al
; Indicate that we have successfully reset the card
	mov   eax,0x0FFFFFFF ;[pci_data]
	mov   [eth_status], eax
	ret

;***************************************************************************
;   Function
;      rtl8139_read_eeprom
;   Description
;      reads eeprom type 93c46 and 93c56
;   Parameters
;      al - word to be read (6bit in case of 93c46 and 8bit otherwise)
;   Return value
;      ax - word read in
;   Destroyed register(s)
;      eax, cx, ebx, edx
;
;***************************************************************************
rtl8139_read_eeprom:
	movzx ebx,al
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_RXCONFIG
	in    al,dx
	test  al,(1 shl RTL8139_BIT_9356SEL)
	jz    .type_93c46
;       and   bl,01111111b ; don't care first bit
	or    bx,EE_93C56_READ_CMD ; it contains start bit
	mov   cx,EE_93C56_CMD_LENGTH-1 ; cmd_loop counter
	jmp   .read_eeprom
.type_93c46:
	and   bl,00111111b
	or    bx,EE_93C46_READ_CMD ; it contains start bit
	mov   cx,EE_93C46_CMD_LENGTH-1 ; cmd_loop counter
.read_eeprom:
	add   edx,RTL8139_REG_9346CR - RTL8139_REG_RXCONFIG_0
;       mov   al,(1 shl RTL8139_BIT_93C46_EEM1)
;       out   dx,al
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) \
		    or (1 shl RTL8139_BIT_93C46_EECS) ; wake up the eeprom
	out   dx,al
.cmd_loop:
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) or (1 shl RTL8139_BIT_93C46_EECS)
	bt    bx,cx
	jnc   .zero_bit
	or    al,(1 shl RTL8139_BIT_93C46_EEDI)
.zero_bit:
	out   dx,al
;       push  eax
;       in    eax,dx ; eeprom delay
;       pop   eax
	or    al,(1 shl RTL8139_BIT_93C46_EESK)
	out   dx,al
;       in    eax,dx ; eeprom delay
	dec   cx
	jns   .cmd_loop
;       in    eax,dx ; eeprom delay
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) or (1 shl RTL8139_BIT_93C46_EECS)
	out   dx,al
	mov   cl,0xf
.read_loop:
	shl   ebx,1
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) \
		    or (1 shl RTL8139_BIT_93C46_EECS) \
		    or (1 shl RTL8139_BIT_93C46_EESK)
	out   dx,al
;       in    eax,dx ; eeprom delay
	in    al,dx
	and   al,(1 shl RTL8139_BIT_93C46_EEDO)
	jz    .dont_set
	inc   ebx
.dont_set:
	mov   al,(1 shl RTL8139_BIT_93C46_EEM1) \
		    or (1 shl RTL8139_BIT_93C46_EECS)
	out   dx,al
;       in    eax,dx ; eeprom delay
	dec   cl
	jns   .read_loop
	xor   al,al
	out   dx,al
	mov   ax,bx
	ret

;***************************************************************************
;   Function
;      transmit
;   Description
;      Transmits a packet of data via the ethernet card
;         Pointer to 48 bit destination address in edi
;         Type of packet in bx
;         size of packet in ecx
;         pointer to packet data in esi
;   Destroyed registers
;      eax, edx, esi, edi
;   ToDo
;      for waiting of timeout the rtl8139 internal timer
;      should be used
;
;***************************************************************************
transmit:
	cmp   ecx, MAX_ETH_FRAME_SIZE
	jg    .finish3 ; packet is too long
	 push  ecx
; check descriptor
	mov   ecx,[curr_tx_desc]
	mov   edx,[io_addr]
	lea   edx,[edx+ecx*4+RTL8139_REG_TSD0]
	push  edx ebx
	in    ax,dx
	and   ax,(1 shl RTL8139_BIT_TOK) or (1 shl RTL8139_BIT_OWN)
	cmp   ax,(1 shl RTL8139_BIT_TOK) or (1 shl RTL8139_BIT_OWN)
	jz    .send_packet
	test  ax,0x1fff ; or no size given
	jz    .send_packet
; wait for timeout
	mov   ax,7*3  ; 7 = 300 ms    ;cccccccccccccccraig
	call  [SetDelay]
	in    ax,dx
	and   ax,(1 shl RTL8139_BIT_TOK) or (1 shl RTL8139_BIT_OWN)
	cmp   ax,(1 shl RTL8139_BIT_TOK) or (1 shl RTL8139_BIT_OWN)
	jz    .send_packet
; chip hung, reset it
	call  reset
; reset the card
.send_packet:
; calculate tx_buffer address
	pop   ebx
	push  esi
	mov   eax,MAX_ETH_FRAME_SIZE
	mul   dword[curr_tx_desc]
	mov   esi,edi
	lea   edi,[rtl8139_tx_buff+eax]
	mov   eax,edi
	add eax,[BaseAddOn]			   ;cccc
	cld
; copy destination address
	movsd
	movsw
; copy source address
	mov   esi,node_addr
	movsd
	movsw
; copy packet type
	mov   [edi],bx
	add	edi, 2
; copy the packet data
	pop   esi edx ecx
	push  ecx
	shr   ecx,2
	rep   movsd
	pop   ecx
	push  ecx
	and   ecx,3
	rep   movsb
; set address
	add   edx,RTL8139_REG_TSAD0 - RTL8139_REG_TSD0
	out   dx,eax
; set size and early threshold
	pop   eax ; pick up the size
	add   eax,ETH_HLEN
	cmp   eax,ETH_ZLEN
	jnc   .no_pad
	mov   eax,ETH_ZLEN
.no_pad:
	or    eax,(RTL8139_ERTXTH shl RTL8139_BIT_ERTXTH)
	add   edx,RTL8139_REG_TSD0 - RTL8139_REG_TSAD0
	out   dx,eax
; get next descriptor 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, ...
	inc   dword[curr_tx_desc]
	and   dword [curr_tx_desc], 3
.finish:

       ret
.finish3:

	ret

;***************************************************************************
; Function
;    poll
;
; Description
;    Polls the ethernet card for a received packet
;    Received data, if any, ends up in Ether_buffer
; Destroyed register(s)
;    eax, edx, ecx
;
;***************************************************************************
poll:
	mov   word[eth_rx_data_len], 0
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_COMMAND
	in    al,dx
	test  al,(1 shl RTL8139_BIT_BUFE)
	jnz   .finish

; new packet received copy it from rx_buffer into Ether_buffer
	mov   eax,rtl8139_rx_buff
	add   eax,[rtl8139_rx_buff_offset]

      
; check if packet is ok
	test   byte[eax],(1 shl RTL8139_BIT_ROK)
	jz    .reset_rx

; packet is ok copy it into the Ether_buffer
	movzx ecx,word [eax+2] ; packet length
	sub   ecx,4 ; don't copy CRC
	mov   word[eth_rx_data_len], cx
	push  ecx
	shr   ecx,2 ; first copy dword-wise
	lea   esi,[eax+4] ; don't copy the packet header
	mov   edi,Ether_buffer
	cld
	rep   movsd ; copy the dwords
	pop   ecx
	and   ecx, 3
	rep   movsb ; copy the rest bytes
; update rtl8139_rx_buff_offset
	movzx eax,word [eax+2] ; packet length
	add   eax,[rtl8139_rx_buff_offset]
	add   eax,4+3 ; packet header is 4 bytes long + dword alignment
	and   eax,not 3 ; dword alignment
	cmp   eax, RTL8139_RX_BUFFER_SIZE
	jl    .no_wrap
	sub   eax, RTL8139_RX_BUFFER_SIZE
.no_wrap:
	mov   [rtl8139_rx_buff_offset], eax
; update CAPR register
	sub   eax,0x10 ; value 0x10 is a constant for CAPR
	add   edx,RTL8139_REG_CAPR - RTL8139_REG_COMMAND
	out   dx,ax
.finish:
; clear active interrupt sources
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_ISR
	in    ax,dx
	out   dx,ax
	ret
.reset_rx:

	in    al,dx ; read command register
	push  eax
	and   al,not (1 shl RTL8139_BIT_RE)
	out   dx,al
	pop   eax
	out   dx,al
	add   edx,RTL8139_REG_RXCONFIG - RTL8139_REG_COMMAND
	mov   ax,RTL8139_RX_CONFIG
	out   dx,ax
	ret


 ;****************************************************;
 ;  poll1.                        ;temp test function ;
 ;----------------------------------------------------;
poll1:
	xor   eax,eax
	mov   word[eth_rx_data_len1], 0
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_COMMAND
	in    al,dx
	mov   word[eth_rx_data_len1],ax
	xor   eax,eax
	mov   edx,[io_addr]
	add   edx,RTL8139_REG_ISR
	in    ax,dx
	mov   word[eth_rx_data_len],ax
	out   dx,ax
	ret

