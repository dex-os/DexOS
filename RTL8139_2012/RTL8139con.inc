ETH_ALEN               equ 6
ETH_HLEN               equ (2 * ETH_ALEN + 2)
ETH_ZLEN               equ 60   ; 60 + 4bytes auto payload for
                                ; mininmum 64bytes frame length
PciRegCommand          equ 0x04 ; command reg
PciBitPio              equ 0    ; bit0: io space control
PciBitMmio             equ 1    ; bit1: memory space control
PciBitMaster           equ 2    ; bit2: device acts as a PCI master

RTL8139_REG_MAR0       equ 0x08 ; multicast filter register 0
RTL8139_REG_MAR4       equ 0x0c ; multicast filter register 4
RTL8139_REG_TSD0       equ 0x10 ; transmit status of descriptor
RTL8139_REG_TSAD0      equ 0x20 ; transmit start address of descriptor
RTL8139_REG_RBSTART    equ 0x30 ; RxBuffer start address
RTL8139_REG_COMMAND    equ 0x37 ; command register
RTL8139_REG_CAPR       equ 0x38 ; current address of packet read
RTL8139_REG_IMR        equ 0x3c ; interrupt mask register
RTL8139_REG_ISR        equ 0x3e ; interrupt status register
RTL8139_REG_TXCONFIG   equ 0x40 ; transmit configuration register
RTL8139_REG_TXCONFIG_0 equ 0x40 ; transmit configuration register 0
RTL8139_REG_TXCONFIG_1 equ 0x41 ; transmit configuration register 1
RTL8139_REG_TXCONFIG_2 equ 0x42 ; transmit configuration register 2
RTL8139_REG_TXCONFIG_3 equ 0x43 ; transmit configuration register 3
RTL8139_REG_RXCONFIG   equ 0x44 ; receive configuration register 0
RTL8139_REG_RXCONFIG_0 equ 0x44 ; receive configuration register 0
RTL8139_REG_RXCONFIG_1 equ 0x45 ; receive configuration register 1
RTL8139_REG_RXCONFIG_2 equ 0x46 ; receive configuration register 2
RTL8139_REG_RXCONFIG_3 equ 0x47 ; receive configuration register 3
RTL8139_REG_MPC        equ 0x4c ; missed packet counter
RTL8139_REG_9346CR     equ 0x50 ; serial eeprom 93C46 command register
RTL8139_REG_CONFIG1    equ 0x52 ; configuration register 1
RTL8139_REG_CONFIG4    equ 0x5a ; configuration register 4
RTL8139_REG_HLTCLK     equ 0x5b ; undocumented halt clock register
RTL8139_REG_BMCR       equ 0x62 ; basic mode control register
RTL8139_REG_ANAR       equ 0x66 ; auto negotiation advertisement register
; 5.1 packet header
RTL8139_BIT_RUNT       equ 4 ; total packet length < 64 bytes
RTL8139_BIT_LONG       equ 3 ; total packet length > 4k
RTL8139_BIT_CRC        equ 2 ; crc error occured
RTL8139_BIT_FAE        equ 1 ; frame alignment error occured
RTL8139_BIT_ROK        equ 0 ; received packet is ok
; 5.4 command register
RTL8139_BIT_RST        equ 4 ; reset bit
RTL8139_BIT_RE         equ 3 ; receiver enabled
RTL8139_BIT_TE         equ 2 ; transmitter enabled
RTL8139_BIT_BUFE       equ 0 ; rx buffer is empty, no packet stored
; 5.6 interrupt status register
RTL8139_BIT_ISR_TOK    equ 2 ; transmit ok
RTL8139_BIT_ISR_RER    equ 1 ; receive error interrupt
RTL8139_BIT_ISR_ROK    equ 0 ; receive ok
; 5.7 transmit configyration register
RTL8139_BIT_TX_MXDMA   equ 8 ; Max DMA burst size per Tx DMA burst
RTL8139_BIT_TXRR       equ 4 ; Tx Retry count 16+(TXRR*16)
; 5.8 receive configuration register
RTL8139_BIT_RXFTH      equ 13 ; Rx fifo threshold
RTL8139_BIT_RBLEN      equ 11 ; Ring buffer length indicator
RTL8139_BIT_RX_MXDMA   equ 8 ; Max DMA burst size per Rx DMA burst
RTL8139_BIT_NOWRAP     equ 7 ; transfered data wrapping
RTL8139_BIT_9356SEL    equ 6 ; eeprom selector 9346/9356
RTL8139_BIT_AER        equ 5 ; accept error packets
RTL8139_BIT_AR         equ 4 ; accept runt packets
RTL8139_BIT_AB         equ 3 ; accept broadcast packets
RTL8139_BIT_AM         equ 2 ; accept multicast packets
RTL8139_BIT_APM        equ 1 ; accept physical match packets
RTL8139_BIT_AAP        equ 0 ; accept all packets
; 5.9 93C46/93C56 command register
RTL8139_BIT_93C46_EEM1 equ 7 ; RTL8139 eeprom operating mode1
RTL8139_BIT_93C46_EEM0 equ 6 ; RTL8139 eeprom operating mode0
RTL8139_BIT_93C46_EECS equ 3 ; chip select
RTL8139_BIT_93C46_EESK equ 2 ; serial data clock
RTL8139_BIT_93C46_EEDI equ 1 ; serial data input
RTL8139_BIT_93C46_EEDO equ 0 ; serial data output
; 5.11 configuration register 1
RTL8139_BIT_LWACT      equ 4 ; see RTL8139_REG_CONFIG1
RTL8139_BIT_SLEEP      equ 1 ; sleep bit at older chips
RTL8139_BIT_PWRDWN     equ 0 ; power down bit at older chips
RTL8139_BIT_PMEn       equ 0 ; power management enabled
; 5.14 configuration register 4
RTL8139_BIT_LWPTN      equ 2 ; see RTL8139_REG_CONFIG4
; 6.2 transmit status register
RTL8139_BIT_ERTXTH     equ 16 ; early TX threshold
RTL8139_BIT_TOK        equ 15 ; transmit ok
RTL8139_BIT_OWN        equ 13 ; tx DMA operation is completed
; 6.18 basic mode control register
RTL8139_BIT_ANE        equ 12 ; auto negotiation enable
; 6.20 auto negotiation advertisement register
RTL8139_BIT_TXFD       equ 8 ; 100base-T full duplex
RTL8139_BIT_TX         equ 7 ; 100base-T
RTL8139_BIT_10FD       equ 6 ; 10base-T full duplex
RTL8139_BIT_10         equ 5 ; 10base-T
RTL8139_BIT_SELECTOR   equ 0 ; binary encoded selector CSMA/CD=00001
; RX/TX buffer size
RTL8139_RBLEN          equ 0 ; 0==8K 1==16k 2==32k 3==64k
RTL8139_RX_BUFFER_SIZE equ (8192 shl RTL8139_RBLEN)
MAX_ETH_FRAME_SIZE     equ 1516 ; exactly 1514 wthout CRC
RTL8139_NUM_TX_DESC    equ 4
RTL8139_TX_BUFFER_SIZE equ (MAX_ETH_FRAME_SIZE * RTL8139_NUM_TX_DESC)
RTL8139_TXRR           equ 8 ; total retries = 16+(TXRR*16)
RTL8139_TX_MXDMA       equ 6 ; 0==16 1==32 2==64 3==128
                             ; 4==256 5==512 6==1024 7==2048
RTL8139_ERTXTH         equ 8 ; in unit of 32 bytes e.g:(8*32)=256
RTL8139_RX_MXDMA       equ 7 ; 0==16 1==32 2==64 3==128
                             ; 4==256 5==512 6==1024 7==unlimited
RTL8139_RXFTH          equ 7 ; 0==16 1==32 2==64 3==128
                             ; 4==256 5==512 6==1024 7==no threshold
RTL8139_RX_CONFIG      equ ((RTL8139_RBLEN shl RTL8139_BIT_RBLEN) \
                            or (RTL8139_RX_MXDMA shl RTL8139_BIT_RX_MXDMA) \
                            or (1 shl RTL8139_BIT_NOWRAP) \
                            or (RTL8139_RXFTH shl RTL8139_BIT_RXFTH) \
                            or (1 shl RTL8139_BIT_AB) or (1 shl RTL8139_BIT_APM) \
                            or (1 shl RTL8139_BIT_AER) or (1 shl RTL8139_BIT_AR) \
                            or (1 shl RTL8139_BIT_AM))
RTL8139_TX_TIMEOUT     equ 30 ; 300 milliseconds timeout
EE_93C46_REG_ETH_ID    equ 7 ; MAC offset
EE_93C46_READ_CMD      equ (6 shl 6) ; 110b + 6bit address
EE_93C56_READ_CMD      equ (6 shl 8) ; 110b + 8bit address
EE_93C46_CMD_LENGTH    equ 9 ; start bit + cmd + 6bit address
EE_93C56_CMD_LENGTH    equ 11 ; start bit + cmd + 8bit ddress

VER_RTL8139            equ 1100000b
VER_RTL8139A           equ 1110000b
;VER_RTL8139AG         equ 1110100b
VER_RTL8139B           equ 1111000b
VER_RTL8130            equ VER_RTL8139B
VER_RTL8139C           equ 1110100b
VER_RTL8100            equ 1111010b
VER_RTL8100B           equ 1110101b
VER_RTL8139D           equ VER_RTL8100B
VER_RTL8139CP          equ 1110110b
VER_RTL8101            equ 1110111b

IDX_RTL8139            equ 0
IDX_RTL8139A           equ 1
IDX_RTL8139B           equ 2
IDX_RTL8139C           equ 3
IDX_RTL8100            equ 4
IDX_RTL8139D           equ 5
IDX_RTL8139D           equ 6
IDX_RTL8101            equ 7

PCI_BASE_ADDRESS_0        equ 0x10  ;32 bit
PCI_BASE_ADDRESS_IO_MASK  equ 0xFFFFFFFE ;C

;----------------------------------------------------------------------
; STACK DATA
;----------------------------------------------------------------------

ETHER_IP                    equ     0x0008      ; Reversed from 0800 for intel
ETHER_ARP                   equ     0x0608      ; Reversed from 0806 for intel
ETHER_RARP                  equ     0x3580
ARP_REQ_OPCODE              equ     0x0100
ARP_REP_OPCODE              equ     0x0200


;*******************************************************************
;   Interface
;      The interfaces defined in ETHERNET.INC plus:
;      stack_init
;      stack_handler
;      app_stack_handler
;      app_socket_handler
;      checksum
;
;*******************************************************************
   
   
   
;
;   IP Packet after reception - Normal IP packet format
;
;    0                   1                   2                   3
;    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
;
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;0  |Version|  IHL  |Type of Service|       Total Length            |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;4  |         Identification        |Flags|      Fragment Offset    |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;8  |  Time to Live |    Protocol   |         Header Checksum       |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;12 |                       Source Address                          |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;16 |                    Destination Address                        |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |      Data                                                     |
;   +-+-+-..........                                               -+
   
  
;   TCP Payload ( Data field in IP datagram )
;
;    0                   1                   2                   3   
;    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;20 |          Source Port          |       Destination Port        |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;24 |                        Sequence Number                        |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;28 |                    Acknowledgment Number                      |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;32 |  Data |           |U|A|P|R|S|F|                               |
;   | Offset| Reserved  |R|C|S|S|Y|I|            Window             |
;   |       |           |G|K|H|T|N|N|                               |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;36 |           Checksum            |         Urgent Pointer        |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;40 |                    Options                    |    Padding    |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                             data                              
   
   
;
;   UDP Payload ( Data field in IP datagram )
;
;    0                   1                   2                   3
;    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
;
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |       Source Port             |      Destination Port         |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | Length ( UDP Header + Data )  |           Checksum            |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |       UDP Data                                                |
;   +-+-+-..........                                               -+
;

   
;
;  Socket Descriptor + Buffer
;
;    0                   1                   2                   3
;    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
;
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                    Status ( of this buffer )                  |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |  Application Process ID                                       |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                  Local IP Address                             |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | Local IP Port                 | Unused ( set to 0 )           |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   |                  Remote IP Address                            |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
;   | Remote IP Port                | Unused ( set to 0 )           |
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 24|   Rx Data Count                                   INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 28|                 TCB STATE                         INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 32|   TCB Timer (seconds)                             INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 36| ISS (Inital Sequence # used by this connection )   INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 40| IRS ( Inital Receive Sequence # )                  INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 44| SND.UNA  Seq # of unack'ed sent packets            INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 48| SND.NXT  Next send seq # to use                    INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 52| SND.WND  Send window                               INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 56| RCV.NXT  Next expected receive sequence #          INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 60| RCV.WND  Receive window                            INET format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 64| SEG.LEN  Segment length                           INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 68| SEG.WND  Segment window                           INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 72| Retransmit queue # NOW WINDOW SIZE TIMER          INTEL format|
;   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
; 76|       RX Data                                                 |
;   +-+-+-..........                                               -+
   

   
; IP protocol numbers
PROTOCOL_ICMP     equ      1
PROTOCOL_TCP      equ      6
PROTOCOL_UDP      equ      17
   
   
; TIPBUFF status values
BUFF_EMPTY         equ     0
BUFF_RX_FULL       equ     1
BUFF_ALLOCATED     equ     2
BUFF_TX_FULL       equ     3
   
NUM_IPBUFFERS      equ     20    ; buffers allocated for TX/RX
   
SOCK_EMPTY         equ     0        ; socket not in use
SOCK_OPEN          equ     1        ; open issued, but no data sent

; TCP opening modes
SOCKET_PASSIVE     equ     0
SOCKET_ACTIVE      equ     1
   
; TCP TCB states
TCB_LISTEN         equ        1
TCB_SYN_SENT       equ        2
TCB_SYN_RECEIVED   equ        3
TCB_ESTABLISHED    equ        4
TCB_FIN_WAIT_1     equ        5
TCB_FIN_WAIT_2     equ        6
TCB_CLOSE_WAIT     equ        7
TCB_CLOSING        equ        8
TCB_LAST_ACK       equ        9
TCB_TIME_WAIT      equ        10
TCB_CLOSED         equ        11
    
TWOMSL              equ     10      ; # of secs to wait before closing socket
   
; socket buffers
SOCKETBUFFSIZE     equ        4096  ; state + config + buffer.
SOCKETHEADERSIZE   equ        76    ; thus 4096 - SOCKETHEADERSIZE bytes data
                               
NUM_SOCKETS        equ        16    ; Number of open sockets supported. Was 20
   

NUMQUEUES          equ        4
EMPTY_QUEUE        equ        0
IPIN_QUEUE         equ        1
IPOUT_QUEUE        equ        2
NET1OUT_QUEUE      equ        3

NO_BUFFER          equ        0xFFFF            
IPBUFFSIZE         equ        1500                ; MTU of an ethernet packet
NUMQUEUEENTRIES    equ        NUM_IPBUFFERS
NUMRESENDENTRIES    equ         18              ; Buffers for TCP resend packets
TCP_RETRIES         equ         5               ; Number of times to resend a packet
TCP_TIMEOUT         equ         10              ; resend if not replied to in x hs
   
; These are the 0x40 function codes for application access to the stack
STACK_DRIVER_STATUS  equ   52
SOCKET_INTERFACE     equ   53

; 32 bit word   
stack_config         equ   stack_data      
; 32 bit word - IP Address in network format
stack_ip             equ   stack_data + 4                    
; 1 byte. 0 == inactive, 1 = active
slip_active          equ   stack_data + 8       ; no longer used
; 1 byte. 0 == inactive, 1 = active
ethernet_active      equ   stack_data + 9
unused               equ   stack_data + 10
;  word. Buffer number, -1 if none
rx_buff_ptr          equ   stack_data + 12
; dword. Buffer number, -1 if none
tx_buff_ptr          equ   stack_data + 16
; byte.
slip_rx_state        equ   stack_data + 20      ; no longer used
; byte
slip_tx_state        equ   stack_data + 21      ; no longer used
; dword. Index into data
rx_data_ptr          equ   stack_data + 22
; dword. Index into data
tx_data_ptr          equ   stack_data + 26
; word. Count of bytes to send
tx_msg_len           equ   stack_data + 30
; Address of selected socket
sktAddr              equ   stack_data + 32
; Parameter to checksum routine - data ptr
checkAdd1            equ   stack_data + 36
; Parameter to checksum routine - 2nd data ptr
checkAdd2            equ   stack_data + 40
; Parameter to checksum routine - data size
checkSize1           equ   stack_data + 44
; Parameter to checksum routine - 2nd data size
checkSize2           equ   stack_data + 46
; result of checksum routine
checkResult          equ   stack_data + 48
   
; holds the TCP/UDP pseudo header. SA|DA|0|prot|UDP len|
pseudoHeader         equ   stack_data + 50
   
; receive and transmit IP buffer allocation
sockets              equ   stack_data + 62
Next_free2           equ   sockets + (SOCKETBUFFSIZE * NUM_SOCKETS)
; 1560 byte buffer for rx / tx ethernet packets
Ether_buffer         equ   Next_free2
Next_free3           equ   Ether_buffer + 1560
last_1sTick          equ   Next_free3
IPbuffs              equ   Next_free3 + 1
queues               equ   IPbuffs + ( NUM_IPBUFFERS * IPBUFFSIZE )
queueList            equ   queues + (2 * NUMQUEUES)
last_1hsTick         equ   queueList + ( 2 * NUMQUEUEENTRIES )

;resendQ              equ   queueList + ( 2 * NUMQUEUEENTRIES )
;resendBuffer         equ   resendQ + ( 4 * NUMRESENDENTRIES ) ; for TCP
;                    equ    resendBuffer + ( IPBUFFSIZE * NUMRESENDENTRIES )



resendQ             equ     0x370000  ;0x770000
resendBuffer        equ     resendQ + ( 4 * NUMRESENDENTRIES ) ; for TCP

hdrtype:             db  0
vendor_device:       dd  0
pci_data:            dd  0
pci_dev:             dd  0
pci_bus:             dd  0

; These will hold pointers to the selected driver functions
drvr_probe:         dd  0
drvr_reset:         dd  0
drvr_poll:          dd  0
drvr_transmit:      dd  0

HwVerArray:   db VER_RTL8139, VER_RTL8139A, VER_RTL8139B, VER_RTL8139C
              db VER_RTL8100, VER_RTL8139D, VER_RTL8139CP, VER_RTL8101
HwVerArraySize = $-HwVerArray

; These two must be 4 byte aligned ( which they are )
align   4
rtl8139_rx_buff      equ     eth_data_start
rtl8139_tx_buff      equ     rtl8139_rx_buff + (RTL8139_RX_BUFFER_SIZE + MAX_ETH_FRAME_SIZE)

align   4
rtl8139_rx_buff_offset: dd 0
curr_tx_desc:           dd 0