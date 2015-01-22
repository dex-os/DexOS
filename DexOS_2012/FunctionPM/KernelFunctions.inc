;=========================================================;
; KernelFunctions                              01/04/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;=======================================================  ;
;  GetModListAddress  ; Returns the modlist address.      ;
;=======================================================  ; 
GetModListAddress:                                        ;
	mov     edi,ModListStruct                         ;
	mov     ecx,[ModListNumbers]                      ;
	ret                                               ;

;=======================================================  ;
;  SetModListNumber                                       ;
;=======================================================  ; 
SetModListNumber:                                         ;
	cmp     ecx,64                                    ;
	ja      SetModListNumberErr                       ;
	mov     [ModListNumbers],ecx                      ;
	clc                                               ;
	ret                                               ;
;=======================================================  ;
;  SetModListNumber Error                                 ;
;=======================================================  ; 
SetModListNumberErr:                                      ;
	stc                                               ;
	ret                                               ;

;=======================================================  ;
;  UpDateTopOfMemory.                                     ;
;=======================================================  ; 
UpDateTopOfMemory:                                        ;
	mov     dword[TopOfFreeMemoryWithBase],eax        ;
	ret                                               ;

