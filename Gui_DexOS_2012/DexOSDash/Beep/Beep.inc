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

 ;----------------------------------------------------;
 ; rand                                               ;
 ;----------------------------------------------------;
rand:                               
        mov   ax,[Seed]             
        imul  [k1Rand]                  
        add   ax,[k2Rand]              
        mov   [Seed],ax             
        ror   al,1                  
        rol   ah,1                  
        ret
 ;----------------------------------------------------;
 ; Delay                                              ;
 ;----------------------------------------------------;
DeLay:
        push  eax
        mov   eax,18
        call  [SetDelay]
        pop   eax
        ret
 ;----------------------------------------------------;
 ; Sound                                              ;
 ;----------------------------------------------------;
Sound:
        mov   bx,[Hz]
        mov   ax,0x34dd                
        mov   dx,0x0012                
        cmp   dx,bx                
        jnc   Done1               
        div   bx                
        mov   bx,ax
        in    al,0x61
        test  al,3
        jnz   A99                
        or    al,3
        out   0x61,al                
        mov   al,0xb6
        out   0x43,al
A99:    
        mov   al,bl 
        out   0x42,al              
        mov   al,bh
        out   0x42,al
Done1:
        ret
 ;----------------------------------------------------;
 ; NoSound                                            ;
 ;----------------------------------------------------;
NoSound:
        in    al,0x61                 
        and   al,11111100b
        out   0x61,al                
        ret                
 
 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;

Hz                      dw            0
Seed                    dw            0
k1Rand                      dw            9821
k2Rand                      dw            1
