bmptoimg_data_area_count=0
macro bmptoimg bmp_load_area,img_dest_area
{
local fileinfo,string,end_bmp,nodix
local converttable,noaddelem,nextbit,convert1bpp,convert4bpp,convert2
local nextelem,convertno32,nomorestring,convert1,nextstring,yespicsize
;local qwe,bmpfn

;  convert:
    movzx eax,word [bmp_load_area+28]
    mul  dword [bmp_load_area+18]
    add  eax,31
    shr  eax,5
    mov  dword [bmptoimg_data_area_dwps],eax ;dwps-doublewords per string
    shl  eax,2
    mov  dword [bmptoimg_data_area_bps],eax  ;bps-bytes per string

    cmp dword [bmp_load_area+34],0
    jne  yespicsize ;if picture size is defined
    mul dword [bmp_load_area+22]
    mov dword [bmp_load_area+34],eax

  yespicsize:
    mov ebp,img_dest_area+8

    mov  eax,bmp_load_area
    mov  ebx,eax
    add  ebx, [bmp_load_area+2];file size
    inc  ebx
    mov  dword [bmptoimg_soi],ebx  ;soi-start of image area for drawing

    add  eax, [bmp_load_area+10]
    mov  dword [bmptoimg_data_area_sop],eax  ;sop-start of picture in file
    add  eax, [bmp_load_area+34]
    mov  dword [bmptoimg_data_area_eop],eax  ;eop-end of picture in file
    mov  eax, [bmp_load_area+18]
    lea  eax,[eax+2*eax]  ;3x pixels in eax

    mov  edi,dword [bmptoimg_soi]  ;initializing
    mov  esi,dword [bmptoimg_data_area_eop]
    sub  esi,dword [bmptoimg_data_area_bps]


  nextstring:
    push edi
    push ebp
    cmp  word [bmp_load_area+28],24
    jne  convertno32

    mov edi,ebp
    mov  ecx,[bmptoimg_data_area_dwps]
    cld
    rep movsd

  convert1:
    pop  ebp
    pop  edi
    sub  esi,dword [bmptoimg_data_area_bps]
    sub  esi,dword [bmptoimg_data_area_bps]
    cmp  esi,dword [bmptoimg_data_area_sop]
    jb   end_bmp
    add  edi,eax
    add  ebp,eax
    jmp  nextstring

  convertno32:
    mov  ebx,bmp_load_area
    add  ebx, [bmp_load_area+14]
    add  ebx,14         ;start of color table
    push esi
    add  esi,dword [bmptoimg_data_area_bps]
    mov  dword [bmptoimg_data_area_eos],esi
    pop  esi
  nextelem:
    push eax
    movzx eax,byte [esi]
    cmp  word [bmp_load_area+28],4
    je   convert4bpp
    cmp  word [bmp_load_area+28],1
    je   convert1bpp
    call converttable

  convert2:
    pop  eax
    inc  esi
    cmp  esi,dword [bmptoimg_data_area_eos]
    jae  convert1
    add  edi,3

    add  ebp,3

    jmp  nextelem

  convert4bpp:
    shl  ax,4
    shr  al,4
    push ax
    movzx eax,ah
    call converttable
    add  edi,3

    add ebp,3

    pop  ax
    movzx eax,al
    call converttable
    jmp  convert2

  convert1bpp:
    mov  ecx,eax
    mov  edx,7
  nextbit:
    xor  eax,eax
    bt   ecx,edx
    jnc  noaddelem
    inc  eax
  noaddelem:
    push edx
    call converttable
    pop  edx
    dec  edx
    js   convert2
    add  edi,3

    add  ebp,3

    jmp  nextbit

  converttable:
    shl  eax,2
    add  eax,ebx
    mov  edx, dword [eax]
;    mov  dword [edi],edx
    mov [ebp],edx
    ret

bmptoimg_data_area_count=bmptoimg_data_area_count+1
if bmptoimg_data_area_count = 1
; DATA AREA
bmptoimg_soi                dd 0
bmptoimg_data_area_bps      dd 0
bmptoimg_data_area_dwps     dd 0
bmptoimg_data_area_sop      dd 0
bmptoimg_data_area_eop      dd 0
bmptoimg_data_area_eos      dd 0
end if

end_bmp:
    mov  eax,dword [bmp_load_area+18]
    mov  ebx,dword [bmp_load_area+22]
    mov  dword [img_dest_area],eax
    mov  dword [img_dest_area+4],ebx
}