;=========================================================;
; RmBuf                                        01/05/2011 ;
;---------------------------------------------------------;
;                                                         ;
; Defines some kernel constants.                          ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;=======================================================  ;
; Buffers.                                                ;
;=======================================================  ;  
align 4                                                   ;  
stackBuffInts:                  rb      1024*2            ;
stackBuffInts1:                                           ; 
stackBuff:                      rb      1024*6            ;
stackBuff1:                                               ;
OffsetDma2Buffer1		rb	1024              ;
OffsetDma2Buffer		rb	1024              ;

