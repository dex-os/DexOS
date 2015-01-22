 PartitionTable = 446
 ;=======================================================;
 ; FS_ReadWriteSectorSetUp                               ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;         EAX  = Unit (drive)                           ;
 ;           0  = Floppy                                 ;
 ;           1  = primary device (0x1f0), master(0x00)   ;
 ;           2  = primary device (0x1f0), slave(0x10)    ;
 ;           3  = secondary device (0x170), master(0x00) ;
 ;           4  = secondary device (0x170), slave(0x10)  ;
 ;                                                       ;
 ;         EDI  = Buffer                                 ;
 ;         ECX  = Count  (just one for now)              ;
 ;                                                       ;
 ;  Output:                                              ;
 ;          AH  = 0  for success                         ;
 ;          AH  = Nonzero for failure                    ;
 ;           1  = Not implemented yet                    ;
 ;           2  = Unit to high                           ;
 ;                                                (100%) ;
 ;.......................................................;

FS_ReadWriteSectorSetUp:
	pushad							; Save the regs
	mov  dword[HddScratchBuffer],edi			; Save the buffer address
	mov  dword[FS_RW_Sector],edx				; Save the sector size (In the current implementation, Sector size will always be 512).
	mov  dword[FS_RW_Count],ecx				; Count (In the current implementation, count will always be 1).
	mov  byte[FS_RW_SectorErrorNum],1			; Set the error flag for Not implemented yet (1)
	cmp  eax,0						; cmp if floppy
	je   FS_ReadWriteSectorSetUpError			; If so error as not done yet
	inc  [FS_RW_SectorErrorNum]				; Put the error code to 2
	cmp  eax,8						; Check if it more then 8
	ja   FS_ReadWriteSectorSetUpError			; if so exit with error
	push eax						; Save whats in eax
	shl  eax,2						; times it by 4
	mov  ebx,dword[BaseTable+eax]				; Use the look up table to find the base
	mov  dword[HddBaseFS],ebx				  ; Store it
	pop  eax						; get the saved eax
	and  al,00000001b					; See if its odd or even number
	xor  al,00000001b					; Flip bit 1 round
	shl  al,4						; Multiply by 16
	mov  byte[MasterSlave],al				; Store it as master or slave
	popad							; restore the regs
	xor  ah,ah						; post success
	ret							; Return

FS_ReadWriteSectorSetUpError:
	popad							; restore the regs
	mov   ah,byte[FS_RW_SectorErrorNum]			; post error number
	ret							; Return


 ;=======================================================;
 ; FS_ReadSector                                         ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;         EAX  = Unit (drive)                           ;
 ;           0  = Floppy                                 ;
 ;           1  = primary device (0x1f0), master(0x00)   ;
 ;           2  = primary device (0x1f0), slave(0x10)    ;
 ;           3  = secondary device (0x170), master(0x00) ;
 ;           4  = secondary device (0x170), slave(0x10)  ;
 ;                                                       ;
 ;           (upto 8 devices)                            ;
 ;                                                       ;
 ;         EDI  = Buffer to write the data               ;
 ;         EDX  = Sector number to read from             ;
 ;         ECX  = Count  Nunmber of sectors (just 1)     ;
 ;                                                       ;
 ;  Output:                                              ;
 ;          AH  = 0  for success                         ;
 ;          AH  = Nonzero for failure                    ;
 ;           1  = Not implemented yet                    ;
 ;           2  = Unit to high                           ;
 ;           3  = TimeOut                                ;
 ;                                                (100%) ;
 ;.......................................................;

FS_ReadSector:
	pushad							; Save the regs
	mov  dword[HddScratchBuffer],edi			; Save the buffer address
	mov  dword[FS_RW_Sector],edx				; Save the sector size (In the current implementation, Sector size will always be 512).
	mov  dword[FS_RW_Count],ecx				; Count (In the current implementation, count will always be 1).
	mov  byte[FS_RW_SectorErrorNum],1			; Set the error flag for Not implemented yet (1)
	cmp  eax,0						; cmp if floppy
	je   FS_ReadSectorError 				; If so error as not done yet
	inc  [FS_RW_SectorErrorNum]				; Put the error code to 2
	cmp  eax,8						; Check if it more then 8
	ja   FS_ReadSectorError 				; if so exit with error
	push eax						; Save whats in eax
	shl  eax,2						; times it by 4
	mov  ebx,dword[BaseTable+eax]				; Use the look up table to find the base
	mov  dword[HddBaseFS],ebx				  ; Store it
	pop  eax						; get the saved eax
	and  al,00000001b					; See if its odd or even number
	xor  al,00000001b					; Flip bit 1 round
	shl  al,4						; Multiply by 16
	mov  byte[MasterSlave],al				; Store it as master or slave
	mov  eax,dword[FS_RW_Sector]				; move sector number in to eax
	call FS_Read_Sector					; call the hdd read function
	popad							; restore the regs
	xor  ah,ah						; post success
	ret							; Return

FS_ReadSectorError:
	popad							; restore the regs
	mov   ah,byte[FS_RW_SectorErrorNum]			; post error number
	ret							; Return

 ;=======================================================;
 ; FS_WriteSector                                        ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;         EAX  = Unit (drive)                           ;
 ;           0  = Floppy                                 ;
 ;           1  = primary device (0x1f0), master(0x00)   ;
 ;           2  = primary device (0x1f0), slave(0x10)    ;
 ;           3  = secondary device (0x170), master(0x00) ;
 ;           4  = secondary device (0x170), slave(0x10)  ;
 ;                                                       ;
 ;           (upto 8 devices)                            ;
 ;                                                       ;
 ;         EDI  = Buffer to write the data               ;
 ;         EDX  = Sector number to read from             ;
 ;         ECX  = Count  Nunmber of sectors (just 1)     ;
 ;                                                       ;
 ;  Output:                                              ;
 ;          AH  = 0  for success                         ;
 ;          AH  = Nonzero for failure                    ;
 ;           1  = Not implemented yet                    ;
 ;           2  = Unit to high                           ;
 ;           3  = TimeOut                                ;
 ;                                                (100%) ;
 ;.......................................................;

FS_WriteSector:
	pushad							; Save the regs
	mov  dword[HddScratchBuffer],edi			; Save the buffer address
	mov  dword[FS_RW_Sector],edx				; Save the sector size (In the current implementation, Sector size will always be 512).
	mov  dword[FS_RW_Count],ecx				; Count (In the current implementation, count will always be 1).
	mov  byte[FS_RW_SectorErrorNum],1			; Set the error flag for Not implemented yet (1)
	cmp  eax,0						; cmp if floppy
	je   FS_WriteSectorError				; If so error as not done yet
	inc  [FS_RW_SectorErrorNum]				; Put the error code to 2
	cmp  eax,8						; Check if it more then 8
	ja   FS_WriteSectorError				; if so exit with error
	push eax						; Save whats in eax
	shl  eax,2						; times it by 4
	mov  ebx,dword[BaseTable+eax]				; Use the look up table to find the base
	mov  dword[HddBaseFS],ebx				  ; Store it
	pop  eax						; get the saved eax
	and  al,00000001b					; See if its odd or even number
	xor  al,00000001b					; Flip bit 1 round
	shl  al,4						; Multiply by 16
	mov  byte[MasterSlave],al				; Store it as master or slave
	mov  eax,dword[FS_RW_Sector]				; move sector number in to eax
	call FS_Write_Sector					; call the hdd read function
	popad							; restore the regs
	xor  ah,ah						; post success
	ret							; Return

FS_WriteSectorError:
	popad							; restore the regs
	mov   ah,byte[FS_RW_SectorErrorNum]			; post error number
	ret							; Return


 ;=======================================================;
 ; FS_ReadSector.                                        ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;         EAX  = Block to read                          ;
 ;                                                       ;
 ; NOTE: You should call FS_ReadWriteSectorSetUp         ;
 ;       on before the first time you call this function ;
 ;                                                (100%) ;
 ;.......................................................;

FS_Read_Sector:
	call HddReadFS						  ; Call the function
	ret							; Return


 ;=======================================================;
 ; FS_WriteSector.                                       ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;         EAX  = Block to write                         ;
 ;                                                       ;
 ; NOTE: You should call FS_ReadWriteSectorSetUp         ;
 ;       on before the first time you call this function ;
 ;                                                (100%) ;
 ;.......................................................;

FS_Write_Sector:
	call HddWriteFS 					  ; Call the function
	ret							; Return

 ;=======================================================;
 ; hd_read.                                              ;
 ;            ;  eax = block to read                     ;
 ;            ; [HddScratchBuffer] = buffer to write too ;
 ;=======================================================;
HddReadFS:
	pushad
	push  eax
newhdreadFS:
	mov edx,[HddBaseFS]
	inc   edx
	mov   al,0
	out   dx,al

	inc   edx
	mov   al,1
	out   dx,al

	inc   edx
	pop   ax
	out   dx,al

	inc   edx
	shr   ax,8
	out   dx,al

	inc   edx
	pop   ax
	out   dx,al

	inc   edx
	shr   ax,8
	and   al,1+2+4+8
	add   al,[MasterSlave]
	add   al,128+64+32
	out   dx,al

	inc   edx
	mov   al,20h
	out   dx,al
hddwaitFS:
	in    al,dx
	test  al,128
	jnz   hddwaitFS

	mov   edi,dword[HddScratchBuffer]
	mov   ecx,256
	mov edx,[HddBaseFS]
	cld
	rep   insw

	popad
	ret



 ;=======================================================;
 ; hd_write.    ; eax = block to write                   ;
 ;              ; HddScratchBuffer = buffer to read from ;
 ;=======================================================;
HddWriteFS:
	pushad 
	push  eax
newhdwrite:
	mov edx,[HddBaseFS]
	inc   edx
	mov   al,0
	out   dx,al

	inc   edx
	mov   al,1
	out   dx,al

	inc   edx
	pop   ax
	out   dx,al

	inc   edx
	shr   ax,8
	out   dx,al

	inc   edx
	pop   ax
	out   dx,al

	inc   edx
	shr   ax,8
	and   al,1+2+4+8
	add   al,[MasterSlave]
	add   al,128+64+32
	out   dx,al

	inc   edx
	mov   al,30h					; write with retry (without = 0x31)
	out   dx,al
hddwaitWrite:
	in    al,dx
	test  al,128
	jnz   hddwaitWrite

	mov   esi,dword[HddScratchBuffer]
	mov   ecx,256
	mov   edx,[HddBaseFS]
	cld
	rep outsw
	popad
	ret




 ;=======================================================;
 ; FS_GetPtnStart                                        ;
 ;-------------------------------------------------------;
 ;   Input:                                              ;
 ;          AL  = PartionNumber                          ;
 ;           0  = first                                  ;
 ;           1  = second                                 ;
 ;           2  = third                                  ;
 ;           3  = forth                                  ;
 ;                                                       ;
 ;         EDI  = ScratchBuffer                          ;
 ;         EDX  = Sector                                 ;
 ;         ECX  = Count                                  ;
 ;                                                       ;
 ;  Output:                                              ;
 ;          AH  = 0  for success                         ;
 ;          AH  = Nonzero for failure                    ;
 ;           1  = Partition number above 3               ;
 ;                                                       ;
 ;         ESI  = partition start                        ;
 ;         ECX  = number of blocks in partition          ;
 ;          DL  = file sys type                          ;
 ;          DH  = Active Byte (0x80 = bootable)          ;
 ;                                                (100%) ;
 ;.......................................................;

FS_GetPtnStart:
	pushad							 ; Save the regs
	push  es						 ; Save the regs
	push  ds						 ; Save the regs
	push  eax						 ; Save reg
	mov   ax,18h						 ; set ax to nonlinear base
	mov   ds,ax						 ; add them to ds
	mov   es,ax						 ; and es.
	pop   eax						 ; Restore reg
	mov   [FS_RW_SectorErrorNum],1				 ; Put error code 1 (partion number higher than 3 ).
	cmp   al,3						 ; cmp if above 3 (DOS ptable supports maximum 4 partitions)
	ja    FS_GetPtnStartError				 ; If so error
	mov   dword[HddScratchBuffer],edi			 ; Save the buffer address
	mov   byte[FS_GetPartionNumber],al			 ; Save the Partion Number.
	mov   eax,0						 ; set block to first (MBR)
	call  FS_ReadSector					 ; Call read hdd function
	mov   esi,dword[HddScratchBuffer]			 ; get the address of bufer
	add   esi,PartitionTable				 ; add the partition offset
	xor   ebx,ebx						 ; zero ebx
	mov   bl,[FS_GetPartionNumber]				 ; set bl to partition number
	shl   ebx,4						 ; Multy by 16
	add   esi,ebx						 ; Add it to partition offset
	mov   al,byte[ds:esi]					 ; get the active byte
	mov   byte[ActiveByte],al				 ; store it
	add   esi,4						 ; move the address 4 placers
	mov   al,byte[ds:esi]					 ; get the file sys type byte
	mov   byte[FileSysType],al				 ; Store it
	add   esi,4						 ; move the address 4 more places on
	mov   eax,dword[ds:esi] 				 ; get the first sector of partition LBA
	mov   dword[LbaFirstSecOfPartition],eax 		 ; store it
	add   esi,4						 ; move the address 4 more places on
	mov   eax,dword[ds:esi] 				 ; get the number of blocks in this partition
	mov   dword[NumBlocksInPartition],eax			 ; store it
	pop   ds						 ; restore the regs
	pop   es						 ; restore the regs
	popad							 ; restore the regs
	xor   ah,ah						 ; post success
	mov   dh,byte[ActiveByte]				 ; set dh Active Byte
	mov   dl,byte[FileSysType]				 ; set dl file sys type
	mov   esi,dword[LbaFirstSecOfPartition] 		 ; set esi first setor of partition
	mov   ecx,dword[NumBlocksInPartition]			 ; set ecx number of blocks in partition
	ret							 ; Return


FS_GetPtnStartError:
	pop   ds						 ; restore the regs
	pop   es						 ; restore the regs
	popad							 ; restore the regs
	mov   ah,byte[FS_RW_SectorErrorNum]			 ; post error number
	ret							 ; Return