;=========================================================;
; 9k Space Invaders                            00/00/2012 ;
;---------------------------------------------------------;
;-------------------------------;                         ;
; 9k Space Invaders Version 1.1 ;                         ;
;                               ;                         ;
; Copyright (c) by Paul S. Reid ;                         ;
;      All rights reserved.     ;                         ;
;-------------------------------;                         ;
; Converted to run on  DexOS                              ;
;  by Craig Bamford (Dex)                                 ;
;                                                         ;
;=========================================================;
format binary as 'dex'					  ;
Sound_delay = 1 					  ; 08000h
use32							  ;
	ORG	0x1A00000				  ; where our program is loaded to
	jmp	start					  ; jump to the start of program.
	db	'DEX6'					  ; We check for this, to make shore it a valid DexOS file ver.
start:							  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
;=======================================================  ;
; Get calltable address.                                  ;
;=======================================================  ;
	mov	edi,Functions				  ; fill the function table
	mov	al,0					  ; so we have some usefull functions
	mov	ah,0x0a 				  ;
	int	50h					  ;
;=======================================================  ;
; Change to mode 13h.                                     ;
;=======================================================  ;
	mov	ax,0x0013				  ;
	call	[RealModeInt10h]			  ;
;=======================================================  ;
;  Replace keyboard with Game keyboard.                   ;
;=======================================================  ;
	call	[GameKeyBoardOn]			  ;   Install game keyboard handler
	mov	[KeyDown],esi				  ;
	call	[GetRmPalette]				  ;
	mov	dword[standardP],edx			  ;
	mov	esi,Palette				  ; Set Palette
	mov	ecx,255 				  ; Number of color registers to set (255 = all of them)
	mov	edx,0x03c8				  ;
	mov	al,0					  ;
	out	dx,al					  ;
	inc	dx					  ;
set_pal:						  ;
	mov	al,[esi]				  ;
	out	dx,al					  ;
	mov	al,[esi+1]				  ;
	out	dx,al					  ;
	mov	al,[esi+2]				  ;
	out	dx,al					  ;
	add	esi,4					  ;
	loop	set_pal 				  ;
	mov	[Seed],935				  ;
	mov	ax,18h					  ;
	mov	es,ax					  ;
	mov	ds,ax					  ;
	mov	edi,VideoBuffer 			  ;
	mov	ecx,320*200/4				  ;
	mov	eax,0x00000000				  ;
	cld						  ;
	rep	stosd					  ;
	mov	edi,VideoBuffer 			  ;
	mov	ecx,10240				  ;
DrawTop:						  ;
	mov	ebx,0008fh				  ; Random number between 0 and 20 in AX
	mov	word[TempStore],cx			  ;
	call	Random					  ; Call random routine
	cmp	[RandomNumber],1			  ;
	jnz	NoStar					  ;
	mov	al,12					  ;
	jmp	DrawTop2				  ;
NoStar: 						  ;
	mov	al,8					  ;
DrawTop2:						  ;
	stosb						  ;
	mov	cx,word[TempStore]			  ;
	loop	DrawTop 				  ;
	mov	edi,VideoBuffer 			  ;
	add	edi,54400				  ; Store address in destination register
	mov	ecx,9600				  ; 32 lines
DrawBottom:						  ;
	mov	ebx,0008fh				  ; Random number between 0 and 20 in AX
	mov	word[TempStore],cx			  ;
	call	Random					  ; Call random routine
	cmp	[RandomNumber],1			  ;
	jnz	NoStar2 				  ;
	mov	al,12					  ;
	jmp	DrawBottom2				  ;
NoStar2:						  ;
	mov	al,8					  ;
DrawBottom2:						  ;
	stosb						  ;
	mov	cx,word[TempStore]			  ;
	loop	DrawBottom				  ;
	mov	eax,LogoOutline 			  ; Get address of sprite
	mov	bx,74					  ; Get X position to draw sprite at
	mov	dl,0					  ; Get Y position to draw sprite at
	mov	dh,10					  ; Get color to draw sprite
	CALL	DrawLogoLayer				  ; Draw sprite
	mov	eax,LogoShadow				  ; Get address of sprite
	mov	bx,74					  ; Get X position to draw sprite at
	mov	dl,0					  ; Get Y position to draw sprite at
	mov	dh,9					  ; Get color to draw sprite
	CALL	DrawLogoLayer				  ; Draw sprite
	mov	eax,LogoLetters 			  ; Get address of sprite
	mov	bx,74					  ; Get X position to draw sprite at
	mov	dl,0					  ; Get Y position to draw sprite at
	mov	dh,11					  ; Get color to draw sprite
	CALL	DrawLogoLayer				  ; Draw sprite
;=======================================================  ;
;  Mov buffer to screen                                   ;
;=======================================================  ;
	push	es					  ;
	mov	ax,8h					  ;
	mov	es,ax					  ;
	mov	esi,VideoBuffer 			  ;
	mov	edi,0xA0000				  ;
	mov	ecx,320*200/4				  ;
	rep	movsd					  ;
	pop	es					  ;
	JMP	TitleScreen				  ;
StartGame:						  ;
	CALL	ResetGame				  ;
	MOV	EDI,VideoBuffer 			  ; Store address in destination register
	MOV	ECX,21760				  ; 136 lines (160 * 136)
	MOV	AX,0					  ;
ClearAll2:						  ;
	STOSW						  ;
	LOOP	ClearAll2				  ;
	CALL	DisplayStatus				  ;
							  ; Outter game loop
RedrawBunkers:						  ;
	CALL	DrawBunkers				  ;
	MOV	[FirstFrame],1				  ;
							  ; Inner game loop
NoExit: 						  ;
	MOV	EAX,PlayersShip 			  ; Get address of sprite
	MOV	BX,[PlayerX]				  ; Get X position to draw sprite at
	MOV	DL,130					  ; Get Y position to draw sprite at
	MOV	DH,1					  ; Get color to draw sprite
	CALL	DrawSprite				  ; Draw sprite
	CALL	CheckPlayerDead 			  ;
	MOV	EAX,Score				  ;
	MOV	[ScoreValueOffset],EAX			  ;
	MOV	[ScoreXOffset],67			  ;
	CALL	DisplayScore				  ;
	CALL	UpdateHighScore 			  ;
	MOV	EAX,HighS				  ;
	MOV	[ScoreValueOffset],EAX			  ;
	MOV	[ScoreXOffset],161			  ;
	CALL	DisplayScore				  ;
							  ; Prepare for buffer blit
	PUSH	ES					  ;
	MOV	AX,8H					  ;
	MOV	ES,AX					  ;
	MOV	ESI,VideoBuffer 			  ;
	MOV	EDI,10560				  ; Store address in destination register
	ADD	EDI,0xA0000				  ; Get segment of video memory
	MOV	ECX,21760				  ; 150 lines (136 * 150)
							  ; Check vertical retrace
	MOV	DX,03DAh				  ; Get vertical retrace port address in DX
;RetraceStart:                                            ;
;       IN      AL,DX                                     ; Grab retrace information
;       TEST    AL,8                                      ; Did it start yet?
;       JNZ     RetraceStart                              ; No, wait until it does
RetraceEnd:						  ;
	IN	AL,DX					  ; Grab retrace information again
	TEST	AL,8					  ; Did it end yet?
	JZ	RetraceEnd				  ; No, loop until it does
							  ; Blit buffer to video memory
BlitAll:						  ;
	MOVSW						  ;
	LOOP	BlitAll 				  ;
	POP	ES					  ;
	CMP	[FirstFrame],1				  ;
	JNZ	NoFirstFrame				  ;
	CALL	DrawInvaders				  ;
	MOV	EAX,GetReady				  ;
	MOV	BX,82					  ;
	MOV	DL,20					  ;
	MOV	DH,7					  ;
	CALL	PrintText				  ;
	MOV	EAX,36					  ;
	CALL	[TimerNoWait]				  ;
Wait1:							  ;
	MOV	AL,0					  ;
	CALL	[TimerNoWait]				  ;
	CMP	AL,1					  ;
	JE	Wait1					  ;
	MOV	EAX,GetReady				  ;
	MOV	BX,82					  ;
	MOV	DL,20					  ;
	MOV	DH,0					  ;
	CALL	PrintText				  ;
	MOV	[FirstFrame],0				  ;
NoFirstFrame:						  ;
       CMP     [SoundToggle],1				  ;
       JNZ	NoSoundToggle				  ;
       MOV     [SoundToggle],0				  ;
       XOR     [Sound],080h				  ;
NoSoundToggle:						  ;
       CALL    EraseInvaders				  ;
       DEC     [MoveCount]				  ;
       JNZ     NoInvaderMove				  ;
       CALL    MoveInvaders				  ;
NoInvaderMove:						  ;
       MOV     ESI,[KeyDown]				  ;
       CMP     BYTE[ESI + Space],1			  ;
       JNZ     NoFire					  ;
       CMP     [MissileX],0				  ;
       JNZ     NoFire					  ;
       CALL    ShootPlayerMissile			  ;
NoFire: 						  ;
       CALL    DrawInvaders				  ;
       CMP     [MissileX],0				  ;
       JZ      NoMissile				  ;
       CALL    MovePlayerMissile			  ;
       CMP     [Collision],0				  ;
       JZ      NoMissile				  ;
       CALL    EraseInvaders				  ;
       MOV     EAX,Score				  ;
       MOV     [ScoreValueOffset],EAX			  ;
       MOV     [ScoreXOffset],67			  ;
       CALL    EraseScore				  ;
       MOV     EAX,HighS				  ;
       MOV     [ScoreValueOffset],EAX			  ;
       MOV     [ScoreXOffset],161			  ;
       CALL    EraseScore				  ;
       CALL    CheckInvaderKill 			  ;
       CALL    DrawInvaders				  ;
       MOV     [Collision],0				  ; Reset collision flag
NoMissile:						  ;
       CMP     [NextLevelToggle],0			  ;
       JZ      NoNextLevel				  ;
       CALL    NextLevel				  ;
       JMP     RedrawBunkers				  ;
NoNextLevel:						  ;
       CMP     [PlayerDead],0				  ;
       JZ      NoPlayerDead				  ;
       jmp     KillPlayer				  ;
NoPlayerDead:						  ;
	MOV	EBX,0ffffh				  ; Random number between 0 and 20 in AX
	CALL	Random					  ; Call random routine
	CMP	[RandomNumber],1			  ;
	JNZ	NoMakeUFO				  ;
	CMP	[UFOX],0				  ;
	JNZ	NoMakeUFO				  ;
	DEC	[UFOCounter]				  ;
	CMP	[UFOCounter],0				  ;
	JNZ	NoMakeUFO				  ;
	MOV	[UFOCounter],6				  ;
	MOV	[UFOX],50				  ;
NoMakeUFO:						  ;
	CMP	[UFOX],0				  ;
	JZ	NoUFO					  ;
	CALL	MoveUFO 				  ;
NoUFO:							  ;
	MOV	BX,[BombFreq]				  ; Random number between 0 and 20 in AX
	CALL	Random					  ; Call random routine
	CMP	[RandomNumber],1			  ;
	JNZ	NoInvaderBomb				  ;
	CALL	InvaderBomb				  ;
NoInvaderBomb:						  ;
	DEC	[BombMove]				  ;
	CMP	[BombMove],0				  ;
	JNZ	NoMoveBombs				  ;
	MOV	AL,[BombSpeed]				  ;
	MOV	[BombMove],AL				  ;
	CALL	MoveBombs				  ;
NoMoveBombs:						  ;
	MOV	EAX,PlayersShip 			  ; Get address of sprite
	MOV	BX,[PlayerX]				  ; Get X position to draw sprite at
	MOV	DL,130					  ; Get Y position to draw sprite at
	CALL	EraseSprite				  ; Draw sprite
	MOV	ESI,[KeyDown]				  ;
	CMP	byte[esi + Left],1			  ;
	JNZ	NoLeft					  ;
	CALL	MovePlayerLeft				  ;
NoLeft: 						  ;
	CMP	byte[esi + Right],1			  ;
	JNZ	NoRight 				  ;
	CALL	MovePlayerRight 			  ;
NoRight:						  ;
	CMP	byte[esi + EscKey],1			  ;
	JZ	Exit					  ;
	JMP	NoExit					  ;
;-------------------------------------------------------------------------
; Kill Player - Never returns!
;-------------------------------------------------------------------------
KillPlayer:						  ;
	MOV	AL,[Lives]				  ;
	MOV	EBX,285 				  ;
	MOV	DL,0					  ;
	MOV	DH,0					  ;
	CALL	DisplayDigit				  ;
	DEC	[Lives] 				  ;
	MOV	AL,[Lives]				  ;
	MOV	EBX,285 				  ;
	MOV	DL,0					  ;
	MOV	DH,5					  ;
	CALL	DisplayDigit				  ;
	CMP	[Lives],48				  ;
	JNZ	LifeLeft				  ;
							  ; No life left - Wait for key before exiting game
	JMP	GameOver				  ;
							  ; Life left - Pause before continuing
LifeLeft:						  ;
	MOV	EAX,18					  ;
	CALL	[TimerNoWait]				  ;
Wait4:							  ;
	MOV	AL,0					  ;
	CALL	[TimerNoWait]				  ;
	CMP	AL,1					  ;
	JE	Wait4					  ;
	CALL	EraseInvaders				  ;
	CALL	ResetLevel				  ;
	MOV	EBX,0					  ;
Search3:						  ;
	CMP	[BombY+EBX],0				  ;
	JNZ	KillBomb2				  ;
	JMP	NoFoundSlot1				  ;
KillBomb2:						  ;
	MOV	[KillBombs],EBX 			  ;
	MOV	DL,[BombY+EBX]				  ; Get Y position to draw sprite at
	CMP	[BombType+EBX],0			  ;
	JZ	AnimatedBomb4				  ;
	MOV	EAX,StraightMissile			  ; Get address of sprite
	JMP	AllDone4				  ;
AnimatedBomb4:						  ;
	CMP	[Frame],1				  ;
	JNZ	IsFrame004				  ;
	MOV	EAX,TwistedMissile1			  ; Get address of sprite
	JMP	AllDone4				  ;
IsFrame004:						  ;
	MOV	EAX,TwistedMissile2			  ; Get address of sprite
AllDone4:						  ;
	SHL	BX,1					  ;
	MOV	BX,[BombX+EBX]				  ; Get X position to draw sprite at
	CALL	EraseSprite				  ; Draw sprite
	MOV	EBX,[KillBombs] 			  ;
	MOV	[BombY+EBX],0				  ; Get X position to draw sprite at
NoFoundSlot1:						  ;
	INC	BX					  ;
	CMP	BX,22					  ;
	JZ	AllBombsDead				  ;
	JMP	Search3 				  ;
AllBombsDead:						  ;
	JMP	RedrawBunkers				  ;
							  ;
KillBombs		DD	0			  ;
Exit:							  ;
;=======================================================  ;
;  Exit Game.          Clean up and exit                  ;
;=======================================================  ;
	call	[GameKeyBoardOff]			  ;
	mov	ax,0x0003				  ;
	call	[RealModeInt10h]			  ;
	call	[Clstext]				  ;
	ret						  ; Exit.
;------------------------------------------------------------------------
; Game Over - Never Returns!
;------------------------------------------------------------------------
GameOver:						  ;
	MOV	[GameOverToggle],1			  ;
	MOV	EAX,GameOverMsg 			  ;
	MOV	BX,82					  ;
	MOV	DL,20					  ;
	MOV	DH,7					  ;
	CALL	PrintText				  ;
	MOV	[GameOverToggle],0			  ;
	MOV	EAX,100 				  ;
	CALL	[TimerNoWait]				  ;
Wait3:							  ;
	MOV	AL,0					  ;
	CALL	[TimerNoWait]				  ;
	CMP	AL,1					  ;
	JE	Wait3					  ;
	JMP	TitleScreen				  ;
;-----------------------------------------------------------------------
; Title Screen - Never Returns!
;-----------------------------------------------------------------------
TitleScreen:						  ; Clear video buffer
	 MOV	 EDI,VideoBuffer			  ; Store address in destination register
	 MOV	 CX,21760				  ; 136 lines (160 * 136)
	 MOV	 AX,0					  ;
ClearAll:						  ;
	 STOSW						  ;
	 LOOP	 ClearAll				  ;
	 MOV	 [KeyPress],0				  ;
	 MOV	 [GameStart],1				  ;
	 CALL	 DisplayStatus				  ;
	 MOV	 EAX,InvadersTitle			  ;
	 MOV	 BX,40					  ;
	 MOV	 DL,20					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,Copyright				  ;
	 MOV	 BX,0					  ;
	 MOV	 DL,30					  ;
	 MOV	 DH,030h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,UFO				  ; Get address of sprite
	 MOV	 DL,50					  ; Get Y position to draw sprite at
	 MOV	 DH,3					  ; Get color to draw sprite
	 MOV	 BX,100 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,UFOScore				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,50					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,TopInvader1			  ; Get address of sprite
	 MOV	 DL,60					  ; Get Y position to draw sprite at
	 MOV	 DH,010h				  ; Get color to draw sprite
	 MOV	 BX,103 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,Row1Score				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,60					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,MiddleInvader2			  ; Get address of sprite
	 MOV	 DL,70					  ; Get Y position to draw sprite at
	 MOV	 DH,020h				  ; Get color to draw sprite
	 MOV	 BX,103 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,Row2Score				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,70					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,MiddleInvader1			  ; Get address of sprite
	 MOV	 DL,80					  ; Get Y position to draw sprite at
	 MOV	 DH,030h				  ; Get color to draw sprite
	 MOV	 BX,103 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,Row3Score				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,80					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,BottomInvader1			  ; Get address of sprite
	 MOV	 DL,90					  ; Get Y position to draw sprite at
	 MOV	 DH,040h				  ; Get color to draw sprite
	 MOV	 BX,103 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,Row4Score				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,90					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,BottomInvader1			  ; Get address of sprite
	 MOV	 DL,100 				  ; Get Y position to draw sprite at
	 MOV	 DH,050h				  ; Get color to draw sprite
	 MOV	 BX,103 				  ; Get X position to draw sprite at
	 CALL	 DrawLetter				  ; Draw sprite
							  ;
	 MOV	 EAX,Row5Score				  ;
	 MOV	 BX,137 				  ;
	 MOV	 DL,100 				  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,StartDocs				  ;
	 MOV	 BX,0					  ;
	 MOV	 DL,119 				  ;
	 MOV	 DH,050h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,Distribution			  ;
	 MOV	 BX,4					  ;
	 MOV	 DL,129 				  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 [GameStart],0				  ;
	 MOV	 EAX,130				  ;
	 CALL	 [TimerNoWait]				  ;
GetKey: 						  ;
	 MOV	 ESI,[KeyDown]				  ;
	 TEST	 BYTE[ESI+EscKey],1			  ;
	 JZ	 @F					  ;
	 JMP	 Exit					  ;
@@:							  ;
	 TEST	 BYTE[ESI+S],1				  ;
	 JZ	 @F					  ;
	 MOV	 [SoundToggle],0			  ;
	 XOR	 [Sound],080h				  ;
	 JMP	 GetKeyLoop				  ;
@@:							  ;
	 TEST	 BYTE[ESI+Space],1			  ;
	 JZ	 GetKeyLoop				  ;
	 JMP	 NotSound2				  ;
GetKeyLoop:						  ;
	 MOV	 AL,0					  ;
	 CALL	 [TimerNoWait]				  ;
	 CMP	 AL,1					  ;
	 JE	 GetKey 				  ;
	 JMP	 TitleScreen2				  ;
NotSound2:						  ;
	 JMP	 StartGame				  ;
;-----------------------------------------------------------------------
; Title Screen 2 - Never Returns!
;-----------------------------------------------------------------------
TitleScreen2:						  ; Clear video buffer
	 MOV	 EDI,VideoBuffer			  ; Store address in destination register
	 MOV	 ECX,21760				  ; 136 lines (160 * 136)
	 MOV	 AX,0					  ;
ClearAllZ:						  ;
	 STOSW						  ;
	 LOOP	 ClearAllZ				  ;
	 MOV	 [KeyPress],0				  ;
	 MOV	 [GameStart],1				  ;
	 CALL	 DisplayStatus				  ;
	 MOV	 EAX,InvadersTitle			  ;
	 MOV	 BX,40					  ;
	 MOV	 DL,20					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,Copyright				  ;
	 MOV	 BX,0					  ;
	 MOV	 DL,30					  ;
	 MOV	 DH,030h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,Dedication 			  ;
	 MOV	 BX,70					  ;
	 MOV	 DL,50					  ;
	 MOV	 DH,010h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,ThankYou				  ;
	 MOV	 BX,64					  ;
	 MOV	 DL,70					  ;
	 MOV	 DH,010h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,SoundTog				  ;
	 MOV	 BX,55					  ;
	 MOV	 DL,90					  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,PlayKeys				  ;
	 MOV	 BX,31					  ;
	 MOV	 DL,100 				  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,StartDocs				  ;
	 MOV	 BX,0					  ;
	 MOV	 DL,119 				  ;
	 MOV	 DH,050h				  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 EAX,Distribution			  ;
	 MOV	 BX,4					  ;
	 MOV	 DL,129 				  ;
	 MOV	 DH,7					  ;
	 CALL	 PrintText				  ;
							  ;
	 MOV	 [GameStart],0				  ;
	 MOV	 EAX,130				  ;
	 CALL	 [TimerNoWait]				  ;
GetKey2:						  ;
	 MOV	 ESI,[KeyDown]				  ;
	 TEST	 BYTE[ESI+EscKey],1			  ;
	 JZ	 @F					  ;
	 JMP	 Exit					  ;
@@:							  ;
	 TEST	 BYTE[ESI+S],1				  ;
	 JZ	 @F					  ;
	 MOV	 [SoundToggle],0			  ;
	 XOR	 [Sound],080h				  ;
	 JMP	 GetKeyLoop2				  ;
@@:							  ;
	 TEST	 BYTE[ESI+Space],1			  ;
	 JZ	 GetKeyLoop2				  ;
							  ;
	 JMP	 NotSound3				  ;
GetKeyLoop2:						  ;
	 MOV	 AL,0					  ;
	 CALL	 [TimerNoWait]				  ;
	 CMP	 AL,1					  ;
	 JE	 GetKey2				  ;
	 JMP	 TitleScreen				  ;
NotSound3:						  ;
	 JMP	 StartGame				  ;
							  ;
;------------------------------------------------------------------------
; Generate Random Number
;------------------------------------------------------------------------
Random: 						  ;
	mov	ax,word[Seed]				  ;
	mul	bx					  ;
	mov	ecx,65531				  ;
	div	cx					  ;
	mov	word[Seed],dx				  ;
	mov	byte[RandomNumber],al			  ;
	ret						  ;
							  ;
Seed			dw	0			  ;
RandomNumber		db	0			  ;
;---------------------------------------------------------------------
; Move UFO
;---------------------------------------------------------------------
MoveUFO:						  ;
	DEC	[UFOMove]				  ;
	CMP	[UFOMove],0				  ;
	JNZ	DoneUFO 				  ;
	MOV	[UFOMove],2				  ;
	MOV	EAX,UFO 				  ; Get address of sprite
	MOV	BX,[UFOX]				  ; Get X position to draw sprite at
	MOV	DL,10					  ; Get Y position to draw sprite at
	CALL	EraseSprite				  ; Draw sprite
							  ;
	INC	[UFOX]					  ;
	CMP	[UFOX],254				  ;
	JNZ	DoNextUFOFrame				  ;
	MOV	[UFOX],0				  ;
	JMP	DoneUFO 				  ;
DoNextUFOFrame: 					  ;
	MOV	EAX,UFO 				  ; Get address of sprite
	MOV	BX,[UFOX]				  ; Get X position to draw sprite at
	MOV	DL,10					  ; Get Y position to draw sprite at
	MOV	DH,3					  ; Get color to draw sprite
	CALL	DrawSprite				  ; Draw sprite
							  ; Make sound
	TEST	[Sound],080h				  ;
	JZ	NoSound1				  ;
							  ;
	MOV	AL,0b6h 				  ;
	OUT	043h,AL 				  ;
	MOV	AL,090h 				  ;
	OUT	042h,AL 				  ;
	MOV	AL,000h 				  ;
	OUT	042h,AL 				  ;
	IN	AL,061h 				  ;
	OR	AL,3					  ;
	OUT	061h,AL 				  ;
							  ;
NoSound1:						  ;
	MOV	ECX,Sound_delay 			  ;
TimerZ: 						  ;
	LOOP	TimerZ					  ;
							  ;
	IN	AL,061h 				  ;
	AND	AL,0fch 				  ;
	OUT	061h,AL 				  ;
							  ;
DoneUFO:						  ;
	RET						  ;
							  ;
UFOMove 		DB	2			  ;
UFOCounter		DB	6			  ;
;-------------------------------------------------------------------------
; Drop Invader Bomb
;-------------------------------------------------------------------------
InvaderBomb:						  ;
	MOV	EBX,0002h				  ;
	CALL	Random					  ;
;-------------------------------------------------------------------------
;                        CMP     RandomNumber[0],0
;                        JNZ     NoGood
;                        MOV     DL,83
;                        MOV     AH,2
;                        INT     21h
;                        JMP     Good
;NoGood:                 MOV     DL,32
;                        MOV     AH,2
;                        INT     21h
;Good:                   MOV     DL,00dh
;                        MOV     AH,2
;                        INT     21h
;--------------------------------------------------------------------------
	MOV	DH,[RandomNumber]			  ;
	MOV	[TempRand],DH				  ;
	MOV	EBX,0000bh				  ; Random number between 0 and 11 in AX
	CALL	Random					  ; Call random routine
	MOV	EAX,08000h				  ;
	XOR	ECX,ECX 				  ;
	MOV	CL,[RandomNumber]			  ;
	INC	CX					  ;
LoopingZ:						  ;
	SHR	AX,1					  ;
	LOOP	LoopingZ				  ;
	MOV	DL,0					  ;
	TEST	[InvadersToggle+8],AX			  ;
	JZ	NoRow5Invader				  ;
	MOV	DL,44					  ;
	JMP	FoundY					  ;
NoRow5Invader:						  ;
	TEST	[InvadersToggle+6],AX			  ;
	JZ	NoRow4Invader				  ;
	MOV	DL,34					  ;
	JMP	FoundY					  ;
NoRow4Invader:						  ;
	TEST	[InvadersToggle+4],AX			  ;
	JZ	NoRow3Invader				  ;
	MOV	DL,24					  ;
	JMP	FoundY					  ;
NoRow3Invader:						  ;
	TEST	[InvadersToggle+2],AX			  ;
	JZ	NoRow2Invader				  ;
	MOV	DL,14					  ;
	JMP	FoundY					  ;
NoRow2Invader:						  ;
	TEST	[InvadersToggle+0],AX			  ;
	JZ	NoInvaders				  ;
	MOV	DL,4					  ;
FoundY: 						  ;
	MOV	AL,[RandomNumber]			  ;
	MOV	AH,0					  ;
	MOV	CL,16					  ;
	MUL	CL					  ;
	ADD	AX,[InvadersX]				  ;
	SUB	AX,200					  ;
	ADD	DL,[InvadersY]				  ;
							  ; AX Holds X, DL holds Y of bomb start position
	MOV	EBX,0					  ;
Search: CMP	[BombY+EBX],0				  ;
	JZ	FoundSlot				  ;
	INC	BX					  ;
	CMP	BX,22					  ;
	JNZ	Search					  ;
	JMP	NoInvaders				  ;
							  ; BX Holds offset (bomb number)
FoundSlot:						  ;
	MOV	[BombY+EBX],DL				  ;
	MOV	DH,[TempRand]				  ;
	MOV	[BombType+EBX],DH			  ;
	SHL	BX,1					  ;
	MOV	[BombX+EBX],AX				  ;
							  ;
NoInvaders:						  ;
	RET						  ;
							  ;
TempRand		DB	0			  ;
;----------------------------------------------------------------------
; Move Invader Bombs
;---------------------------------------------------------------------
MoveBombs:						  ;
							  ;
	MOV	EBX,0					  ;
Search2:						  ;
	CMP	[BombY+EBX],0				  ;
	JNZ	GotSlot 				  ;
	JMP	NoFoundSlot				  ;
							  ;
GotSlot:						  ;
	MOV	[TempCounter],BX			  ;
	MOV	DL,[BombY+EBX]				  ; Get Y position to draw sprite at
	CMP	[BombType+EBX],0			  ;
	JZ	AnimatedBomb1				  ;
	MOV	EAX,StraightMissile			  ; Get address of sprite
	JMP	AllDone1				  ;
AnimatedBomb1:						  ;
	CMP	[Frame],1				  ;
	JNZ	IsFrame001				  ;
	MOV	EAX,TwistedMissile1			  ; Get address of sprite
	JMP	AllDone1				  ;
IsFrame001:						  ;
	MOV	EAX,TwistedMissile2			  ; Get address of sprite
AllDone1:						  ;
	SHL	BX,1					  ;
	MOV	BX,[BombX+EBX]				  ; Get X position to draw sprite at
	CALL	EraseSprite				  ; Draw sprite
	MOV	BX,[TempCounter]			  ;
	INC	[BombY+EBX]				  ;
	INC	[BombY+EBX]				  ;
	CMP	[BombY+EBX],130 			  ;
	JNZ	DrawNextFrameA				  ;
	MOV	[BombY+EBX],0				  ;
	JMP	NoFoundSlot				  ;
DrawNextFrameA: 					  ;
	MOV	DL,[BombY+EBX]				  ; Get Y position to draw sprite at
	CMP	[BombType+EBX],0			  ;
	JZ	AnimatedBomb2				  ;
	MOV	EAX,StraightMissile			  ; Get address of sprite
	JMP	AllDone2				  ;
AnimatedBomb2:						  ;
	CMP	[Frame],1				  ;
	JNZ	IsFrame002				  ;
	MOV	EAX,TwistedMissile1			  ; Get address of sprite
	JMP	AllDone2				  ;
IsFrame002:						  ;
	MOV	EAX,TwistedMissile2			  ; Get address of sprite
AllDone2:						  ;
	SHL	BX,1					  ;
	MOV	BX,[BombX+EBX]				  ; Get X position to draw sprite at
	MOV	DH,06h					  ; Get color to draw sprite
	MOV	[Collision],0				  ;
	CALL	DrawSprite				  ; Draw sprite
	CMP	[Collision],1				  ;
	JNZ	NoDeadPlayer				  ;
	JMP	DoneMoveBombKill			  ;
NoDeadPlayer:						  ;
	CMP	[Collision],2				  ;
	JZ	KillBomb				  ;
	CMP	[Collision],4				  ;
	JNZ	NoAction				  ;
KillBomb:						  ;
	MOV	BX,[TempCounter]			  ;
	MOV	DL,[BombY+EBX]				  ; Get Y position to draw sprite at
	CMP	[BombType+EBX],0			  ;
	JZ	AnimatedBomb3				  ;
	MOV	EAX,StraightMissile			  ; Get address of sprite
	JMP	AllDone3				  ;
AnimatedBomb3:						  ;
	CMP	[Frame],1				  ;
	JNZ	IsFrame003				  ;
	MOV	EAX,TwistedMissile1			  ; Get address of sprite
	JMP	AllDone3				  ;
IsFrame003:						  ;
	MOV	EAX,TwistedMissile2			  ; Get address of sprite
AllDone3:						  ;
	SHL	BX,1					  ;
	MOV	BX,[BombX+EBX]				  ; Get X position to draw sprite at
	CALL	EraseSprite				  ; Draw sprite
	MOV	BX,[TempCounter]			  ;
	MOV	[BombY+EBX],0				  ; Get X position to draw sprite at
	CMP	[Collision],4				  ;
	JNZ	NoAction				  ;
	CALL	KillMissile				  ;
NoAction:						  ;
	MOV	[Collision],0				  ;
	MOV	BX,[TempCounter]			  ;
NoFoundSlot:						  ;
	INC	BX					  ;
	CMP	BX,22					  ;
	JZ	DoneMoveBombs				  ;
	JMP	Search2 				  ;
DoneMoveBombs:						  ;
	RET						  ;
DoneMoveBombKill:					  ;
	POP	EDI					  ;
	JMP	KillPlayer				  ;
							  ;
TempCounter	DW	0				  ;
							  ;
;------------------------------------------------------------------------
; Erase Sprite
;------------------------------------------------------------------------
EraseSprite:						  ;
	MOV	ESI,EAX 				  ; Setup source pointer to sprite data
	XOR	EAX,EAX 				  ;
	MOV	AL,DL					  ; Get Y position of sprite in AL
	SHL	AX,1					  ; Calculate Y position * 320 (find address of row)
	SHL	AX,1					  ; Y Position shifted left 6 times + Y Position shifted left 8 times
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	MOV	CX,AX					  ;
	SHL	CX,1					  ;
	SHL	CX,1					  ;
	ADD	AX,CX					  ;
	MOV	ECX,VideoBuffer 			  ; Get address of video buffer
	ADD	EAX,ECX 				  ; Add offset to video buffer
	ADD	EAX,EBX 				  ; Add X position to address
	MOV	EDI,EAX 				  ; Store address in destination register
	MOV	DL,7					  ; Do 7 lines
	CLD						  ; Make sure we increment SI and DI
DrawLines2:						  ;
	LODSW						  ; Get 2 bytes from sprite data (16 bits = 1 line of sprite image)
	MOV	BX,AX					  ; Transfer data to an unused register
	MOV	ECX,16					  ; Scan all 16 bits
DoLine2:						  ;
	TEST	BX,8000h				  ; Check left most bit
	JZ	BitIsZero2				  ; Jump if bit is 0
	MOV	AL,0					  ; Bit is 1, get color of sprite
	STOSB						  ; And draw the pixel on the screen
	JMP	SkipAA					  ; Skip the bit is 0 stuff
BitIsZero2:						  ;
	INC	EDI					  ; Bit was 0, increment destination register
SkipAA: 						  ;
	SHL	BX,1					  ; Shift data left for next bit
	LOOP	DoLine2 				  ; Keep going until no more bits to check
	ADD	EDI,304 				  ; Increment address to next screen line (320 - 16 bits)
	DEC	DL					  ; Decrease line count
	JNZ	DrawLines2				  ; Draw lines until no more lines left to draw
							  ;
	RET						  ; Done drawing, return to caller
;------------------------------------------------------------------------
; Move Player's Ship Left
;------------------------------------------------------------------------
MovePlayerLeft: 					  ;
	 CMP	 [PlayerX],50				  ;
	 JZ	 NoMoreLeft				  ;
	 DEC	 [PlayerX]				  ;
NoMoreLeft:						  ;
	 RET						  ;
;-------------------------------------------------------------------------
; Move Player's Ship Right
;-------------------------------------------------------------------------
MovePlayerRight:					  ;
	 CMP	 [PlayerX],259				  ;
	 JZ	 NoMoreRight				  ;
	 INC	 [PlayerX]				  ;
NoMoreRight:						  ;
	 RET						  ;
							  ;
ResetLevel:						  ;
	 MOV	 [PlayerDead],0 			  ;
	 MOV	 [NextLevelToggle],0			  ;
	 MOV	 [Frame],0				  ;
	 MOV	 [InvadersX],275			  ;
	 MOV	 AL,[CurrentInvaderY]			  ;
	 MOV	 [InvadersY],AL 			  ;
	 MOV	 [InvadersToggle+0],07ff0h		  ;
	 MOV	 [InvadersToggle+2],07ff0h		  ;
	 MOV	 [InvadersToggle+4],07ff0h		  ;
	 MOV	 [InvadersToggle+6],07ff0h		  ;
	 MOV	 [InvadersToggle+8],07ff0h		  ;
	 MOV	 [MoveCount],1				  ;
	 MOV	 AL,[CurrentInvaderSpeed]		  ;
	 MOV	 [InvaderSpeed],AL			  ;
	 MOV	 [Direction],0				  ;
	 MOV	 [Reversing],0				  ;
	 MOV	 [Collision],0				  ;
	 MOV	 AX,[CurrentBombFreq]			  ;
	 MOV	 [BombFreq],AX				  ;
	 RET						  ;
							  ;
CurrentInvaderSpeed	DB	55			  ;
CurrentInvaderY 	DB	30			  ;
CurrentBombFreq 	DW	010h			  ;
;-------------------------------------------------------------------------
; Erase Score
;-------------------------------------------------------------------------
EraseScore:						  ;
	 MOV	 ESI,[ScoreValueOffset] 		  ;
	 LODSB						  ;
	 MOV	 BX,[ScoreXOffset]			  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 INC	 [ScoreValueOffset]			  ;
	 ADD	 [ScoreXOffset],6			  ;
	 MOV	 ESI,[ScoreValueOffset] 		  ;
	 LODSB						  ;
	 MOV	 BX,[ScoreXOffset]			  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 INC	 [ScoreValueOffset]			  ;
	 ADD	 [ScoreXOffset],6			  ;
	 MOV	 ESI,[ScoreValueOffset] 		  ;
	 LODSB						  ;
	 MOV	 BX,[ScoreXOffset]			  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 INC	 [ScoreValueOffset]			  ;
	 ADD	 [ScoreXOffset],6			  ;
	 MOV	 ESI,[ScoreValueOffset] 		  ;
	 LODSB						  ;
	 MOV	 BX,[ScoreXOffset]			  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 INC	 [ScoreValueOffset]			  ;
	 ADD	 [ScoreXOffset],6			  ;
	 MOV	 ESI,[ScoreValueOffset] 		  ;
	 LODSB						  ;
	 MOV	 BX,[ScoreXOffset]			  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 RET						  ;
;-------------------------------------------------------------------------
; Draw Bunkers
;-------------------------------------------------------------------------
DrawBunkers:						  ;
	 MOV	 [BunkerXL],70				  ;
	 MOV	 [BunkerYL],100 			  ;
	 CALL	 DrawBunker				  ;
	 MOV	 [BunkerXL],122 			  ;
	 MOV	 [BunkerYL],100 			  ;
	 CALL	 DrawBunker				  ;
	 MOV	 [BunkerXL],174 			  ;
	 MOV	 [BunkerYL],100 			  ;
	 CALL	 DrawBunker				  ;
	 MOV	 [BunkerXL],226 			  ;
	 MOV	 [BunkerYL],100 			  ;
	 CALL	 DrawBunker				  ;
	 RET						  ;
							  ; Draw Single Bunker
DrawBunker:						  ;
	 MOV	 EAX,BunkerLeftTop			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
	 ADD	 [BunkerYL],7				  ;
	 MOV	 EAX,BunkerLeftMiddle			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
	 ADD	 [BunkerYL],7				  ;
	 MOV	 EAX,BunkerLeftBottom			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
	 ADD	 [BunkerXL],16				  ;
	 SUB	 [BunkerYL],14				  ;
	 MOV	 EAX,BunkerRightTop			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
	 ADD	 [BunkerYL],7				  ;
	 MOV	 EAX,BunkerRightMiddle			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
	 ADD	 [BunkerYL],7				  ;
	 MOV	 EAX,BunkerRightBottom			  ; Get address of sprite
	 MOV	 BX,[BunkerXL]				  ; Get X position to draw sprite at
	 MOV	 DL,[BunkerYL]				  ; Get Y position to draw sprite at
	 MOV	 DH,2					  ; Get color to draw sprite
	 CALL	 DrawSprite				  ; Draw sprite
							  ;
	 RET						  ; Done drawing, return to caller
							  ;
BunkerXL	DW	0				  ;
BunkerYL	DB	0				  ;
							  ;
;-----------------------------------------------------------------------
; Check If Player Is Dead
;-----------------------------------------------------------------------
CheckPlayerDead:					  ;
	  CMP	  [InvadersToggle+8],0			  ;
	  JZ	  NoRow5Left				  ;
	  CMP	  [InvadersY],84			  ;
	  JNZ	  NotDead				  ;
	  MOV	  [PlayerDead],1			  ;
	  JMP	  NotDead				  ;
NoRow5Left:						  ;
	  CMP	  [InvadersToggle+6],0			  ;
	  JZ	  NoRow4Left				  ;
	  CMP	  [InvadersY],94			  ;
	  JNZ	  NotDead				  ;
	  MOV	  [PlayerDead],1			  ;
	  JMP	  NotDead				  ;
NoRow4Left:						  ;
	  CMP	  [InvadersToggle+4],0			  ;
	  JZ	  NoRow3Left				  ;
	  CMP	  [InvadersY],104			  ;
	  JNZ	  NotDead				  ;
	  MOV	  [PlayerDead],1			  ;
	  JMP	  NotDead				  ;
							  ;
NoRow3Left:						  ;
	  CMP	  [InvadersToggle+2],0			  ;
	  JZ	  NoRow2Left				  ;
	  CMP	  [InvadersY],114			  ;
	  JNZ	  NotDead				  ;
	  MOV	  [PlayerDead],1			  ;
	  JMP	  NotDead				  ;
							  ;
NoRow2Left:						  ;
	  CMP	  [InvadersY],124			  ;
	  JNZ	  NotDead				  ;
	  MOV	  [PlayerDead],1			  ;
							  ;
NotDead:						  ;
	  RET						  ;
;-----------------------------------------------------------------------
; Increase Score
;-----------------------------------------------------------------------
IncreaseScore:						  ;
RackScore:						  ;
	 CALL	 ScorePlusOne				  ;
	 LOOP	 RackScore				  ;
	 RET						  ;
ScorePlusOne:						  ;
	 INC	 [Score+4]				  ;
	 CMP	 [Score+4],58				  ;
	 JNZ	 Done					  ;
	 MOV	 [Score+4],48				  ;
	 INC	 [Score+3]				  ;
	 CMP	 [Score+3],58				  ;
	 JNZ	 Done					  ;
	 MOV	 [Score+3],48				  ;
	 INC	 [Score+2]				  ;
	 CMP	 [Score+2],58				  ;
	 JNZ	 Done					  ;
	 MOV	 [Score+2],48				  ;
	 INC	 [Score+1]				  ;
	 CMP	 [Score+1],58				  ;
	 JNZ	 Done					  ;
	 MOV	 [Score+1],48				  ;
	 INC	 [Score+0]				  ;
	 MOV	 [TempCX],CX				  ;
	 MOV	 AL,[Lives]				  ;
	 MOV	 BX,285 				  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,0					  ;
	 CALL	 DisplayDigit				  ;
	 INC	 [Lives]				  ;
	 MOV	 AL,[Lives]				  ;
	 MOV	 BX,285 				  ;
	 MOV	 DL,0					  ;
	 MOV	 DH,5					  ;
	 CALL	 DisplayDigit				  ;
	 MOV	 CX,[TempCX]				  ;
	 CMP	 [Score+0],58				  ;
	 JNZ	 Done					  ;
							  ;
Done:	 RET						  ;
							  ;
TempCX	 DW	 0					  ;
;----------------------------------------------------------------------
; Check Invader Killed
;----------------------------------------------------------------------
CheckInvaderKill:					  ;
	  MOV	  AL,[Collision]			  ;
	  AND	  AL,0f0h				  ;
	  CMP	  AL,050h				  ;
	  JNZ	  NoRow5Kill				  ;
	  CALL	  SpeedUpInvaders			  ;
	  XOR	  ECX,ECX				  ;
	  MOV	  CL,[Collision]			  ;
	  AND	  CL,00fh				  ;
	  INC	  CL					  ;
	  MOV	  EAX,08000h				  ;
Shifting1:						  ;
	  SHR	  AX,1					  ;
	  LOOP	  Shifting1				  ;
	  XOR	  [InvadersToggle+8],AX 		  ;
	  MOV	  ECX,5 				  ;
	  CALL	  IncreaseScore 			  ;
	  JMP	  NoRow1Kill				  ;
NoRow5Kill:						  ;
	  MOV	  AL,[Collision]			  ;
	  AND	  AL,0f0h				  ;
	  CMP	  AL,040h				  ;
	  JNZ	  NoRow4Kill				  ;
	  CALL	  SpeedUpInvaders			  ;
	  XOR	  ECX,ECX				  ;
	  MOV	  CL,[Collision]			  ;
	  AND	  CL,00fh				  ;
	  INC	  CL					  ;
	  MOV	  EAX,08000h				  ;
Shifting2:						  ;
	  SHR	  AX,1					  ;
	  LOOP	  Shifting2				  ;
	  XOR	  [InvadersToggle+6],AX 		  ;
	  MOV	  ECX,10				  ;
	  CALL	  IncreaseScore 			  ;
	  JMP	  NoRow1Kill				  ;
NoRow4Kill:						  ;
	  MOV	  AL,[Collision]			  ;
	  AND	  AL,0f0h				  ;
	  CMP	  AL,030h				  ;
	  JNZ	  NoRow3Kill				  ;
	  CALL	  SpeedUpInvaders			  ;
	  XOR	  ECX,ECX				  ;
	  MOV	  CL,[Collision]			  ;
	  AND	  CL,00fh				  ;
	  INC	  CL					  ;
	  MOV	  EAX,08000h				  ;
Shifting3:						  ;
	  SHR	  AX,1					  ;
	  LOOP	  Shifting3				  ;
	  XOR	  [InvadersToggle+4],AX 		  ;
	  MOV	  ECX,15				  ;
	  CALL	  IncreaseScore 			  ;
	  JMP	  NoRow1Kill				  ;
NoRow3Kill:						  ;
	  MOV	  AL,[Collision]			  ;
	  AND	  AL,0f0h				  ;
	  CMP	  AL,020h				  ;
	  JNZ	  NoRow2Kill				  ;
	  CALL	  SpeedUpInvaders			  ;
	  XOR	  ECX,ECX				  ;
	  MOV	  CL,[Collision]			  ;
	  AND	  CL,00fh				  ;
	  INC	  CL					  ;
	  MOV	  eAX,08000h				  ;
Shifting4:						  ;
	  SHR	  AX,1					  ;
	  LOOP	  Shifting4				  ;
	  XOR	  [InvadersToggle+2],AX 		  ;
	  MOV	  ECX,20				  ;
	  CALL	  IncreaseScore 			  ;
	  JMP	  NoRow1Kill				  ;
NoRow2Kill:						  ;
	  MOV	  AL,[Collision]			  ;
	  AND	  AL,0f0h				  ;
	  CMP	  AL,010h				  ;
	  JNZ	  NoRow1Kill				  ;
	  CALL	  SpeedUpInvaders			  ;
	  XOR	  ECX,ECX				  ;
	  MOV	  CL,[Collision]			  ;
	  AND	  CL,00fh				  ;
	  INC	  CL					  ;
	  MOV	  EAX,08000h				  ;
Shifting5:						  ;
	  SHR	  AX,1					  ;
	  LOOP	  Shifting5				  ;
	  XOR	  [InvadersToggle+0],AX 		  ;
	  MOV	  ECX,25				  ;
	  CALL	  IncreaseScore 			  ;
	  JMP	  NoUFOKill				  ;
NoRow1Kill:						  ;
	  CMP	  [Collision],3 			  ;
	  JNZ	  NoUFOKill				  ;
	  MOV	  EAX,UFO				  ; Get address of sprite
	  MOV	  BX,[UFOX]				  ; Get X position to draw sprite at
	  MOV	  DL,10 				  ; Get Y position to draw sprite at
	  CALL	  EraseSprite				  ; Draw sprite
	  MOV	  [UFOX],0				  ;
	  MOV	  ECX,100				  ;
	  CALL	  IncreaseScore 			  ;
NoUFOKill:						  ;
	  CMP	  [InvadersToggle+0],00000h		  ;
	  JNZ	  NotAllDeadYet 			  ;
	  CMP	  [InvadersToggle+2],00000h		  ;
	  JNZ	  NotAllDeadYet 			  ;
	  CMP	  [InvadersToggle+4],00000h		  ;
	  JNZ	  NotAllDeadYet 			  ;
	  CMP	  [InvadersToggle+6],00000h		  ;
	  JNZ	  NotAllDeadYet 			  ;
	  CMP	  [InvadersToggle+8],00000h		  ;
	  JNZ	  NotAllDeadYet 			  ;
	  MOV	  [NextLevelToggle],1			  ;
NotAllDeadYet:						  ;
	  RET						  ;
							  ; Speed up invaders upon a kill
SpeedUpInvaders:					  ;
	  CMP	  [InvaderSpeed],1			  ;
	  JZ	  NoSpeedIncrease			  ;
	  DEC	  [InvaderSpeed]			  ;
NoSpeedIncrease:					  ;
	  RET						  ;
;----------------------------------------------------------------------
; Update High Score
;----------------------------------------------------------------------
UpdateHighScore:					  ;
	  MOV	  AL,[Score+0]				  ;
	  CMP	  AL,[HighS+0]				  ;
	  JZ	  NextDigit1				  ;
	  JA	  Update				  ;
	  JMP	  NoUpdate				  ;
NextDigit1:						  ;
	  MOV	  AL,[Score+1]				  ;
	  CMP	  AL,[HighS+1]				  ;
	  JZ	  NextDigit2				  ;
	  JA	  Update				  ;
	  JMP	  NoUpdate				  ;
NextDigit2:						  ;
	  MOV	  AL,[Score+2]				  ;
	  CMP	  AL,[HighS+2]				  ;
	  JZ	  NextDigit3				  ;
	  JA	  Update				  ;
	  JMP	  NoUpdate				  ;
NextDigit3:						  ;
	  MOV	  AL,[Score+3]				  ;
	  CMP	  AL,[HighS+3]				  ;
	  JZ	  NextDigit4				  ;
	  JA	  Update				  ;
	  JMP	  NoUpdate				  ;
NextDigit4:						  ;
	  MOV	  AL,[Score+4]				  ;
	  CMP	  AL,[HighS+4]				  ;
	  JA	  Update				  ;
	  JMP	  NoUpdate				  ;
							  ;
Update: 						  ;
	  MOV	  AL,[Score+0]				  ;
	  MOV	  [HighS+0],AL				  ;
	  MOV	  AL,[Score+1]				  ;
	  MOV	  [HighS+1],AL				  ;
	  MOV	  AL,[Score+2]				  ;
	  MOV	  [HighS+2],AL				  ;
	  MOV	  AL,[Score+3]				  ;
	  MOV	  [HighS+3],AL				  ;
	  MOV	  AL,[Score+4]				  ;
	  MOV	  [HighS+4],AL				  ;
							  ;
NoUpdate:						  ;
	  RET						  ;
;-------------------------------------------------------------------------
; Draw Invaders
;-------------------------------------------------------------------------
DrawInvaders:						  ;
	MOV	AX,[InvadersX]				  ;
	MOV	[InvadersXL],AX 			  ;
	MOV	AL,[InvadersY]				  ;
	MOV	[InvadersYL],AL 			  ;
	MOV	ESI,InvadersToggle			  ; Setup source pointer to sprite data
	MOV	[ColorL],010h				  ;
	TEST	[Frame],80h				  ;
	JZ	FrameIsZero1				  ;
	MOV	EAX,TopInvader2 			  ;
	JMP	Skip1					  ;
FrameIsZero1:						  ;
	MOV	EAX,TopInvader1 			  ;
Skip1:							  ;
	MOV	[SpriteAddressL],EAX			  ;
	CLD						  ; Make sure we increment SI
	LODSW						  ; Get 2 bytes from invaders alive toggles
	MOV	[Temporary2L],ESI			  ;
	CALL	DrawInvaderRow				  ;
	MOV	[ColorL],020h				  ;
	TEST	[Frame],80h				  ;
	JZ	FrameIsZero2				  ;
	MOV	EAX,MiddleInvader2			  ;
	JMP	Skip2					  ;
FrameIsZero2:						  ;
	MOV	EAX,MiddleInvader1			  ;
Skip2:							  ;
	MOV	[SpriteAddressL],EAX			  ;
	MOV	ESI,[Temporary2L]			  ;
	CLD						  ; Make sure we increment SI
	LODSW						  ; Get 2 bytes from invaders alive toggles
	MOV	[Temporary2L],ESI			  ;
	CALL	DrawInvaderRow				  ;
	MOV	[ColorL],030h				  ;
	TEST	[Frame],80h				  ;
	JZ	FrameIsZero3				  ;
	MOV	EAX,MiddleInvader1			  ;
	JMP	Skip3					  ;
FrameIsZero3:						  ;
	MOV	EAX,MiddleInvader2			  ;
Skip3:							  ;
	MOV	[SpriteAddressL],EAX			  ;
	MOV	ESI,[Temporary2L]			  ;
	CLD						  ; Make sure we increment SI
	LODSW						  ; Get 2 bytes from invaders alive toggles
	MOV	[Temporary2L],ESI			  ;
	CALL	DrawInvaderRow				  ;
	MOV	[ColorL],040h				  ;
	TEST	[Frame],80h				  ;
	JZ	FrameIsZero4				  ;
	MOV	EAX,BottomInvader2			  ;
	JMP	Skip4					  ;
FrameIsZero4:						  ;
	MOV	EAX,BottomInvader1			  ;
Skip4:							  ;
	MOV	[SpriteAddressL],EAX			  ;
	MOV	ESI,[Temporary2L]			  ;
	CLD						  ; Make sure we increment SI
	LODSW						  ; Get 2 bytes from invaders alive toggles
	MOV	[Temporary2L],ESI			  ;
	CALL	DrawInvaderRow				  ;
	MOV	[ColorL],050h				  ;
	MOV	ESI,[Temporary2L]			  ;
	CLD						  ; Make sure we increment SI
	LODSW						  ; Get 2 bytes from invaders alive toggles
	MOV	[Temporary2L],ESI			  ;
	CALL	DrawInvaderRow				  ;
	MOV	[Collision],0				  ; No collision detection on invaders - Causes problems when invaders get to bunkers
	RET						  ; Done drawing, return to caller
							  ;
InvadersXL		DW	0			  ;
InvadersYL		DB	0			  ;
Temporary2L		DD	0			  ;
							  ; Draw Row of Invaders
DrawInvaderRow: 					  ;
	SHL	AX,1					  ;
	MOV	[CounterL],11				  ;
DrawRow:						  ;
	TEST	AX,8000h				  ; Check left most bit     ;
	JZ	BitIsZero1				  ; Jump if bit is 0
	MOV	[Temporary1L],EAX			  ;
	MOV	EAX,[SpriteAddressL]			  ; Get address of sprite
	MOV	BX,[InvadersXL] 			  ; Get X position to draw sprite at
	SUB	BX,200					  ;
	MOV	DL,[InvadersYL] 			  ; Get Y position to draw sprite at
	MOV	DH,[ColorL]				  ; Get color to draw sprite
	CALL	DrawSprite				  ; Draw sprite
	MOV	EAX,[Temporary1L]			  ;
BitIsZero1:						  ;
	SHL	AX,1					  ; Shift data left for next bit
	ADD	[InvadersXL],16 			  ;
	DEC	[CounterL]				  ;
	INC	[ColorL]				  ;
	JNZ	DrawRow 				  ;
	ADD	[InvadersYL],10 			  ;
	MOV	AX,[InvadersX]				  ;
	MOV	[InvadersXL],AX 			  ;
	RET						  ; Done drawing, return to caller
							  ;
CounterL		DB	0			  ;
SpriteAddressL		DD	0			  ;
ColorL			DB	0			  ;
Temporary1L		DD	0			  ;
;-------------------------------------------------------------------------
; Move Invaders
;-------------------------------------------------------------------------
MoveInvaders:						  ;
       TEST    [Reversing],080h 			  ;
       JNZ     NoReverse				  ;
       MOV     EBX,250					  ;
       MOV     AX,[InvadersToggle+0]			  ;
       OR      AX,[InvadersToggle+2]			  ;
       OR      AX,[InvadersToggle+4]			  ;
       OR      AX,[InvadersToggle+6]			  ;
       OR      AX,[InvadersToggle+8]			  ;
       MOV     ECX,10					  ;
FindLeft:						  ;
       SHL     AX,1					  ;
       TEST    AX,08000h				  ;
       JNZ     DoneLeft 				  ;
       SUB     BX,16					  ;
       LOOP    FindLeft 				  ;
							  ;
DoneLeft:						  ;
       MOV     DX,298					  ;
       MOV     AX,[InvadersToggle+0]			  ;
       OR      AX,[InvadersToggle+2]			  ;
       OR      AX,[InvadersToggle+4]			  ;
       OR      AX,[InvadersToggle+6]			  ;
       OR      AX,[InvadersToggle+8]			  ;
       MOV     ECX,10					  ;
       SHR     AX,1					  ;
       SHR     AX,1					  ;
       SHR     AX,1					  ;
FindRight:						  ;
       SHR     AX,1					  ;
       TEST    AX,00001h				  ;
       JNZ     DoneRight				  ;
       ADD     DX,16					  ;
       LOOP    FindRight				  ;
DoneRight:						  ;
       CMP     [InvadersX],DX				  ;
       JZ      Reverse					  ;
       CMP     [InvadersX],BX				  ;
       JNZ     NoReverse				  ;
Reverse:						  ;
       XOR     [Direction],080h 			  ;
       ADD     [InvadersY],2				  ;
       MOV     [Reversing],080h 			  ;
       JMP     Animate					  ;
							  ;
NoReverse:						  ;
       MOV     [Reversing],0				  ;
       TEST    [Direction],080h 			  ;
       JZ      MoveInvadersLeft 			  ;
							  ;
       INC     [InvadersX]				  ;
       JMP     Animate					  ;
							  ;
MoveInvadersLeft:					  ;
       DEC     [InvadersX]				  ;
							  ;
Animate:						  ;
       XOR     [Frame],080h				  ;
       MOV     AH,[InvaderSpeed]			  ;
       MOV     [MoveCount],AH				  ;
							  ;
; Make sound                                              ;
       TEST    [Sound],080h				  ;
       JZ      NoSound2 				  ;
       MOV     AL,0b6h					  ;
       OUT     043h,AL					  ;
       MOV     AL,090h					  ;
       OUT     042h,AL					  ;
       MOV     AL,00Fh					  ;
       OUT     042h,AL					  ;
       IN      AL,061h					  ;
       OR      AL,3					  ;
       OUT     061h,AL					  ;
							  ;
NoSound2:						  ;
       MOV     ECX,Sound_delay				  ;
Timer1: 						  ;
       LOOP    Timer1					  ;
       IN      AL,061h					  ;
       AND     AL,0fch					  ;
       OUT     061h,AL					  ;
       RET						  ;
;--------------------------------------------------------------------------
; Shoot Player Missile
;--------------------------------------------------------------------------
ShootPlayerMissile:					  ;
       MOV     AX,[PlayerX]				  ;
       MOV     [MissileX],AX				  ;
       MOV     [MissileY],123				  ;
							  ;
; Make sound                                              ;
       TEST    [Sound],080h				  ;
       JZ      NoSound3 				  ;
							  ;
       MOV     AL,0b6h					  ;
       OUT     043h,AL					  ;
       MOV     AL,090h					  ;
       OUT     042h,AL					  ;
       MOV     AL,001h					  ;
       OUT     042h,AL					  ;
       IN      AL,061h					  ;
       OR      AL,3					  ;
       OUT     061h,AL					  ;
							  ;
NoSound3:						  ;
       MOV     ECX,Sound_delay				  ;
Timer2: 						  ;
       LOOP    Timer2					  ;
       IN      AL,061h					  ;
       AND     AL,0fch					  ;
       OUT     061h,AL					  ;
       RET						  ;
;-------------------------------------------------------------------------
; Move Player Missile
;-------------------------------------------------------------------------
MovePlayerMissile:					  ;
       MOV     EAX,StraightMissile			  ; Get address of sprite
       MOV     BX,[MissileX]				  ; Get X position to draw sprite at
       MOV     DL,[MissileY]				  ; Get Y position to draw sprite at
       CALL    EraseSprite				  ; Draw sprite
       DEC     [MissileY]				  ;
       DEC     [MissileY]				  ;
       CMP     [MissileY],09				  ;
       JNZ     DrawNextFrame				  ;
       MOV     [MissileX],0				  ;
       JMP     MissileDead				  ;
DrawNextFrame:						  ;
       MOV     EAX,StraightMissile			  ; Get address of sprite
       MOV     BX,[MissileX]				  ; Get X position to draw sprite at
       MOV     DL,[MissileY]				  ; Get Y position to draw sprite at
       MOV     DH,04h					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       CMP     [Collision],0				  ;
       JZ      MissileDead				  ;
       CMP     [Collision],4				  ;
       JZ      MissileDead				  ;
       CMP     [Collision],6				  ;
       JZ      MissileDead				  ;
       CALL    KillMissile				  ;
MissileDead:						  ;
       RET						  ;
							  ;
KillMissile:						  ; Kill Player Missile
       MOV     EAX,StraightMissile			  ; Get address of sprite
       MOV     BX,[MissileX]				  ; Get X position to draw sprite at
       MOV     DL,[MissileY]				  ; Get Y position to draw sprite at
       CALL    EraseSprite				  ; Draw sprite
       MOV     [MissileX],0				  ;
       RET						  ;
;--------------------------------------------------------------------------
; Erase Invaders
;--------------------------------------------------------------------------
EraseInvaders:						  ;
       MOV     AX,[InvadersX]				  ;
       MOV     [InvadersXL],AX				  ;
       MOV     AL,[InvadersY]				  ;
       MOV     [InvadersYL],AL				  ;
       MOV     ESI,InvadersToggle			  ; Setup source pointer to sprite data
       TEST    [Frame],80h				  ;
       JZ      FrameIsZeroA				  ;
       MOV     EAX,TopInvader2				  ;
       JMP     SkipA					  ;
FrameIsZeroA:						  ;
       MOV     EAX,TopInvader1				  ;
SkipA:							  ;
       MOV     [SpriteAddressL],EAX			  ;
       CLD						  ; Make sure we increment SI
       LODSW						  ; Get 2 bytes from invaders alive toggles
       MOV     [Temporary2L],ESI			  ;
       CALL    EraseInvaderRow				  ;
       TEST    [Frame],80h				  ;
       JZ      FrameIsZeroB				  ;
       MOV     EAX,MiddleInvader2			  ;
       JMP     SkipB					  ;
FrameIsZeroB:						  ;
       MOV     EAX,MiddleInvader1			  ;
SkipB:							  ;
       MOV     [SpriteAddressL],EAX			  ;
       MOV     ESI,[Temporary2L]			  ;
       CLD						  ; Make sure we increment SI
       LODSW						  ; Get 2 bytes from invaders alive toggles
       MOV     [Temporary2L],ESI			  ;
       CALL    EraseInvaderRow				  ;
       TEST    [Frame],80h				  ;
       JZ      FrameIsZeroC				  ;
       MOV     EAX,MiddleInvader1			  ;
       JMP     SkipC					  ;
FrameIsZeroC:						  ;
       MOV     EAX,MiddleInvader2			  ;
SkipC:							  ;
       MOV     [SpriteAddressL],EAX			  ;
       MOV     ESI,[Temporary2L]			  ;
       CLD						  ; Make sure we increment SI
       LODSW						  ; Get 2 bytes from invaders alive toggles
       MOV     [Temporary2L],ESI			  ;
       CALL    EraseInvaderRow				  ;
       TEST    [Frame],80h				  ;
       JZ      FrameIsZeroD				  ;
       MOV     EAX,BottomInvader2			  ;
       JMP     SkipD					  ;
FrameIsZeroD:						  ;
       MOV     EAX,BottomInvader1			  ;
SkipD:							  ;
       MOV     [SpriteAddressL],EAX			  ;
       MOV     ESI,[Temporary2L]			  ;
       CLD						  ; Make sure we increment SI
       LODSW						  ; Get 2 bytes from invaders alive toggles
       MOV     [Temporary2L],ESI			  ;
       CALL    EraseInvaderRow				  ;
       MOV     ESI,[Temporary2L]			  ;
       CLD						  ; Make sure we increment SI
       LODSW						  ; Get 2 bytes from invaders alive toggles
       MOV     [Temporary2L],ESI			  ;
       CALL    EraseInvaderRow				  ;
       RET						  ; Done drawing, return to caller
							  ; Erase Row of Invaders
EraseInvaderRow:					  ;
       SHL     AX,1					  ;
       MOV     [CounterL],11				  ;
DrawRowA:						  ;
       TEST    AX,8000h 				  ; Check left most bit
       JZ      BitIsZeroA				  ; Jump if bit is 0
       MOV     [Temporary1L],EAX			  ;
       MOV     EAX,[SpriteAddressL]			  ; Get address of sprite
       MOV     BX,[InvadersXL]				  ; Get X position to draw sprite at
       SUB     BX,200					  ;
       MOV     DL,[InvadersYL]				  ; Get Y position to draw sprite at
       CALL    EraseSprite				  ; Draw sprite
       MOV     EAX,[Temporary1L]			  ;
BitIsZeroA:						  ;
       SHL     AX,1					  ; Shift data left for next bit
       ADD     [InvadersXL],16				  ;
       DEC     [CounterL]				  ;
       JNZ     DrawRowA 				  ;
       ADD     [InvadersYL],10				  ;
       MOV     AX,[InvadersX]				  ;
       MOV     [InvadersXL],AX				  ;
       RET						  ; Done drawing, return to caller
;------------------------------------------------------------------------
; Display Status
;------------------------------------------------------------------------
DisplayStatus:						  ; Display "SCORE"
       MOV     EAX,SCO					  ; Get address of sprite
       MOV     BX,30					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     EAX,ORE					  ; Get address of sprite
       MOV     BX,46					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     EAX,Score				  ;
       MOV     [ScoreValueOffset],EAX			  ;
       MOV     [ScoreXOffset],67			  ;
       CALL    DisplayScore				  ;
							  ; Display "HIGH"
       MOV     EAX,HIG					  ; Get address of sprite
       MOV     BX,130					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     EAX,GH					  ; Get address of sprite
       MOV     BX,146					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     EAX,HighS				  ;
       MOV     [ScoreValueOffset],EAX			  ;
       MOV     [ScoreXOffset],161			  ;
       CALL    DisplayScore				  ;
							  ; Display lives
       MOV     EAX,PlayersShip				  ; Get address of sprite
       MOV     BX,266					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     EAX,Equal				  ; Get address of sprite
       MOV     BX,278					  ; Get X position to draw sprite at
       MOV     DL,0					  ; Get Y position to draw sprite at
       MOV     DH,5					  ; Get color to draw sprite
       CALL    DrawSprite				  ; Draw sprite
       MOV     AL,[Lives]				  ;
       MOV     BX,285					  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
       RET						  ;
;------------------------------------------------------------------------
; Display Score
;------------------------------------------------------------------------
DisplayScore:						  ;
       MOV     ESI,[ScoreValueOffset]			  ;
       LODSB						  ;
       MOV     BX,[ScoreXOffset]			  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
       INC     [ScoreValueOffset]			  ;
       ADD     [ScoreXOffset],6 			  ;
       MOV     ESI,[ScoreValueOffset]			  ;
       LODSB						  ;
       MOV     BX,[ScoreXOffset]			  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
							  ;
       INC     [ScoreValueOffset]			  ;
       ADD     [ScoreXOffset],6 			  ;
       MOV     ESI,[ScoreValueOffset]			  ;
       LODSB						  ;
       MOV     BX,[ScoreXOffset]			  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
       INC     [ScoreValueOffset]			  ;
       ADD     [ScoreXOffset],6 			  ;
       MOV     ESI,[ScoreValueOffset]			  ;
       LODSB						  ;
       MOV     BX,[ScoreXOffset]			  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
       INC     [ScoreValueOffset]			  ;
       ADD     [ScoreXOffset],6 			  ;
       MOV     ESI,[ScoreValueOffset]			  ;
       LODSB						  ;
       MOV     BX,[ScoreXOffset]			  ;
       MOV     DL,0					  ;
       MOV     DH,5					  ;
       CALL    DisplayDigit				  ;
       RET						  ;
							  ;
ScoreXOffset		DW	0			  ;
ScoreValueOffset	DD	0			  ;
;-------------------------------------------------------------------------
; Next Level
;-------------------------------------------------------------------------
NextLevel:						  ;
	DEC	[CurrentInvaderSpeed]			  ;
	ADD	[CurrentInvaderY],002h			  ;
	CMP	[CurrentBombFreq],002h			  ;
	JZ	NoDecrease				  ;
	DEC	[CurrentBombFreq]			  ;
NoDecrease:						  ;
	CALL	ResetLevel				  ;
	RET						  ;
;-----------------------------------------------------------------------
;-----------------------------------------------------------------------
DisplayDigit:						  ;
	CALL	FindDigit				  ;
	CALL	DrawSprite				  ; Draw sprite
	RET						  ;
FindDigit:						  ;
							  ; Find Digit
	CMP	AL,48					  ;
	JNZ	NotZero 				  ;
	MOV	EAX,Zero				  ;
	JMP	GotDigit				  ;
NotZero:						  ;
	CMP	AL,49					  ;
	JNZ	NotOne					  ;
	MOV	EAX,One 				  ;
	JMP	GotDigit				  ;
NotOne: 						  ;
	CMP	AL,50					  ;
	JNZ	NotTwo					  ;
	MOV	EAX,Two 				  ;
	JMP	GotDigit				  ;
NotTwo: 						  ;
	CMP	AL,51					  ;
	JNZ	NotThree				  ;
	MOV	EAX,Three				  ;
	JMP	GotDigit				  ;
NotThree:						  ;
	CMP	AL,52					  ;
	JNZ	NotFour 				  ;
	MOV	EAX,Four				  ;
	JMP	GotDigit				  ;
NotFour:						  ;
	CMP	AL,53					  ;
	JNZ	NotFive 				  ;
	MOV	EAX,Five				  ;
	JMP	GotDigit				  ;
NotFive:						  ;
	CMP	AL,54					  ;
	JNZ	NotSix					  ;
	MOV	EAX,Six 				  ;
	JMP	GotDigit				  ;
NotSix: 						  ;
	CMP	AL,55					  ;
	JNZ	NotSeven				  ;
	MOV	EAX,Seven				  ;
	JMP	GotDigit				  ;
NotSeven:						  ;
	CMP	AL,56					  ;
	JNZ	NotEight				  ;
	MOV	EAX,Eight				  ;
	JMP	GotDigit				  ;
NotEight:						  ;
	CMP	AL,57					  ;
	JNZ	NotNine 				  ;
	MOV	EAX,Nine				  ;
	JMP	GotDigit				  ;
NotNine:						  ;
	CMP	AL,65					  ;
	JNZ	NotA					  ;
	MOV	EAX,LetterA				  ;
	JMP	GotDigit				  ;
NotA:							  ;
	CMP	AL,66					  ;
	JNZ	NotB					  ;
	MOV	EAX,LetterB				  ;
	JMP	GotDigit				  ;
NotB:							  ;
	CMP	AL,67					  ;
	JNZ	NotC					  ;
	MOV	EAX,LetterC				  ;
	JMP	GotDigit				  ;
NotC:							  ;
	CMP	AL,68					  ;
	JNZ	NotD					  ;
	MOV	EAX,LetterD				  ;
	JMP	GotDigit				  ;
NotD:							  ;
	CMP	AL,69					  ;
	JNZ	NotE					  ;
	MOV	EAX,LetterE				  ;
	JMP	GotDigit				  ;
NotE:							  ;
	CMP	AL,70					  ;
	JNZ	NotF					  ;
	MOV	EAX,LetterF				  ;
	JMP	GotDigit				  ;
NotF:							  ;
	CMP	AL,71					  ;
	JNZ	NotG					  ;
	MOV	EAX,LetterG				  ;
	JMP	GotDigit				  ;
NotG:							  ;
	CMP	AL,72					  ;
	JNZ	NotH					  ;
	MOV	EAX,LetterH				  ;
	JMP	GotDigit				  ;
NotH:							  ;
	CMP	AL,73					  ;
	JNZ	NotI					  ;
	MOV	EAX,LetterI				  ;
	JMP	GotDigit				  ;
NotI:							  ;
	CMP	AL,74					  ;
	JNZ	NotJ					  ;
	MOV	EAX,LetterJ				  ;
	JMP	GotDigit				  ;
NotJ:							  ;
	CMP	AL,75					  ;
	JNZ	NotK					  ;
	MOV	EAX,LetterK				  ;
	JMP	GotDigit				  ;
NotK:							  ;
	CMP	AL,76					  ;
	JNZ	NotL					  ;
	MOV	EAX,LetterL				  ;
	JMP	GotDigit				  ;
NotL:							  ;
	CMP	AL,77					  ;
	JNZ	NotM					  ;
	MOV	EAX,LetterM				  ;
	JMP	GotDigit				  ;
NotM:							  ;
	CMP	AL,78					  ;
	JNZ	NotN					  ;
	MOV	EAX,LetterN				  ;
	JMP	GotDigit				  ;
NotN:							  ;
	CMP	AL,79					  ;
	JNZ	NotO					  ;
	MOV	EAX,LetterO				  ;
	JMP	GotDigit				  ;
NotO:							  ;
	CMP	AL,80					  ;
	JNZ	NotP					  ;
	MOV	EAX,LetterP				  ;
	JMP	GotDigit				  ;
NotP:							  ;
	CMP	AL,81					  ;
	JNZ	NotQ					  ;
	MOV	EAX,LetterQ				  ;
	JMP	GotDigit				  ;
NotQ:							  ;
	CMP	AL,82					  ;
	JNZ	NotR					  ;
	MOV	EAX,LetterR				  ;
	JMP	GotDigit				  ;
NotR:							  ;
	CMP	AL,83					  ;
	JNZ	NotS					  ;
	MOV	EAX,LetterS				  ;
	JMP	GotDigit				  ;
NotS:							  ;
	CMP	AL,84					  ;
	JNZ	NotT					  ;
	MOV	EAX,LetterT				  ;
	JMP	GotDigit				  ;
NotT:							  ;
	CMP	AL,85					  ;
	JNZ	NotU					  ;
	MOV	EAX,LetterU				  ;
	JMP	GotDigit				  ;
NotU:							  ;
	CMP	AL,86					  ;
	JNZ	NotV					  ;
	MOV	EAX,LetterV				  ;
	JMP	GotDigit				  ;
NotV:							  ;
	CMP	AL,87					  ;
	JNZ	NotW					  ;
	MOV	EAX,LetterW				  ;
	JMP	GotDigit				  ;
NotW:							  ;
	CMP	AL,88					  ;
	JNZ	NotX					  ;
	MOV	EAX,LetterX				  ;
	JMP	GotDigit				  ;
NotX:							  ;
	CMP	AL,89					  ;
	JNZ	NotY					  ;
	MOV	EAX,LetterY				  ;
	JMP	GotDigit				  ;
NotY:							  ;
	CMP	AL,90					  ;
	JNZ	NotZ					  ;
	MOV	EAX,LetterZ				  ;
	JMP	GotDigit				  ;
NotZ:							  ;
	CMP	AL,61					  ;
	JNZ	NotEqual				  ;
	MOV	EAX,Equal				  ;
	JMP	GotDigit				  ;
NotEqual:						  ;
	CMP	AL,40					  ;
	JNZ	NotCopyright				  ;
	MOV	EAX,CopyrightSymbol			  ;
	JMP	GotDigit				  ;
NotCopyright:						  ;
	MOV	EAX,Period				  ;
GotDigit:						  ;
	RET						  ;
;------------------------------------------------------------------------
; Draw Sprite
;------------------------------------------------------------------------
DrawSprite:						  ;
	MOV	ESI,EAX 				  ; Setup source pointer to sprite data
	XOR	EAX,EAX 				  ;
	MOV	AL,DL					  ; Get Y position of sprite in AL
	SHL	AX,1					  ; Calculate Y position * 320 (find address of row)
	SHL	AX,1					  ; Y Position shifted left 6 times + Y Position shifted left 8 times
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	MOV	ECX,EAX 				  ;
	SHL	CX,1					  ;
	SHL	CX,1					  ;
	ADD	EAX,ECX 				  ;
	MOV	ECX,VideoBuffer 			  ; Get address of video buffer
	ADD	EAX,ECX 				  ; Add offset to video buffer
	ADD	EAX,EBX 				  ; Add X position to address
	MOV	EDI,EAX 				  ; Store address in destination register
	MOV	DL,7					  ; Do 7 lines
	CLD						  ; Make sure we increment SI and DI
DrawLines:						  ;
	LODSW						  ; Get 2 bytes from sprite data (16 bits = 1 line of sprite image)
	MOV	BX,AX					  ; Transfer data to an unused register
	MOV	ECX,16					  ; Scan all 16 bits
DoLine: 						  ;
	TEST	BX,8000h				  ; Check left most bit
	JZ	BitIsZero				  ; Jump if bit is 0
	CMP	[Collision],0				  ;
	JNZ	NoCollision				  ;
	MOV	AL,BYTE[ES:EDI] 			  ;
	CMP	AL,0					  ;
	JZ	NoCollision				  ;
	MOV	[Collision],AL				  ;
NoCollision:						  ;
	MOV	AL,DH					  ; Bit is 1, get color of sprite
	STOSB						  ; And draw the pixel on the screen
	JMP	Skip					  ; Skip the bit is 0 stuff
BitIsZero:						  ;
	INC	EDI					  ; Bit was 0, increment destination register
Skip:							  ;
	SHL	BX,1					  ; Shift data left for next bit
	LOOP	DoLine					  ; Keep going until no more bits to check
	ADD	EDI,304 				  ; Increment address to next screen line (320 - 16 bits)
	DEC	DL					  ; Decrease line count
	JNZ	DrawLines				  ; Draw lines until no more lines left to draw
	RET						  ; Done drawing, return to caller
;-----------------------------------------------------------------------
; Print Text
;-----------------------------------------------------------------------
PrintText:						  ;
	MOV	[LetterCounter],EAX			  ;
	MOV	[LetterXPos],BX 			  ;
	MOV	[LetterYPos],DL 			  ;
	MOV	[LetterColor],DH			  ;
	SUB	[LetterXPos],6				  ;
	CMP	[KeyPress],1				  ;
	JNZ	PrintNext				  ;
	MOV	[KeyPress],0				  ;
	CMP	[ExitToggle],1				  ;
	JNZ	NotExit2				  ;
	JMP	Exit					  ;
NotExit2:						  ;
	CMP	[SoundToggle],1 			  ;
	JNZ	NotSound				  ;
	MOV	[SoundToggle],0 			  ;
	XOR	[Sound],080h				  ;
	JMP	PrintNext				  ;
NotSound:						  ;
	CMP	[GameStart],1				  ;
	JNZ	PrintNext				  ;
	MOV	[GameStart],0				  ;
	JMP	StartGame				  ;
PrintNext:						  ;
	MOV	ESI,[LetterCounter]			  ;
	LODSB						  ;
	INC	[LetterCounter] 			  ;
	ADD	[LetterXPos],6				  ;
	CMP	AL,0					  ;
	JZ	DonePrinting				  ;
	CMP	AL,32					  ;
	JZ	PrintNext				  ;
	MOV	BX,[LetterXPos] 			  ;
	MOV	DL,[LetterYPos] 			  ;
	MOV	DH,[LetterColor]			  ;
	CALL	DisplayDigit				  ;
	PUSH	ES					  ;
	MOV	AX,8h					  ;
	MOV	ES,AX					  ;
	MOV	ESI,VideoBuffer 			  ;
	MOV	EDI,10560				  ; Store address in destination register
	ADD	EDI,0xA0000				  ;
	MOV	ECX,21760				  ; 150 lines (136 * 150)
	MOV	DX,03DAh				  ; Get vertical retrace port address in DX
RetraceEnd3:						  ;
	IN	AL,DX					  ; Grab retrace information again
	TEST	AL,8					  ; Did it end yet?
	JZ	RetraceEnd3				  ; No, loop until it does
BlitAll3:						  ;
	MOVSW						  ;
	LOOP	BlitAll3				  ;
	POP  ES 					  ;
	JMP	PrintNext				  ;
DonePrinting:						  ;
	RET						  ;
							  ; Draw Letter with pause
DrawLetter:						  ;
	CALL	DrawSprite				  ; Draw sprite
							  ; Prepare for buffer blit
	PUSH	ES					  ;
	MOV	AX,8h					  ;
	MOV	ES,AX					  ;
	MOV	ESI,VideoBuffer 			  ;
	MOV	EDI,10560				  ; Store address in destination register
	ADD	EDI,0xA0000				  ;
	MOV	ECX,21760				  ; 150 lines (136 * 150)
							  ; Check vertical retrace
	MOV	DX,03DAh				  ; Get vertical retrace port address in DX
RetraceEnd2:						  ;
	IN	AL,DX					  ; Grab retrace information again
	TEST	AL,8					  ; Did it end yet?
	JZ	RetraceEnd2				  ; No, loop until it does
							  ; Blit buffer to video memory
BlitAll2:						  ;
	MOVSW						  ;
	LOOP	BlitAll2				  ;
	POP	ES					  ;
;NoKey:                                                   ;
	MOV	AL,3					  ;
	CALL	[TimerNoWait]				  ;
Wait2:							  ;
	MOV	AL,0					  ;
	CALL	[TimerNoWait]				  ;
	CMP	AL,1					  ;
	JE	Wait2					  ;
							  ;
	RET						  ;
							  ;
LetterCounter		DD	0			  ;
LetterXPos		DW	0			  ;
LetterYPos		DB	0			  ;
LetterColor		DB	0			  ;
;----------------------------------------------------------------------
; Reset Game
;----------------------------------------------------------------------
ResetGame:						  ;
	MOV	[Frame],0				  ;
	MOV	[InvadersX],275 			  ;
	MOV	[InvadersY],30				  ;
	MOV	[InvadersToggle],07ff0h 		  ;
	MOV	[InvadersToggle+2],07ff0h		  ;
	MOV	[InvadersToggle+4],07ff0h		  ;
	MOV	[InvadersToggle+6],07ff0h		  ;
	MOV	[InvadersToggle+8],07ff0h		  ;
	MOV	[PlayerX],154				  ;
	MOV	[LeftToggle],0				  ;
	MOV	[RightToggle],0 			  ;
	MOV	[FireToggle],0				  ;
	MOV	[ExitToggle],0				  ;
	MOV	[NextLevelToggle],0			  ;
	MOV	[MissileX],0				  ;
	MOV	[MissileY],0				  ;
	MOV	[UFOX],0				  ;
	MOV	EDI,BombX				  ;
	MOV	ECX,22					  ;
	MOV	AX,0					  ;
ClearLoop1:						  ;
	STOSW						  ;
	LOOP	ClearLoop1				  ;
	MOV	EDI,BombY				  ;
	MOV	ECX,22					  ;
ClearLoop2:						  ;
	STOSB						  ;
	LOOP	ClearLoop2				  ;
	MOV	EDI,BombType				  ;
	MOV	ECX,22					  ;
ClearLoop3:						  ;
	STOSB						  ;
	LOOP	ClearLoop3				  ;
	MOV	[BombFreq],010h 			  ;
	MOV	[MoveCount],1				  ;
	MOV	[InvaderSpeed],55			  ;
	MOV	[Direction],0				  ;
	MOV	[Reversing],0				  ;
	MOV	[Collision],0				  ;
	MOV	[Score],48				  ;
	MOV	[Score+1],48				  ;
	MOV	[Score+2],48				  ;
	MOV	[Score+3],48				  ;
	MOV	[Score+4],48				  ;
	MOV	[Lives],51				  ;
	MOV	[BombMove],2				  ;
	MOV	[BombSpeed],2				  ;
	MOV	[PlayerDead],0				  ;
	MOV	[GameOverToggle],0			  ;
	MOV	[CurrentInvaderSpeed],55		  ;
	MOV	[CurrentInvaderY],30			  ;
	MOV	[CurrentBombFreq],010h			  ;
	RET						  ;
;-------------------------------------------------------------------------
; Draw Logo Layer
;-------------------------------------------------------------------------
DrawLogoLayer:						  ;
	MOV	ESI,EAX 				  ; Setup source pointer to sprite data
	XOR	EAX,EAX 				  ;
	MOV	AL,DL					  ; Get Y position of sprite in AL
	MOV	AH,0					  ; Zero high byte of AX for following shifts
	SHL	AX,1					  ; Calculate Y position * 320 (find address of row)
	SHL	AX,1					  ; Y Position shifted left 6 times + Y Position shifted left 8 times
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	SHL	AX,1					  ;
	MOV	CX,AX					  ;
	SHL	CX,1					  ;
	SHL	CX,1					  ;
	ADD	AX,CX					  ;
	ADD	AX,BX					  ; Add X position to address
	MOV	EDI,EAX 				  ; Store address in destination register
	ADD	EDI,VideoBuffer 			  ;
	MOV	[Row],5 				  ;
DoNextRow:						  ;
	MOV	[Column],11				  ;
DoNextSprite:						  ;
	MOV	DL,7					  ; Do 7 lines of sprite
	CLD						  ; Make sure we increment SI and DI
DrawLinesZ:						  ;
	LODSW						  ; Get 2 bytes from sprite data (16 bits = 1 line of sprite image)
	MOV	BX,AX					  ; Transfer data to an unused register
	MOV	ECX,16					  ; Scan all 16 bits
DoLineZ:						  ;
	TEST	BX,8000h				  ; Check left most bit
	JZ	BitIsZeroZ				  ; Jump if bit is 0
	MOV	AL,DH					  ; Bit is 1, get color of sprite
	STOSB						  ; And draw the pixel on the screen
	JMP	SkipZ					  ; Skip the bit is 0 stuff
BitIsZeroZ:						  ;
	INC	EDI					  ; Bit was 0, increment destination register
SkipZ:							  ;
	SHL	BX,1					  ; Shift data left for next bit
	LOOP	DoLineZ 				  ; Keep going until no more bits to check
	ADD	EDI,304 				  ; Increment address to next screen line (320 - 16 bits)
	DEC	DL					  ; Decrease line count
	JNZ	DrawLinesZ				  ; Draw lines until no more lines left to draw
	SUB	EDI,2224				  ; Do next sprite in row
	DEC	[Column]				  ;
	CMP	[Column],0				  ;
	JNZ	DoNextSprite				  ;
	ADD	EDI,2064				  ; Do next sprite in row
	DEC	[Row]					  ;
	CMP	[Row],0 				  ;
	JNZ	DoNextRow				  ;
	RET						  ; Done drawing, return to caller
							  ;
Column			DB	0			  ;
Row			DB	0			  ;
standardP		dd	0			  ;
;------------------------------------------------------------------------
; Variables
;------------------------------------------------------------------------
PauseCounter		DB	0
GameStart		DB	0
Frame			DB	0
InvadersX		DW	275
InvadersY		DB	30
InvadersToggle		DW	07ff0h,07ff0h,07ff0h,07ff0h,07ff0h
PlayerX 		DW	154
LeftToggle		DB	0
RightToggle		DB	0
FireToggle		DB	0
ExitToggle		DB	0
SoundToggle		DB	0
KeyPress		DB	0
Sound			DB	080h
NextLevelToggle 	DB	0
MissileX		DW	0
MissileY		DB	0
UFOX			DW	0
BombX			DW	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
BombY			DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
BombType		DB	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
BombFreq		DW	010h
MoveCount		DB	1
InvaderSpeed		DB	55
Direction		DB	0
Reversing		DB	0
Collision		DB	0
Score			DB	48,48,48,48,48
HighS			DB	48,48,48,48,48
FirstFrame		DB	0
Lives			DB	48
BombMove		DB	2
BombSpeed		DB	2
PlayerDead		DB	0
GameOverToggle		DB	0
InvadersTitle		DB	"S  P  A  C  E     I  N  V  A  D  E  R  S",0
Copyright		DB	"COPYRIGHT (  1995 BY PAUL S REID. ALL RIGHTS RESERVED",0
UFOScore		DB	"=   100 POINTS",0
Row1Score		DB	"=    25 POINTS",0
Row2Score		DB	"=    20 POINTS",0
Row3Score		DB	"=    15 POINTS",0
Row4Score		DB	"=    10 POINTS",0
Row5Score		DB	"=     5 POINTS",0
StartDocs		DB	"    SPACE BAR TO START GAME.  ESC TO EXIT TO DEXOS  ",0
Dedication		DB	"...DEDICATED TO MY WIFE DEB...",0
ThankYou		DB	"THANKS TO BRENT KYLE AND TOM SWAN",0
SoundTog		DB	"PRESS S TO TOGGLE SOUND AT ANY TIME",0
PlayKeys		DB	"LEFT AND RIGHT CURSOR TO MOVE. SPACE BAR TO FIRE",0
Distribution		DB	".THIS GAME AND SOURCE CODE ARE FREELY DISTRIBUTABLE.",0
GameOverMsg		DB	"G  A  M  E      O  V  E  R",0
GetReady		DB	"G  E  T      R  E  A  D  Y",0
TempStore		DW	0
;----------------------------------------------------------------------------------------------------------------------------------
; Sprite data
;----------------------------------------------------------------------------------------------------------------------------------

TopInvader1		DW	0c00h,1e00h,2d00h,3f00h,1200h,2100h,1200h
TopInvader2		DW	0c00h,1e00h,2d00h,3f00h,1200h,2100h,4080h

MiddleInvader1		DW	2100h,9e40h,0ad40h,7f80h,3f00h,2100h,4080h
MiddleInvader2		DW	2100h,1e00h,2d00h,7f80h,0bf40h,0a140h,1200h

BottomInvader1		DW	01e00h,7f80h,0ccc0h,0ffc0h,2100h,4c80h,2100h
BottomInvader2		DW	01e00h,7f80h,0ccc0h,0ffc0h,2100h,4c80h,8040h

TwistedMissile1 	DW	0000h,0000h,0000h,0800h,0400h,0800h,0400h
TwistedMissile2 	DW	0000h,0000h,0000h,0400h,0800h,0400h,0800h

UFO			DW	0ff0h,0ff0h,0ffffh,0ffffh,0ffffh,7ffeh,3ffch

PlayersShip		DW	0400h,0e00h,7fc0h,0ffe0h,0ffe0h,0ffe0h,0000h

StraightMissile 	DW	0000h,0000h,0000h,0400h,0400h,0400h,0400h

BunkerLeftTop		DW	0fffh,1fffh,3fffh,7fffh,0ffffh,0ffffh,0ffffh
BunkerLeftMiddle	DW	0ffffh,0ffffh,0ffffh,0ffffh,0ffffh,0ffffh,0ffffh
BunkerLeftBottom	DW	0ff81h,0fe00h,0fc00h,0f800h,0f800h,0f000h,0000h
BunkerRightTop		DW	0f000h,0f800h,0fc00h,0fe00h,0ff00h,0ff00h,0ff00h
BunkerRightMiddle	DW	0ff00h,0ff00h,0ff00h,0ff00h,0ff00h,0ff00h,0ff00h
BunkerRightBottom	DW	0ff00h,7f00h,3f00h,1f00h,1f00h,0f00h,0000h

SCO			DW	071c7h,08a28h,08208h,07208h,00a08h,08a28h,071c7h
ORE			DW	03cf8h,0a280h,0a280h,0bcf0h,0a280h,0a280h,022f8h

HIG			DW	08be7h,08888h,08888h,0f88bh,08888h,08888h,08be7h
GH			DW	02200h,0a200h,02200h,0be00h,0a200h,0a200h,02200h

Zero			DW	3800h,4400h,4c00h,5400h,6400h,4400h,3800h
One			DW	1000h,3000h,1000h,1000h,1000h,1000h,3800h
Two			DW	3800h,4400h,0400h,1800h,2000h,4000h,7c00h
Three			DW	3800h,4400h,0400h,1800h,0400h,4400h,3800h
Four			DW	4400h,4400h,4400h,7c00h,0400h,0400h,0400h
Five			DW	7c00h,4000h,4000h,7800h,0400h,4400h,3800h
Six			DW	3800h,4400h,4000h,7800h,4400h,4400h,3800h
Seven			DW	7c00h,0400h,0800h,1000h,1000h,1000h,1000h
Eight			DW	3800h,4400h,4400h,3800h,4400h,4400h,3800h
Nine			DW	3800h,4400h,4400h,3c00h,0400h,4400h,3800h

Equal			DW	0000h,0000h,7c00h,0000h,7c00h,0000h,0000h
Period			DW	0000h,0000h,0000h,0000h,0000h,0000h,1000h
CopyrightSymbol 	DW	1e00h,2100h,4c80h,4880h,4c80h,2100h,1e00h

LetterA 		DW	3800h,4400h,4400h,7c00h,4400h,4400h,4400h
LetterB 		DW	7800h,4400h,4400h,7800h,4400h,4400h,7800h
LetterC 		DW	3800h,4400h,4000h,4000h,4000h,4400h,3800h
LetterD 		DW	7800h,4400h,4400h,4400h,4400h,4400h,7800h
LetterE 		DW	7c00h,4000h,4000h,7800h,4000h,4000h,7c00h
LetterF 		DW	7c00h,4000h,4000h,7800h,4000h,4000h,4000h
LetterG 		DW	3800h,4400h,4000h,5c00h,4400h,4400h,3800h
LetterH 		DW	4400h,4400h,4400h,7c00h,4400h,4400h,4400h
LetterI 		DW	7c00h,1000h,1000h,1000h,1000h,1000h,7c00h
LetterJ 		DW	0400h,0400h,0400h,0400h,0400h,4400h,3800h
LetterK 		DW	4400h,4800h,5000h,6000h,5000h,4800h,4400h
LetterL 		DW	4000h,4000h,4000h,4000h,4000h,4000h,7c00h
LetterM 		DW	4400h,6c00h,5400h,4400h,4400h,4400h,4400h
LetterN 		DW	4400h,6400h,5400h,4c00h,4400h,4400h,4400h
LetterO 		DW	3800h,4400h,4400h,4400h,4400h,4400h,3800h
LetterP 		DW	7800h,4400h,4400h,7800h,4000h,4000h,4000h
LetterQ 		DW	3800h,4400h,4400h,4400h,4400h,4c00h,3c00h
LetterR 		DW	7800h,4400h,4400h,7800h,4400h,4400h,4400h
LetterS 		DW	3800h,4400h,4000h,3800h,0400h,4400h,3800h
LetterT 		DW	7c00h,1000h,1000h,1000h,1000h,1000h,1000h
LetterU 		DW	4400h,4400h,4400h,4400h,4400h,4400h,3800h
LetterV 		DW	4400h,4400h,4400h,4400h,4400h,2800h,1000h
LetterW 		DW	4400h,4400h,4400h,4400h,5400h,6c00h,4400h
LetterX 		DW	4400h,4400h,2800h,1000h,2800h,4400h,4400h
LetterY 		DW	4400h,4400h,2800h,1000h,1000h,1000h,1000h
LetterZ 		DW	7c00h,0400h,0800h,1000h,2000h,4000h,7c00h

 LogoOutline		DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,007ffh,01800h,02000h,04000h,040ffh,0403fh
			DW	00000h,0e3ffh,01400h,00c00h,00400h,00607h,0fa03h
			DW	00000h,0f80fh,00610h,00120h,000a0h,0c0e0h,0c0c1h
			DW	00000h,0fc00h,00200h,00101h,00102h,00082h,08084h
			DW	00000h,01fffh,06000h,08000h,00060h,000a0h,0011fh
			DW	00000h,003ffh,08400h,04800h,04800h,0500fh,0900fh
			DW	00000h,0ff80h,00040h,00040h,00080h,0ff00h,0c000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	02000h,01c00h,003fch,00fc3h,0103eh,01000h,00800h
			DW	0e200h,01900h,00503h,00301h,00281h,00280h,00440h
			DW	00181h,00682h,0f903h,00100h,00200h,08207h,04408h
			DW	08044h,04048h,0c028h,00028h,00018h,0e018h,0100ch
			DW	00100h,00200h,00200h,0047eh,00381h,00001h,00003h
			DW	02000h,02000h,0403fh,04020h,0803fh,08000h,00000h
			DW	02000h,02000h,0c000h,00000h,0f000h,00800h,00800h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,00000h,01fe0h,02011h,02009h,01009h
			DW	00000h,00000h,00000h,0fc0fh,00310h,000d0h,00038h
			DW	00700h,000ffh,00000h,0f1feh,00a01h,00600h,00300h
			DW	03840h,0c03fh,00000h,00fe0h,01010h,09010h,05010h
			DW	04408h,083f0h,00000h,07fe0h,08010h,08008h,08004h
			DW	0100bh,00ff0h,00000h,00fffh,01000h,01000h,01000h
			DW	0000dh,0fff0h,00000h,0f80fh,00610h,00110h,00090h
			DW	00000h,0ffffh,00000h,0fff8h,00005h,00005h,0000ah
			DW	01000h,0e000h,00000h,0ffffh,00000h,00000h,00000h
			DW	00000h,00000h,00000h,0803fh,060c0h,01100h,00a00h
			DW	00000h,00000h,00000h,0ff00h,00080h,00040h,00040h

			DW	01005h,00804h,00802h,00402h,00401h,00201h,00200h
			DW	00008h,0800ch,08003h,08000h,080c0h,04070h,0c04ch
			DW	00280h,00140h,00140h,000a0h,00090h,00048h,00044h
			DW	02809h,02809h,01809h,00c0ah,0040ah,00406h,00206h
			DW	00002h,00002h,00001h,00600h,00500h,00780h,00000h
			DW	0100fh,01009h,0100ah,0900ah,0900ah,0500ch,0300ch
			DW	000a0h,000a0h,000a0h,000a0h,000c0h,000c0h,000c0h
			DW	03ff2h,03f04h,00084h,00084h,0ff08h,08008h,0ffc8h
			DW	001c0h,001c0h,00000h,00000h,003c0h,00240h,00440h
			DW	00a07h,00a07h,03100h,040e0h,0201ch,017e6h,00818h
			DW	0e040h,09f80h,07000h,00800h,00400h,00400h,00400h

			DW	00100h,00100h,00080h,0007fh,00000h,00000h,00000h
			DW	0a023h,06020h,06010h,09fe0h,00000h,00000h,00000h
			DW	00022h,0c021h,03010h,00fe0h,00000h,00000h,00000h
			DW	00004h,00004h,0c004h,03ffbh,00000h,00000h,00000h
			DW	00000h,003e0h,00420h,0f81fh,00000h,00000h,00000h
			DW	03000h,01000h,01000h,0efffh,00000h,00000h,00000h
			DW	00140h,00680h,01880h,0e07fh,00000h,00000h,00000h
			DW	00030h,00030h,00050h,0ff8fh,00000h,00000h,00000h
			DW	00420h,00410h,00810h,0f00fh,00000h,00000h,00000h
			DW	00800h,00800h,00c00h,0f3ffh,00000h,00000h,00000h
			DW	00400h,00800h,07000h,08000h,00000h,00000h,00000h

LogoLetters		DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,007ffh,01fffh,03fffh,03f00h,03fc0h
			DW	00000h,00000h,0e3ffh,0f3ffh,0fbffh,0f9f8h,001fch
			DW	00000h,00000h,0f80fh,0fe1fh,0ff1fh,03f3fh,03f3eh
			DW	00000h,00000h,0fc00h,0fe00h,0fe01h,0ff01h,07f03h
			DW	00000h,00000h,01fffh,07fffh,0ff9fh,0ff1fh,0fe00h
			DW	00000h,00000h,003ffh,087ffh,087ffh,08ff0h,00ff0h
			DW	00000h,00000h,0ff80h,0ff80h,0ff00h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	01fffh,003ffh,00003h,00000h,00fc1h,00fffh,007ffh
			DW	001ffh,0e0ffh,0f8fch,0fcfeh,0fc7eh,0fc7fh,0f83fh
			DW	0fe7eh,0f87ch,000fch,000ffh,001ffh,001f8h,083f0h
			DW	07f83h,03f87h,03fc7h,0ffc7h,0ffe7h,01fe7h,00ff3h
			DW	0fe00h,0fc00h,0fc00h,0f800h,0fc7eh,0fffeh,0fffch
			DW	01fffh,01fffh,03fc0h,03fc0h,07fc0h,07fffh,0ffffh
			DW	0c000h,0c000h,00000h,00000h,00000h,0f000h,0f000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,00000h,00000h,01fe0h,01ff0h,00ff0h
			DW	00000h,00000h,00000h,00000h,0fc0fh,0ff0fh,0ffc7h
			DW	000ffh,00000h,00000h,00000h,0f1feh,0f9ffh,0fcffh
			DW	0c03fh,00000h,00000h,00000h,00fe0h,00fe0h,08fe0h
			DW	083f0h,00000h,00000h,00000h,07fe0h,07ff0h,07ff8h
			DW	00ff0h,00000h,00000h,00000h,00fffh,00fffh,00fffh
			DW	0fff0h,00000h,00000h,00000h,0f80fh,0fe0fh,0ff0fh
			DW	0ffffh,00000h,00000h,00000h,0fff8h,0fff8h,0fff1h
			DW	0e000h,00000h,00000h,00000h,0ffffh,0ffffh,0ffffh
			DW	00000h,00000h,00000h,00000h,0803fh,0e0ffh,0f1ffh
			DW	00000h,00000h,00000h,00000h,0ff00h,0ff80h,0ff80h

			DW	00ff8h,007f8h,007fch,003fch,003feh,001feh,001ffh
			DW	0fff7h,07ff3h,07ffch,07fffh,07f3fh,03f8fh,03f83h
			DW	0fc7fh,0fe3fh,0fe3fh,0ff1fh,0ff0fh,0ff87h,0ff83h
			DW	0c7f0h,0c7f0h,0e7f0h,0f3f1h,0fbf1h,0fbf9h,0fdf9h
			DW	0fffch,0fffch,0fffeh,0f9ffh,0f8ffh,0f87fh,0ffffh
			DW	00ff0h,00ff0h,00ff1h,00ff1h,00ff1h,08ff3h,0cff3h
			DW	0ff1fh,0ff1fh,0ff1fh,0ff1fh,0ff3fh,0ff3fh,0ff3fh
			DW	0c001h,0c003h,0ff03h,0ff03h,00007h,00007h,00007h
			DW	0fe3fh,0fe3fh,0ffffh,0ffffh,0fc3fh,0fc3fh,0f83fh
			DW	0f1f8h,0f1f8h,0c0ffh,0801fh,0c003h,0e001h,0f7e7h
			DW	01f80h,00000h,08000h,0f000h,0f800h,0f800h,0f800h

			DW	000ffh,000ffh,0007fh,00000h,00000h,00000h,00000h
			DW	01fc0h,09fc0h,09fe0h,00000h,00000h,00000h,00000h
			DW	0ffc1h,03fc0h,00fe0h,00000h,00000h,00000h,00000h
			DW	0fffbh,0fffbh,03ffbh,00000h,00000h,00000h,00000h
			DW	0ffffh,0f81fh,0f81fh,00000h,00000h,00000h,00000h
			DW	0cfffh,0efffh,0efffh,00000h,00000h,00000h,00000h
			DW	0fe3fh,0f87fh,0e07fh,00000h,00000h,00000h,00000h
			DW	0ffcfh,0ffcfh,0ff8fh,00000h,00000h,00000h,00000h
			DW	0f81fh,0f80fh,0f00fh,00000h,00000h,00000h,00000h
			DW	0f7ffh,0f7ffh,0f3ffh,00000h,00000h,00000h,00000h
			DW	0f800h,0f000h,08000h,00000h,00000h,00000h,00000h

LogoShadow		DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	01fffh,07800h,0e000h,0c000h,08000h,00000h,00000h
			DW	087ffh,00400h,00000h,00000h,00000h,00000h,00000h
			DW	0f00fh,00410h,00000h,00000h,00000h,00000h,00000h
			DW	0fc00h,00200h,00000h,00000h,00000h,00000h,00000h
			DW	00fffh,02000h,08000h,00000h,00000h,00000h,00000h
			DW	080ffh,0c000h,04000h,00000h,00000h,00000h,00000h
			DW	0ffe0h,00060h,00000h,00000h,00000h,00000h,03000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,03c00h,0303ch,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00001h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,08000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,0003fh,00381h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,0001fh,00000h,00000h,00000h
			DW	00000h,00000h,00000h,0fc00h,00c00h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h

			DW	00000h,00000h,0ff03h,0e003h,04002h,04002h,02000h
			DW	00000h,00000h,0f03fh,00030h,00000h,00000h,00000h
			DW	00000h,00000h,0c3fch,00200h,00000h,00000h,00000h
			DW	00000h,00000h,01fc0h,01000h,00000h,00000h,00000h
			DW	00000h,00000h,07fe0h,00010h,00008h,00004h,00000h
			DW	00000h,00000h,00fffh,00000h,00000h,00000h,00000h
			DW	00000h,00000h,0f807h,00600h,00100h,00000h,00000h
			DW	00000h,00000h,0fffch,00004h,00000h,00000h,00000h
			DW	00000h,00000h,03fffh,00000h,00000h,00000h,00000h
			DW	00000h,00000h,0e007h,07800h,01c00h,00c00h,00400h
			DW	00000h,00000h,0ffe0h,000f0h,00070h,00030h,00000h

			DW	02000h,01001h,01001h,00800h,00800h,00400h,00400h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00001h,00000h,00000h,00200h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,04000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00080h,00000h,00000h,00000h,07fe0h,00020h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00200h
			DW	00000h,00000h,00000h,03800h,01ce0h,00818h,00000h
			DW	00000h,06000h,00f00h,00700h,00300h,00300h,00200h

			DW	00200h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	04000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			DW	00000h,00000h,00000h,00000h,00000h,00000h,00000h
			db	00,00,00,00    ; Background
Palette 		DB	00,00,00,00    ; Background
			DB	21,63,63,00    ; Player's Ship
			DB	63,21,21,00    ; Bunker
			DB	63,21,21,00    ; UFO
			DB	63,63,63,00    ; Missiles
			DB	21,63,21,00    ; Status Letters
			DB	63,63,00,00    ; Bombs
			DB	63,63,63,00    ; DOS text (just for diagnostic printing if required)
			DB	12,00,21,00    ; Top and bottom border backgrounds
					       ; DB      32,00,57,00
			DB	33,00,42,00    ; Logo shadow color
			DB	00,00,00,00    ; Logo outline color
			DB	63,63,00,00    ; Logo letters color
			DB	41,41,41,00    ; Logo stars
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	21,63,21,00    ; Row 1 invader 1
			DB	21,63,21,00    ; Row 1 invader 2
			DB	21,63,21,00    ; Row 1 invader 3
			DB	21,63,21,00    ; Row 1 invader 4
			DB	21,63,21,00    ; Row 1 invader 5
			DB	21,63,21,00    ; Row 1 invader 6
			DB	21,63,21,00    ; Row 1 invader 7
			DB	21,63,21,00    ; Row 1 invader 8
			DB	21,63,21,00    ; Row 1 invader 9
			DB	21,63,21,00    ; Row 1 invader 10
			DB	21,63,21,00    ; Row 1 invader 11
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	21,63,21,00    ; Row 2 invader 1
			DB	21,63,21,00    ; Row 2 invader 2
			DB	21,63,21,00    ; Row 2 invader 3
			DB	21,63,21,00    ; Row 2 invader 4
			DB	21,63,21,00    ; Row 2 invader 5
			DB	21,63,21,00    ; Row 2 invader 6
			DB	21,63,21,00    ; Row 2 invader 7
			DB	21,63,21,00    ; Row 2 invader 8
			DB	21,63,21,00    ; Row 2 invader 9
			DB	21,63,21,00    ; Row 2 invader 10
			DB	21,63,21,00    ; Row 2 invader 11
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	21,63,63,00    ; Row 3 invader 1
			DB	21,63,63,00    ; Row 3 invader 2
			DB	21,63,63,00    ; Row 3 invader 3
			DB	21,63,63,00    ; Row 3 invader 4
			DB	21,63,63,00    ; Row 3 invader 5
			DB	21,63,63,00    ; Row 3 invader 6
			DB	21,63,63,00    ; Row 3 invader 7
			DB	21,63,63,00    ; Row 3 invader 8
			DB	21,63,63,00    ; Row 3 invader 9
			DB	21,63,63,00    ; Row 3 invader 10
			DB	21,63,63,00    ; Row 3 invader 11
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	21,63,63,00    ; Row 4 invader 1
			DB	21,63,63,00    ; Row 4 invader 2
			DB	21,63,63,00    ; Row 4 invader 3
			DB	21,63,63,00    ; Row 4 invader 4
			DB	21,63,63,00    ; Row 4 invader 5
			DB	21,63,63,00    ; Row 4 invader 6
			DB	21,63,63,00    ; Row 4 invader 7
			DB	21,63,63,00    ; Row 4 invader 8
			DB	21,63,63,00    ; Row 4 invader 9
			DB	21,63,63,00    ; Row 4 invader 10
			DB	21,63,63,00    ; Row 4 invader 11
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	00,00,00,00
			DB	63,21,63,00    ; Row 5 invader 1
			DB	63,21,63,00    ; Row 5 invader 2
			DB	63,21,63,00    ; Row 5 invader 3
			DB	63,21,63,00    ; Row 5 invader 4
			DB	63,21,63,00    ; Row 5 invader 5
			DB	63,21,63,00    ; Row 5 invader 6
			DB	63,21,63,00    ; Row 5 invader 7
			DB	63,21,63,00    ; Row 5 invader 8
			DB	63,21,63,00    ; Row 5 invader 9
			DB	63,21,63,00    ; Row 5 invader 10
			DB	63,21,63,00    ; Row 5 invader 11

 ;-----------------------------------------------------------------------
 ; Data
 ;-----------------------------------------------------------------------
KeyDown 		dd	      0 		  ;
Up			 =	      0x48		  ; Up arrow
Left			 =	      0x4b		  ;
Right			 =	      0x4d		  ;
S			 =	      0x1f		  ;
Space			 =	      0x39		  ;
Up1			 =	      0x4e		  ;
Down1			 =	      0x4a		  ;
EscKey			 =	      1 		  ;
							  ;
include 'Dex.inc'					  ;
VideoBuffer		rd	      320*190		  ;
BSrceenBuffer		rd	      320*10		  ;
justincase		rd	      500		  ;