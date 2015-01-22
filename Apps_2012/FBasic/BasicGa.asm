include "BasicG\FBasic_G.inc"

FONT_SIZE 2
SCREEN 800 600 32

CLS    0x00000000
COLOR  0x0057ffff
LOCATE 20,20
PRINT  "This app is written in Macro Basic, for DexOS "
COLOR  0x00ff5757
LOCATE 20,40
PRINT  "With the ease of Basic and the power of ASM "
COLOR  0x00ffffff
LOCATE 20,60
PRINT  "It user's the basic commands:"
PRINT  " "
PRINT  "  CLS"
PRINT  "  SCREEN"
PRINT  "  COLOR"
PRINT  "  LOCATE"
PRINT  "  PRINT"
PRINT  "  GOSUB"
PRINT  "  RETURN"
PRINT  "  SLEEP"
PRINT  "  END"
PRINT  " "
GOSUB  TestSub
SYNC
SLEEP
END

TestSub:
PRINT "Press any key to quit."
RETURN

align 4
include 'Dex.inc'
align 4
VesaBuffer: