;=========================================================;
; Vesa2                                          01/05/11 ;
;---------------------------------------------------------;
; Tests and sets up vesa, (runs in realmode).             ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;=======================================================  ;
; Vesa.                                                   ;
;=======================================================  ;  
;0x4112 =  640 , 480 , 0                                  ;
;0x4115 =  800 , 600 , 0                                  ;
;0x4118 = 1024 , 768 , 0                                  ;
;0x411B = 1280 ,1024 , 0                                  ;
;=======================================================  ;
Vesa:                                                     ;
        mov     dword [VESAInfo_Signature],'VBE2'         ;
	mov     ax,4f00h				  ;   
	mov     di,VESA_Info		                  ;
	int     10h                                       ;
                                                          ;
	cmp     ax,004Fh                                  ;
	jne     novesa2	                                  ;
                                                          ;
	mov     ax,4f01h			          ; set vesa screen mode information
	mov     di,Mode_Info	                          ;
	mov     cx,0x4112 				  ; 0x4118 ;0x4112 ;0x4101
	and     cx,0xfff                                  ;
	int     10h                                       ;
                                                          ;
	cmp     [VESAInfo_Version],0x200                  ;
	jb      novesa2                                   ;
                                                          ;
	mov     ax,[ModeInfo_ModeAttributes]		  ; (ModeAttributes)
	and     al,00000001b                              ;
	cmp     al,1					  ; 1=Mode supported
	jne     novesa2                                   ;
                                                          ;
	mov     ax,[ModeInfo_ModeAttributes]		  ; (ModeAttributes)
	and     al,10000000b                              ;
	cmp     al,10000000b	                          ; 1=LFB available
	jne     novesa2                                   ;
                                                          ;
	mov     byte[NoVesa],0                            ;
	clc                                               ;
	ret                                               ;

;=======================================================  ;
; VesaModeSet                                             ;
;=======================================================  ;   
VesaModeSet:	                                          ;
	mov     ax,4f02h				  ; set vesa screen mode
	mov     bx,0x4112 				  ; 0x4118;0x4112;0x4101                                     
	int     10h                                       ;
                                                          ;
	cmp     ax,004Fh                                  ;
	jne     novesa2                                   ;
	mov     byte[NoVesa],0                            ;
	clc                                               ;
	ret                                               ;
                                                          ;
  novesa2:                                                ;
	mov     byte[NoVesa],1                            ;
	stc                                               ;
	ret                                               ;

;=======================================================  ;
; Data                                                    ;
;=======================================================  ;   
NoVesa	db  0                                             ;
