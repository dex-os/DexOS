;=========================================================;
; Dex Image Format  (DIF)                        02-04-09 ;
; Also can be used for (DFF)                              ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
use32

	       org    0x0

	       db     'DIF1'		      ; 4 byte id
	       dd     Image1		      ; Normal image
	       dd     Image2		      ; Alpha image + Normal Image size
	       dd     Image2Size-Image2       ; Alpha image size
               dd     Image2Size              ; info

Image1:
file   'Start.jpg'                          ; NORMAL Jpeg
Image2:
file   'StartA.jpg'                         ; ALPHA Jpeg
Image2Size:
