;=========================================================;
; Dex Image Format  (DIF)                        02-04-09 ;
; Also can be used for (DFF)                              ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
format binary as 'dif'
use32

	       org    0x0

	       db     'DIF1'		      ; 4 byte id
	       dd     Image1		      ; Normal image
	       dd     Image2		      ; Alpha image + Normal Image size
	       dd     Image2Size-Image2       ; Alpha image size
	       dd     Image2Size	      ; info

Image1:
file   'ScrMenu.jpg'			      ; NORMAL Jpeg
Image2:
file   'ScrMenuAA.jpg'			      ; ALPHA Jpeg
Image2Size:
