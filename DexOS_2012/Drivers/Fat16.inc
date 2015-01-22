;=========================================================;
; Fat16                                        01/04/2011 ;
;---------------------------------------------------------;
;                                                         ;
; DexOS V0.05                                             ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
SectorSize  = 512
;=======================================================  ;
;  LoadMBR.                                               ;
;=======================================================  ; 
LoadMBR:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	[count],0				  ;
	mov	cl,1					  ;
	mov	ch,00					  ;
	mov	dh,00					  ;      
	mov	dl,byte[Drive]				  ;                                               
	call	FddReadB				  ;
	jc	LoadVbrError				  ;
UpDateBuffer:						  ;
;=======================================================  ;
;  Load MBR.                                              ;
;=======================================================  ; 
	mov	dword[buff],esi 			  ;
	mov	ecx,512					  ;
	mov	esi,dword[buff] 			  ;
	mov	edi,MbrBuffer				  ;
	rep	movsb					  ;
;=======================================================  ;
;  Find first partion.                                    ;
;=======================================================  ;        
	mov	esi,MbrBuffer				  ;
	add	esi,446 				  ;
	mov	al,byte[ds:esi] 			  ;
	cmp	al,80h					  ;
	jne	VolumeBootRecord			  ;
	mov	dx,word[ds:esi] 			  ; set DH/DL for INT 13 call
	mov	cx,word[ds:esi+2]			  ; set CH/CL for INT 13 call
	mov	[count],0				  ;
LoadVolumeBootRecord:					  ;
	call	FddReadB				  ;
	jc	LoadVbrError				  ;
VolumeBootRecord:					  ;
	mov	ecx,512					  ;
	mov	esi,dword[buff] 			  ;
	mov	edi,BpbFat				  ;
	rep	movsb					  ;
	jmp	LoadVBRSuccsess 			  ;
;=======================================================  ;
;  Find first partion.                                    ;
;=======================================================  ;     
LoadVBRSuccsess:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
;  Hdd VBR Error.                                         ;
;=======================================================  ;  
LoadVbrError:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	stc						  ;
	ret						  ;
							  ;
;=======================================================  ;
; DirFillRootUSB.                                         ;
;=======================================================  ;
DirFillRootUSB: 					  ;
	pushad						  ;
	cmp	[count],1				  ;
	jae	DirFillRootUSBSuccsess			  ;
DirFillRootFddUSB:					  ;
	mov	ax,word[bpbHiddenSectors]		  ;
	mov	dx,word[bpbHiddenSectors+2]		  ;
	add	ax,word [bpbReservedSectors]		  ;
	mov	[FatStartDx],dx 			  ;
	mov	[FatStart],ax				  ;
	xor	eax,eax 				  ;
	mov	ecx,eax 				  ;
	mov	al, byte [bpbNumberOfFATs]		  ;
	mul	word [bpbSectorsPerFAT] 		  ;
	mov	[FatSize2],ax				  ;
	xor	eax,eax 				  ;
	mov	ax,word [bpbSectorsPerFAT]		  ;
	mov	[FatSize1],ax				  ;
	mov	ax,[bpbRootEntries]			  ;
	mov	ecx, 32					  ;
	mul	cx					  ;
	div	word [bpbBytesPerSector]		  ;     
	mov	[RootSize],ax				  ;
;=======================================================  ;
; Root Dir Location - Reserved sectors + Fat sectors      ;
;=======================================================  ;                             
	xor	eax,eax 				  ;
	mov	edx,eax 				  ;
	mov	al, byte [bpbNumberOfFATs]		  ;
	mul	word [bpbSectorsPerFAT] 		  ;
	add	ax, [FatStart]				  ;
	mov	[RootStartDx],dx			  ;
	mov	[RootStart],ax				  ;
;=======================================================  ;
; Data start Reserved sectors + FAT sectors + Root        ;
;=======================================================  ;                        
	mov	cx,[RootSize]				  ;
	add	ax,cx					  ;
	mov	[DataStart],ax				  ;
	mov	[FloppyTest],0				  ;
	mov	[FileLoadCount],0			  ;
	mov	[FddSectorsLoadedCount],0		  ;
	mov	edi,Fat 				  ;
	mov	[FileLoadAddress],edi			  ;
	xor	eax,eax 				  ;
	mov	ecx,eax 				  ;
	mov	dx,[FatStartDx] 			  ;
	mov	ax,[FatStart]				  ;
	mov	cx,[FatSize1]				  ;
ReadSectorsLoopUSB1:					  ;
	mov	[FloppyTest],0				  ;
	call	ReadSectors				  ;
	jc	DirErrorUSB				  ;
	inc	ax					  ;
	loop	ReadSectorsLoopUSB1			  ;
	mov	[FloppyTest],0				  ;
	mov	[FileLoadCount],0			  ;
	mov	[FddSectorsLoadedCount],0		  ;
	mov	edi,RootDirBuffer			  ;
	mov	[FileLoadAddress],edi			  ;
	xor	eax,eax 				  ;
	mov	dx,[RootStartDx]			  ;
	mov	ax,[RootStart]				  ;
	mov	cx,[RootSize]				  ;
ReadSectorsLoopUSB2:					  ;
	mov	[FloppyTest],0				  ;
	call	ReadSectors				  ;
	jc	DirErrorUSB				  ;
	inc	ax					  ;
	loop	ReadSectorsLoopUSB2			  ;
	mov	[count],1				  ;
DirFillRootUSBSuccsess: 				  ;
	popad						  ;
	clc						  ;
	ret						  ;
							  ;
DirErrorUSB:						  ;
	popad						  ;
	stc						  ;
	ret						  ;
							  ;
;=======================================================  ;
; ReadSectors                                             ;
;=======================================================  ;
ReadSectors:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	call	LBAcHs					  ;
TryReadFloppyAgain:					  ;
	push	ax					  ;
	mov	ch,byte [absoluteTrack] 		  ;     
	mov	cl,byte [absoluteSector]		  ;             
	mov	dh,byte [absoluteHead]			  ;
	mov	dl,byte [bsDriveNumber] 		  ;     
	pop	ax					  ;
	call	FddReadB				  ;
	jc	FloppyReadError 			  ;
;=======================================================  ;
; Move data from DMA buffer, to image address.            ;
;=======================================================  ;
FloppyReadOK:						  ;
	mov	ecx,512					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	esi,[buff]				  ;
	mov	edi,[FileLoadAddress]			  ;
	add	edi,[FileLoadCount]			  ;
	rep	movsb					  ;
	mov	byte[es:edi],0				  ;
	add	[FileLoadCount],512			  ;
	add	[FddSectorsLoadedCount],1		  ;
;=======================================================  ;
; ExitSucsess.                                            ;
;=======================================================  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
; ExitError.                                              ;
;=======================================================  ;
FloppyReadError:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	stc						  ;
	ret						  ;
							  ;

F16FileSizeB dd 0					  ;
;=======================================================  ;
; RunCommand16.                                           ;
;=======================================================  ;
RunCommand16:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	dword[F16FileSizeB],0			  ;
	mov	[HddFileName],edi			  ;
	mov	[FileLoadAddress],edx			  ;
	mov	[FileLoadCount],0			  ;
	mov	[FddSectorsLoadedCount],0		  ;
	xor	ecx,ecx 				  ;
	mov	ax,[bpbRootEntries]			  ; 
	mov	ecx, 32					  ;                
	mul	cx					  ;                         
	div	word [bpbBytesPerSector]		  ;             
	mov	[RootSize],ax				  ;
;=======================================================  ;
; Root Dir Location - Reserved sectors + Fat sectors      ;
;=======================================================  ;                         
	mov	dx,ax					  ;
	xor	ax,ax					  ;
	mov	al, byte [bpbNumberOfFATs]		  ;
	mul	word[bpbSectorsPerFAT]			  ;
	add	ax,word[FatStart]			  ;     
	mov	[RootStart],ax				  ;
	mov	[RootStartDx],dx			  ;
;=======================================================  ;
; Data start Reserved sectors + FAT sectors + Root        ;
;=======================================================  ;
	mov	cx,[RootSize]				  ;     
	add	ax,cx					  ;     
	mov	[DataStart],ax				  ;     
	mov	ax,18h					  ; 
	mov	es,ax					  ;
	mov	ds,ax					  ;
	mov	edi,RootDirBuffer			  ;
	mov	esi,[HddFileName]			  ;
	mov	dx,[bpbRootEntries]			  ;
;=======================================================  ;
; FindName.                                               ;
;=======================================================  ;
FindName:						  ;
	mov	ecx,11					  ;
FindNameCycleFL:					  ;
	cmp	byte [es:edi],ch			  ;
	je	FindNameFailedFL			  ;        
	pushad						  ;                    
	repe	cmpsb					  ;
	popad						  ;       
	je	FindNameFoundFL 			  ;
	add	edi, 32 				  ;
	dec	dx					  ;
	jnz	FindNameCycleFL 			  ;  
FindNameFailedFL:					  ;
	jmp	ErrorFindingNameFat16			  ;
FindNameFoundFL:					  ;
	mov	dx,word[es:edi+1Ah]			  ; 
	mov	word [cluster],dx			  ;
	push	edx					  ;
	mov	edx,dword[es:edi+1ch]			  ;
	mov	dword[F16FileSizeB],edx 		  ;
	pop	edx					  ;
;=======================================================  ;
; Fat16.                                                  ;
;=======================================================  ;            
LoadImageFat16: 					  ;
	mov	ax,[cluster]				  ;            
ReadNextClusterFat16:					  ; 
	call	ClusterLBA				  ;
	mov	[clusterSPC],ax 			  ;
	xor	ecx,ecx 				  ; 
	mov	cl,byte [bpbSectorsPerCluster]		  ; 
ReadNextClusterFat16loop:				  ;
	push	ecx					  ;       
	mov	[FloppyTest],0				  ;    
	call	ReadSectors				  ;        
	jc	ErrorFloppyPop				  ;   
	inc	[clusterSPC]				  ;        
	mov	ax,[clusterSPC] 			  ;
	pop	ecx					  ;
	loop   ReadNextClusterFat16loop 		  ;
;=======================================================  ;
; compute next cluster.                                   ;
;=======================================================  ;                
	xor	ecx,ecx 				  ;
	xor	eax,eax 				  ;
	mov	ax,word[cluster]			  ;       
	mov	cx,ax					  ; 
	add	ecx,eax 				  ;   
	mov	esi,Fat 				  ;               
	add	esi,ecx 				  ;                       
	mov	dx,word [ds:esi]			  ;                       
	mov	word [cluster],dx			  ; 
	cmp	dx,0xFFF0				  ;   
	jb	LoadImageFat16				  ;
;=======================================================  ;
; ExitSucsess.                                            ;
;=======================================================  ;
ExitSucsess:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ebx,[FddSectorsLoadedCount]		  ;
	mov	ecx,dword[F16FileSizeB] 		  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
;  ErrorFind.                                             ;
;=======================================================  ;
ErrorFindingNameFat16:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ah,0xfc 				  ;
	stc						  ;
	ret						  ;
;=======================================================  ;
; ErrorFloppy.                                            ;
;=======================================================  ;
ErrorFloppyPop: 					  ;
	pop	ecx					  ;
ErrorFloppy:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	stc						  ;
	ret						  ;
							  ;
;=======================================================  ;
; ClusterLBA.                                             ;
;=======================================================  ;
ClusterLBA:						  ;
	sub	ax,2					  ;
	xor	ecx,ecx 				  ;
	mov	cl,byte [bpbSectorsPerCluster]		  ;
	mul	cx					  ;
	add	ax, word [DataStart]			  ;
	ret						  ;
							  ;
							  ;
;=======================================================  ;
; LBAcHs.                                                 ;
;=======================================================  ;
LBAcHs: 						  ;
	;xor    dx,dx                                     ;
	div	word [bpbSectorsPerTrack]		  ;
	inc	dl					  ;
	mov	byte [absoluteSector],dl		  ;
	xor	dx,dx					  ;
	div	word [bpbHeadsPerCylinder]		  ;
	mov	byte [absoluteHead],dl			  ;
	mov	byte [absoluteTrack],al 		  ;
	ret						  ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Write part                                              ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
							  ;
;=======================================================  ;
; WriteCommandFat16                                       ;
;=======================================================  ;
; IN:   EAX = File size in bytes to write                 ;
;       ESI = Name of file to write                       ;
;       EDI = Data to write                               ;
;                                                         ;
; OUT:  Carry set on error,                               ;
;        AX = Error code                                  ;
;                                                         ;
;       EBX = Number of sector written                    ;
;       Carry cleared on no errors                        ;
;=======================================================  ;
WriteCommandFat16:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	[ErrorCodeFat16],0			  ; Make sure the var is 0.
	mov	[HddSectorsWriteCountFat16],0		  ; Make sure the var is 0.
	mov	[FileLoadCountFat16],dword 0		  ; Make sure the var is 0.
	mov	dword[NameOfFileToMakeFat16],esi	  ; ESI should point to name of file, save it.
	cmp	byte[ds:esi],' '			  ; Check that the file name is valid.
	je	writeCommandNameErrorFat16		  ; If not jump to exit and set error flag.
	cmp	byte[ds:esi],0				  ; Check that the file name is valid.
	je	writeCommandNameErrorFat16		  ; If not jump to exit and set error flag.
	mov	dword[LoadFileFromAddressFat16],edi	  ; EDI should point to address of file, save it.
	mov	dword[FileSizeInBytesFat16],eax 	  ; EAX should = file size in bytes, save it.
	call	GetFileSizeInClusters			  ; Convert file size from bytes, to number of clusters.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).       
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing). 
	;call   LoadMBR                                   ; Make sure MBR has bean loaded.
	;jc     writeCommandErrorFat16                    ; If we have a error jump exit, set error flag.
	call	DirFillRootUSB				  ; Load FAT and ROOT dir.       
	jc	writeCommandErrorFat16			  ; If we have a error jump exit,set error flag.
	cmp	byte[FileSysTypeNew],12 		  ;
	jne	NotFat12_USectors			  ;
	call	UsedSectorsFat12Write			  ; We need to see if theres any space on the fdd.
	jc	writeCommandErrorFat16			  ;
	jmp	WasFat12_USectors			  ;
  NotFat12_USectors:					  ;
	cmp	byte[FileSysTypeNew],16 		  ;
	jne	writeCommandErrorFat16			  ; must be fat12 or fat16 to write   
	call	UsedSectorsFat16Write			  ; We need to see if theres any space on the hdd.
	jc     writeCommandErrorFat16			  ;
  WasFat12_USectors:					  ;
	mov	[ErrorCodeFat16],5			  ; Put the right error code in error var, for next bit.
	mov	eax,dword[FreeClustersFat16]		  ; We need to move nunber of free clusters into a reg, as memory to memory is not possable
	cmp	[FileSizeInClustersFat16],eax		  ; Can we fit the file on the Hdd ?.
	ja	writeCommandErrorFat16			  ; If not end with error.
	mov	[ErrorCodeFat16],4 ;0                     ; Put the right error code in error var, for next bit.
	call	FindNextFreeRootEntryFat16		  ; Need to find a free root entry.
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
	mov	esi,dword[NameOfFileToMakeFat16]	  ; We need the address of file towrite for next function, in esi.
	cmp	byte[FileSysTypeNew],12 		  ;
	jne	NotFat12_MakeNewFileFat 		  ;
	call	MakeNewFileFat12			  ;
	jc	writeCommandErrorFat16			  ;
	jmp	WasFat12_MakeNewFileFat 		  ;
  NotFat12_MakeNewFileFat:				  ;
	call	MakeNewFileFat16			  ; Call function.
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
  WasFat12_MakeNewFileFat:				  ;
	call	FillOutRootEntryFAT16			  ; Put name of file in root dir entry.
	cmp	byte[FileSysTypeNew],12 		  ;
	jne	NotFat12_UpdateClusterChain		  ;
	call	UpdateClusterChain12			  ;
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
	jmp	WasFat12_UpdateClusterChain		  ;
 NotFat12_UpdateClusterChain:				  ;
	cmp	byte[FileSysTypeNew],16 		  ;
	jne	writeCommandErrorFat16			  ;
	call	UpdateClusterChain			  ; Up date FAT.
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
  WasFat12_UpdateClusterChain:				  ;
	cmp	byte[FileSysTypeNew],12 		  ;
	jne	NotFat12_WriteFat			  ;
	call	WriteFat12				  ; Write file to floppy fat12
	jc	writeCommandErrorFat16			  ; 
	mov	dword[HddSectorsWriteCountFat16],ebx	  ; EBX is returned with number of sector wrote to hdd.
	jmp	WasFat12_WriteFat			  ;
  NotFat12_WriteFat:					  ;
	cmp	byte[FileSysTypeNew],16 		  ;            
	jne	writeCommandErrorFat16			  ;
	call	WriteFat16				  ; Write file to Hdd fat16
	jc	writeCommandErrorFat16			  ; 
	mov	dword[HddSectorsWriteCountFat16],ebx	  ; EBX is returned with number of sector wrote to hdd.
  WasFat12_WriteFat:					  ;
	cmp	byte[FileSysTypeNew],12 		  ;
	jne	NotFat12_DirFatRootUpdate12		  ;
	call	DirFatRootUpdate12			  ;
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
	jmp	WasFat12_DirFatRootUpdate12		  ;
  NotFat12_DirFatRootUpdate12:				  ;
	cmp	byte[FileSysTypeNew],16 		  ;
	jne	writeCommandErrorFat16			  ;
	call	DirFatRootUpdateFat16			  ; If not errors write new root and fat to hdd.
	jc	writeCommandErrorFat16			  ; If we have a error jump exit, set error flag.
  WasFat12_DirFatRootUpdate12:				  ;
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore DS.
	popad						  ; Restore 32bit general registers.
	mov	ebx,dword[HddSectorsWriteCountFat16]	  ; Number of Sectors written.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Error1.                                                 ;
;=======================================================  ;
writeCommandErrorFat16: 				  ; Label.
	pop   ds					  ; Restore DS.
	pop   es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	mov   ah,byte[ErrorCodeFat16]			  ; Return error code.
	stc						  ; Error :(.
	ret						  ; Return.
;=======================================================  ;
; Error2.                                                 ;
;=======================================================  ;
writeCommandNameErrorFat16:				  ; Label.
	pop   ds					  ; Restore DS.
	pop   es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	mov   ah,8					  ; Return error code.
	stc						  ; Error :(.                   
	ret						  ; Return.

;=======================================================  ;
; GetFileSizeInClusters.                                  ;
;=======================================================  ;
; IN:   EAX = File size in bytes to write                 ;
;                                                         ;
; OUT:  Carry set on error,                               ;
;        AX = Error code                                  ;
;                                                         ;
;       'FileSizeInClusters' var updated                  ;
;        with size in clusters                            ;
;       Carry cleared on no errors                        ;
;=======================================================  ;
GetFileSizeInClusters:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	push	eax					  ; Save whats in EAX, on stack.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).       
	mov	es,ax					  ; Now add AX to eS (NO direct segment reg addressing).       
	pop	eax					  ; Restore EAX.
	mov	dword[FileSizeInSectorsFat16],eax	  ; EAX should have size of file to write in bytes.
	cmp	dword[FileSizeInSectorsFat16],511	  ; Check if it only 1 sector, in size.
	ja	@f					  ; If jump if it bigger.
	mov	[FileSizeInSectorsFat16],1		  ; Only 1 sector.
	jmp	JustOneCluster				  ; Exit file is only 1 cluster in size.
@@:							  ; Label.
	xor	edx,edx 				  ; Make sure edx is 0.
	mov	eax,dword[FileSizeInSectorsFat16]	  ; Move file size into EAX (size is still in bytes).
	mov	ecx,512 				  ; Move sector size, (hard coded as 512b).
	div	ecx					  ; Div EAX and ECX.
	mov	dword[FileSizeInSectorsFat16],eax	  ; Store result 'FileSizeInSectors' var.
	cmp	edx,0					  ; Check any remainders ?.
	je	ExitFileSizeInSectorsFat16		  ; Jump over next bit if not.
	inc	[FileSizeInSectorsFat16]		  ; If remainder add 1 to 'FileSizeInSectors' var.
ExitFileSizeInSectorsFat16:				  ; Label.
	xor	edx,edx 				  ; Make sure edx is 0.
	xor	ebx,ebx 				  ; Make sure ebx is 0.
	mov	bl,[bpbSectorsPerCluster]		  ; Load the number of sectors per cluster into bl.
	cmp	[FileSizeInSectorsFat16],ebx		  ; Compare files size with 'SectorsPerCluster'
	jbe	JustOneCluster				  ; If below, then only need to write 1 clusters. 
	xor	edx,edx 				  ; Make sure edx is 0.
	mov	eax,[FileSizeInSectorsFat16]		  ; Move file size into EAX (size is still in bytes).
	mov	ecx,ebx 				  ; EBX, should still hold number of sectors per cluster,move it into ECX.
	div	ecx					  ; Div  EAX and ECX.
	mov	[FileSizeInClustersFat16],eax		  ; Store result 'FileSizeInClusters' var.
	cmp	edx,0					  ; Check any remainders ?.
	je	@f					  ; Jump over next bit if not.
	inc	[FileSizeInClustersFat16]		  ; If remainder add 1 to 'FileSizeInClusters' var.
@@:							  ; Label.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.
;=======================================================  ;
; Just need one cluster.                                  ;
;=======================================================  ;
JustOneCluster: 					  ; Label.
	mov	[FileSizeInClustersFat16],1		  ; File is only 1 cluster in size, put 1 'FileSizeInClusters' var.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; UsedSectorsFat16Write.                                  ;
;=======================================================  ;
UsedSectorsFat16Write:					  ; Name of function.
	pushad						  ; Save 32bit general registers.                 
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).       
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing).       
	mov	[FreeSectorsFat16],0			  ; Make sure the var is 0.
	mov	[UsedClusterCountFat16],0		  ; Make sure the var is 0.
	xor	ecx,ecx 				  ; Make sure ecx is 0.
	xor	edx,edx 				  ; Make sure edx is 0.
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ax,[bpbTotalSectors]			  ; We check if 'bpbTotalSectors'is used.
	cmp	ax,0					  ; Is it 0 ?.
	jnz	@f					  ; If not it's used, so jump over next bit.
	mov	eax,[bpbTotalSectorsBig]		  ; We need to use 'bpbTotalSectorsBig' instead.
@@:							  ; Label.
	mov	dx,[DataStart]				  ; The var has the start address of Data area in it.
	sub	eax,edx 				  ; Take the used sector from total sectors
	xor	edx,edx 				  ; Make sure edx is 0.
	mov	cl,[bpbSectorsPerCluster]		  ; Move number of sectors per cluster into CL.                 
	div	ecx					  ; DIV eax and ecx
	cmp	dx,0					  ; Check for any remainders
	je	@f					  ; No jump to label
	inc	eax					  ; If we have remainders add 1 to eax.
@@:							  ; Label.
	mov	cx,ax					  ; Move ax into cx for loop count.
	mov	word[DataSizeInClusters],ax		  ; Also store it for later.
	mov	esi,Fat 				  ; Put address of FAT in esi
	add	esi,4					  ; Add 4 as, we start from cluster 3 (2bytes per cluster, 4 + 2 clusters).
UsedSectorsLoopFat16:					  ; Label.
	lodsw						  ; load a word into AX.
	cmp	ax,0x0000				  ; Check for free cluster.
	je	@f					  ; If so jump to label.
	inc	[UsedClusterCountFat16] 		  ; Not 0, then inc used count.
@@:							  ; Label.
	loop	UsedSectorsLoopFat16			  ; CX not 0 loop again.
	mov	ax,word[DataSizeInClusters]		  ; Load data area size into AX.
	sub	ax,[UsedClusterCountFat16]		  ; Take away used clusters.
	mov	[FreeClusterCountFat16],ax		  ; Save number of free clusters.
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ecx,eax 				  ; Make sure ecx is 0.
	mov	edx,eax 				  ; Make sure edx is 0.
	mov	ax,[FreeClusterCountFat16]		  ; Move number of free clusters into AX.
	mov	cl,[bpbSectorsPerCluster]		  ; Move number of sectors clusters into CL.
	mul	ecx					  ; MUL EAX and ECX.
	mov	[FreeSectorsFat16],eax			  ; Save results in var
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ax,[FreeClusterCountFat16]		  ; Mov free clusters number into AX.
	mov	[FreeClustersFat16],eax 		  ; Pack free clusters number size from 16bit to 32bit and save.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; UsedSectorsFat12Write.                                  ;
;=======================================================  ;
UsedSectorsFat12Write:					  ; Name of function.
	pushad						  ; Save 32bit general registers.                 
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).       
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing).       
	mov	[FreeSectorsFat16],0			  ; Make sure the var is 0.
	mov	[UsedClusterCountFat16],0		  ; Make sure the var is 0.
	xor	ecx,ecx 				  ; Make sure ecx is 0.
	xor	edx,edx 				  ; Make sure edx is 0.
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ax,[bpbTotalSectors]			  ; We check if 'bpbTotalSectors'is used.
	cmp	ax,0					  ; Is it 0 ?.
	jnz	@f					  ; If not it's used, so jump over next bit.
	mov	eax,[bpbTotalSectorsBig]		  ; We need to use 'bpbTotalSectorsBig' instead.
@@:							  ; Label.
	mov	dx,[DataStart]				  ; The var has the start address of Data area in it.
	sub	eax,edx 				  ; Take the used sector from total sectors
	xor	edx,edx 				  ; Make sure edx is 0.
	mov	cl,[bpbSectorsPerCluster]		  ; Move number of sectors per cluster into CL.                 
	div	ecx					  ; DIV eax and ecx
	cmp	dx,0					  ; Check for any remainders
	je	@f					  ; No jump to label
	inc	eax					  ; If we have remainders add 1 to eax.
@@:							  ; Label.
	mov	cx,ax					  ; Move ax into cx for loop count.
	mov	word[DataSizeInClusters],ax		  ; Also store it for later.
	mov	esi,Fat 				  ; Put address of FAT in esi
	add	esi,3					  ; Add 3 as, we start from cluster 3 (2bytes per cluster, 4 + 2 clusters).
UsedSectorsLoopFat12:					  ;
	lodsw						  ;
	and	ax,0x0FFF				  ;
	jz	@f					  ;
	inc	[UsedClusterCountFat16] 		  ; Not 0, then inc used count.
  @@:							  ;
	dec	esi					  ;
	lodsw						  ;
	shr	ax,4					  ;
	or	ax,ax					  ;
	jz	@f					  ;
	inc	[UsedClusterCountFat16] 		  ; Not 0, then inc used count.
  @@:							  ;
	loop	UsedSectorsLoopFat12			  ;
	mov	ax,word[DataSizeInClusters]		  ; Load data area size into AX.
	sub	ax,[UsedClusterCountFat16]		  ; Take away used clusters.
	mov	[FreeClusterCountFat16],ax		  ; Save number of free clusters.
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ecx,eax 				  ; Make sure ecx is 0.
	mov	edx,eax 				  ; Make sure edx is 0.
	mov	ax,[FreeClusterCountFat16]		  ; Move number of free clusters into AX.
	mov	cl,[bpbSectorsPerCluster]		  ; Move number of sectors clusters into CL.
	mul	ecx					  ; MUL EAX and ECX.
	mov	[FreeSectorsFat16],eax			  ; Save results in var
	xor	eax,eax 				  ; Make sure eax is 0.
	mov	ax,[FreeClusterCountFat16]		  ; Mov free clusters number into AX.
	mov	[FreeClustersFat16],eax 		  ; Pack free clusters number size from 16bit to 32bit and save.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; FindNextFreeRootEntryFat16.                             ;
;=======================================================  ;
FindNextFreeRootEntryFat16:				  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	mov	ax,18h					  ; Move data descriptors into AX.       
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing).       
	mov	edi,RootDirBuffer			  ; Get the root buffer address.
	mov	dx,[bpbRootEntries]			  ; ccccccccccccccccccccc
FindNextFreeRootEntryLoop:				  ; Label.
	cmp	byte[es:edi],byte 0			  ; Test for a emtpy root entry.
	je	FoundSpaceInRootdir			  ; Is it emtpy ?
	cmp	byte[es:edi],0xe5			  ; Test for deleted file
	je	FoundSpaceInRootdir			  ; If so we can use it.
	add	edi, 32 				  ; Try the next entry.
	dec	dx					  ; Take 1 from loop number var.
	jnz	FindNextFreeRootEntryLoop		  ; If more to check then loop.
	jmp	FindNextFreeRootEntryError		  ; No more end with error.
FoundSpaceInRootdir:					  ; Label.
	mov	dword[RootDirEntryAddressFat16],edi	  ; Save the free root entry address.
FindNextFreeRootEntrySucsess:				  ; Label.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Error.                                                  ;
;=======================================================  ;
FindNextFreeRootEntryError:				  ; Label.
	mov	[ErrorCodeFat16],3			  ; Error so set right error number.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	stc						  ; Errors :(.
	ret						  ; Return.

;=======================================================  ;
; MakeNewFileFat16                                        ;
;=======================================================  ;
MakeNewFileFat16:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	call	FindFileNameFat16			  ; Check for file of same name.
	jnc	CheckForSameNameFound			  ; Is there a file same name ?.
CheckForSameNameNotFound:				  ; Label.
	mov	edi,FileEntry				  ; We need to set a root entry out.
	mov	esi,NameOfFileToLoadBufferFat16 	  ; First name of file.
	mov	ecx,11					  ; Number of letters 8+3 ;;;;;
	rep	movsb					  ; move from a to b.
	call	GetNextCluster				  ; Get the next free cluster for next bit.
	jc	CheckForSameNameFound			  ; Jump error exit if no free clusters.
	mov	edi,FileCluster 			  ; Point it to the cluster address.
	mov	ax,[ClusterCount]			  ; Move next free cluster number into AX
	mov	word [StartCluster],ax			  ; Move AX into start cluster var.
	stosw						  ; Move next free cluster into 'FileCluster'
	mov	edi,FileSize				  ; Get file size address   
	mov	eax,dword[FileSizeInBytesFat16] 	  ; Move file size in bytes into eax
	stosd						  ; Set file size in root entry.
	call	FloppyTime				  ; Call time stamp function.
	mov	edi,FileTime				  ; Put address of filetime into edi.
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileCreateTime			  ;
	stosw						  ; Move whats in AX to address edi points to.
	call	FloppyDate				  ; Call Date stamp function.
	mov	edi,FileDate				  ; Put address of fileDate into edi.
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileCreateDate			  ;
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileAccessDate			  ;
	stosw						  ;
	mov	edi,FileAttr				  ; Put address of FileAttr into edi.
	mov	al,byte[FileAttrVar]			  ; Move attr into AX
	stosb						  ; Set File Attr.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Check For Same Name Found                               ;
;=======================================================  ;
CheckForSameNameFound:					  ; Label.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	mov	[ErrorCodeFat16],4			  ; Error so set right error number.
	stc						  ; Errors :(.
	ret						  ; Return.

;=======================================================  ;
; FindFileNameFat16.                                      ;
;=======================================================  ;
FindFileNameFat16:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	mov	dword[RootFileAddressFat16],0		  ; Make sure the var is 0.
	mov	dword[NameOfFileToLoadFat16],esi	  ; Save name of file to write.
	call	ConvertFileNameFat16			  ; Convert name for uses in root entry.
	mov	edi,NameOfFileToLoadBufferFat16 	  ; Converted name should be here.
	call	UpperCaseFat16				  ; Convert name to upper case.
	mov	edi,RootDirBuffer			  ; Move root buffer address into edi
	mov	esi,NameOfFileToLoadBufferFat16 	  ; Move name buffer address into esi.
	mov	dx,[bpbRootEntries]			  ; Number of root dir entrys, load in to DX.
CheckForSameName:					  ; Label.      
	mov	ecx,11					  ; Number of loops ;;;;;;;
CheckForSameNameLoop:					  ; Label.    
	cmp	byte [es:edi],ch			  ; Check for no more entrys.
	je	CheckForSameNameNotFoundinRootDir	  ; If so exit.                         
	pushad						  ; Save 32bit general registers.               
	repe	cmpsb					  ; Compare them.
	popad						  ; Restore 32bit general registers.                         
	je	CheckForSameNameFoundinRootDir		  ; Jume = to found file same name label
	add	edi, 32 				  ; If not try the next root entry.
	dec	dx					  ; Sub 1 off loop count.
	jnz	CheckForSameNameLoop			  ; If 0 end.
CheckForSameNameNotFoundinRootDir:			  ; Label.      
	popad						  ; Restore 32bit general registers.
	stc						  ; Errors :(, but we want that here :).
	ret						  ; Return.
;=======================================================  ;
; CheckForSameNameFoundinRootDir                          ;
;=======================================================  ;
CheckForSameNameFoundinRootDir: 			  ; Label.    
	mov	dword[RootFileAddressFat16],edi 	  ; Store the address in root entry, file by same name.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :), but we want that here :(.
	ret						  ; Return.

;=======================================================  ;
; MakeNewFileFat12                                        ;
;=======================================================  ;
MakeNewFileFat12:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	call	FindFileNameFat16			  ; Check for file of same name.
	jnc	CheckForSameNameFound12 		  ; Is there a file same name ?.
CheckForSameNameNotFound12:				  ; Label.
	mov	edi,FileEntry				  ; We need to set a root entry out.
	mov	esi,NameOfFileToLoadBufferFat16 	  ; First name of file.
	mov	ecx,11					  ; Number of letters 8+3 ;;;;;;;;
	rep	movsb					  ; move from a to b.
	call	GetNextCluster12			  ; Get the next free cluster for next bit.
	jc	CheckForSameNameFound12 		  ; Jump error exit if no free clusters.
	mov	edi,FileCluster 			  ; Point it to the cluster address.
	mov	ax,[ClusterCount]			  ; Move next free cluster number into AX
	mov	word [StartCluster],ax			  ; Move AX into start cluster var.
	stosw						  ; Move next free cluster into 'FileCluster'
	mov	edi,FileSize				  ; Get file size address   
	mov	eax,dword[FileSizeInBytesFat16] 	  ; Move file size in bytes into eax
	stosd						  ; Set file size in root entry.
	call	FloppyTime				  ; Call time stamp function.
	mov	edi,FileTime				  ; Put address of filetime into edi.
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileCreateTime			  ;
	stosw						  ; Move whats in AX to address edi points to.
	call	FloppyDate				  ; Call Date stamp function.
	mov	edi,FileDate				  ; Put address of fileDate into edi.
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileCreateDate			  ;
	stosw						  ; Move whats in AX to address edi points to.
	mov	edi,FileAccessDate			  ;
	stosw						  ;
	mov	edi,FileAttr				  ; Put address of FileAttr into edi.
	mov	al,byte[FileAttrVar]			  ; Move attr into AX
	stosb						  ; Set File Attr.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Check For Same Name Found                               ;
;=======================================================  ;
CheckForSameNameFound12:				  ; Label.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	mov	[ErrorCodeFat16],4			  ; Error so set right error number.
	stc						  ; Errors :(.
	ret						  ; Return.

;=======================================================  ;
; GetNextCluster.                                         ;
;=======================================================  ;    
GetNextCluster12:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	mov	ax,word[DataSizeInClusters]		  ; Get data size in clusters.
	mov	[MaxClusters],1534			  ; Max number of clusters for fat12
	mov	esi,Fat 				  ; Get fat address.
	add	esi,3					  ; Add 4 to address.
	mov	[ClusterCount],2			  ; Add first 2 clusters to count var.
	xor	ecx,ecx 				  ;
	mov	cx,[MaxClusters]			  ; Get max number of loops                            
GetNextClusterLoop12:					  ;
	lodsw						  ;
	and	ax,0x0FFF				  ;
	jz	FoundFreeCluster12			  ;
	inc	[ClusterCount]				  ;
	dec	esi					  ;
	lodsw						  ;
	shr	ax,4					  ;
	or	ax,ax					  ;
	jz	FoundFreeCluster12			  ;
	inc	[ClusterCount]				  ;
	loop	GetNextClusterLoop12			  ;
	popad						  ; Restore 32bit general registers. 
	stc						  ; Error, run out of free clusters.
	ret						  ; Return.
;=======================================================  ;
; FoundFreeCluster.                                       ;
;=======================================================  ;    
FoundFreeCluster12:					  ; Label.
	popad						  ; Restore 32bit general registers. 
	clc						  ; No errors :).
	ret						  ; Return.

;=======================================================  ;
; ConvertFileNameFat16.                                   ;
;=======================================================  ;
ConvertFileNameFat16:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	mov	esi,[NameOfFileToLoadFat16]		  ; Move name of file to convert.
	mov	edi,NameOfFileToLoadBufferFat16 	  ; Buffer address used for converting.
	mov	ecx,8					  ; Number of letters
ConvertCliFileNameU:					  ; Label.  
	cmp	byte [ds:esi],'.'			  ; Check a .
	je	@f					  ; Jump = to label.
	cld						  ; Set left to right.
	movsb						  ; Move a byte.
	dec	cx					  ; dec count.
	jnz	ConvertCliFileNameU			  ; If not 0 do another loop.
	jmp	FinConertCliFnameU			  ; If 0 jump label
@@:							  ; Label.  
	mov	al,0x20 				  ; Put a space code in al.
	cld						  ; Set left to right.
	rep	stosb					  ; loop till cx is 0
FinConertCliFnameU:					  ; Label. 
	cmp	byte [ds:esi],'.'			  ; Check a . ( have not been here before).
	jne	@f					  ; Not = jump other next bit.
	inc	esi					  ; Inc the pointer
@@:							  ; Label. 
	mov	cx,3					  ; Number of Ext
	cld						  ; Set left to right.
	rep	movsb					  ; Move from a to b.
	mov	al,0					  ; We need a 0 at end of name.
	stosb						  ; So let put one there.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; UpperCaseFat16.                                         ;
;=======================================================  ;
UpperCaseFat16: 					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	mov	ax,18h					  ; Move data descriptors into AX.            
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing).       
UcaseNextChar16:					  ; Label.
	mov	al,byte[es:edi] 			  ; Move the first letter.
	cmp	al,0					  ; Is it 0
	je	UcaseDone16				  ; If yes, then we are done.
	cmp	al,0x61 				  ; Test being below 0x61
	jb	DontUcaseChar16 			  ; If so do another loop
	cmp	al,0x7a 				  ; Test for above 0x7a
	ja	DontUcaseChar16 			  ; If so do another loop
	sub	al,0x20 				  ; If not take 0x20 off number
	mov	byte[es:edi],al 			  ; Put it back modithed 
DontUcaseChar16:					  ; Label.
	inc	edi					  ; inc the pointer
	jmp	UcaseNextChar16 			  ; Ant try next char.
UcaseDone16:						  ; Label.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; FillOutRootEntryFAT16.                                  ;
;=======================================================  ;
FillOutRootEntryFAT16:					  ; Name of function.
       pushad						  ; Save 32bit general registers.
       push    es					  ; Save ES.
       push    ds					  ; Save ES.
       mov     ax,18h					  ; Move data descriptors into AX.
       mov     ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).            
       mov     es,ax					  ; Now add AX to ES (NO direct segment reg addressing).       
       mov     edi,[RootDirEntryAddressFat16]		  ; Get the destanation address in edi.
       mov     esi,FileEntry				  ; Get the source address in esi.
       mov     ecx,8					  ; Get number of letters.
       rep     movsd					  ; Move from A to B.
       pop     ds					  ; Restore DS.
       pop     es					  ; Restore ES.
       popad						  ; Restore 32bit general registers.
       ret						  ; Return.

;=======================================================  ;
; UpdateClusterChain.                                     ;
;=======================================================  ;
UpdateClusterChain:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).    
	mov	es,ax					  ; Now add AX to eS (NO direct segment reg addressing).   
	mov	ax,[FileCluster]			  ; Get start cluster.
	cmp	[FileSizeInClustersFat16],1		  ; Test if we only need 1 cluster.
	jbe	EndOfFile				  ; If so jump to exit and mark end cluster.
	mov	ecx,dword[FileSizeInClustersFat16]	  ; If not get hoe many clusters we need.
	dec	ecx					  ; Sub 1 from number.
@@:							  ; Label.
	push	ecx					  ; Save number of cluster for latter.
	call	GetNextTwoClusters			  ; Find the next two free clusters.
	jc	UpdateClusterChainError 		  ; If we have a error jump exit, set error flag eg: no free cluster..
	mov	ax,[ClusterOne] 			  ; Save cluster A.
	mov	bx,[ClusterTwo] 			  ; Save cluster B.
	call	UpdateClusterValue			  ; Up date FAT.
	pop	ecx					  ; Get saved 'number of cluster'.
	loop	@b					  ; Not 0 do another loop.
	mov	ax,[ClusterTwo] 			  ; Save the sccond to lest cluster.
EndOfFile:						  ; Label.
	mov	bx,0xffff				  ; Put the last cluster mark in BX.
	call	UpdateClusterValue			  ; Write them to fat.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Error.                                                  ;
;=======================================================  ;
UpdateClusterChainError:				  ; Label.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore eS.
	popad						  ; Restore 32bit general registers.
	stc						  ; Errors :(.
	ret						  ; Return.

;=======================================================  ;
; UpdateClusterChain12.                                   ;
;=======================================================  ;
UpdateClusterChain12:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	ax,18h					  ; Move data descriptors into AX.
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).    
	mov	es,ax					  ; Now add AX to eS (NO direct segment reg addressing).   
	mov	ax,[FileCluster]			  ; Get start cluster.
	cmp	[FileSizeInClustersFat16],1		  ; Test if we only need 1 cluster.
	jbe	EndOfFile12				  ; If so jump to exit and mark end cluster.
	mov	ecx,dword[FileSizeInClustersFat16]	  ; If not get hoe many clusters we need.
	dec	ecx					  ; Sub 1 from number.
@@:							  ; Label.
	push	ecx					  ; Save number of cluster for latter.
	call	GetNextTwoClusters12			  ; Find the next two free clusters.
	jc	UpdateClusterChainError12		  ; If we have a error jump exit, set error flag eg: no free cluster..
	mov	ax,[ClusterOne] 			  ; Save cluster A.
	mov	bx,[ClusterTwo] 			  ; Save cluster B.
	call	UpdateClusterValue12			  ; Up date FAT.
	pop	ecx					  ; Get saved 'number of cluster'.
	loop	@b					  ; Not 0 do another loop.
	mov	ax,[ClusterTwo] 			  ; Save the sccond to lest cluster.
EndOfFile12:						  ; Label.
	mov	bx,0x0fff				  ; Put the last cluster mark in BX.
	call	UpdateClusterValue12			  ; Write them to fat.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore ES.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Error.                                                  ;
;=======================================================  ;
UpdateClusterChainError12:				  ; Label.
	pop	ds					  ; Restore DS.
	pop	es					  ; Restore eS.
	popad						  ; Restore 32bit general registers.
	stc						  ; Errors :(.
	ret						  ; Return.

;=======================================================  ;
; GetNextTwoClusters12.                                   ;
;=======================================================  ;
GetNextTwoClusters12:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
        xor     ecx,ecx
	mov	ax,word[DataSizeInClusters]		  ; Get data area size inclusters.
	mov	[MaxClusters],ax			  ; Put it in MaxCluster var.
	mov	esi,Fat 				  ; Get address of FAT
	add	esi,3					  ; Jump other fist 2 clusters.
	mov	[ClusterCount],2			  ; set some vars
	mov	[Clusterloop],0 			  ; Make sure the var is 0
	mov	cx,[MaxClusters]			  ; Set the number of loops.      
GetNextTwoClusterLoop12:				  ; Label.
	lodsw						  ; Load the first cluster.
	and	ax,0x0FFF				  ; Check for 0.
	jz	FoundFreeClusterTwoA12			  ; If its free jump to save address.
	inc	[ClusterCount]				  ; If not inc address.
	dec	esi					  ;
GetNextFreeCluster12:					  ; Label.
	lodsw						  ;
	shr	ax,4					  ;
	or	ax,ax					  ;
	jz	FoundFreeClusterTwoB12			  ; If its free jump to save address.
	inc	[ClusterCount]				  ; If not inc address.
	loop	GetNextTwoClusterLoop12 		  ; Not 0 loop.
	mov	[ErrorCodeFat16],5			  ; If we end up here, we have a run out of clusters error.
	popad						  ; Restore 32bit general registers.
	stc						  ; Error :(.
	ret						  ; Return.

;=======================================================  ;
; FoundFreeClusterTwoA                                    ;
;=======================================================  ;
FoundFreeClusterTwoA12: 				  ; Name of function.
	dec	esi					  ;
	cmp	[Clusterloop],1 			  ; Is this the first time we have come here?.
	je	@f					  ; Label.
	mov	ax,[ClusterCount]			  ; Get the cluster address
	mov	[ClusterOne],ax 			  ; Save it for latter.
	mov	[Clusterloop],1 			  ; Mark we have been here before.
	inc	[ClusterCount]				  ; Inc the count now instead of later.
	jmp	GetNextFreeCluster12			  ; Jump back to get one more cluster address.       
@@:							  ; Label.
	mov	ax,[ClusterCount]			  ; Move cluster address into AX
	mov	[ClusterTwo],ax 			  ; Put it in the second of the two clusters.
	popad						  ; Restore 32bit general registers.
	clc						  ; No Errors :).
	ret						  ; Return.

;=======================================================  ;
; FoundFreeClusterTwoB12                                  ;
;=======================================================  ;
FoundFreeClusterTwoB12: 				  ;
	cmp	[Clusterloop],1 			  ;
	je	@f					  ;
	mov	ax,[ClusterCount]			  ;
	mov	[ClusterOne],ax 			  ;
	mov	[Clusterloop],1 			  ;
	inc	[ClusterCount]				  ;
	jmp	GetNextTwoClusterLoop12 		  ;
  @@:							  ;
	mov	ax,[ClusterCount]			  ;
	mov	[ClusterTwo],ax 			  ;
	popad						  ;
	clc						  ;
	ret						  ;

;=======================================================  ;
; UpdateClusterValue12.                                   ;
;=======================================================  ;
UpdateClusterValue12:					  ;
	pushad						  ;
	mov	word[Cluster],ax			  ;
	mov	word[UpdateValue],bx			  ;
	xor	ecx,ecx 				  ;
	mov	cx,ax					  ;                                     
	mov	dx,ax					  ;                             
	shr	dx,0x0001				  ;                     
	add	cx,dx					  ;                             
	mov	esi,Fat 				  ;                     
	add	esi,ecx 				  ;
	mov	dx,word [ds:esi]			  ;
	test	ax,0x0001				  ;
	jnz	OddUpdateClusterValue12 		  ;
EvenUpdateClusterValue12:				  ;
	mov	bx,dx					  ;
	and	bx,0000111111111111b			  ;
	mov	[ClustOrigValue ],bx			  ;
	and	dx,1111000000000000b			  ;
	mov	bx,word[UpdateValue]			  ;
	and	bx,0000111111111111b			  ;
	add	dx,bx					  ;
	mov	word [ds:esi],dx			  ;      
	jmp	DoneUpdateClusterValue12		  ;
OddUpdateClusterValue12:				  ;
	mov	bx,dx					  ;
	shr	bx,4					  ;
	mov	[ClustOrigValue],bx			  ;
	and	dx,0000000000001111b			  ;
	mov	bx,word[UpdateValue]			  ;
	shl	bx,4					  ;
	add	dx,bx					  ;
	mov	word [ds:esi],dx			  ;
DoneUpdateClusterValue12:				  ;
	popad						  ;          
	ret						  ;

;=======================================================  ;
; GetNextTwoClusters.                                     ;
;=======================================================  ;
GetNextTwoClusters:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
        xor     ecx,ecx                                   ;;;;;;;;;;;;;;;;;;;
	mov	ax,word[DataSizeInClusters]		  ; Get data area size inclusters.
	mov	[MaxClusters],ax			  ; Put it in MaxCluster var.
	mov	esi,Fat 				  ; Get address of FAT
	add	esi,4					  ; Jump other fist 2 clusters.
	mov	[ClusterCount],2			  ; set some vars
	mov	[Clusterloop],0 			  ; Make sure the var is 0
	mov	cx,[MaxClusters]			  ; Set the number of loops.      
GetNextTwoClusterLoop:					  ; Label.
	lodsw						  ; Load the first cluster.
	and	ax,0xFFFF				  ; Check for 0.
	jz	FoundFreeClusterTwoA			  ; If its free jump to save address.
GetNextFreeCluster16:					  ; Label.
	inc	[ClusterCount]				  ; If not inc address.
	loop	GetNextTwoClusterLoop			  ; Not 0 loop.
	mov	[ErrorCodeFat16],5			  ; If we end up here, we have a run out of clusters error.
	popad						  ; Restore 32bit general registers.
	stc						  ; Error :(.
	ret						  ; Return.

;=======================================================  ;
; FoundFreeClusterTwoA                                    ;
;=======================================================  ;
FoundFreeClusterTwoA:					  ; Name of function.
	cmp	[Clusterloop],1 			  ; Is this the first time we have come here?.
	je	@f					  ; Label.
	mov	ax,[ClusterCount]			  ; Get the cluster address
	mov	[ClusterOne],ax 			  ; Save it for latter.
	mov	[Clusterloop],1 			  ; Mark we have been here before.
	;inc    [ClusterCount]                            ; Inc the count now instead of later.
	jmp	GetNextFreeCluster16			  ; Jump back to get one more cluster address.       
@@:							  ; Label.
	mov	ax,[ClusterCount]			  ; Move cluster address into AX
	mov	[ClusterTwo],ax 			  ; Put it in the second of the two clusters.
	popad						  ; Restore 32bit general registers.
	clc						  ; No Errors :).
	ret						  ; Return.

;=======================================================  ;
; UpdateClusterValue.                                     ;
;=======================================================  ;
UpdateClusterValue:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	mov	word[Cluster],ax			  ; Address of cluster                 
	mov	word[UpdateValue],bx			  ; Up date value. 
	xor	ecx,ecx 				  ; Make sure top half of ecx is emptpy
	mov	cx,ax					  ; Move the address into cx.
	shl	cx,1					  ; Mul by 2.           
	mov	esi,Fat 				  ; Load address of FAT.                        
	add	esi,ecx 				  ; Add offset to fat, to get cluster address.
	mov	dx,word [ds:esi]			  ; Get what's at that address.
	mov	[ClustOrigValue],dx			  ; Save it.  
	mov	word [ds:esi],bx			  ; Put the new value.        
DoneUpdateClusterValue: 				  ; Label.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; WriteFat16.                                             ;
;=======================================================  ;
WriteFat16:						  ; Name of function.
	pushad						  ; Save 32bit general registers.         
	push	es					  ; Save ES.
	push	ds					  ; Save DS.
	mov	[HddFileName],edi			  ; Store name of file.
	mov	[FileLoadAddress],edx			  ; 0x200000
	mov	esi,[LoadFileFromAddressFat16]		  ; Address of file to write, put in esi
	mov	[FileLoadAddress],esi			  ; Move whats in esi into the right var. 
	mov	[FileLoadCount],0			  ; 0 file load count,adds 512bytes to load address
	mov	[FddSectorsLoadedCount],0		  ; 0 file number of sectors load count.
	xor	ecx,ecx 				  ; 0 ecx
	mov	ax,[bpbRootEntries]			  ; Move value to register to work on. ax=Arithmatic
	mov	cx, 32					  ; Move value to multiply into register
	mul	cx					  ; Multiply
	div	word [bpbBytesPerSector]		  ; Divide
	mov	[RootSize],ax				  ; Put the value into a nice storage area for a bit
	mov	dx,ax					  ; Put root size in DX.
	xor	ax,ax					  ; 0 AX
	mov	al, byte [bpbNumberOfFATs]		  ; Load up number of FAT tables (/info) into AL
	mul	word[bpbSectorsPerFAT]			  ; Multiply AL by the number of sectors per FAT table (/info)
	add	ax,word[FatStart]			  ; Add to the FAT total (AX) the number of reserved sectors
	mov	[RootStart],ax				  ; Put the start of the root address into RootStart variable
	mov	[RootStartDx],dx			  ; Keep this we may need it.
	mov	cx,[RootSize]				  ; Mov the root size into CX
	add	ax,cx					  ; Add ax (RootStart) to cx (RootSize)
	mov	[DataStart],ax				  ; Move the answer into DataStart
	mov	ax,18h					  ; Move how data base,into ax
	mov	es,ax					  ; Now add AX to ES (NO direct segment reg addressing).    
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).    
	mov	edi,RootDirBuffer			  ; Where we are loading to.
	mov	esi,[HddFileName]			  ; RootConvertedFileName
	mov	dx,[bpbRootEntries]			  ; Move number of root entrys into count.
	mov	dx,[FileCluster]			  ; Get start cluster.
	mov	word [cluster],dx			  ; files first cluster.
LoadImageWriteFat16:					  ; Label.
	mov	ax,[cluster]				  ; mov cluster address, into ax
	mov	[FloppyTest],0				  ; need to try to read floppy aleast 3 times
WriteNextClusterFat16:					  ; useally has more that one sector per cluster.
	call	ClusterLBA				  ; Convert cluster to LBA
	mov	[clusterSPC],ax 			  ; Save it, so we can inc it on loop.
	xor	ecx,ecx 				  ; Clear ecx
	mov	cl,byte [bpbSectorsPerCluster]		  ; Mov sectors per cluster in to cx, as fat16/32-
ReadNextClusterWriteFat16loop:				  ; Label.
	push	ecx					  ; Save number of loops
	mov	[FloppyTest],0				  ; Set number of retrys to 0
	call	WriteSectorsFat16			  ; Write sector
	jc	ErrorFloppyPopFat16			  ; jmp if error
	inc	[clusterSPC]				  ; inc, for next sector.                
	mov	ax,[clusterSPC] 			  ; move it to AX
	pop	ecx					  ; Unsave loop number.
	loop	ReadNextClusterWriteFat16loop		  ; Not 0, then loop.
	xor	ecx,ecx 				  ; Make sure reg is 0.
	xor	eax,eax 				  ; Make sure reg is 0.
	mov	ax,word[cluster]			  ; identify current cluster
	mov	cx,ax					  ; copy current cluster
	add	ecx,eax 				  ; Time the cluster number by 2, use 32bit regs.
	mov	esi,Fat 				  ; location of FAT in memory
	add	esi,ecx 				  ; index into FAT
	mov	dx,word [ds:esi]			  ; read two bytes from FAT
	mov	word [cluster],dx			  ; store new cluster
	cmp	dx,0xFFF0				  ; test for end of file (maybe 0FFF8h ?)
	jb	LoadImageWriteFat16			  ; If not do another loop.
ExitSucsessWriteFat16:					  ; Label.
	pop	ds					  ; Restore DS
	pop	es					  ; Restore ES
	popad						  ; Restore 32bit general registers.
	mov	ebx,[FddSectorsLoadedCount]		  ; Put number of bytes written in ebx.
	clc						  ; No errors :).
	ret						  ; Return.
;=======================================================  ;
; Error1.                                                 ;
;=======================================================  ;      
ErrorFindFat16: 					  ; Label.
	pop	ds					  ; Restore DS
	pop	es					  ; Restore ES
	popad						  ; Restore 32bit general registers.
	stc						  ; Set error :(.
	ret						  ; Return.
;=======================================================  ;
; Error2 with pop.                                        ;
;=======================================================  ;    
ErrorFloppyPopFat16:					  ; Label.
	pop	ecx					  ; Restore ECX
ErrorFloppyFat16:					  ; Label.
	pop	ds					  ; Restore DS
	pop	es					  ; Restore ES
	popad						  ; Restore 32bit general registers.
	mov	ax,word[WriteSectorsErrorCode]		  ;
	stc						  ; Set error :(.
	ret						  ; Return.

;=======================================================  ;
; DirFatRootUpdateFat16.                                  ;
;=======================================================  ;    
DirFatRootUpdateFat16:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
DirWriteFillRootHddUSB: 				  ; Label.
	mov	ax,word[bpbHiddenSectors]		  ; Get Hidden Sectors.
	mov	dx,word[bpbHiddenSectors+2]		  ; Get Hidden Sectors +.
	add	ax,word [bpbReservedSectors]		  ; Add Reserved Sectors to Hidden Sectors.
	mov	[FatStartDx],dx 			  ; Save Hidden Sectors +.
	mov	[FatStart],ax				  ; Save Reserved Sectors + Hidden Sectors.
	xor	eax,eax 				  ; 0 eax.
	mov	ecx,eax 				  ; 0 ecx.     
	mov	al, byte [bpbNumberOfFATs]		  ; Get number of fats.                
	mul	word [bpbSectorsPerFAT] 		  ; Mul with Sectors Per FAT.
	mov	[FatSize2],ax				  ; Save result.
	xor	eax,eax 				  ; 0 eax.
	mov	ax,word [bpbSectorsPerFAT]		  ; Get Sectors Per FAT.
	mov	[FatSize1],ax				  ; Save it
	mov	ax,[bpbRootEntries]			  ; Get root entrys          
	mov	cx, 32					  ; Move CX 32               
	mul	cx					  ; Mul number of root entrys by 32.            
	div	word [bpbBytesPerSector]		  ; Div result by Bytes Per Sector.       
	mov	[RootSize],ax				  ; Save results.
	xor	eax,eax 				  ; 0 eax.                     
	mov	edx,eax 				  ; 0 edx.
	mov	al, byte [bpbNumberOfFATs]		  ; Get number of fats.         
	mul	word [bpbSectorsPerFAT] 		  ; Mul with Sectors Per FAT.          
	add	ax, [FatStart]				  ; Add fat start.
	mov	[RootStartDx],dx			  ; Save any left over.
	mov	[RootStart],ax				  ; Save results of above mul + fat start address in var.
	mov	cx,[RootSize]				  ; Get root size
	add	ax,cx					  ; Add it to AX.          
	mov	[DataStart],ax				  ; Save as data start address.
	mov	[FloppyTest],0				  ; 0 Write Hdd error var.
	mov	[FileLoadCount] ,0			  ; 0 File Load Count var.
	mov	[FddSectorsLoadedCount],0		  ; 0 Sectors Loaded Count var
	mov	edi,Fat 				  ; Get FAT address.
	mov	[FileLoadAddress],edi			  ; Save it.
	xor	eax,eax 				  ; 0 eax.  
	mov	ecx,eax 				  ; 0 ecx.  
	mov	dx,[FatStartDx] 			  ; Get Fat Start +.
	mov	ax,[FatStart]				  ; Get Fat Start.
	mov	cx,[FatSize1]				  ; Get Fat Size.
WriteSectorsLoopUSB1:					  ; Label.
	mov	[FloppyTest],0				  ; 0 Write Hdd error var.
	call	WriteSectorsFat16			  ; Write a sector to Hdd.
	jc	DirErrorUSBWrite			  ; Jump if error.
	inc	ax					  ; Next sector.
	loop	WriteSectorsLoopUSB1			  ; Loop if cx not 0.
	mov	[FloppyTest],0				  ; 0 Write Hdd error var.
	mov	[FileLoadCount],0			  ; 0 File Load Count var.
	mov	[FddSectorsLoadedCount],0		  ; 0 Sector Write Count var.
	mov	edi,RootDirBuffer			  ; Get address of root buffer.
	mov	[FileLoadAddress],edi			  ; Save it.
	xor	eax,eax 				  ; 0 eax. 
	mov	dx,[RootStartDx]			  ; Get Fat Start +.
	mov	ax,[RootStart]				  ; Get Fat Start.
	mov	cx,[RootSize]				  ; Get Fat Size.
WriteSectorsLoopUSB2:					  ; Label.
	mov	[FloppyTest],0				  ; 0 Write Hdd error var.
	call	WriteSectorsFat16			  ; Write a sector to Hdd.
	jc	DirErrorUSBWrite			  ; Jump if error.
	inc	ax					  ; Next sector.
	loop	WriteSectorsLoopUSB2			  ; Loop if cx not 0.
DirWriteFillRootUSBSuccsess:				  ; Label.
	popad						  ; Restore 32bit general registers.
	clc						  ; No errors :).
	ret						  ;                                
;=======================================================  ;
; Error.                                                  ;
;=======================================================  ;    
DirErrorUSBWrite:					  ; Label.
	popad						  ; Restore 32bit general registers.
	mov	ax,word[WriteSectorsErrorCode]		  ;  
	stc						  ; Error :(.
	ret						  ; Return.

;=======================================================  ;
; GetNextCluster.                                         ;
;=======================================================  ;    
GetNextCluster: 					  ; Name of function.
	pushad						  ; Save 32bit general registers.
        xor     ecx,ecx                                   ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov	ax,word[DataSizeInClusters]		  ; Get data size in clusters.
	mov	[MaxClusters],ax			  ; Save it as Max number of clusters.
	mov	esi,Fat 				  ; Get fat address.
	add	esi,4					  ; Add 4 to address.
	mov	[ClusterCount],2			  ; Add first 2 clusters to count var.
	mov	cx,[MaxClusters]			  ; Get max number of loops                            
GetNextClusterLoop:					  ; Label.
	lodsw						  ; Load a word from fat address.
	and	ax,0xFFFF				  ; Check for 0.
	jz	FoundFreeCluster			  ; If 0 we have found a free cluster.
	inc	[ClusterCount]				  ; If not inc count
	loop	GetNextClusterLoop			  ; And loop if cx not 0.
	popad						  ; Restore 32bit general registers. 
	stc						  ; Error, run out of free clusters.
	ret						  ; Return.
;=======================================================  ;
; FoundFreeCluster.                                       ;
;=======================================================  ;    
FoundFreeCluster:					  ; Label.
	popad						  ; Restore 32bit general registers. 
	clc						  ; No errors :).
	ret						  ; Return.

WriteSectorsErrorCode dw 0                                ;
;=======================================================  ;
; WriteSectorsFat16                                       ;
;=======================================================  ;
; IN:   AX = Cluster # to read                            ;
;                                                         ;
; OUT:  Carry set on error,                               ;
;       AX = Error code                                   ;
;                                                         ;
;       Carry cleared on no errors                        ;
;=======================================================  ;
WriteSectorsFat16:					  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
;=======================================================  ;
; Move data from image address, to DMA buffer.            ;
;=======================================================  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ecx,SectorSize				  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	edi,OffsetDma2Buffer1			  ;                   
	mov	esi,[FileLoadAddress]			  ;
	add	esi,[FileLoadCount]			  ;
	rep	movsb					  ;
	add	[FileLoadCount],SectorSize		  ;
	add	[FddSectorsLoadedCount],1		  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	call	LBAcHs					  ;
TryWriteFloppyAgain:					  ;
	push	ax					  ;
	mov	ch,byte [absoluteTrack] 		  ;     
	mov	cl,byte [absoluteSector]		  ;             
	mov	dh,byte [absoluteHead]			  ;
	mov	dl,byte [bsDriveNumber] 		  ;     
	pop	ax					  ;
	call	FddWriteB				  ;
	jc	FloppyWriteError			  ;
FloppyWriteOK:						  ;
;=======================================================  ;
; ExitSucsess.                                            ;
;=======================================================  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
; ExitError.                                              ;
;=======================================================  ;
FloppyWriteError:					  ;
	shr	ax,8					  ;
	mov	word[WriteSectorsErrorCode],ax		  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ax,word[WriteSectorsErrorCode]		  ;
	stc						  ;
	ret						  ;

;=======================================================  ;
; DirFatRootUpdate12.                                     ;
;=======================================================  ;
DirFatRootUpdate12:					  ;
	mov	[BpbCount],0				  ;
	mov	[number],0				  ;
	mov	ecx,11					  ;
  UpdateBpbLoop:					  ;
	mov	[FddErrCount],0 			  ;
	push	ecx					  ;
	mov	ecx,512 				  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	edi,OffsetDma2Buffer1			  ;
	mov	esi,BpbFat				  ;
	add	esi,[BpbCount]				  ;
	rep	movsb					  ;
	inc	[number]				  ;
  TryWriteFloppyAgainABC:				  ;
	mov	cl,[number]				  ;
	mov	ch,00					  ;
	mov	dh,00					  ;              
	mov	dl,00					  ;      
	call	FddWriteB				  ;
	jc	@f					  ;
  FloppyWriteOK1:					  ;
	add	[BpbCount],512				  ;
	jmp	DoAnotherLoopFatUpdate			  ;
  @@:							  ;
;=======================================================  ;
;  We come here if floppy error.                          ;
;=======================================================  ;
	mov	byte[ErrorCode],1			  ;
	pop	ecx					  ;
	jmp	DirFatError				  ;
  DoAnotherLoopFatUpdate:				  ;
	pop	ecx					  ;
	loop	UpdateBpbLoop				  ;
;=======================================================  ;
;  FILL ROOT DIR FROM MEMORY.                             ;
;=======================================================  ;
	mov	[RdirCount],0				  ;
	mov	[number],1				  ;
	mov	ecx,14					  ;
  UpdateRootDirLoop:					  ;
	mov	[FddErrCount],0 			  ;
	push	ecx					  ;
	mov	ecx,512 				  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	edi,OffsetDma2Buffer1			  ;
	mov	esi,RootDirBuffer			  ;
	add	esi,[RdirCount] 			  ;
	rep	movsb					  ;
	inc	[number]				  ;
  WriteFddRootUuDateLoop:				  ;
	mov	cl,[number]				  ;
	mov	ch,00					  ;
	mov	dh,01					  ;   
	mov	dl,00					  ;   
	call	FddWriteB				  ;
	jc	@f					  ;
  FddWriteOK:						  ;
	add	[RdirCount],512 			  ;
	jmp	DoAnotherLoopRootUpdate 		  ;
;=======================================================  ;
;  We come here if floppy error.                          ;
;=======================================================  ;
  @@:							  ;
	mov	byte[ErrorCode],1			  ;
	pop	ecx					  ;
	jmp	DirFatError				  ;
  DoAnotherLoopRootUpdate:				  ;
	pop	ecx					  ;
	loop	UpdateRootDirLoop			  ;
;=======================================================  ;
;  Exit OK.                                               ;
;=======================================================  ;
GoodThatWorked: 					  ;
	clc						  ; No errors :).
	ret						  ;
;=======================================================  ;
;  Exit Error.                                            ;
;=======================================================  ;
DirFatError:						  ;
	stc						  ;
	ret						  ;

ClusterUPFat16	dw 0					  ;
;=======================================================  ;
;  Exit Error.                                            ;
;=======================================================  ;
ReleaseClusterChain:					  ; Name of function.
	pushad						  ; Save 32bit general registers.
	push	ds					  ; Save DS.
	mov	word [ClusterUPFat16],ax		  ; Save first cluster.
	mov	ax,18h					  ; Move data descriptors into AX.       
	mov	ds,ax					  ; Now add AX to DS (NO direct segment reg addressing).       
ReleaseClusterChainFat16Loop:				  ; Label.
	mov	ax,word [ClusterUPFat16]		  ; Get cluster number
	mov	bx,0x0000				  ; Get update value
	call	UpdateClusterValue			  ; Call function.
	mov	dx,[ClustOrigValue]			  ; Get what was stored there
	mov	word [ClusterUPFat16],dx		  ; Use it as next cluster                       
	cmp	dx,0xFFF0				  ; Check for end.                       
	jb	ReleaseClusterChainFat16Loop		  ; if not do another loop.
ReleaseClusterChainFat16Exit:				  ; Label.
	pop	ds					  ; Restore DS.
	popad						  ; Restore 32bit general registers.
	ret						  ; Return.

;=======================================================  ;
; Data.                                                   ;
;=======================================================  ;  
NameOfFileToLoadBufferFat16   rb   16			  ; Buffer for working on file to write name.NameOfFileToLoadBufferFat16a  rb   16                     ; Second buffer for working on two file names eg: rename file
