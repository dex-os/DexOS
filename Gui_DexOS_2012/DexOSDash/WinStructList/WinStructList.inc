;=======================================================================;
;                                                                       ;
;               Windows Struct List Table Structures                    ;
;                                                                       ;
;=======================================================================;
align 4
WinStruct:
AppWinID               rb    8                 ;  This is the ID of the window, must be 8 ASCII letters or space (in cap).
AppWinStartX           rw    1                 ;  Start X of window
AppWinStartY           rw    1                 ;  Start Y of window
AppWinSizeY            rw    1                 ;  Window Y size
AppWinSizeX            rw    1                 ;  Window X size
AppWinColor            rd    1                 ;  Color of window
PutImageWinType        rd    1                 ;  Image that sets look of window
                       rd    1                 ;  To be added
                       rd    1                 ;  To be added
WinStructListSpace     rb    32*64             ;  Max 64 windows open at a time