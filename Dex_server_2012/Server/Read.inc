ReadFile:
	pushad
	push  ds
	push  es
	mov   ax,18h
	mov   es,ax
	mov   ds,ax 
	mov   [countvar],0
	mov   [file_type],unk
	mov   [type_len],unkl-unk
	mov   [GETorHEAD],1
	mov   esi,Header+5
	mov   edi,Header
	cmp   [edi],dword 'GET '
	je    ItsGET
	mov   [GETorHEAD],0
ItsGET:
	mov   edi,wanted_file
	cld
new_let:
	cmp   [esi],byte ' '
	je    no_new_let
	inc   [countvar]
	cmp   [countvar],13
	jge   no_new_let
	movsb
	jmp   new_let
no_new_let:
	cmp   esi,Header+5
	jne   no_index
	mov   edi,wanted_file
	mov   [edi+0],dword 'inde'
	mov   [edi+4],dword 'x.ht'
	mov   [edi+8],byte  'm'
	mov   [edi+9],byte  0
	add   edi,10
	jmp   htm_header
no_index:
	mov   al,0
	stosb
	cmp   [edi-4],dword 'htm'+0
	je    htm_header
	cmp   [edi-4],dword 'HTM'+0
	je    htm_header
	jmp   no_htm_header
htm_header:
	mov   [file_type],htm
	mov   [type_len],html-htm
	jmp   found_file_type
no_htm_header:
	cmp   [edi-4],dword 'png'+0
	je    png_header
	cmp   [edi-4],dword 'PNG'+0
	je    png_header
	jmp   no_png_header
png_header:
	mov   [file_type],png
	mov   [type_len],pngl-png
	jmp   found_file_type
no_png_header:
	cmp   [edi-4],dword 'gif'+0
	je    gif_header
	cmp   [edi-4],dword 'GIF'+0
	je    gif_header
	jmp   no_gif_header
gif_header:
	mov   [file_type],gif
	mov   [type_len],gifl-gif
	jmp   found_file_type
no_gif_header:
	cmp   [edi-4],dword 'jpg'+0
	je    jpg_header
	cmp   [edi-4],dword 'JPG'+0
	je    jpg_header
	jmp   no_jpg_header
jpg_header:
	mov   [file_type],jpg
	mov   [type_len],jpgl-jpg
	jmp   found_file_type
no_jpg_header:
	cmp   [edi-4],dword 'asm'+0
	je    txt_header
	cmp   [edi-4],dword 'ASM'+0
	je    txt_header
	cmp   [edi-4],dword 'txt'+0
	je    txt_header
	cmp   [edi-4],dword 'TXT'+0
	je    txt_header
	jmp   no_txt_header
txt_header:
	mov   [file_type],txt
	mov   [type_len],txtl-txt
	jmp   found_file_type
no_txt_header:
html_file:
found_file_type:
;------------load wanted file----------------
	;Try fdd
	;mov   esi,wanted_file
	;mov   edi,wanted_file_buffer
	;call  [FloppyfileLoad]
	;jnc   file_found
	;Try hdd
	;mov   edx,wanted_file_buffer
	;mov   edi,wanted_file
	;call  [HddfileLoad]
	;jnc   file_found
	;stc
	mov   edi,wanted_file
	mov   dword[NameOfFileToLoad],edi
	call  ConevertFileName
	mov   edi,NameOfFileToLoadBuffer
	call  UpperCaseNew
	call  FindFile
	jnc   file_found
	; Send a 404
	mov   esi,fnf
	mov   edi,wanted_file_buffer
	mov   ecx,fnfe-fnf
	cld
	rep   movsb
	mov   ecx,fnfe-fnf
	mov   [file_type],htm
	mov   [type_len],html-htm
	mov   dword[FileAddress],fnf
	inc   [NumbersVar4]
	mov   [filesize],ecx
	mov   [file_left],ecx
	mov   eax,ecx
	mov   eax,[filesize]
	add   [bytes_transferred],eax
	add   [NumbersVarB],eax
	pop   es
	pop   ds
	popad
	stc
	ret

file_found:
	mov   ecx,dword[FileSize]
file_loaded:
	mov   [filesize],ecx
	mov   [file_left],ecx
	mov   eax,ecx
	mov   eax,[filesize]
	add   [bytes_transferred],eax
	add   [NumbersVarB],eax
	pop   es
	pop   ds
	popad
	clc
	ret



countvar	  dd  0
GETorHEAD	  dd  0
file_left	  dd  0
filesize	  dd  0
bytes_transferred dd  0

html_header:
db 'HTTP/1.0 200 OK',13,10,'Content-Length: '
.len = $ - html_header
inuse db '                                   ',10
.len = $ - inuse

fnf:
db 'HTTP/1.0 404 Not Found',13,10,13,10
db '<html>',13,10
db '<head>',13,10
db '  <title>404 Not Found</title> ',13,10
db '</head>',13,10
db '<body>',13,10
db '<h1 style="text-align: center;"><font size="+5">404 File Not Found</font><br>',13,10
db '</h1>',13,10
db '<p> </p>',13,10
db '<div style="text-align: center;"><img src="daemon.jpg" title="" alt=""',13,10
db ' style="width: 387px; height: 397px;"><br>',13,10
db 'You have tryed to go where DexOS will not go !<br>',13,10
db '</div>',13,10
db '<hr>',13,10
db '<address>DexServer v 0.1 for DexOS Port 80</address>',13,10
db '</body>',13,10
db '</html>',13,10
fnfe:

htm:   db  'Content-Type: text/html',13,10,13,10
html:
txt:   db  'Content-Type: text/plain',13,10,13,10
txtl:
png:   db  'Content-Type: image/png',13,10,13,10
pngl:
gif:   db  'Content-Type: image/gif',13,10,13,10
gifl:
jpg:   db  'Content-Type: image/jpeg',13,10,13,10
jpgl:
unk:   db  'Content-Type: unknown/unknown',13,10,13,10
unkl:						 
wanted_file1 db "index.htm",0,0
wanted_file: times 100 db 0

