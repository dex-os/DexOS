;=========================================================;
; Dex Image Format  (DIF)                        02-04-09 ;
; Also can be used for (DFF)                              ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
use32

	       org    0x0

	       db     'DFF1'		      ; 4 byte id
	       dd     Image1		      ; Normal image
	       dd     Image2		      ; Alpha image + Normal Image size
	       dd     Image2Size-Image2       ; Alpha image size
               dd     Image2Size              ; Font info

Image1:
file   'Sans.jpg'                             ; NORMAL Jpeg
Image2:
file   'SansAA.jpg'                           ; ALPHA Jpeg
Image2Size:
              dd      0                       ; the address of converted image(here + 0)
              dd      13                      ; cell Width (here + 4)
              dd      16                      ; cell Height (here + 8)
              dd      16                      ; Font height (here + 12)
              dd      32                      ; start Char (here + 16)
              dd      0xffffffff              ; Font color not = 0xffffffff (here + 20)
              dd      CharWidth               ; (here + 24)
              dd      CharOffSet              ; (here + 28)
align 4
CharOffSet:
include 'FontOffSet11.inc'                    ; Char offset into the image
align 4
CharWidth:
include  'FontData.inc'                       ; Char Width, 256 byte size for each font