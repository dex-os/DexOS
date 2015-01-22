;=========================================================;
; keyMapUSA.                                   12/12/2010 ;
;---------------------------------------------------------;
; USA ASCII KeyMap.                                       ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
normal_keymap:
     db   '6',27
     db   '1234567890-=',8,9
     db   'qwertyuiop[]',13
     db   '~asdfghjkl;',39,96,0,'\zxcvbnm,./',0,'*',0,' '
     db   '@2345678901',0,'3',55,'89-45'
     db   54,'+',49,50,51,48,46
     db   'AB<D',255,'FGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


shift_keymap:
     db   '6',27
     db   '!@#$%^&*()_+',8,9
     db   'QWERTYUIOP{}',13
     db   '~ASDFGHJKL:"~',0,'|ZXCVBNM<>?',0,'*',0,' '
     db   '@2345678901',0,'3',55,'89-45'
     db   54,'+',49,50,51,48,46
     db   'AB>D',255,'FGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


alt_keymap:
     db   ' ',27
     db   ' @ $  {[]}\ ',8,9
     db   '            ',13
     db   '             ',0,'           ',0,'4',0,' '
     db   '             ',180,178,184,'6',176,'7'
     db   179,'8',181,177,183,185,182
     db   'ABCD',255,'FGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'


caplock_keymap:
     db   '6',27
     db   '1234567890-=',8,9
     db   'QWERTYUIOP[]',13
     db   '~ASDFGHJKL;',39,96,0,'\ZXCVBNM,./',0,'*',0,' '
     db   '@2345678901',0,'3',55,'89-45'
     db   54,'+',49,50,51,48,46
     db   'AB<D',255,'FGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
     db   'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
