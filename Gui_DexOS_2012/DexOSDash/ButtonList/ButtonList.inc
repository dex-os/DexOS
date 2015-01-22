;=======================================================================;
;                                                                       ;
;               Button Struct List Table Structures                     ;
;                                                                       ;
;=======================================================================;
align 4
ButtonStruct:
ButtonID               rd    1                 ;  This is the ID of the Button (a number from 1-255)
ButtonStartX           rw    1                 ;  Start X of Button
ButtonSizeX            rw    1                 ;  Button X size
ButtonStartY           rw    1                 ;  Start Y of Button
ButtonSizeY            rw    1                 ;  Button Y size
;ButtonStatus          rd    1                 ;  Button status 1 = click release in zone, 2 = click release not in zone,3 in zone no click 
ButtonEnd              rd    1                 ;  Button end just zero
ButtonStructListSpace  rb    255*16            ;  Max 255 Button open at a time.
