;=========================================================;
; WebPages                                       05/05/08 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
align 4
WebPagesBuffer:
WebPage1:	  db 'INDEX   HTM'
WebPage1Address:  dd  WebPage1File
WebPage1Count:	  dd  WebPage1FileSize-WebPage1File

WebPage2:	  db 'DEXLOGO1PNG'
WebPage2Address:  dd  WebPage2File
WebPage2Count:	  dd  WebPage2FileSize-WebPage2File

WebPage3:	  db 'GUIINDEXJPG'
WebPage3Address:  dd  WebPage3File
WebPage3Count:	  dd  WebPage3FileSize-WebPage3File

WebPage4:	  db 'TEST    OGG'
WebPage4Address:  dd  WebPage4File
WebPage4Count:	  dd  WebPage4FileSize-WebPage4File

WebPage5:	  db 'DAEMON  JPG'
WebPage5Address:  dd  WebPage5File
WebPage5Count:	  dd  WebPage5FileSize-WebPage5File

WebPage6:	  db 'FAVICON ICO'
WebPage6Address:  dd  WebPage6File
WebPage6Count:	  dd  WebPage6FileSize-WebPage6File

WebPageEnd:	  dd  0
align 4
WebPage1File:	  file 'index.htm'
WebPage1FileSize:
rb 4
WebPage2File:	  file 'Dexlogo1.png'
WebPage2FileSize:
rb 4
WebPage3File:	  file 'guiindex.jpg'
WebPage3FileSize:
rb 4
WebPage4File:	  file 'test.ogg'
WebPage4FileSize:
rb 4
WebPage5File:	  file 'daemon.jpg'
WebPage5FileSize:
rb 4
WebPage6File:	  file 'favicon.ico'
WebPage6FileSize:
rb 4


