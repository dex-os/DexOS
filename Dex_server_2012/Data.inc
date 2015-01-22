;====================================================;
; Data.                                              ;
;====================================================;
;WinHead1     db '   File     Edit     Search     Help  '
WinHead1     db ' Network  Help      '
align 4
Ytable		    dd 0, 160, 320, 480, 640, 800, 960,1120,1280,1440,1600,1760,1920
		    dd 2080,2240,2400,2560,2720,2880,3040,3200,3360,3520,3680,3840
		    dd 4000,4160, 4320, 4480, 4640, 4800, 4960,5120,5280,5440,5600,5760,5920
		    dd 6080,6240,6400,6560,6720,6880,7040,7200,7360,7520,7680,7840

WinFile      db 17,8,color2a
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³ Netstats      ³'
	     db '³ Start Server  ³'
	     db '³ Net Settings  ³'
	     db '³ Auto Config   ³'
	     db '³ Manual Config ³'
	     db '³ Exit....      ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinHelp      db 17,4,color2a
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³ TCP/IP Help   ³'
	     db '³ About DexServ ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

Stack_Error:   db 32
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³  *****  STACK ERROR!  *****  ³'
	     db '³    No TCP/IP Stack Loaded    ³'
	     db '³   Press anykey to continue   ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

Dhcp_Error:   db 32
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³  *****  DHCP  ERROR!  *****  ³'
	     db '³     No DHCP Server Found!    ³'
	     db '³   Press anykey to continue   ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinAbout:    db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³       ______          _____                        ³'
	     db '³       |  _  \        /  ___|                       ³'
	     db '³       | | | |_____  _\ `--.  ___ _ ____   __       ³'
	     db "³       | | | / _ \ \/ /`--. \/ _ \ '__\ \ / /       ³"
	     db '³       | |/ /  __/>  </\__/ /  __/ |   \ V /        ³'
	     db '³       |___/ \___/_/\_\____/ \___|_|    \_/         ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                 HTTP SERVER v0.01                  ³'
	     db '³             Coded by Team DexOS 2007               ³'
	     db '³                   WWW.DEX4U.COM                    ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

ServerStats: db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³       ______          _____                        ³'
	     db '³       |  _  \        /  ___|                       ³'
	     db '³       | | | |_____  _\ `--.  ___ _ ____   __       ³'
	     db "³       | | | / _ \ \/ /`--. \/ _ \ '__\ \ / /       ³"
	     db '³       | |/ /  __/>  </\__/ /  __/ |   \ V /        ³'
	     db '³       |___/ \___/_/\_\____/ \___|_|    \_/         ³'
	     db '³                                                    ³'
	     db '³              DexOS Http Server v0.01               ³'
	     db '³                                                    ³'
	     db '³         Server status           :                  ³'
	     db '³         Current Time            :                  ³'
	     db '³         Current Date            :                  ³'
	     db '³         Restart Time            :                  ³'
	     db '³         Restart Date            :                  ³'
	     db '³         Total Accesses          :                  ³'
	     db '³         Total Bytes             :                  ³'
	     db '³         Total 404               :                  ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

NetstatAbout: db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³                     Netstat                        ³'
	     db '³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³'
	     db '³                                                    ³'
	     db '³         Ethernet card status    : xxxxxxxx         ³'
	     db '³                                                    ³'
	     db '³         IP packets received     : xxxxxxxx         ³'
	     db '³         ARP packets received    : xxxxxxxx         ³'
	     db '³         Dumped received packets : xxxxxxxx         ³'
	     db '³                                                    ³'
	     db '³         EMPTY QUEUE             : xxxxxxxx         ³'
	     db '³         IPOUT QUEUE             : xxxxxxxx         ³'
	     db '³         IPIN  QUEUE             : xxxxxxxx         ³'
	     db '³         NET1OUT QUEUE           : xxxxxxxx         ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinCommands: db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³                     Help Text                      ³'
	     db '³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinFasm:     db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³              Set Network Properties                ³'
	     db '³       Please enter numbers and press enter         ³'
	     db '³  to move onto next, then comfirm settings at end.  ³'
	     db '³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³'
	     db '³                                                    ³'
	     db '³   IP Address:  [               ]   <press enter>   ³'
	     db '³   Gateway IP:  [               ]   <press enter>   ³'
	     db '³   Subnet Mask: [               ]   <press enter>   ³'
	     db '³   DNS IP:      [               ]   <press enter>   ³'
	     db '³                                                    ³'
	     db '³    Save Exit      New Settings      Exit NoSave    ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinSettings: db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³                Network Properties                  ³'
	     db '³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³'
	     db '³                                                    ³'
	     db '³   IP Address:  [                            ]      ³'
	     db '³   Gateway IP:  [                            ]      ³'
	     db '³   Subnet Mask: [                            ]      ³'
	     db '³   DNS IP:      [                            ]      ³'
	     db '³   Lease :      [                            ]      ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

WinAuto:     db 54
	     db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿'
	     db '³              AutoDHCP 1.01 By hidnplayr            ³'
	     db '³        Looking for dhcp server, Please wait        ³'
	     db '³  Testing ',0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,0b1h,'   ³'
	     db '³ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ³'
	     db '³                                                    ³'
	     db '³   IP Address:  [                             ]     ³'
	     db '³   Gateway IP:  [                             ]     ³'
	     db '³   Subnet Mask: [                             ]     ³'
	     db '³   DNS IP:      [                             ]     ³'
	     db '³                                                    ³'
	     db '³            Save Exit       Exit NoSave             ³'
	     db '³                                                    ³'
	     db 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ'

AltMessage db 'Press Alt for Menu',0,0,0,0

MenuBar 	    db 0
color4		    db 0
OutPutMonitor	    dd 0
PosX		    db 0
PosY		    db 0
ScanKey 	    db 0
AsciiKey	    db 0
Char		    db 0
ErrorCode	    db 0

ExitEditor	    db 0
ShowText	    db 0
WinY		    dw 0
WinX		    dw 0
WinXC		    dw 0
WinXDHCP	    dw 0
BarExit 	    db 0
Hz		    dw 0
StackNotLoaded	    db 0
StandaredPalette    dd 0