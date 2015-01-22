 ;----------------------------------------------------;
 ; BuffToScreen.  ;Puts whats in the buffer to screen ;
 ;----------------------------------------------------;
BuffToScreen:
	cmp   [ModeInfo_BitsPerPixel],24
	jne   Try32
	call  BuffToScreen24
	jmp   wehavedone24
Try32:
	cmp   [ModeInfo_BitsPerPixel],32
	jne   wehavedone24
	call  BuffToScreen32 
wehavedone24:
	ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               32bpp ;
 ;----------------------------------------------------;
BuffToScreen32:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[ModeInfo_PhysBasePtr]
	 mov   esi,VesaBuffer 
	 xor   eax,eax
	 mov   ecx,eax	
	 mov   ax,[ModeInfo_XResolution]
	 mov   cx,[ModeInfo_YResolution]
	 mul   ecx
	 mov   ecx,eax
	 shr   ecx,1
	; cld
	; cli
	; rep   movsd
	; sti
 align 16
MMXloop:
	 movq  mm0, [esi]
	 movq  [edi], mm0
	 lea   esi,[esi+8]
	 lea   edi,[edi+8]
	 loop  MMXloop
	 emms
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               24bpp ;
 ;----------------------------------------------------;
BuffToScreen24:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax ;ccc
	 mov   ax,[ModeInfo_YResolution]
	 mov   ebp,eax
	 lea   eax,[ebp*2+ebp]
	 mov   edi,[ModeInfo_PhysBasePtr]
	 mov   esi,VesaBuffer  
	 cld
.l1:
	 mov   cx,[ModeInfo_XResolution]
	 shr   ecx,2	     
.l2:
	 mov   eax,[esi]	 ;eax = -- R1 G1 B1
	 mov   ebx,[esi+4]	 ;ebx = -- R2 G2 B2
	 shl   eax,8		 ;eax = R1 G1 B1 --
	 shrd  eax,ebx,8	 ;eax = B2 R1 G1 B1
	 stosd

	 mov   ax,[esi+8]	 ;eax = -- -- G3 B3
	 shr   ebx,8		 ;ebx = -- -- R2 G2
	 shl   eax,16		 ;eax = G3 B3 -- --
	 or    eax,ebx		 ;eax = G3 B3 R2 G2
	 stosd

	 mov   bl,[esi+10]	 ;ebx = -- -- -- R3
	 mov   eax,[esi+12]	 ;eax = -- R4 G4 B4
	 shl   eax,8		 ;eax = R4 G4 B4 --
	 mov   al,bl		 ;eax = R4 G4 B4 R3
	 stosd

	 add esi,16
	 loop  .l2

	 sub ebp,1
	 ja .l1

	 pop   es
	 popad
	 ret	

