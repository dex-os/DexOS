include "BasicG\FBasic_G.inc"

FONT_SIZE 2
SCREEN 800 600 32
LOAD_IMAGE file_area_Menu, MenuBuffer
SPRITE 128, 2, MenuBuffer

COLOR  0x0057ffff
LOCATE 150,30
PRINT "This app is written in Macro Basic, for DexOS "
COLOR  0x00ff5757
LOCATE 150,50
PRINT "With the ease of Basic and the power of ASM "
COLOR  0x00ffffff
LOCATE 150,70
PRINT "It user's the basic commands:"
PRINT " "
PRINT "  CLS"
PRINT "  SCREEN"
PRINT "  COLOR"
PRINT "  LOCATE"
PRINT "  PRINT"
PRINT "  GOSUB"
PRINT "  RETURN"
PRINT "  SLEEP"
PRINT "  END"
PRINT " "
GOSUB TestSub
SYNC
SLEEP
END

TestSub:
PRINT "Press any key to quit."
RETURN


align 4
file_area_Menu:
file   'ScrMenu\ScrMenu.dif'
file_area_Menu_FileEnd:
rd 1
align 4
include 'Dex.inc'
align 4
VesaBufferTemp: rd 800*600
align 4
MenuBuffer:	rd 544*512+2
align 4
VesaBuffer:
