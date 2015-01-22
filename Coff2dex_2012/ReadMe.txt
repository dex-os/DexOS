;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.03                                                       ;;
;; Author           :   Dex                                                         ;;
;; Website          :   www.dex4u.com                                               ;;
;; Forum            :   http://jas2o.forthworks.com/dexforum/                       ;;
;; wiki             :   http://tonymac.asmhackers.net/Wiki/pmwiki.php               ;;
;; Date             :   24/04/2008                                                  ;;
;; Copy Right Owners:   Team DexOS (c)2002-2008                                     ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Team DexOS       :   0x4e71, bubach, crc, Dex, hidnplayr, jas2o, roboman         ;;
;;                  :   Solidus, smiddy, tony(mac), viki.                           ;;
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
04/24/08
This is used for making driver or modules for DexOS, You simple assemble your module as a coff file.
Then use "Coff2Dex" to convert it to a driver/module.
eg: type coff2dex myfile.obj myfile.dri myfile.rel  <enter>
in windows.

This will make two files "myfile.dri" "myfile.rel" which are DexOS drivers.
See example folder for example of module layout.

Any ? ask in the forum
Regards Dex.