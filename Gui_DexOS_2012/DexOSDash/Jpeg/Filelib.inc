open: ;esi=name_string
      ;returns eax
	pushad
	mov   dword[VesaBufferVar],edi
	mov   dword[FileHandler],1
	mov   dword[FileAddress],esi
	mov   dword[Position],0 
	mov   dword[FileSize],ecx
	clc
	popad
	mov   eax,dword[FileHandler]
	ret



close:
	pushad
     .b:
	popad
	xor   eax,eax
	ret

read:	  ;(f,bufer,nbytes)  eax,edi,ecx  returns bytes read in cx and cf If error
	pushad
	cmp   eax,dword[FileHandler]
	jne   error2
	mov   ebx,dword[Position]
	add   ebx,ecx
	cmp   ebx,dword[FileSize]
	jb    SizeOK
	mov   ebx,dword[FileSize]
	sub   ebx,dword[Position]
	mov   ecx,ebx
SizeOK:
	mov   dword[SavedECX],ecx
	mov   esi,dword[FileAddress]
	add   esi,dword[Position]
	cld
	rep   movsb
	mov   dword[Position],edi
	sub   esi,dword[FileAddress]
	mov   dword[Position],esi
	popad
	mov   ecx,dword[SavedECX]
	clc
	ret

 error2:
	add   esp,8
	popad
	xor   ecx,ecx
	stc
	ret

ftell:	mov   edx,dword[Position]
	ret
lseek: ;eax=file edx=pos
	mov   dword[Position],edx
	ret
skip:  ;eax=file edx=bytes to skip
	add   dword[Position],edx
	ret

VesaBufferVar dd 0
JpegStart     dd 0
FileHandler   dd 0
Position      dd 0
FileAddress   dd 0
FileSize      dd 0
SavedECX      dd 0