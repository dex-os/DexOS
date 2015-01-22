 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;
KeyDown                 dd            0
Up                       =            0x48                        ; Up arrow
Down                     =            0x50                        ; Down arrow
Left                     =            0x4b
Q                        =            0x10
A                        =            0x1e
W                        =            0x11
Up1                      =            0x4e
Down1                    =            0x4a
EscKey                   =            1 
DelayCount              dw            0
ExitYesNo               db            0
Bstart                  db            1    
Row                     db            0
Count                   db            0
Count2                  db            0
Switch                  dw            0
Switch1                 dw            0
Switch2                 dw            0
Switch3                 dw            0
Color                   db            0
InDex                   dw            0
IndexBX1                dw            0
IndexBX2                dw            0
IndexBX12               dw            0
IndexBX22               dw            0
IndexBX13               dw            0
IndexBX23               dw            0 
IndexY                  dw            0
IndexX                  dw            0
IndexY2                 dw            0
IndexX2                 dw            0
IndexY3                 dw            0
IndexX3                 dw            0
VesaStartX              dd            0
VesaStartY              dd            0
PadLx                   dw            2
PadLy                   dw            0
PadRx                   dw            312
PadRy                   dw            0
BallX                   dw            0
BallY                   dw            0
BallXstate              db            0
BallYstate              db            0
Score1                  db            0
Score2                  db            0
Hz                      dw            0

BluePad           Dw            0001h,0101h,0100h
                  DW            0109h,0F51h,0901h
                  DW            0001h,0101h,0100h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0109h,0F51h,0901h
                  DW            0001h,0101h,0100h
                  DW            0109h,0F51h,0901h
                  Dw            0001h,0101h,0100h

RedPad            Dw            0004h,0404h,0400h
                  DW            040Ch,0F41h,0C04h
                  DW            0004h,0404h,0400h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            040Ch,0F41h,0C04h
                  DW            0004h,0404h,0400h
                  DW            040Ch,0F41h,0C04h
                  Dw            0004h,0404h,0400h

Ball1             Dw            0017h,1818h,1700h
                  DW            1719h,1A1Ah,1917h
                  DW            181Ah,1E1Ch,1A18h
                  DW            181Ah,1C1Ch,1A18h
                  DW            1719h,1A1Ah,1917h
                  Dw            0017h,1818h,1700h


Star3Y:           db            186,83,26,130,25,47,91,106,37,130,175,115,79,50,98,172,81,89
                  db            63,5,35,45,141,169,12,8,24,16,99,66,157,145,90,103,4,107,49,157

Star2Y:           db            89,36,145,90,103,4,107,49,157,66,196,169,4,39,20,12,24,25,8
                  db            88,47,145,90,103,4,107,49,157,199,28,9,138,160,111,63,5,35,45

Star1Y:           db            199,28,9,138,160,111,63,5,35,45,141,169,12,8,24,16,99,66,157,66,196
                  db            26,3,154,118,88,32,9,56,63,5,35,45,141,36,6,32,130,25,47,91,00

TextA:            dw            0xffff,0x8001,0x8001,0x8001,0x87E1,0x8421,0x8421,0x87E1
                  dw            0x8001,0x8001,0x87E1,0x8421,0x8421,0x8421,0x8421,0xFC3F

TextC:            dw            0xFFFE,0x8002,0x8002,0x87E2,0x843E,0x8400,0x8400,0x8400
                  dw            0x8400,0x8400,0x843E,0x8422,0x87E2,0x8002,0x8002,0xFFFE

TextE:            dw            0xFFFF,0x8001,0x8001,0x87FF,0x8400,0x8400,0x87FE,0x8002
                  dw            0x8002,0x87FE,0x8400,0x8400,0x87FF,0x8001,0x8001,0xFFFF

TextS:            dw            0xFFFF,0x8001,0x8001,0x87E1,0x843F,0x8400,0x87FF,0x8001
                  dw            0x8001,0xFFE1,0x0021,0xFC21,0x87E1,0x8001,0x8001,0xFFFF

TextM1:           dw            0xFFFF,0x8000,0x8000,0x8000,0x9FE7,0x9024,0x9024,0x9024
                  dw            0x9024,0x9024,0x9024,0x9024,0x9024,0x9024,0x9024,0xF03C

TextM2:           dw            0xFF00,0x0100,0x0100,0x0100,0xF900,0x0900,0x0900,0x0900
                  dw            0x0900,0x0900,0x0900,0x0900,0x0900,0x0900,0x0900,0x0F00

TextP:            dw            0xFFFF,0x8001,0x8001,0x87F9,0x8409,0x8409,0x8409,0x87F9
                  dw            0x8001,0x8001,0x87FF,0x8400,0x8400,0x8400,0x8400,0xFC00 

TextO:            dw            0xFFFF,0x8001,0x8001,0x87E1,0x8421,0x8421,0x8421,0x8421
                  dw            0x8421,0x8421,0x8421,0x8421,0x87E1,0x8001,0x8001,0xFFFF  

TextN:            dw            0xFFFF,0x8001,0x8001,0x87E1,0x8421,0x8421,0x8421,0x8421
                  dw            0x8421,0x8421,0x8421,0x8421,0x8421,0x8421,0x8421,0xFC3F 

TextG:            dw            0xFFFE,0x8002,0x8002,0x87E2,0x843E,0x8400,0x8400,0x8400
                  dw            0x847F,0x8441,0x8463,0x8422,0x87E2,0x8002,0x8002,0xFFFE

message1:         db            'HELLO WORLD',0  
message2:         db            'BY CRAIG BAMFORD 2005 # ',0  
message3:         db            'BLUE PAD IS THE WINNER',0 
message4:         db            'RED PAD IS THE WINNER',0
message5:         db            'CHICKEN',0 

Star1x:           rw            35 
Star2x:           rw            35
Star3x:           rw            35 
;TScreenBuffer    rd            320*20
ScreenBuffer	  rd	        320*190
BSrceenBuffer     rd            320*10
justincase        rd            500