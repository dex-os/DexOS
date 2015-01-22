;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.05                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2011                                                ;;
;; Filename         :   Gui.inc                                                    ;;
;; Copy Right Owner :   Craig Bamford                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Team DexOS       :   0x4e71, bubach, Dex, hidnplayr, jas2o, steve, Cloud         ;;
;;                  :   smiddy, viki.                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Disclaimer       :                                                               ;;
;; This software is provided "AS IS" without warranty of any kind, either           ;;
;; expressed or implied, including, but not limited to, the implied                 ;;
;; warranties of merchantability and fitness for a particular purpose. The          ;;
;; entire risk as to the quality and performance of this software is with           ;;
;; you.                                                                             ;;
;; In no event will the author's, distributor or any other party be liable to       ;;
;; you for damages, including any general, special, incidental or                   ;;
;; consequential damages arising out of the use, misuse or inability to use         ;;
;; this software (including but not limited to loss of data or losses               ;;
;; sustained by you or third parties or a failure of this software to operate       ;;
;; with any other software), even if such party has been advised of the             ;;
;; possibility of such damages.                                                     ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Thanks to Alexei Frounze, Tomasz Grysztar, Jim Hall, Pat Villani, Pype.Clicker.  ;;
;;                                                                                  ;;
;; Also a thanks to  the many forums i am a member of, i would like to thank anyone ;;
;; who  has helped me, by answering my ? (too many too list).                       ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;-----------------------------------
; mouse bitmaps for 32bpp
;-----------------------------------
P1	equ	00010101h	; almost black
P2	equ	005F5F5Fh	; almost white

P3	equ	00010101h ;00989898h       ; dark yellow
P4	equ	00efefefh ;00cfcf00h       ; light yellow
align 4
mouse3_bmp32	dd	P1,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	P3,P1,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P1,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P1,00,00,00,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P1,00,00,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P4,P1,00,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P4,P4,P1,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P4,P4,P4,P1,00,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P4,P4,P4,P4,P1,00,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P4,P4,P4,P4,P4,P1,00,00,00,00,00,00
		dd	P3,P4,P4,P4,P1,P1,P1,P1,P1,P1,P1,00,00,00,00,00
		dd	P3,P4,P4,P1,P4,P4,P2,00,00,00,00,00,00,00,00,00
		dd	P3,P4,P1,00,P2,P4,P4,P2,00,00,00,00,00,00,00,00
		dd	P3,P1,00,00,P2,P4,P4,P2,00,00,00,00,00,00,00,00
		dd	P1,00,00,00,00,P2,P4,P4,P2,00,00,00,00,00,00,00
		dd	00,00,00,00,00,P2,P4,P4,P2,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,P2,P4,P4,P2,00,00,00,00,00,00
		dd	00,00,00,00,00,00,P2,P4,P4,P2,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,P2,P2,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
		dd	00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
align 4
VesaStartXx	    dw	 0
VesaStartYy	    dw	 0
VesaStartXold	    dw	 0
VesaStartYold	    dw	 0
VesaRes 	    dd	 0
SaveEdi 	    dd	 0
VesaResRMBG	    dd	 0
align 4 


MouseBack  rd   16*32
MouseBack2 rd	16*32
		
