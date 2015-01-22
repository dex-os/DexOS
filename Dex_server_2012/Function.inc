;====================================================;
; Man_Config.                                        ;
;====================================================;
Man_Config:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Config Shadow.                                ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,14 ;19
loopFShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_FShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_FShadow
	pop   ecx
	loop  loopFShadow
	cld
 ;---------------------;
 ; Draw Config Window. ;
 ;---------------------;
DrawWinFasm:
	mov   esi,WinFasm+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,14 ;19
loopWA2F:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinFasm]
loopWA1F:
	movsb
	stosb
	loop  loopWA1F
	pop   ecx
	loop  loopWA2F
       ; call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Man_Config2.                                       ;
;====================================================;
Man_Config2:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
	cld
 ;---------------------;
 ; Draw Config Window. ;
 ;---------------------;
DrawWinFasm2:
	mov   esi,WinFasm+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,14 ;19
loopWA2F2:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinFasm]
loopWA1F2:
	movsb
	stosb
	loop  loopWA1F2
	pop   ecx
	loop  loopWA2F2
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Win_Settings.                                      ;
;====================================================;
Win_Settings:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Config Shadow.                                ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,11
loopFShadowSet:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_FShadowSet:
	mov   [es:edi],al
	add   edi,2
	loop  rev_FShadowSet
	pop   ecx
	loop  loopFShadowSet
	cld
 ;---------------------;
 ; Draw Config Window. ;
 ;---------------------;
DrawWinFasmSet:
	mov   esi,WinSettings+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,11
loopWA2FSet:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinSettings]
loopWA1FSet:
	movsb
	stosb
	loop  loopWA1FSet
	pop   ecx
	loop  loopWA2FSet
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Auto_Config.                                       ;
;====================================================;
Auto_Config:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Config Shadow.                                ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,14
loopFShadowA:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_FShadowA:
	mov   [es:edi],al
	add   edi,2
	loop  rev_FShadowA
	pop   ecx
	loop  loopFShadowA
	cld
 ;---------------------;
 ; Draw Config Window. ;
 ;---------------------;
DrawWinFasmA:
	mov   esi,WinAuto+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,14 ;19
loopWA2FA:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinAuto]
loopWA1FA:
	movsb
	stosb
	loop  loopWA1FA
	pop   ecx
	loop  loopWA2FA
       ; call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret


;====================================================;
; Win_About.                                         ;
;====================================================;
Win_About:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,14
loopabShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_abShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_abShadow
	pop   ecx
	loop  loopabShadow
	cld
 ;-------------------;
 ; Draw About Window.;
 ;-------------------;
DrawWinAbout: 
	mov   esi,WinAbout+1			       
	xor   ebx,ebx
	mov   bx,8 ;52
	mov   ecx,14 ;19
loopWA2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinAbout]
loopWA1: 
	movsb
	stosb
	loop  loopWA1
	pop   ecx
	loop  loopWA2
	call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Win_Commands.                                      ;
;====================================================;
Win_Commands:
       pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,16
	mov   ecx,17
loopcShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_cShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_cShadow
	pop   ecx
	loop  loopcShadow
	cld
 ;----------------------;
 ; Draw Commands Window.;
 ;----------------------;
DrawWinCommands: 
	mov   esi,WinCommands+1 			  
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,17
loopWC2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinCommands]
loopWC1: 
	movsb
	stosb
	loop  loopWC1
	pop   ecx
	loop  loopWC2
	call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; DrawHelp.                                          ;
;====================================================;
DrawHelp:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4
	mov   ecx,4
loophShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,23
	mov   ecx,17
	mov   al,Shadow1
rev_fhShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fhShadow
	pop   ecx
	loop  loophShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_h: 
	mov   esi,WinHelp+3			      
	xor   ecx,ecx
	mov   cl,[WinHelp+1]
	xor   ebx,ebx
	mov   bx,0 ;12
looph2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,20
	push  ecx
	xor   ecx,ecx
	mov   al,[WinHelp+2]
	mov   cl,[WinHelp]
looph1: 
	movsb
	stosb
	loop  looph1
	pop   ecx
	loop  looph2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,23
	xor   cx,cx
	mov   cl,[WinHelp]
	sub   cl,2
	mov   al,reverseMark
rev_h:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_h
get_h: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_lh
	mov   [BarExit],0
	jmp   end_h
n_r_lh:  
	mov   dx,Help_max
	call  UorD
	cmp   al,0
	je    n_u_dh 
	jmp   draw_h  
n_u_dh: 
	cmp  [AsciiKey],escape_a
	je   end_escape_h
	cmp   [AsciiKey],enter_a
	je    chk_yenter
	jmp   get_h 
 ;----------------------------------------------------;
 ; Help.                                              ;
 ;----------------------------------------------------;
chk_yenter:  
	cmp   [WinY],1
	jne   chk_NotCommends
	call  Cls
	call  Win_Commands
	jmp   end_escape_h
chk_NotCommends:
	cmp   [WinY],2
	jne   chk_NotAbout
	call  Cls
	call  Win_About
	jmp   end_escape_h
chk_NotAbout:
	jmp  get_h	  
end_escape_h:
	mov   [BarExit],1
end_h:
	pop   es
	popad
	ret

;====================================================;
; DrawFile.                                          ;
;====================================================;
DrawFile:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4 ;12
	mov   ecx,8
loopfShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,5 ;7
	mov   ecx,17
	mov   al,Shadow1
rev_fShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fShadow
	pop   ecx
	loop  loopfShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_f: 
	mov   esi,WinFile+3			      
	xor   ecx,ecx
	mov   cl,[WinFile+1]
	xor   ebx,ebx
	mov   bx,0
loopf2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,2
	push  ecx
	xor   ecx,ecx
	mov   al,[WinFile+2]
	mov   cl,[WinFile]
loopf1: 
	movsb
	stosb
	loop  loopf1
	pop   ecx
	loop  loopf2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,5 ;7
	xor   cx,cx
	mov   cl,[WinFile]
	sub   cl,2
	mov   al,reverseMark
rev_f:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_f
get_f: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_l
	mov   [BarExit],0
	jmp   end_f
n_r_l:	
	mov   dx,file_max
	call  UorD
	cmp   al,0
	je    n_u_df 
	jmp   draw_f  
n_u_df: 
	cmp  [AsciiKey],escape_a
	je   end_escape_f
nextf1: 
	cmp   [AsciiKey],enter_a
	je    chk_y
	jmp   get_f
 ;----------------------------------------------------;
 ; Netstats.                                          ;
 ;----------------------------------------------------;
chk_y:	
	cmp   [WinY],1 ;4
	jne   chk_y2
	cmp   [StackNotLoaded],1
	je    StackErrorYes				      ;test stack loaded
	call  Cls
	mov   al,0x7f
	call  [TextColor]
	call  Win_Netstat
	call  GoToXY
	call  Cursor_0
	jmp   end_escape_f
chk_y2:
 ;----------------------------------------------------;
 ; Start server.                                      ;
 ;----------------------------------------------------;
	cmp   [WinY],2 ;5
	jne   chk_y3
	cmp   [StackNotLoaded],1
	je    StackErrorYes				      ;test stack loaded
	call  Cls
	mov   al,0x7f
	call  [TextColor]
	call  Win_ServerStats
	call  GoToXY
	call  Cursor_0
	jmp   end_escape_f
chk_y3:
 ;----------------------------------------------------;
 ; Net settings.                                      ;
 ;----------------------------------------------------;
	cmp   [WinY],3
	jne   chk_y4
	cmp   [StackNotLoaded],1
	je    StackErrorYes	    ;test stack loaded
	call  Cls
	call  Win_Settings
	call  GetStackSettings
	mov   al,0x7f
	call  [TextColor]
;ip
	mov   al,31
	mov   ah,7
	call  [SetCursorPos]
	mov   edx,ip_address
	call  printIP
;GatewayIp
	mov   al,31
	mov   ah,8
	call  [SetCursorPos]
	mov   edx,gateway_ip
	call  printIP
;SubNet
	mov   al,31
	mov   ah,9
	call  [SetCursorPos]
	mov   edx,subnet_mask
	call  printIP
;DnsIp
	mov   al,31
	mov   ah,10
	call  [SetCursorPos]
	mov   edx,dns_ip
	call  printIP
;Lease
	mov   al,31
	mov   ah,11
	call  [SetCursorPos]
	cmp   [YesForEver],1
	jne   NotForever0
	mov   esi,msg10
	call  [PrintString_0]
	jmp   NotForever1
NotForever0:
	mov   eax,[LeaseTime]
       ; call  [WriteHex32]
NotForever1:
	call  [WaitForKeyPress]
EscPressed:
	call  GoToXY
	call  Cursor_0
	jmp   end_escape_f 
chk_y4:
 ;----------------------------------------------------;
 ; Auto config.                                       ;
 ;----------------------------------------------------;
	cmp   [WinY],4
	jne   chk_y5
	cmp   [StackNotLoaded],1       ;test stack loaded
	je    StackErrorYes
	call  Cls
	call  Auto_Config
	call  TestForDHCP
	jnc   DHCPok
	cmp   [DhcpErrorCode],1
	je    StackErrorYes
	call  Cls
	call  Cursor_0
	call  DhcpError
	jmp   end_escape_f
StackErrorYes:
	call  Cls
	call  Cursor_0
	call  StackError
	jmp   end_escape_f
DHCPok:
	mov   al,0x7f
	call  [TextColor]
;ip
	mov   al,31
	mov   ah,9
	call  [SetCursorPos]
	mov   edx,dhcpClientIP
	call  printIP
;GatewayIp
	mov   al,31
	mov   ah,10
	call  [SetCursorPos]
	mov   edx,dhcpClientGatewayIp
	call  printIP
;SubNet
	mov   al,31
	mov   ah,11
	call  [SetCursorPos]
	mov   edx,dhcpClientSubNet
	call  printIP
;DnsIp
	mov   al,31
	mov   ah,12
	call  [SetCursorPos]
	mov   edx,dhcpClientDnsIp
	call  printIP
	mov   [WinXDHCP],1
	call  WinHeadSetColorreversedDHCP
NotAltDHCP:
	call  GetKey
	call  LorRDHCP
	cmp   al,0
	je    NoLorRDHCP
	call  WinHeadSetColorreversedDHCP
	jmp   NotAltDHCP
NoLorRDHCP:
	cmp   [AsciiKey],enter_a
	jne   NotAltDHCP
	cmp   [WinXDHCP],1
	jne   DoNotSaveDHCP
	call  SetDhcpSettings
	jmp   EscPressed
DoNotSaveDHCP:
	jmp   EscPressed
 chk_y5:
 ;----------------------------------------------------;
 ; Manual config.                                     ;
 ;----------------------------------------------------;
	cmp   [WinY],5
	jne   chk_y6
	cmp   [StackNotLoaded],1       ;test stack loaded
	je    StackErrorYes
	call  Cls
	call  Man_Config
TryAgain:
	call  Cursor_1
	mov   dl,20
	mov   dh,6
	call  GoToXYTexWin
;1
	mov   al,31
	mov   ah,9
	call  [SetCursorPos]
	mov   al,0x7f
	call  [TextColor]
	mov   bx,100
	mov   dx,16
	call  [GetUserInput]
	jc    EscPressed
	call  IPAddress
;2
	mov   al,31
	mov   ah,10
	call  [SetCursorPos]
	mov   bx,100
	mov   dx,16
	call  [GetUserInput]
	jc    EscPressed
	call  GatewayIP
;3
	mov   al,31
	mov   ah,11
	call  [SetCursorPos]
	mov   bx,100
	mov   dx,16
	call  [GetUserInput]
	jc    EscPressed
	call  SubnetMask
;4
	mov   al,31
	mov   ah,12
	call  [SetCursorPos]
	mov   bx,100
	mov   dx,16
	call  [GetUserInput]
	jc    EscPressed
	call  DNSIP
	mov   [WinXC],1
	call  WinHeadSetColorreversedC
NotAltC:
	call  GetKey
	call  LorRC
	cmp   al,0
	je    NoLorRC
	call  WinHeadSetColorreversedC
	jmp   NotAltC
NoLorRC:
	cmp   [AsciiKey],enter_a
	jne   NotAltC
	cmp   [WinXC],1
	jne   DoNotSave
	call  SetAddress
	jnc   EscPressed
	call  Cls
	call  Cursor_0
	call  StackError
	jmp   EscPressed
DoNotSave:
	cmp   [WinXC],2
	jne   EscPressed ; TryAgain
	call  Man_Config2
	jmp   TryAgain
	jmp   EscPressed
chk_y6:
 ;----------------------------------------------------;
 ; Exit.                                              ;
 ;----------------------------------------------------;  
	cmp   [WinY],6
	je    ExitTex4u
	jmp   get_f			     
ExitTex4u:
       mov   [ExitEditor],1
       jmp   end_escape_f
end_escape_f:
	mov   [BarExit],1
end_f:
	pop   es
	popad
	ret

;====================================================;
; LorRDHCP.                ;Right or Left Key <- ->  ;
;====================================================;
LorRDHCP:
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],left
	jne   next1DHCP
	cmp   [WinXDHCP],1
	ja    x_minDHCP
	mov   [WinXDHCP],win_max+1
x_minDHCP:
	dec   [WinXDHCP]
	mov   al,1
next1DHCP:
	cmp   [ScanKey],right
	jne   next2DHCP
	cmp   [WinXDHCP],win_max
	jb    x_maxDHCP
	mov   [WinXDHCP],0
x_maxDHCP:
	inc   [WinXDHCP]
	mov   al,1		   ;Right or Left is True
next2DHCP:
	ret

;====================================================;
; LorRC.                   ;Right or Left Key <- ->  ;
;====================================================;
LorRC:
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],left
	jne   next1C
	cmp   [WinXC],1
	ja    x_minC
	mov   [WinXC],win_maxC+1
x_minC:
	dec   [WinXC]
	mov   al,1
next1C:
	cmp   [ScanKey],right
	jne   next2C
	cmp   [WinXC],win_maxC
	jb    x_maxC
	mov   [WinXC],0
x_maxC:
	inc   [WinXC]
	mov   al,1		   ;Right or Left is True
next2C:
	ret

;====================================================;
; LorR.                    ;Right or Left Key <- ->  ;
;====================================================;
LorR:			  
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],left
	jne   next1
	cmp   [WinX],1
	ja    x_min
	mov   [WinX],win_max+1
x_min: 
	dec   [WinX]
	mov   al,1
next1:	
	cmp   [ScanKey],right
	jne   next2
	cmp   [WinX],win_max
	jb    x_max
	mov   [WinX],0
x_max: 
	inc   [WinX]
	mov   al,1		   ;Right or Left is True
next2:	
	ret

;====================================================;
; UorD.                            ;Up or Down Key   ;
;====================================================;
UorD:
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],up
	jne   nextv1
	cmp   [WinY],1 ;4 ;1
	ja    y_min
	mov   [WinY],dx
	inc   [WinY]
y_min:	
	dec   [WinY]
	mov   al,1		   ;Up or Down is True
nextv1: 
	cmp   [ScanKey],down
	jne   nextv2
	cmp   [WinY],dx
	jb    y_max
	mov   [WinY],0 ;3
y_max:	
	inc   [WinY]
	mov   al,1		   ;Up or Down is True
nextv2: 
	ret


;====================================================;
; DrawScreen.                                        ;
;====================================================;
WinHeadSetColorreversed:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   esi,edi
	cmp   [WinX],1
	jne   N1
	add   edi,3
	xor   ecx,ecx 
	mov   cx,9		   ;[File] be reversed
Loop1:	  
	cmp   byte [es:edi],0x8f
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse1
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse1:
	add   edi,2
	loop  Loop1
N1: 
	cmp   [WinX],2
	jne   N2
	add   edi,21
	mov   cx,6		   ;[Edit] be reversed
Loop2:	  
	cmp   byte [es:edi],0x8f
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse2
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse2:
	add   edi,2
	loop  Loop2
N2:

	pop   es
	popad
	ret
;====================================================;
; TopMenuBar.                                        ;
;====================================================;
TopMenuBar:
	pushad
	push  es
	mov   [WinY],4
	mov   [WinX],1
	mov   [BarExit],0
	mov   [MenuBar],1
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
NotAlt:
	call  GetKey
	call  LorR 
	cmp   al,0
	je    NoLorR
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	jmp   NotAlt
NoLorR:
	cmp   [ScanKey],Alt
	je    TopMenuBarExit 
	cmp   [AsciiKey],'n'
	jne   TryNext1
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],1
	call  WinHeadSetColorreversed

	call  DrawFile
	call  Cls


	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext1:
	cmp   [AsciiKey],'N'
	jne   TryNext2
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],1
	call  WinHeadSetColorreversed
	call  DrawFile
	call  Cls

	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext2:
	cmp   [AsciiKey],'h'
	jne   TryNext3
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],2
	call  WinHeadSetColorreversed
	call  DrawHelp
	call  Cls

	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext3:
	cmp   [AsciiKey],'H'
	jne   TryNext4
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],2
	call  WinHeadSetColorreversed
	call  DrawHelp
	call  Cls
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext4:
	cmp   [AsciiKey],enter_a
	jne   TryNext9
LorRpressed:
	cmp   [WinX],1
	jne   WinXisNot1
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  DrawFile
	call  Cls
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
WinXisNot1:
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  Cls
	call  DrawHelp
	call  Cls
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext9:
	jmp   NotAlt
TopMenuBarExit:
	mov   [MenuBar],0
	call  WinHeadSetColor
	pop   es
	popad
	ret 


;====================================================;
; WinHeadSetColor.                                   ;
;====================================================;
WinHeadSetColor:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   esi,WinHead1
	add   edi,2
	mov   al,0x2f
	xor   ecx,ecx
	mov   cx,20
loopWinHeadSetColor1: 
	movsb
	stosb
	loop  loopWinHeadSetColor1
	mov   [color4],0x2e
	cmp   [MenuBar],1
	je    @f
	mov   [color4],0x2f
@@:
	mov   edi,0xb8000
	add   edi,4 ;8
	mov   ah,[color4]
	mov   al,'N'
	stosw
	mov   edi,0xb8000
	add   edi,22 ;30
	mov   ah,[color4]
	mov   al,'H'
	stosw
	pop   es
	popad
	ret
;====================================================;
; FullWhiteBar.                                      ;
;====================================================;
TopGreenBar:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   cx,80
	mov   ah,0x2f
	mov   al,' '
	rep   stosw
	pop   es
	popad
	ret

;====================================================;
; MediumWhiteBar.                                    ;
;====================================================;
BottomGreenBar:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	add   edi,160*24
	mov   cx,80
	mov   ah,0x2f
	mov   al,' '
	rep   stosw
	cld
	mov   edi,0xb8000
	add   edi,160*24
	mov   ecx,18 ;10
	mov   esi,AltMessage
	add   edi,1*2
	mov   ah,0x2f
loop_ALT:
	lodsb
	stosw
	loop  loop_ALT
	pop   es
	popad
	ret 

;====================================================;
; Cls.                                               ;
;====================================================;
Cls:
	pushad
	push  es
	mov   ax,18h
	mov   es,ax
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+4*1]
	mov   ax,8h
	mov   es,ax
	mov   ah,color2
	mov   al,' '
	mov   cx,23*80	     ;22*80
	rep   stosw
	pop   es
	popad
	ret

;====================================================;
; GetChar.                                           ;
;====================================================;
GetChar:  
	push  ax
	pushf
	call  [WaitForKeyPress]  
	mov   byte[Char],ah
	popf
	pop   ax
	ret

;====================================================;
; GetKey.                                            ;
;====================================================;
GetKey:   
	push  ax
	pushf
	call  [WaitForKeyPress]
	mov   byte[ScanKey],ah
	mov   byte[AsciiKey],al
	popf
	pop   ax
	ret

;====================================================;
; Cursor_1.                                          ;
;====================================================;
Cursor_1:
	push  cx
	mov   ch,0x0b ;00
	mov   cl,0x0c ;14
	mov   ah,1
	call  [RealModeInt10h]
	pop   cx
	ret  

;====================================================;
; Cursor_0.                                          ;
;====================================================;
Cursor_0:
	push  cx
	mov   ch,0x1c
	mov   cl,0x1b
	mov   ah,1
	call  [RealModeInt10h]
	pop   cx
	ret  

;====================================================;
; GoToXYBios.                             ;AL=X,AH=Y ;
;====================================================;
GoToXYDex:
	mov   al,byte[PosX]
	mov   ah,byte[PosY]
	call  [SetCursorPos]
	ret

;====================================================;
; GoToXY.                                            ;
;====================================================;
GoToXY:
	pushad
	mov	dl,byte[PosX]
	mov	dh,byte[PosY]
	mov	al,80
	mul	dh
	xor	dh,dh
	add	ax,dx
	mov	cx,ax
	mov	dx,0x03d4
	mov	al,0x0e
	out	dx,al
	inc	dx
	mov	al,ch
	out	dx,al
	mov	dx,0x3d4
	mov	al,0x0f
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	popad
	ret

;====================================================;
; GoToXY.                                            ;
;====================================================;
GoToXYTexWin:
	pushad
	mov	al,80
	mul	dh
	xor	dh,dh
	add	ax,dx
	mov	cx,ax
	mov	dx,0x03d4
	mov	al,0x0e
	out	dx,al
	inc	dx
	mov	al,ch
	out	dx,al
	mov	dx,0x3d4
	mov	al,0x0f
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	popad
	ret

;====================================================;
; DrawScreen.                                        ;
;====================================================;
WinHeadSetColorreversedC:
	pushad
	push  es
	mov   edi,0xb8000
	add   edi,80*14*2
	mov   ax,8h
	mov   es,ax
	add   edi,35
	xor   ecx,ecx 
	mov   cx,11		     ;[File] be reversed
	cmp   [WinXC],1
	jne   Loop1Ca
Loop1C:
	mov   al,reverseMark
	mov   [es:edi],al
	add   edi,2
	loop  Loop1C
	jmp   N1C
Loop1Ca:
	mov   al,0x78
	mov   [es:edi],al
	add   edi,2
	loop  Loop1Ca
N1C:
	add   edi,8
	mov   cx,14		    ;[Edit] be reversed
	cmp   [WinXC],2
	jne   N2C
Loop2C:
	mov   al,reverseMark
	mov   [es:edi],al
	add   edi,2
	loop  Loop2C
	jmp   N3C
N2C:
	mov   al,0x78
	mov   [es:edi],al
	add   edi,2
	loop  N2C
N3C:
	add   edi,8
	mov   cx,13		    ;[Edit] be reversed
	cmp   [WinXC],3
	jne   N3Ca
Loop3C:
	mov   al,reverseMark
	mov   [es:edi],al
	add   edi,2
	loop  Loop3C
	jmp   EndC
N3Ca:
	mov   al,0x78
	mov   [es:edi],al
	add   edi,2
	loop  N3Ca
EndC:
	pop   es
	popad
	ret

;====================================================;
; DrawScreen.                                        ;
;====================================================;
WinHeadSetColorreversedDHCP:
	pushad
	push  es
	mov   edi,0xb8000
	add   edi,80*14*2
	mov   ax,8h
	mov   es,ax
	add   edi,51 ;35
	xor   ecx,ecx 
	mov   cx,11		     ;[File] be reversed
	cmp   [WinXDHCP],1
	jne   Loop1DHCPa
Loop1DHCP:
	mov   al,reverseMark
	mov   [es:edi],al
	add   edi,2
	loop  Loop1DHCP
	jmp   N1DHCP
Loop1DHCPa:
	mov   al,0x78
	mov   [es:edi],al
	add   edi,2
	loop  Loop1DHCPa
N1DHCP:
	add   edi,10 ;8
	mov   cx,13		    ;[Edit] be reversed
	cmp   [WinXDHCP],2
	jne   N2DHCP
Loop2DHCP:
	mov   al,reverseMark
	mov   [es:edi],al
	add   edi,2
	loop  Loop2DHCP
	jmp   N3DHCP
N2DHCP:
	mov   al,0x78
	mov   [es:edi],al
	add   edi,2
	loop  N2DHCP
N3DHCP:
	pop   es
	popad
	ret

;====================================================;
; Stack Error.                                       ;
;====================================================;
StackError:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,24; 12
	mov   ecx,5 ;4
loopfdeShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,49 ;7
	mov   ecx,32
	mov   al,Shadow1
rev_fdeShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fdeShadow
	pop   ecx
	loop  loopfdeShadow
	cld
 ;-------------------;
 ; Draw File Window. ;
 ;-------------------;
DrawFddError: 
	mov   esi,Stack_Error+1
	xor   ebx,ebx
	mov   bx,20 ;8
	mov   ecx,5 ;4
loopFE2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,46 ;4
	push  ecx
	xor   ecx,ecx
	mov   al,error1      
	mov   cl,[Stack_Error]
loopFE1: 
	movsb
	stosb
	loop  loopFE1
	pop   ecx
	loop  loopFE2
	call  Beep
	call  [WaitForKeyPress] 
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Dhcp Error.                                        ;
;====================================================;
DhcpError:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Shadow.                                       ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,24
	mov   ecx,5
loopfdeShadowDhcp:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,49
	mov   ecx,32
	mov   al,Shadow1
rev_fdeShadowDhcp:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fdeShadowDhcp
	pop   ecx
	loop  loopfdeShadowDhcp
	cld
 ;-------------------;
 ; Draw File Window. ;
 ;-------------------;
DrawFddErrorDhcp:
	mov   esi,Dhcp_Error+1
	xor   ebx,ebx
	mov   bx,20
	mov   ecx,5
loopFE2Dhcp:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,46
	push  ecx
	xor   ecx,ecx
	mov   al,error1      
	mov   cl,[Dhcp_Error]
loopFE1Dhcp:
	movsb
	stosb
	loop  loopFE1Dhcp
	pop   ecx
	loop  loopFE2Dhcp
	call  Beep
	call  [WaitForKeyPress] 
	pop   ds
	pop   es
	popad
	ret


 ;----------------------------------------------------;
 ; Beep                                               ;
 ;----------------------------------------------------; 
Beep:
	pushad
	mov   [Hz],0x200 
	call  Sound
	mov   ax,3
	call  [SetDelay]
	call  NoSound
	popad
	ret
 ;----------------------------------------------------;
 ; Sound                                              ;
 ;----------------------------------------------------;
Sound:
	mov   bx,[Hz]
	mov   ax,0x34dd 	       
	mov   dx,0x0012 	       
	cmp   dx,bx		   
	jnc   Done1		  
	div   bx		
	mov   bx,ax
	in    al,0x61
	test  al,3
	jnz   A99		 
	or    al,3
	out   0x61,al		     
	mov   al,0xb6
	out   0x43,al
A99:	
	mov   al,bl 
	out   0x42,al		   
	mov   al,bh
	out   0x42,al
Done1:
	ret

 ;----------------------------------------------------;
 ; NoSound                                            ;
 ;----------------------------------------------------;
NoSound:
	in    al,0x61		      
	and   al,11111100b
	out   0x61,al		     
	ret 
