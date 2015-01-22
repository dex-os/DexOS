include 'FBasic_L.inc'
CLS
COLOR  11
LOCATE 2,1
PRINT "This app is written in Macro Basic, for Linux "
COLOR  12
LOCATE 2,2
PRINT "With the ease of Basic and the power of ASM "
COLOR  15
LOCATE 2,3
PRINT "It user's the basic commands:"
PRINT " "
PRINT "    CLS"
PRINT "    SCREEN"
PRINT "    COLOR"
PRINT "    LOCATE"
PRINT "    PRINT"
PRINT "    GOSUB"
PRINT "    RETURN"
PRINT "    SLEEP"
PRINT "    END"
PRINT " "
GOSUB TestSub
SLEEP
END

TestSub:
PRINT "  Press any key to quit."
RETURN