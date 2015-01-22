;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                          ;;
;;          "BootProg" Loader v 1.2 by Alexei A. Frounze (c) 2000           ;;
;;                                                                          ;;
;;                                                                          ;;
;;                            Contact Information:                          ;;
;;                            ~~~~~~~~~~~~~~~~~~~~                          ;;
;; E-Mail:   alexfru@chat.ru                                                ;;
;; Homepage: http://alexfru.chat.ru                                         ;;
;; Mirror:   http://members.xoom.com/alexfru                                ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                  Thanks:                                 ;;
;;                                  ~~~~~~~                                 ;;
;;      Thanks Thomas Kjoernes (aka NowhereMan) for his excelent idea       ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                 Features:                                ;;
;;                                 ~~~~~~~~~                                ;;
;; - FAT16 supported                                                        ;;
;;                                                                          ;;
;; - Loads particular COM or EXE file placed to the root directory of a disk;;
;;   ("ProgramName" variable holds name of a file to be loaded)             ;;
;;                                                                          ;;
;; - Provides simple information about errors occured during load process   ;;
;;   ("RE" message stands for "Read Error",                                 ;;
;;    "NF" message stands for "file Not Found")                             ;;
;;                                                                          ;;
;;                                                                          ;;
;;                             Known Limitations:                           ;;
;;                             ~~~~~~~~~~~~~~~~~~                           ;;
;; - Works only on the 1st MBR partition which must be a PRI DOS partition  ;;
;;   with FAT16 (File System ID: 4, 6)                                      ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                Known Bugs:                               ;;
;;                                ~~~~~~~~~~~                               ;;
;; - All bugs are fixed as far as I know. The boot sector tested on my      ;;
;;   HDD.                                                                   ;;
;;                                                                          ;;
;;                                                                          ;;
;;                                Memory Map:                               ;;
;;                                ~~~~~~~~~~~                               ;;
;;                 ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿                               ;;
;;                 ³ Interrupt Vector Table ³ 0000                          ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³     BIOS Data Area     ³ 0040                          ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³ PrtScr Status / Unused ³ 0050                          ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³   Image Load Address   ³ 0060                          ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³    Available Memory    ³ nnnn                          ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³     2KB Boot Stack     ³ A000 - 512 - 2KB              ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                 ³       Boot Sector      ³ A000 - 512                    ;;
;;                 ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´                               ;;
;;                                            A000                          ;;
;;                                                                          ;;
;;                                                                          ;;
;;                   Boot Image Startup (register values):                  ;;
;;                   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                  ;;
;;  dl = boot drive number                                                  ;;
;;  cs:ip = program entry point                                             ;;
;;  ss:sp = program stack (don't confuse with boot sector's stack)          ;;
;;  COM program defaults: cs = ds = es = ss = 50h, sp = 0, ip = 100h        ;;
;;  EXE program defaults: ds = es = 50h, other stuff depends on EXE header  ;;
;;                                                                          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]

?			equ	0
ImageLoadSeg		equ	60h

[SECTION .text]
[ORG 0]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot sector starts here ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	jmp	short	start
	nop
bsOemName		DB	"BootProg"	; 0x03

;;;;;;;;;;;;;;;;;;;;;
;; BPB starts here ;;
;;;;;;;;;;;;;;;;;;;;;

bpbBytesPerSector	DW	?		; 0x0B
bpbSectorsPerCluster	DB	?		; 0x0D
bpbReservedSectors	DW	?		; 0x0E
bpbNumberOfFATs 	DB	?		; 0x10
bpbRootEntries		DW	?		; 0x11
bpbTotalSectors 	DW	?		; 0x13
bpbMedia		DB	?		; 0x15
bpbSectorsPerFAT	DW	?		; 0x16
bpbSectorsPerTrack	DW	?		; 0x18
bpbHeadsPerCylinder	DW	?		; 0x1A
bpbHiddenSectors	DD	?		; 0x1C
bpbTotalSectorsBig	DD	?		; 0x20

;;;;;;;;;;;;;;;;;;;
;; BPB ends here ;;
;;;;;;;;;;;;;;;;;;;

bsDriveNumber		DB	?		; 0x24
bsUnused		DB	?		; 0x25
bsExtBootSignature	DB	?		; 0x26
bsSerialNumber		DD	?		; 0x27
bsVolumeLabel		DB	"NO NAME    "	; 0x2B
bsFileSystem		DB	"FAT16   "	; 0x36

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Boot sector code starts here ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start:
	cld

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; How much RAM is there? ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	int	12h		; get conventional memory size (in KBs)
	shl	ax, 6		; and convert it to paragraphs

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for the boot sector and the stack ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	sub	ax, 512 / 16	; reserve 512 bytes for the boot sector code
	mov	es, ax		; es:0 -> top - 512

	sub	ax, 2048 / 16	; reserve 2048 bytes for the stack
	mov	ss, ax		; ss:0 -> top - 512 - 2048
	mov	sp, 2048	; 2048 bytes for the stack

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Copy ourself to top of memory ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov	cx, 256
	mov	si, 7C00h
	xor	di, di
	mov	ds, di
	rep	movsw

;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Jump to relocated code ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	push	es
	push	word main
	retf

main:
	push	cs
	pop	ds

	mov	[bsDriveNumber], dl	; store boot drive number

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for a FAT16 image (128KB max) and load it whole ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov	ax, [bpbBytesPerSector]
	shr	ax, 4			; ax = sector size in paragraphs
	mov	cx, [bpbSectorsPerFAT]	; cx = FAT size in sectors
	mul	cx			; ax = FAT size in paragraphs

	mov	di, ss
	sub	di, ax
	mov	es, di
	xor	bx, bx			; es:bx -> buffer for the FAT

	mov	ax, [bpbHiddenSectors]
	mov	dx, [bpbHiddenSectors+2]
	add	ax, [bpbReservedSectors]
	adc	dx, bx			; dx:ax = LBA

	call	ReadSector

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reserve some memory for a root directory and load it whole (16KB max) ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov	bx, ax
	mov	di, dx			; save LBA to di:bx

	mov	ax, 32
	mov	si, [bpbRootEntries]
	mul	si
	div	word [bpbBytesPerSector]
	mov	cx, ax			; cx = root directory size in sectors

	mov	al, [bpbNumberOfFATs]
	cbw
	mul	word [bpbSectorsPerFAT]
	add	ax, bx
	adc	dx, di			; dx:ax = LBA

	push	es			; push FAT segment (2nd parameter)

	push	word ImageLoadSeg
	pop	es
	xor	bx, bx			; es:bx -> buffer for root directory

	call	ReadSector

	add	ax, cx
	adc	dx, bx			; adjust LBA for cluster data

	push	dx
	push	ax			; push LBA for data (1st parameter)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Look for a COM/EXE program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov	di, bx			; es:di -> root entries array
	mov	dx, si			; dx = number of root entries
	mov	si, ProgramName 	; ds:si -> program name

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Looks for a file with particular name ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  DS:SI -> file name (11 chars) ;;
;;         ES:DI -> root directory array ;;
;;         DX = number of root entries   ;;
;; Output: SI = cluster number           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

FindName:
	mov	cx, 11
FindNameCycle:
	cmp	byte [es:di], ch
	je	FindNameFailed		; end of root directory
	pusha
	repe	cmpsb
	popa
	je	FindNameFound
	add	di, 32
	dec	dx
	jnz	FindNameCycle		; next root entry
FindNameFailed:
	jmp	ErrFind
FindNameFound:
	mov	si, [es:di+1Ah] 	; si = cluster no.

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load entire a program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadNextCluster:
	call	ReadCluster
	cmp	si, 0FFF8h
	jc	ReadNextCluster 	; if not End Of File

;;;;;;;;;;;;;;;;;;;
;; Type checking ;;
;;;;;;;;;;;;;;;;;;;

	cli				; for stack adjustments

	mov	ax, ImageLoadSeg
	mov	es, ax

	cmp	word [es:0], 5A4Dh	; "MZ" signature?
	je	RelocateEXE		; yes, it's an EXE program

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Setup and Run COM program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	mov	ax, es
	sub	ax, 10h 		; "org 100h" stuff :)
	mov	es, ax
	mov	ds, ax
	mov	ss, ax
	xor	sp, sp
	push	es
	push	word 100h
	jmp	Run

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Relocate, setup and run EXE program ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RelocateEXE:
	mov	ds, ax

	add	ax, [ds:08h]		; ax = image base
	mov	cx, [ds:06h]		; cx = reloc items
	mov	bx, [ds:18h]		; bx = reloc table pointer

	jcxz	RelocationDone

ReloCycle:
	mov	di, [ds:bx]		; di = item ofs
	mov	dx, [ds:bx+2]		; dx = item seg (rel)
	add	dx, ax			; dx = item seg (abs)

	push	ds
	mov	ds, dx			; ds = dx
	add	[ds:di], ax		; fixup
	pop	ds

	add	bx, 4			; point to next entry
	loop	ReloCycle

RelocationDone:

	mov	bx, ax
	add	bx, [ds:0Eh]
	mov	ss, bx			; ss for EXE
	mov	sp, [ds:10h]		; sp for EXE

	add	ax, [ds:16h]		; cs
	push	ax
	push	word [ds:14h]		; ip
Run:
	mov	dl, [cs:bsDriveNumber]	; let program know boot drive
	mov	dh, 0xff		; let DexOS know it booted from bootprog
	sti
	retf

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a FAT16 cluster      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Inout:  ES:BX -> buffer    ;;
;;         SI = cluster no    ;;
;; Output: SI = next cluster  ;;
;;         ES:BX -> next addr ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadCluster:
	mov	bp, sp

	lea	ax, [si-2]
	xor	ch, ch
	mov	cl, [bpbSectorsPerCluster]
		; cx = sector count
	mul	cx

	add	ax, [ss:bp+1*2]
	adc	dx, [ss:bp+2*2]
		; dx:ax = LBA

	call	ReadSector

	mov	ax, [bpbBytesPerSector]
	shr	ax, 4			; ax = paragraphs per sector
	mul	cx			; ax = paragraphs read

	mov	cx, es
	add	cx, ax
	mov	es, cx			; es:bx updated

	add	si, si			; si = cluster * 2

	push	ds
	mov	ax, [ss:bp+3*2] 	; ds = FAT segment
	jnc	First64
	add	ax, 1000h		; adjust segemnt for 2nd part of FAT16
First64:
	mov	ds, ax
	mov	si, [ds:si]		; si = next cluster
	pop	ds

ReadClusterDone:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Reads a sector using BIOS Int 13h fn 2 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Input:  DX:AX = LBA                    ;;
;;         CX    = sector count           ;;
;;         ES:BX -> buffer address        ;;
;; Output: CF = 1 if error                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ReadSector:
	push	es
	pusha

ReadSectorNext:
	mov	di, 5			; attempts to read

ReadSectorRetry:
	pusha

	div	word [bpbSectorsPerTrack]
		; ax = LBA / SPT
		; dx = LBA % SPT         = sector - 1

	mov	cx, dx
	inc	cx
		; cx = sector no.

	xor	dx, dx
	div	word [bpbHeadsPerCylinder]
		; ax = (LBA / SPT) / HPC = cylinder
		; dx = (LBA / SPT) % HPC = head

	mov	ch, al
		; ch = LSB 0...7 of cylinder no.
	shl	ah, 6
	or	cl, ah
		; cl = MSB 8...9 of cylinder no. + sector no.

	mov	dh, dl
		; dh = head no.

	mov	dl, [bsDriveNumber]
		; dl = drive no.

	mov	ax, 201h
					; al = sector count = 1
					; ah = 2 = read function no.

	int	13h			; read sectors
	jnc	ReadSectorDone		; CF = 0 if no error

	xor	ah, ah			; ah = 0 = reset function
	int	13h			; reset drive

	popa
	dec	di
	jnz	ReadSectorRetry 	; extra attempt
	jmp	short ErrRead

ReadSectorDone:
	popa
	dec	cx
	jz	ReadSectorDone2 	; last sector

	mov	di, [bpbBytesPerSector]
	shr	di, 4			; paragraphs per sector
	mov	si, es
	add	si, di
	mov	es, si			; adjust segment for next sector

	add	ax, 1
	adc	dx, 0			; adjust LBA for next sector
	jmp	short ReadSectorNext

ReadSectorDone2:
	popa
	pop	es
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error Messaging Code ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

ErrRead:
	mov	si, MsgErrRead
	jmp	short Error
ErrFind:
	mov	si, MsgErrFind
Error:
	mov	ah, 0Eh
	mov	bx, 7

	lodsb
	int	10h			; 1st char
	lodsb
	int	10h			; 2nd char

	jmp	short $ 		; hang

;;;;;;;;;;;;;;;;;;;;;;
;; String constants ;;
;;;;;;;;;;;;;;;;;;;;;;

MsgErrRead	db	"RE"
MsgErrFind	db	"NF"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Fill free space with zeroes ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		times (512-13-($-$$)) db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Name of a program to be load and run ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ProgramName	db	"KERNEL32EXE"	; name and extension must be padded
					; with spaces (11 bytes total)

;;;;;;;;;;;;;;;;;;;;;;;;;;
;; End of the sector ID ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

		dw	0AA55h
