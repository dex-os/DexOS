;=========================================================;
; Dex include                                  05/02/2012 ;
;---------------------------------------------------------;
;                                                         ;
; DexOS                                                   ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;=======================================================  ;
;  Functions                        Function calltable.   ;
;=======================================================  ; 
Functions:						  ;
CallTableAddress     rd 1				  ;  0. call-table address,DO NOT call as function.
RealModeInt10h	     rd 1				  ;  1. Call realmode int 10h,fill in EAX,CX,DX, with function number etc, NOTE: ( EAX = bits 32 to 17 = BX, bits 16 to 1 =AX ), be carefull may not work will all functions.
ModuleFunction	     rd 1				  ;  2. Call a Module function, ESI=Address of module function ID string, AH=Module function number, Return CF=1 If error, AH=Error code.
RealModeRegs	     rd 1				  ;  3. Fills in AX,BX,CX,DX with result from above function,set CF to 1 on error.
PrintString_0	     rd 1				  ;  4. Prints zero termanated string,also needs, 10, 13 for nextline..
Clstext 	     rd 1				  ;  5. Clears text mode screen.
TextColor	     rd 1				  ;  6. Sets the text color, Entry: AL=color.
GetTextColor	     rd 1				  ;  7. Gets the text color, Return AL=color.
GetTextPos	     rd 1				  ;  8. get text pos, Return AL= X, AH = Y.
PrintString_$	     rd 1				  ;  9. prints a string,esi should point to a '$' termanated string,also needs, 10, 13 for nextline..
PrintChar	     rd 1				  ; 10. prints the char,in al
PrintCharCursor      rd 1				  ; 11. prints the char,in al and moves cursor
SetCursorPos	     rd 1				  ; 12. Sets Cursor Pos, Entry: AL=X, AH=Y, if the X or Y is bigger than screen size, CF to 1, succes AH=0.
WriteHex32	     rd 1				  ; 13. print the hex value,in eax
WriteHex16	     rd 1				  ; 14. print the hex value,in AX
WriteHex8	     rd 1				  ; 15. Print's the hex value, in AL
UpperCase	     rd 1				  ; 16. Converts a 0 termanated string,to uppercase (es:edi = string buffer)
LowerCase	     rd 1				  ; 17. Converts a 0 termanated string,to lowercase (es:edi = string buffer)
CompareString	     rd 1				  ; 18. Compare two strings,set CF to 1, if strings are not the same
GetParams	     rd 1				  ; 19. Gets the address of the command line buffer,return ESI=Command line buffer address.
GetUserInput	     rd 1				  ; 20. Gets user input, until enter pressed. Exit: EDI = buffer address, cx = string len.
GetCommandStored     rd 1				  ; 21. Gets user input,until enter is pressed. Exit: EDI = buffer address, cx = string len.This ver work with right arrow key, to get last input.
WaitForKeyPress      rd 1				  ; 22. Wait for keypress,Exit: ASCII code of keypressed in AL .
KeyPressedNoWait     rd 1				  ; 23. If keyperssed AL=1,if no keypressed AL=0.
KeyPressedScanNW     rd 1				  ; 24. IF keyperssed AL= ASCII code, Ah = Scancode, if no keypressed AL=0, eg no wait.
GameKeyBoardOn	     rd 1				  ; 25. Puts the game keyboard on, ESI points to KeyDown a 128 byte array.
GameKeyBoardOff      rd 1				  ; 26. Turns the Game keyboard off.
KeyBoardLanguage     rd 1				  ; 27. Returns a pointer to keyMap, EAX = 'KEYB' if sucsessfull, EDI = pointer to keyMap, KeyMap size = 1024 bytes
ExtendedMemory	     rd 1				  ; 28. puts extended memory in eax (in KBs),puts total ram in ebx (in MB)
ConvenMemorySize     rd 1				  ; 29. puts conventional memory in eax (in KBs)
TopOfMemory	     rd 1				  ; 30. Returns in EAX top of useable memory, in bytes.
GetBaseAddOn	     rd 1				  ; 31. Returns the base add on, in EAX.
DetectPciBus	     rd 1				  ; 32. Detect Pci Bus present, set's CF to 1, if no pci bus
PciFindDevice	     rd 1				  ; 33. Scan through PCI space looking for a device+vendor ID, Entry: EAX=Device+vendor ID ,RETURN: EAX=PCI address if device found,  CF to 1,if no vender/device found.
PciRegWrite8	     rd 1				  ; 34. Entry: EAX=PCI Bus/Device/fn/register number,DL=data to write.
PciRegWrite16	     rd 1				  ; 35. Entry: EAX=PCI Bus/Device/fn/register number,DX=data to write.
PciRegWrite32	     rd 1				  ; 36. Entry: EAX=PCI Bus/Device/fn/register number,EDX=data to write.
PciRegRead8	     rd 1				  ; 37. Entry: EAX=PCI Bus/Device/fn/register number,Exit: DL register data.
PciRegRead16	     rd 1				  ; 38. Entry: EAX=PCI Bus/Device/fn/register number,Exit: DX register data.
PciRegRead32	     rd 1				  ; 39. Entry: EAX=PCI Bus/Device/fn/register number,Exit: EDX register data.
InterruptTimer	     rd 1				  ; 40. Reprograms interrupt timer, CX = number of times a second, at program end the timer is restored with CX = 0 .
SetDelay	     rd 1				  ; 41. Sets a delay, ax = delay,18.2 = 1 second.
TimerNoWait	     rd 1				  ; 42. Sets a delay, but does not wait, AX=timer to delay, AL=0 is call if AL returns 1 than timer not finished, if AL returns 0 them timer finished.
GetClockTicks	     rd 1				  ; 43. Count clock ticks.
TimerSecondCount     rd 1				  ; 44. Returns in EAX,the number of ticks, since OS started, 18.2 ticks a second are added. 
CountTicks	     rd 1				  ; 45. Returns CF to 1 on clock tick (defaults to 18.2 times a second), CF = 0, the rest of the time. 
LoadVesaInfo	     rd 1				  ; 46. loads the address of vesainfo in esi, use this to fill a vesa info structure (in Dex.inc).
SetVesaMode	     rd 1				  ; 47. Sets vesa mode, BX = MODE NUMBER, Return CF to 1 on error. NOTE: You need to call the above function after this one.
ResetMouse	     rd 1				  ; 48. enables mouse, AX = 0x0000 if no mouse found, AX = 0xffff if found, next function needed to show mouse.
ShowMouse	     rd 1				  ; 49. Shows mouse pointer (text mode).
HideMouse	     rd 1				  ; 50. Hides mouse text mode.
GetMousePos	     rd 1				  ; 51. Gets the XY and button press, on return BL = button pressed, 00000001 = rightbutton, 00000010 = centerbutton, 00000100 = leftbutton. CX=X, DX=Y.
SetMouseMaxMinXY     rd 1				  ; 52. Set mouses min/max xy,input EAX bits 32to17=minX bits16to1=maxX, EDX bits 32to17=minY bits16to1=maxY.
SetMouseXY	     rd 1				  ; 53. Sets mouse xy, input AX=X, DX=Y.
SetMouseOff	     rd 1				  ; 54. Turns the mouse off.
RamSize 	     rd 1				  ; 55. Eax = ram size
Function56	     rd 1				  ; 
Function57	     rd 1				  ; 
Function58	     rd 1				  ; 
Function59	     rd 1				  ;             
PlayAllTracks	     rd 1				  ; 60. Play all tracks on CD, Return number of tracks in AL.
PlayTrack	     rd 1				  ; 61. Play one track, AL = Track to play, returns AL = Number of tracks on cd.
CheckCdBusy	     rd 1				  ; 62. Checks if CD is busy, return set's CF to 1 on error (busy).
CheckCdStateus	     rd 1				  ; 63. Checks CD stateus, return AL = Stateus code, set's CF to 1 on error.
TrackInFo	     rd 1				  ; 64. Returns number of tracks on audio CD, AL= Number.
RebootSystem	     rd 1				  ; 65. Reboots the machine.
ExitSystem	     rd 1				  ; 66. Exits to DOS, if in DOS, Reboots otherwise.
SetIntVector	     rd 1				  ; 67. Hooks in the interrupt vector, AL = interrupt number, EDX = interrupt handler address (Note: selector, is not updated, but you can unhighlight it in the kernel32)
GetIntVector	     rd 1				  ; 68. Gets the the interrupt vector address, Entry: AL = interrupt number, Return EDX = offset, CX = selector (Note: only offset need updating, on returning old address or setting new)
GetDate 	     rd 1				  ; 69. Gets the currant date return : EAX=century, year, month,day, each a byte in bcd format.
GetTime 	     rd 1				  ; 70. Gets the currant time return : EAX=hour, minute, second, each a byte in bcd format.
FatTime 	     rd 1				  ; 71. Returns a time stamp for including in FAT, Output AX = TIME.
FatDate 	     rd 1				  ; 72. Returns a date stamp for including in FAT, Output AX = DATE.
GetDmaBufferAddress  rd 1				  ; 73. Return's the Dma floppy buffer address, ESI = READ, EDI = WRITE, There is no error that can be returned.
FddReadB	     rd 1				  ; 74. Floppy/Hdd read sector, using bios driver, CH=Track/cylinder, CL=Sector, DH=Head, DL=Drive (A:  use 00 , C: use 80h ), RETURN AH=Status, AL=Sector number read, CF=1 If error.
FddWriteB	     rd 1				  ; 75. Floppy/Hdd write sector, using bios driver, CH=Track/cylinder, CL=Sector, DH=Head, DL=Drive (A: use 00 , C: use 80h), RETURN AH=Status, AL=Sector number read, CF=1 If error.
FS_ReadSector	     rd 1				  ; 76. Reads a sector from hdd, using PM driver, EAX = Unit (drive number) 1 = primary device master, 2 = primary device slave and so on, EDI = Buffer to write the data to (must be 512bytes), EDX = Sector number to read from (LBA format), ECX = Count  Nunmber of sectors (just 1 for now)
FS_WriteSector	     rd 1				  ; 77. Write a sector from hdd, using PM driver, EAX = Unit (drive number) 1 = primary device master, 2 = primary device slave and so on, EDI = Buffer to read the data from (must be 512bytes), EDX = Sector number to write to (LBA format), ECX = Count  Nunmber of sectors (just 1 for now)
DirFileList	     rd 1				  ; 78. Fills a buffer with file names from root dir Entry: EDI = Buffer address ECX = Max number AX = Ext eg 'GE' for gex files or AX = 0 for all files Exit: EDI, ESI = BUFFER Address ECX = Number of file listed.
ReadFileFat	     rd 1				  ; 79. Loads a file from default drive (eg: A: C: ..)Entry: EDI = File name EDX = Address to load file too. Exit:  CF=1 If error. or CF clear and EBX = Number of sectors read, ECX = Nunmber of bytes
WriteFileFat	     rd 1				  ; 80. Writes a file to default drive (fat16 only), EAX = File size in bytes, ESI = Name of file to write, EDI = Data to write, Carry set on error , no errors EBX = Number of sector written, ECX = Nunmber of bytes
GetBootDrive	     rd 1				  ; 81. Loads al, with C or A drive letter, bl = (0=A, 1=B, 2=C, etc).
print_time	     rd 1				  ; 82. Prints time in text mode
print_date	     rd 1				  ; 83. Prints date in text mode
AnyFatUpdate	     rd 1				  ; 84. Call this to check if there as been any thing written to FAT, EAX = 1 if no update needed or EAX = 0 if needed
DeleteFile	     rd 1				  ; 85. Deletes a file from the default drive, ESI = Name of file to delete, AL = 1 if not to write to disk or AL = 0 to upade fat/root dir to disk, Carry set on error, AH = Error code.
ReMountFdd	     rd 1				  ; 86. Remount the root dir, eg: if you change floppy or USB.
ReNameFile	     rd 1				  ; 87. Rename's a file on the default drive. ESI = Old file name(zero ending string), EDI = New file name(zero ending string), Carry set on error, AX = Error code.
GetRmPalette	     rd 1				  ; 88. Gets the standared realmode Palette, EDX = pointer to 20 byte Palette
Function89	     rd 1
Function90	     rd 1
Function91	     rd 1
Function92	     rd 1
Function93	     rd 1
Function94	     rd 1
Function95	     rd 1
Function96	     rd 1
Function97	     rd 1
Function98	     rd 1
Function99	     rd 1
Function100	     rd 1
Function101	     rd 1
Function102	     rd 1
Function103	     rd 1
Function104	     rd 1
Function105	     rd 1
Function106	     rd 1
Function107	     rd 1
Function108	     rd 1
Function109	     rd 1
Function110	     rd 1
Function111	     rd 1
Function112	     rd 1
Function113	     rd 1
Function114	     rd 1
Function115	     rd 1
Function116	     rd 1
Function117	     rd 1
Function118	     rd 1
Function119	     rd 1
Function120	     rd 1
Function121	     rd 1
Function122	     rd 1
Function123	     rd 1
Function124	     rd 1
Function125	     rd 1
Function126	     rd 1
Function127	     rd 1
Function128	     rd 1
Function129	     rd 1
Function130	     rd 1
Function131	     rd 1
Function132	     rd 1
Function133	     rd 1
Function134	     rd 1
Function135	     rd 1
Function136	     rd 1
Function137	     rd 1
Function138	     rd 1
Function139	     rd 1
Function140	     rd 1
Function141	     rd 1
Function142	     rd 1
Function143	     rd 1
Function144	     rd 1
Function145	     rd 1
Function146	     rd 1
Function147	     rd 1
Function148	     rd 1
Function149	     rd 1
Function150	     rd 1
Function151	     rd 1
Function152	     rd 1
Function153	     rd 1
Function154	     rd 1
Function155	     rd 1
Function156	     rd 1
Function157	     rd 1
Function158	     rd 1
Function159	     rd 1
Function160	     rd 1
Function161	     rd 1
Function162	     rd 1
Function163	     rd 1
Function164	     rd 1
Function165	     rd 1
Function166	     rd 1
Function167	     rd 1
Function168	     rd 1
Function169	     rd 1
Function170	     rd 1
Function171	     rd 1
Function172	     rd 1
Function173	     rd 1
Function174	     rd 1
Function175	     rd 1
Function176	     rd 1
Function177	     rd 1
Function178	     rd 1
Function179	     rd 1
Function180	     rd 1
Function181	     rd 1
Function182	     rd 1
Function183	     rd 1
Function184	     rd 1
Function185	     rd 1
Function186	     rd 1
Function187	     rd 1
Function188	     rd 1
Function189	     rd 1
Function190	     rd 1
Function191	     rd 1
Function192	     rd 1
Function193	     rd 1
Function194	     rd 1
Function195	     rd 1
Function196	     rd 1
Function197	     rd 1
Function198	     rd 1
Function199	     rd 1
Function200	     rd 1
Function201	     rd 1
Function202	     rd 1
Function203	     rd 1
Function204	     rd 1
Function205	     rd 1
Function206	     rd 1
Function207	     rd 1
Function208	     rd 1
Function209	     rd 1
Function210	     rd 1
Function211	     rd 1
Function212	     rd 1
Function213	     rd 1
Function214	     rd 1
Function215	     rd 1
Function216	     rd 1
Function217	     rd 1
Function218	     rd 1
Function219	     rd 1
Function220	     rd 1
Function221	     rd 1
Function222	     rd 1
Function223	     rd 1
Function224	     rd 1
Function225	     rd 1
Function226	     rd 1
Function227	     rd 1
Function228	     rd 1
Function229	     rd 1
Function230	     rd 1
Function231	     rd 1
Function232	     rd 1
Function233	     rd 1
Function234	     rd 1
Function235	     rd 1
Function236	     rd 1
Function237	     rd 1
Function238	     rd 1
Function239	     rd 1
Function240	     rd 1
Function241	     rd 1
Function242	     rd 1
Function243	     rd 1
Function244	     rd 1
Function245	     rd 1
Function246	     rd 1
Function247	     rd 1
Function248	     rd 1
Function249	     rd 1
Function250	     rd 1
Function251	     rd 1
Function252	     rd 1
Function253	     rd 1
Function254	     rd 1
Function255	     rd 1
Function256	     rd 256
;=======================================================;
;   VESA INFORMATION BLOCK.                             ;
;=======================================================; 
VESA_Info:		
VESAInfo_Signature		rb	4
VESAInfo_Version		rw	1
VESAInfo_OEMStringPtr		rd	1
VESAInfo_Capabilities		rb	4
VESAInfo_VideoModePtr		rd	1
VESAInfo_TotalMemory		rw	1
VESAInfo_OEMSoftwareRev 	rw	1
VESAInfo_OEMVendorNamePtr	rd	1
VESAInfo_OEMProductNamePtr	rd	1
VESAInfo_OEMProductRevPtr	rd	1
VESAInfo_Reserved		rb	222
VESAInfo_OEMData		rb	256
;=======================================================;
;   VESA MODE INFORMATION.                              ;
;=======================================================; 
Mode_Info:		
ModeInfo_ModeAttributes 	rw	1
ModeInfo_WinAAttributes 	rb	1
ModeInfo_WinBAttributes 	rb	1
ModeInfo_WinGranularity 	rw	1
ModeInfo_WinSize		rw	1
ModeInfo_WinASegment		rw	1
ModeInfo_WinBSegment		rw	1
ModeInfo_WinFuncPtr		rd	1
ModeInfo_BytesPerScanLine	rw	1
ModeInfo_XResolution		rw	1
ModeInfo_YResolution		rw	1
ModeInfo_XCharSize		rb	1
ModeInfo_YCharSize		rb	1
ModeInfo_NumberOfPlanes 	rb	1
ModeInfo_BitsPerPixel		rb	1
ModeInfo_NumberOfBanks		rb	1
ModeInfo_MemoryModel		rb	1
ModeInfo_BankSize		rb	1
ModeInfo_NumberOfImagePages	rb	1
ModeInfo_Reserved_page		rb	1
ModeInfo_RedMaskSize		rb	1
ModeInfo_RedMaskPos		rb	1
ModeInfo_GreenMaskSize		rb	1
ModeInfo_GreenMaskPos		rb	1
ModeInfo_BlueMaskSize		rb	1
ModeInfo_BlueMaskPos		rb	1
ModeInfo_ReservedMaskSize	rb	1
ModeInfo_ReservedMaskPos	rb	1
ModeInfo_DirectColorModeInfo	rb	1
;=======================================================;
;   VBE 2.0 extensions                                  ;
;=======================================================; 
ModeInfo_PhysBasePtr		rd	1
ModeInfo_OffScreenMemOffset	rd	1
ModeInfo_OffScreenMemSize	rw	1
;=======================================================;
;   VBE 3.0 extensions                                  ;
;=======================================================; 
ModeInfo_LinBytesPerScanLine	rw	1
ModeInfo_BnkNumberOfPages	rb	1
ModeInfo_LinNumberOfPages	rb	1
ModeInfo_LinRedMaskSize 	rb	1
ModeInfo_LinRedFieldPos 	rb	1
ModeInfo_LinGreenMaskSize	rb	1
ModeInfo_LinGreenFieldPos	rb	1
ModeInfo_LinBlueMaskSize	rb	1
ModeInfo_LinBlueFieldPos	rb	1
ModeInfo_LinRsvdMaskSize	rb	1
ModeInfo_LinRsvdFieldPos	rb	1
ModeInfo_MaxPixelClock		rd	1
;=======================================================;
;   Reserved                                            ;
;=======================================================; 
ModeInfo_Reserved		rb	190*4
;=======================================================;
;   VESA MODE INFORMATION END.                          ;
;=======================================================; 
