;=========================================================;
; Function                                       11/12/06 ;
;---------------------------------------------------------;
; Checks for command.                                     ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

;----------------------------------------------------;
; CallTable                    ; Function calltable. ;
;----------------------------------------------------;
CallTable:
Function0   dd CallTable		   ; Address of this call-table.
Function1   dd int_10h			   ; Call realmode int 10h,fill in AX,BX,CX,DX, with function number etc.
Function2   dd ModService		   ; Call a Module function, ESI=Address of module function ID string, AH=Module function number, Return CF=1 If error, AH=Error code.
Function3   dd RealModeRegs		   ; Fills in AX,BX,CX,DX, with result from above function,set CF to 1 on error .
Function4   dd print_string_zero	   ; Prints zero termanated string.
Function5   dd cls_text 		   ; Clears text mode screen (25x80).
Function6   dd TextColor		   ; Sets the text color, Entry: AL=color.
Function7   dd GetTextColor		   ; Gets the text color, Return AL=color.
Function8   dd get_text_pos		   ; Get text pos, Return AL= X, AH = Y.
Function9   dd print_string		   ; prints a '$' ending string
Function10  dd print_char		   ; prints the char,in al
Function11  dd PrintCharCursorPos	   ; prints the char,in al and moves cursor
Function12  dd SetCursorPos		   ; Sets Cursor Pos, Entry: AL=X, AH=Y, if the X or Y is bigger than screen size, CF to 1, succes AH=0. 
Function13  dd write_hex32		   ; print the hex value,in eax
Function14  dd write_hex16		   ; print the hex value, in ax
Function15  dd write_hex8		   ; print the hex value,in al
Function16  dd UpperCase		   ; Converts a 0 termanated string,to uppercase (es:edi = string buffer)
Function17  dd LowerCase		   ; Converts a 0 termanated string,to lowercase (es:edi = string buffer)
Function18  dd CompearString		   ; Compear two strings,set CF to 1,if strings do not match
Function19  dd GetParams		   ; Gets the address of the command line buffer,return ESI=Command line buffer address.
Function20  dd GetCommandUser		   ; Gets user input,until enter press. Exit: EDI = buffer address, cx = string len.
Function21  dd GetCommandNEW		   ; Gets user input,until enter press. Exit: EDI = buffer address, cx = string len.This ver work with right arrow key, to get last input.
Function22  dd WaitForKeyPressInt46OOH	   ; Wait for keypress,ASCII code in AL .
Function23  dd KeyPressedNoWait 	   ; IF keyperssed AL=1,if no keypressed AL=0.
Function24  dd KeyPressedNoWaitScanCodes   ; IF keyperssed AL= ASCII code, Ah = Scancode, if no keypressed AL=0.
Function25  dd GameKeyBoardOn2		   ; Puts the game keyboard on.
Function26  dd GameKeyBoardOff		   ; Turns the Game keyboard off.
Function27  dd KeyBoardLanguage 	   ; Returns a pointer to keyMap, EAX = 'KEYB' if sucsessfull, EDI = pointer to keyMap, KeyMap size = 1024 bytes
Function28  dd ExtMemory		   ; puts extended memory in eax(in KBs)
Function29  dd ConventionalMemory	   ; puts conventional memory in eax(in KBs)
Function30  dd TopOfMemory		   ; Returns in EAX top of useable memory,in bytes.
Function31  dd GetBaseAddOn		   ; Returns the base add on, in EAX.
Function32  dd DetectPciBus1		   ; Detect Pci Bus present,set's CF to 1,if no pci bus
Function33  dd PciFindDevice		   ; Scan through PCI space looking for a device+vendor ID,CF to 1,if no vender/device found. 
Function34  dd PciRegWrite8		   ; Entry: EAX=PCI Bus/Device/fn/register number,DL=data to write.
Function35  dd PciRegWrite16		   ; Entry: EAX=PCI Bus/Device/fn/register number,DX=data to write.
Function36  dd PciRegWrite32		   ; Entry: EAX=PCI Bus/Device/fn/register number,EDX=data to write.
Function37  dd PciRegRead8		   ; Entry: EAX=PCI Bus/Device/fn/register number,Exit: DL register data. 
Function38  dd PciRegRead16		   ; Entry: EAX=PCI Bus/Device/fn/register number,Exit: DX register data. 
Function39  dd PciRegRead32		   ; Entry: EAX=PCI Bus/Device/fn/register number,Exit: EDX register data. 
Function40  dd InterruptTimer		   ; Reprograms interrupt timer, CX = number of times a second, at program end the timer is restored with CX = 0 .
Function41  dd SetTimer 		   ; Sets a delay, ax = delay,18.2 = 1 second.
Function42  dd TimerNoWait		   ; Sets a delay,but does not wait, AX=timer to delay, AL=0 is call if AX returns 0, them time end.
Function43  dd GetClockTicks		   ; Get Clock Ticks.
Function44  dd TimerSecondCount 	   ; Returns in EAX,the number of ticks, since OS started, 18 ticks a second are added (unless reprogrammed.
Function45  dd CountTicks		   ; returns CF to 1 on clock tick (defaults to 18.2 times a second), CF = 0, the rest of the time. 
Function46  dd LoadVesaInfo		   ; loads the address of vesainfo in esi.
Function47  dd VesaRmInt		   ; Sets vesa mode, CX = MODE NUMBER, Return CF to 1 on error. NOTE: You need to call the above function after this one.
Function48  dd ResetMouse		   ; enables mouse, AX = 0x0000 if no mouse found, AX = 0xffff if found, next function needed to show mouse.
Function49  dd ShowMouse		   ; Shows mouse pointer text mode.
Function50  dd HideMouse		   ; Hides mouse text mode.
Function51  dd GetMousePos		   ; Gets the XY and button press, BL = button pressed, 00000001 = rightbutton, 00000010 = centerbutton, 00000100 = leftbutton. CX=X, DX=Y.
Function52  dd SetMouseMaxMinXY 	   ; Set mouses min/max xy, EAX bits 32to17=minX bits16to1=maxX, EDX bits 32to17=minY bits16to1=maxY.
Function53  dd SetMouseXY		   ; Sets the xy,AX=X, DX=Y.
Function54  dd SetMouseOff		   ; Turns the mouse off.
Function55  dd RamSize			   ; Eax = ram size
Function56  dd EmptyFunction		   ; 
Function57  dd EmptyFunction		   ; 
Function58  dd EmptyFunction		   ; 
Function59  dd EmptyFunction		   ;
Function60  dd PlayAllTracks		   ;
Function61  dd PlayTrack		   ;
Function62  dd WaitUnitReadyNoWait	   ;
Function63  dd ReadSubChannal		   ;
Function64  dd TrackInFo		   ;
Function65  dd RebootSystem		   ; Reboots the machine.
Function66  dd ExitDOS			   ; Exits to DOS, if in DOS, Reboots otherwise.
Function67  dd SetIntVector		   ; Hooks in the interrupt vector, AL = interrupt number, EDX = interrupt handler address (Note: selector, is not updated, but you can unhighlight it in the kernel32)
Function68  dd GetIntVector		   ; Gets the the interrupt vector address, Entry: AL = interrupt number, Return EDX = offset, CX = selector (Note: only the offset needs updating, on returning old address or setting new)
Function69  dd GetDate			   ; Gets the currant date return : EAX=century, year, month,day, each a byte in bcd format.
Function70  dd GetTime			   ; Gets the currant time return : EAX=hour, minute, second, each a byte in bcd format.
Function71  dd FloppyTime		   ; Returns a time stamp for including in FAT, Output AX = TIME.
Function72  dd FloppyDate		   ; Returns a date stamp for including in FAT, Output AX = DATE.
Function73  dd GetDMAbufferaddress	   ; Return the Dma floppy buffer address ESI = READ, EDI = WRITE, There is no error can be returned.
Function74  dd FddReadB 		   ; Floppy read sector, using bios driver, CH=Track/cylinder, CL=Sector, DH=Head, DL=Drive (only A: drive used 00 ), RETURN AH=Status, AL=Sector number read, CF=1 If error.
Function75  dd FddWriteB		   ; Floppy write sector, using bios driver, CH=Track/cylinder, CL=Sector, DH=Head, DL=Drive (only A: drive used 00 ), RETURN AH=Status, AL=Sector number read, CF=1 If error.
Function76  dd FS_ReadSector		   ; Reads a sector from hdd, EAX = Unit (drive number) 1 = primary device master, 2 = primary device slave and so on, EDI = Buffer to write the data to (must be 512bytes), EDX = Sector number to read from (LBA format), ECX = Count  Nunmber of sectors (just 1 for now)
Function77  dd FS_WriteSector		   ; Write a sector from hdd, EAX = Unit (drive number) 1 = primary device master, 2 = primary device slave and so on, EDI = Buffer to read the data from (must be 512bytes), EDX = Sector number to write to (LBA format), ECX = Count  Nunmber of sectors (just 1 for now)
Function78  dd DirFileList		   ; Fills a buffer with file names from root dir Entry: EDI = Buffer address ECX = Max number AX = Ext eg 'GE' for gex files or AX = 0 for all files Exit: EDI, ESI = BUFFER Address ECX = Number of file listed.
Function79  dd RunCommand		   ; Loads a file from default drive (eg: A: C: ..)Entry: EDI = File name EDX = Address to load file too. Exit:  CF=1 If error. or CF clear and EBX = Number of sectors read.
Function80  dd WriteFileFat		   ; Writes a file to default drive, EAX = File size in bytes, ESI = Name of file to write,EDI = Data to write, Carry set on error , no errors EBX = Number of sector written
Function81  dd Get_Boot_Drive		   ; Loads al, with 'C' or 'A' drive letter , bl = (0=A, 1=B, 2=C, etc).
Function82  dd print_time		   ; Prints time in text mode
Function83  dd print_date		   ; Prints date in text mode
Function84  dd AnyFatUpdate		   ; Call this to check if there as been any thing written to FAT, EAX = 1 if no update needed or EAX = 0 if needed
Function85  dd DeleteCommand		   ; Deletes a file from the default drive, ESI = Name of file to delete, AL = 1 if not to write to disk or AL = 0 to upade fat/root dir to disk.
Function86  dd ReMountFdd		   ; Remount the root dir, eg: if you change floppy or USB.
Function87  dd ReNameFile		   ; Rename's a file on the default drive. ESI = Old file name(zero ending string), EDI = New file name(zero ending string), Carry set on error, AX = Error code.
Function88  dd GetRmPalette		   ; Gets the standared realmode Palette, EDX = pointer to 20 byte Palette
Function89  dd EmptyFunction
Function90  dd EmptyFunction
Function91  dd EmptyFunction
Function92  dd EmptyFunction
Function93  dd EmptyFunction
Function94  dd EmptyFunction
Function95  dd EmptyFunction
Function96  dd EmptyFunction
Function97  dd EmptyFunction
Function98  dd EmptyFunction
Function99  dd EmptyFunction
Function100 dd EmptyFunction
Function101 dd EmptyFunction
Function102 dd EmptyFunction
Function103 dd EmptyFunction
Function104 dd EmptyFunction
Function105 dd EmptyFunction
Function106 dd EmptyFunction
Function107 dd EmptyFunction
Function108 dd EmptyFunction
Function109 dd EmptyFunction
Function110 dd EmptyFunction
Function111 dd EmptyFunction
Function112 dd EmptyFunction
Function113 dd EmptyFunction
Function114 dd EmptyFunction
Function115 dd EmptyFunction
Function116 dd EmptyFunction
Function117 dd EmptyFunction
Function118 dd EmptyFunction
Function119 dd EmptyFunction
Function120 dd EmptyFunction
Function121 dd EmptyFunction
Function122 dd EmptyFunction
Function123 dd EmptyFunction
Function124 dd EmptyFunction
Function125 dd EmptyFunction
Function126 dd EmptyFunction
Function127 dd EmptyFunction
Function128 dd EmptyFunction
Function129 dd EmptyFunction
Function130 dd EmptyFunction
Function131 dd EmptyFunction
Function132 dd EmptyFunction
Function133 dd EmptyFunction
Function134 dd EmptyFunction
Function135 dd EmptyFunction
Function136 dd EmptyFunction
Function137 dd EmptyFunction
Function138 dd EmptyFunction
Function139 dd EmptyFunction
Function140 dd EmptyFunction
Function141 dd EmptyFunction
Function142 dd EmptyFunction
Function143 dd EmptyFunction
Function144 dd EmptyFunction
Function145 dd EmptyFunction
Function146 dd EmptyFunction
Function147 dd EmptyFunction
Function148 dd EmptyFunction
Function149 dd EmptyFunction
Function150 dd EmptyFunction
Function151 dd EmptyFunction
Function152 dd EmptyFunction
Function153 dd EmptyFunction
Function154 dd EmptyFunction
Function155 dd EmptyFunction
Function156 dd EmptyFunction
Function157 dd EmptyFunction
Function158 dd EmptyFunction
Function159 dd EmptyFunction
Function160 dd EmptyFunction
Function161 dd EmptyFunction
Function162 dd EmptyFunction
Function163 dd EmptyFunction
Function164 dd EmptyFunction
Function165 dd EmptyFunction
Function166 dd EmptyFunction
Function167 dd EmptyFunction
Function168 dd EmptyFunction
Function169 dd EmptyFunction
Function170 dd EmptyFunction
Function171 dd EmptyFunction
Function172 dd EmptyFunction
Function173 dd EmptyFunction
Function174 dd EmptyFunction
Function175 dd EmptyFunction
Function176 dd EmptyFunction
Function177 dd EmptyFunction
Function178 dd EmptyFunction
Function179 dd EmptyFunction
Function180 dd EmptyFunction
Function181 dd EmptyFunction
Function182 dd EmptyFunction
Function183 dd EmptyFunction
Function184 dd EmptyFunction
Function185 dd EmptyFunction
Function186 dd EmptyFunction
Function187 dd EmptyFunction
Function188 dd EmptyFunction
Function189 dd EmptyFunction
Function190 dd EmptyFunction
Function191 dd EmptyFunction
Function192 dd EmptyFunction
Function193 dd EmptyFunction
Function194 dd EmptyFunction
Function195 dd EmptyFunction
Function196 dd EmptyFunction
Function197 dd EmptyFunction
Function198 dd EmptyFunction
Function199 dd EmptyFunction
Function200 dd EmptyFunction
Function201 dd EmptyFunction
Function202 dd EmptyFunction
Function203 dd EmptyFunction
Function204 dd EmptyFunction
Function205 dd EmptyFunction
Function206 dd EmptyFunction
Function207 dd EmptyFunction
Function208 dd EmptyFunction
Function209 dd EmptyFunction
Function210 dd EmptyFunction
Function211 dd EmptyFunction
Function212 dd EmptyFunction
Function213 dd EmptyFunction
Function214 dd EmptyFunction
Function215 dd EmptyFunction
Function216 dd EmptyFunction
Function217 dd EmptyFunction
Function218 dd EmptyFunction
Function219 dd EmptyFunction
Function220 dd EmptyFunction
Function221 dd EmptyFunction
Function222 dd EmptyFunction
Function223 dd EmptyFunction
Function224 dd EmptyFunction
Function225 dd EmptyFunction
Function226 dd EmptyFunction
Function227 dd EmptyFunction
Function228 dd EmptyFunction
Function229 dd EmptyFunction
Function230 dd EmptyFunction
Function231 dd EmptyFunction
Function232 dd EmptyFunction
Function233 dd EmptyFunction
Function234 dd EmptyFunction
Function235 dd EmptyFunction
Function236 dd EmptyFunction
Function237 dd EmptyFunction
Function238 dd EmptyFunction
Function239 dd EmptyFunction
Function240 dd EmptyFunction
Function241 dd EmptyFunction
Function242 dd EmptyFunction
Function243 dd EmptyFunction
Function244 dd EmptyFunction
Function245 dd EmptyFunction
Function246 dd EmptyFunction
Function247 dd EmptyFunction
Function248 dd EmptyFunction
Function249 dd EmptyFunction
Function250 dd EmptyFunction
Function251 dd EmptyFunction
Function252 dd EmptyFunction
Function253 dd EmptyFunction
Function254 dd EmptyFunction
Function255 dd EmptyFunction
KernelCallTable:
FunctionK0   dd KernelCallTable 	    ; Address of this call-table.
FunctionK1   dd GetModListAddress	    ; Gets the modlist address, Returns EDI = ModList Address.
FunctionK2   dd SetModListNumber	    ; Set mod list numbers, this is inc or dec, on loading or unloading mod, Enter: ECX = NEW NUMBER OF MODS, Return  set's CF to 1 on error (eg: more than 64 mods loaded,   (NOTE: Kernel function)
FunctionK3   dd UpDateTopOfMemory	    ; Set's the new top of ram, Entry: eax = New top of ram.
FunctionK4   dd EmptyFunction
FunctionK5   dd EmptyFunction
FunctionK6   dd EmptyFunction
FunctionK7   dd EmptyFunction
FunctionK8   dd EmptyFunction
FunctionK9   dd EmptyFunction
FunctionK10  dd EmptyFunction
FunctionK11  dd EmptyFunction
FunctionK12  dd EmptyFunction
FunctionK13  dd EmptyFunction
FunctionK14  dd EmptyFunction
FunctionK15  dd EmptyFunction
FunctionK16  dd EmptyFunction
FunctionK17  dd EmptyFunction
FunctionK18  dd EmptyFunction
FunctionK19  dd EmptyFunction
FunctionK20  dd EmptyFunction
FunctionK21  dd EmptyFunction
FunctionK22  dd EmptyFunction
FunctionK23  dd EmptyFunction
FunctionK24  dd EmptyFunction
FunctionK25  dd EmptyFunction
FunctionK26  dd EmptyFunction
FunctionK27  dd EmptyFunction
FunctionK28  dd EmptyFunction
FunctionK29  dd EmptyFunction
FunctionK30  dd EmptyFunction
FunctionK31  dd EmptyFunction
FunctionK32  dd EmptyFunction
FunctionK33  dd EmptyFunction
FunctionK34  dd EmptyFunction
FunctionK35  dd EmptyFunction
FunctionK36  dd EmptyFunction
FunctionK37  dd EmptyFunction
FunctionK38  dd EmptyFunction
FunctionK39  dd EmptyFunction
FunctionK40  dd EmptyFunction
FunctionK41  dd EmptyFunction
FunctionK42  dd EmptyFunction
FunctionK43  dd EmptyFunction
FunctionK44  dd EmptyFunction
FunctionK45  dd EmptyFunction
FunctionK46  dd EmptyFunction
FunctionK47  dd EmptyFunction
FunctionK48  dd EmptyFunction
FunctionK49  dd EmptyFunction
FunctionK50  dd EmptyFunction
FunctionK51  dd EmptyFunction
FunctionK52  dd EmptyFunction
FunctionK53  dd EmptyFunction
FunctionK54  dd EmptyFunction
FunctionK55  dd EmptyFunction
FunctionK56  dd EmptyFunction
FunctionK57  dd EmptyFunction
FunctionK58  dd EmptyFunction
FunctionK59  dd EmptyFunction
FunctionK60  dd EmptyFunction
FunctionK61  dd EmptyFunction
FunctionK62  dd EmptyFunction
FunctionK63  dd EmptyFunction
FunctionK64  dd EmptyFunction
FunctionK65  dd EmptyFunction
FunctionK66  dd EmptyFunction
FunctionK67  dd EmptyFunction
FunctionK68  dd EmptyFunction
FunctionK69  dd EmptyFunction
FunctionK70  dd EmptyFunction
FunctionK71  dd EmptyFunction
FunctionK72  dd EmptyFunction
FunctionK73  dd EmptyFunction
FunctionK74  dd EmptyFunction
FunctionK75  dd EmptyFunction
FunctionK76  dd EmptyFunction
FunctionK77  dd EmptyFunction
FunctionK78  dd EmptyFunction
FunctionK79  dd EmptyFunction
FunctionK80  dd EmptyFunction
FunctionK81  dd EmptyFunction
FunctionK82  dd EmptyFunction
FunctionK83  dd EmptyFunction
FunctionK84  dd EmptyFunction
FunctionK85  dd EmptyFunction
FunctionK86  dd EmptyFunction
FunctionK87  dd EmptyFunction
FunctionK88  dd EmptyFunction
FunctionK89  dd EmptyFunction
FunctionK90  dd EmptyFunction
FunctionK91  dd EmptyFunction
FunctionK92  dd EmptyFunction
FunctionK93  dd EmptyFunction
FunctionK94  dd EmptyFunction
FunctionK95  dd EmptyFunction
FunctionK96  dd EmptyFunction
FunctionK97  dd EmptyFunction
FunctionK98  dd EmptyFunction
FunctionK99  dd EmptyFunction
FunctionK100 dd EmptyFunction
FunctionK101 dd EmptyFunction
FunctionK102 dd EmptyFunction
FunctionK103 dd EmptyFunction
FunctionK104 dd EmptyFunction
FunctionK105 dd EmptyFunction
FunctionK106 dd EmptyFunction
FunctionK107 dd EmptyFunction
FunctionK108 dd EmptyFunction
FunctionK109 dd EmptyFunction
FunctionK110 dd EmptyFunction
FunctionK111 dd EmptyFunction
FunctionK112 dd EmptyFunction
FunctionK113 dd EmptyFunction
FunctionK114 dd EmptyFunction
FunctionK115 dd EmptyFunction
FunctionK116 dd EmptyFunction
FunctionK117 dd EmptyFunction
FunctionK118 dd EmptyFunction
FunctionK119 dd EmptyFunction
FunctionK120 dd EmptyFunction
FunctionK121 dd EmptyFunction
FunctionK122 dd EmptyFunction
FunctionK123 dd EmptyFunction
FunctionK124 dd EmptyFunction
FunctionK125 dd EmptyFunction
FunctionK126 dd EmptyFunction
FunctionK127 dd EmptyFunction
FunctionK128 dd EmptyFunction
FunctionK129 dd EmptyFunction
FunctionK130 dd EmptyFunction
FunctionK131 dd EmptyFunction
FunctionK132 dd EmptyFunction
FunctionK133 dd EmptyFunction
FunctionK134 dd EmptyFunction
FunctionK135 dd EmptyFunction
FunctionK136 dd EmptyFunction
FunctionK137 dd EmptyFunction
FunctionK138 dd EmptyFunction
FunctionK139 dd EmptyFunction
FunctionK140 dd EmptyFunction
FunctionK141 dd EmptyFunction
FunctionK142 dd EmptyFunction
FunctionK143 dd EmptyFunction
FunctionK144 dd EmptyFunction
FunctionK145 dd EmptyFunction
FunctionK146 dd EmptyFunction
FunctionK147 dd EmptyFunction
FunctionK148 dd EmptyFunction
FunctionK149 dd EmptyFunction
FunctionK150 dd EmptyFunction
FunctionK151 dd EmptyFunction
FunctionK152 dd EmptyFunction
FunctionK153 dd EmptyFunction
FunctionK154 dd EmptyFunction
FunctionK155 dd EmptyFunction
FunctionK156 dd EmptyFunction
FunctionK157 dd EmptyFunction
FunctionK158 dd EmptyFunction
FunctionK159 dd EmptyFunction
FunctionK160 dd EmptyFunction
FunctionK161 dd EmptyFunction
FunctionK162 dd EmptyFunction
FunctionK163 dd EmptyFunction
FunctionK164 dd EmptyFunction
FunctionK165 dd EmptyFunction
FunctionK166 dd EmptyFunction
FunctionK167 dd EmptyFunction
FunctionK168 dd EmptyFunction
FunctionK169 dd EmptyFunction
FunctionK170 dd EmptyFunction
FunctionK171 dd EmptyFunction
FunctionK172 dd EmptyFunction
FunctionK173 dd EmptyFunction
FunctionK174 dd EmptyFunction
FunctionK175 dd EmptyFunction
FunctionK176 dd EmptyFunction
FunctionK177 dd EmptyFunction
FunctionK178 dd EmptyFunction
FunctionK179 dd EmptyFunction
FunctionK180 dd EmptyFunction
FunctionK181 dd EmptyFunction
FunctionK182 dd EmptyFunction
FunctionK183 dd EmptyFunction
FunctionK184 dd EmptyFunction
FunctionK185 dd EmptyFunction
FunctionK186 dd EmptyFunction
FunctionK187 dd EmptyFunction
FunctionK188 dd EmptyFunction
FunctionK189 dd EmptyFunction
FunctionK190 dd EmptyFunction
FunctionK191 dd EmptyFunction
FunctionK192 dd EmptyFunction
FunctionK193 dd EmptyFunction
FunctionK194 dd EmptyFunction
FunctionK195 dd EmptyFunction
FunctionK196 dd EmptyFunction
FunctionK197 dd EmptyFunction
FunctionK198 dd EmptyFunction
FunctionK199 dd EmptyFunction
FunctionK200 dd EmptyFunction
FunctionK201 dd EmptyFunction
FunctionK202 dd EmptyFunction
FunctionK203 dd EmptyFunction
FunctionK204 dd EmptyFunction
FunctionK205 dd EmptyFunction
FunctionK206 dd EmptyFunction
FunctionK207 dd EmptyFunction
FunctionK208 dd EmptyFunction
FunctionK209 dd EmptyFunction
FunctionK210 dd EmptyFunction
FunctionK211 dd EmptyFunction
FunctionK212 dd EmptyFunction
FunctionK213 dd EmptyFunction
FunctionK214 dd EmptyFunction
FunctionK215 dd EmptyFunction
FunctionK216 dd EmptyFunction
FunctionK217 dd EmptyFunction
FunctionK218 dd EmptyFunction
FunctionK219 dd EmptyFunction
FunctionK220 dd EmptyFunction
FunctionK221 dd EmptyFunction
FunctionK222 dd EmptyFunction
FunctionK223 dd EmptyFunction
FunctionK224 dd EmptyFunction
FunctionK225 dd EmptyFunction
FunctionK226 dd EmptyFunction
FunctionK227 dd EmptyFunction
FunctionK228 dd EmptyFunction
FunctionK229 dd EmptyFunction
FunctionK230 dd EmptyFunction
FunctionK231 dd EmptyFunction
FunctionK232 dd EmptyFunction
FunctionK233 dd EmptyFunction
FunctionK234 dd EmptyFunction
FunctionK235 dd EmptyFunction
FunctionK236 dd EmptyFunction
FunctionK237 dd EmptyFunction
FunctionK238 dd EmptyFunction
FunctionK239 dd EmptyFunction
FunctionK240 dd EmptyFunction
FunctionK241 dd EmptyFunction
FunctionK242 dd EmptyFunction
FunctionK243 dd EmptyFunction
FunctionK244 dd EmptyFunction
FunctionK245 dd EmptyFunction
FunctionK246 dd EmptyFunction
FunctionK247 dd EmptyFunction
FunctionK248 dd EmptyFunction
FunctionK249 dd EmptyFunction
FunctionK250 dd EmptyFunction
FunctionK251 dd EmptyFunction
FunctionK252 dd EmptyFunction
FunctionK253 dd EmptyFunction
FunctionK254 dd EmptyFunction
FunctionK255 dd EmptyFunction


