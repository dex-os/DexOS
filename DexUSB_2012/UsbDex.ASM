;Dex USB Boot
format PE GUI 4.0
entry start 
;------- FO ---------
FO_MOVE 		  = 0x0001
FO_COPY 		  = 0x0002
FO_DELETE		  = 0x0003
FO_RENAME		  = 0x0004
;------ FOF ---------
FOF_MULTIDESTFILES	  = 0x0001
FOF_CONFIRMMOUSE	  = 0x0002
FOF_SILENT		  = 0x0004    ; don't create progress/report
FOF_RENAMEONCOLLISION	  = 0x0008
FOF_NOCONFIRMATION	  = 0x0010    ; Don't prompt the user.
FOF_WANTMAPPINGHANDLE	  = 0x0020    ; Fill in SHFILEOPSTRUCT.hNameMappings (Must be freed using SHFreeNameMappings)
FOF_ALLOWUNDO		  = 0x0040
FOF_FILESONLY		  = 0x0080    ; on *.*, do only files
FOF_SIMPLEPROGRESS	  = 0x0100    ; means don't show names of files
FOF_NOCONFIRMMKDIR	  = 0x0200    ; don't confirm making any needed dirs
FOF_NOERRORUI		  = 0x0400    ; don't put up error UI
FOF_NOCOPYSECURITYATTRIBS = 0x0800    ; dont copy NT file Security Attributes
FOF_NORECURSION 	  = 0x1000    ; don't recurse into directories.
FOF_NO_CONNECTED_ELEMENTS = 0x2000    ; don't operate on connected elements.
FOF_WANTNUKEWARNING	  = 0x4000    ; during delete operation, warn if nuking instead of recycling (partially overrides FOF_NOCONFIRMATION)
FOF_NORECURSEREPARSE	  = 0x8000    ; treat reparse points as objects, not containers
;------Drive types ---
UNKNOWN 		  = 0	      ; the drive type is unknown
DOES_NOT_EXIST		  = 1	      ; the drive does not exist
DRIVE_REMOVABLE 	  = 2	      ; floppy
DRIVE_FIXED		  = 3	      ; harddrives
DRIVE_REMOTE		  = 4					 ; USB?
DRIVE_CDROM		  = 5					  ; cdrom units
DRIVE_RAMDISK		  = 6					; ramdisks
include '%include%\win32a.inc'

section '.data' data readable writeable
 ;----------------------------------------------------;
 ; STORAGE_DEVICE_NUMBER  Vars                        ;
 ;----------------------------------------------------;
STORAGE_DEVICE_NUMBER:
DeviceType	dd ?
DeviceNumber	dd ?
PartitionNumber dd ?

DrivePath1	db "\\.\"
DrivePath1a	db 0,':',0
hDrive		dd ?
cbReturned	dd ?
sdn		=  STORAGE_DEVICE_NUMBER
caption 	=  MsgBoxCaption
 ;----------------------------------------------------;
 ; Strings & data                                     ;
 ;----------------------------------------------------;
MsgBoxCaption  db 'DexOS USB Installer',0

ErrorMes       db 'Error!, No usb stick found.',13
	       db 'Or it maybe in uses, by    ',13
	       db 'another program.           ',0

Mes	       db 'Can not be opened',0
Mes1	       db 'No data',0
i	       dd 0
hFile	       dd 0
BytesRead      dd 0
DriveCount     db 0
BytesWritten   dd 0
BootScBuff     db 1024*2 dup(?)
IoBuffer       db 512 dup(?)
BUFFERend      db  0
Buffer.size    equ ($ - Buffer)
NULL	       equ 0
DrivePath      db  '\\.\PHYSICALDRIVE'
DriveNumber:   db 0,0
IndexCount     db 0
DriveLetter    db 0
drive2	       dd 0
drive	       db '?:\',0
USBDrive       db 'Use this removable drive ?:\',0
USBDriveSize:
USBDrive1      db '                            ',0
USBDrive2      db '                            ',0
USBDrive3      db '                            ',0
USBDrive4      db '                            ',0
USBDriveMes    db 'No more removable drives found  ',0

MesHdd	       db 'Do you want to use hdd emulation',0
MesFdd	       db 'Do you want to use fdd emulation',0

MesWarn        db 'Are you SURE, as it will over   ',13
	       db 'WRITE any data on the removable ',13
	       db 'drive.                          ',0

YesHdd	       db 'Use Hdd emulation               ',0
Yesfdd	       db 'Use fdd emulation               ',0

ExitMes        db 'Thankyou for using DexOS, Goto  ',13
	       db 'www.dex-os.com for more info     ',0

Fat16FddErrorMes  db 'Error!, For safety  the removable  ',13
	       db 'drive needs to be formated FAT16.',13
	       db 'Please right click on drive and  ',13
	       db 'format it as FAT. Than try again.',0

Fat16ErrorMes db 'Error!, For safety, the removable  ',13
	       db 'drive needs to be formated FAT16.  ',13
	       db 'Please read the ReadMe file for the',13
	       db 'best tool to use, as XP does not   ',13
	       db 'format it right,Then try again.    ',0

MoveFileMess   db 'Do you want to copy the DexOS   ',13
	       db 'programs from DexHdd_programs   ',13
	       db 'folder,to removable drive '
newdir	       db "?:\",0,0

olddir	       db "DexHdd_programs\*.*",0,0



struct SHFILEOPSTRUCT
  hwnd	       dd ?
  wFunc        dd ?
  pFrom        dd ?
  pTo	       dd ?
  fFlags       dd ?
  fAnyOperationsAborted dd ? 
  hNameMappings 	dd ?
  lpszProgressTitle	dd ?
ends

sfo	       SHFILEOPSTRUCT
sfoabort       dd 0
letter	       dd ?				   ; to hold the current drive letter

image:
file 'FddHddImages\DexOS.ima'
rb  1024
HddImage:
file 'FddHddImages\DexHdd.bin'
rb  1024

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section '.code' code readable executable 
start:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ;----------------------------------------------------;
 ; Start                                              ;
 ;----------------------------------------------------;
	mov   [IndexCount],0
	mov   [DriveCount],0
	mov   [letter],'D'
 ;----------------------------------------------------;
 ; loop through letters                               ;
 ;----------------------------------------------------;
theLoop:
	inc   [DriveCount]
	mov   ecx,[letter]			    ; Craig this is use for drive number.
	mov   [drive], cl			    ; add current letter to drive name (example: 'A:\')
	call  get				    ; get info for current drive letter
	cmp   eax,IDYES
	je    WhatEmulationShouldWeMes1
	inc   [letter]				    ; next letter
	cmp   [letter],'Z'			    ; compare the current letter to 'Z'...
	jg    ExitError 			    ;... and jump if current letter is greater than 'Z'
	jmp   theLoop
 ;----------------------------------------------------;
 ; No more drives found mesage                        ;
 ;----------------------------------------------------;
ExitError:
	invoke MessageBox,HWND_DESKTOP,USBDriveMes,caption,MB_ICONASTERISK+MB_OK
 ;----------------------------------------------------;
 ; By  mesage                                         ;
 ;----------------------------------------------------;
exit:
	invoke MessageBox,HWND_DESKTOP,ExitMes,caption,MB_ICONASTERISK+MB_OK
	invoke ExitProcess,0
	ret
WhatEmulationShouldWeMes1:
 ;----------------------------------------------------;
 ; Ask if hdd emulation                               ;
 ;----------------------------------------------------;
	invoke MessageBox,HWND_DESKTOP,MesHdd,caption,MB_ICONQUESTION+MB_YESNO
	cmp eax,IDYES
	jne  WhatEmulationMes2
 ;----------------------------------------------------;
 ; Hdd emulation                                      ;
 ;----------------------------------------------------;
 ; Best Give them a warning                           ;
 ;----------------------------------------------------;
YesItsHddEmulation:
	invoke MessageBox,HWND_DESKTOP,MesWarn,caption,MB_YESNO + MB_ICONWARNING
	cmp eax,IDYES
	jne exit				    ; If they do not want  to continue, exit
 ;----------------------------------------------------;
 ; Write to stick  (Hdd emulation)                    ;
 ;----------------------------------------------------;
	call WriteBootSectorHdd 		    ; write as hdd emulation.
	jmp exit				    ; Finished so exit
 ;----------------------------------------------------;
 ; Ask if fdd emulation                               ;
 ;----------------------------------------------------;
WhatEmulationMes2:
	invoke MessageBox,HWND_DESKTOP,MesFdd,caption,MB_ICONQUESTION+MB_YESNO
	cmp eax,IDYES
	jne exit
 ;----------------------------------------------------;
 ; Fdd emulation                                      ;
 ;----------------------------------------------------;
 ; Best Give them a warning                           ;
 ;----------------------------------------------------;
UseFdd:
	invoke MessageBox,HWND_DESKTOP,MesWarn,caption,MB_YESNO + MB_ICONWARNING
	cmp eax,IDYES
	jne exit				    ; If they do not want  to continue, exit
 ;----------------------------------------------------;
 ; Write to stick  (Fdd emulation)                    ;
 ;----------------------------------------------------;
	call WriteBootSectorFdd
	jmp  exit				    ; Finished so exit
 ;----------------------------------------------------;
 ; GetDriveType                                       ;
 ;----------------------------------------------------;
get: 
	invoke GetDriveType,drive
	cmp eax, DRIVE_REMOVABLE		    ; Test for REMOVABLE_DRIVE
	jz foundONE
ret						    ; If not try another drive
 ;----------------------------------------------------;
 ; Found  Removeable drive                            ;
 ;----------------------------------------------------;
foundONE:
	cmp [IndexCount],0
	jne Next1
	mov esi,USBDrive
	mov edi,USBDrive1
	mov [DriveLetter],cl
	mov ecx,USBDriveSize-USBDrive
	rep movsb
	mov edi,USBDrive1
	mov [drive2],edi
	mov eax,[letter]
	add edi,USBDriveSize-USBDrive-4
	stosb
	jmp PrintItOut
 ;----------------------------------------------------;
 ; Found  Removeable drive2                           ;
 ;----------------------------------------------------;
Next1:
	cmp [IndexCount],1
	jne Next2
	mov esi,USBDrive
	mov edi,USBDrive2
	mov [DriveLetter],cl
	mov ecx,USBDriveSize-USBDrive
	rep movsb
	mov edi,USBDrive2
	mov [drive2],edi
	mov eax,[letter]
	add edi,USBDriveSize-USBDrive-4
	stosb
	jmp PrintItOut
 ;----------------------------------------------------;
 ; Found  Removeable drive3                           ;
 ;----------------------------------------------------;
Next2:
	cmp [IndexCount],2
	jne Next3
	mov esi,USBDrive
	mov edi,USBDrive3
	mov [DriveLetter],cl
	mov ecx,USBDriveSize-USBDrive
	rep movsb
	mov edi,USBDrive3
	mov [drive2],edi
	mov eax,[letter]
	add edi,USBDriveSize-USBDrive-4
	stosb
	jmp PrintItOut
 ;----------------------------------------------------;
 ; Found  Removeable drive4                           ;
 ;----------------------------------------------------;
Next3:
	cmp [IndexCount],3
	jne PrintItOutExit
	mov esi,USBDrive
	mov edi,USBDrive4
	mov [DriveLetter],cl
	mov ecx,USBDriveSize-USBDrive
	rep movsb
	mov edi,USBDrive4
	mov [drive2],edi
	mov eax,[letter]
	add edi,USBDriveSize-USBDrive-4
	stosb
	jmp PrintItOut
 ;----------------------------------------------------;
 ; PrintItOut                                         ;
 ;----------------------------------------------------;
PrintItOut:
	inc [IndexCount]
	invoke MessageBox,HWND_DESKTOP,[drive2],caption,MB_ICONQUESTION+MB_YESNO
	PrintItOutExit:
	ret

 ;*******************************************************************************************;
 ; WriteBootSectorHdd                                                                        ;
 ;*******************************************************************************************;
WriteBootSectorHdd:
 ;----------------------------------------------------;
 ; GET HANDLE TO DISK TO COPY FROM                    ;
 ;----------------------------------------------------;
	mov   al,[drive]
	mov   byte[DrivePath1a],al
	;;;
	mov   byte[newdir],al
	;;;
	invoke CreateFile, DrivePath1, 0,FILE_SHARE_READ + FILE_SHARE_WRITE, 0, 3, 0, 0
	mov   [hFile],EAX
 ;----------------------------------------------------;
 ; Fill the STORAGE_DEVICE_NUMBER var out             ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 02D1080h , 0, 0,STORAGE_DEVICE_NUMBER,3*4,cbReturned, 0
	invoke CloseHandle, [hFile]
 ;----------------------------------------------------;
 ; We need to check, right drive                      ;
 ;----------------------------------------------------;
	cmp  dword[DeviceType],7
	jne  ReadErrorExit
 ;----------------------------------------------------;
 ; GET HANDLE TO DISK TO COPY FROM                    ;
 ;----------------------------------------------------;
 ;----------------------------------------------------;
 ; OPEN_EXISTING = 3, GENERIC_READ = 0x80000000,      ;
 ; GENERIC_WRITE = 0x20000000, FILE_SHARE_READ = 1,   ;
 ; FILE_SHARE_READ = 1, FILE_SHARE_WRITE = 2          ;
 ;----------------------------------------------------;
	mov   eax,[DeviceNumber]
 ;----------------------------------------------------;
 ; ***** WE NEED TO CHECK IT NOT THE C:\ DRIVE *****  ;
 ;----------------------------------------------------;
	cmp   eax,0
	je    ReadErrorExit			  ; If people have problem with read error here could be the problem
	add   al,48
	mov   byte[DriveNumber],al
	invoke CreateFile, DrivePath, 80000000h or 40000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; ALLOCATE  PAGE_READWRITE = 4, MEM_COMMIT = 1000h   ;
 ;----------------------------------------------------;
	invoke VirtualAlloc, IoBuffer, 512, 1000h, 4
 ;----------------------------------------------------;
 ; READ DISK                                          ;
 ;----------------------------------------------------;
; mov  [BytesRead],0
	invoke ReadFile, [hFile], IoBuffer, 512, BytesRead, 0
 ;----------------------------------------------------;
 ; FREE                                               ;
 ;----------------------------------------------------;
	invoke VirtualFree, IoBuffer, 512, 4000h
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit
 ;----------------------------------------------------;
 ; Test for FAT16.                                    ;
 ;----------------------------------------------------;
	mov   esi,IoBuffer
	add   esi,450
	mov   al,byte[ds:esi]	
	cmp   al,0x04					; Fat16
	je    YesFat16
	cmp   al,0x06					; Fat16
	je    YesFat16
	cmp   al,0x0E					; Fat16
	je    YesFat16
NoFat16Error:
 ;----------------------------------------------------;
 ; NoFat16Error                                       ;
 ;----------------------------------------------------;
	 invoke  MessageBox,0,Fat16ErrorMes,MsgBoxCaption,0
	 ;invoke  CloseHandle,[hFile]
	 ;invoke ExitProcess,0
	 ret


 ;----------------------------------------------------;
 ; YesFat16                                           ;
 ;----------------------------------------------------;
YesFat16:
 ;----------------------------------------------------;
 ; Find first partion.                                ;
 ;----------------------------------------------------;        
	mov   esi,IoBuffer
	add   esi,446
	mov   al,byte[ds:esi]	
	cmp   al,80h
	je    PartionBootableOK
 ;----------------------------------------------------;
 ; PartionNotBootable                                 ;
 ;----------------------------------------------------;
PartionNotBootable:
	mov   al,80h
	mov   edi,IoBuffer
	add   edi,446
	stosb

;;;;;;;
	mov   eax,[DeviceNumber]
 ;----------------------------------------------------;
 ; ***** WE NEED TO CHECK IT NOT THE C:\ DRIVE *****  ;
 ;----------------------------------------------------;
	cmp   eax,0
	je    ReadErrorExit			  ; If people have problem with read error here could be the problem
;;;;;;;
 ;----------------------------------------------------;
 ; GET HANDLE TO DISK TO WRITE TO                     ;
 ;----------------------------------------------------;
	invoke CreateFile, DrivePath, 20000000h or 10000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; WRITE DISK                                         ;
 ;----------------------------------------------------;
      invoke WriteFile, [hFile], IoBuffer, 512, BytesRead, 0
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit
 ;----------------------------------------------------;
 ; PartionBootableOK                                  ;
 ;----------------------------------------------------;
PartionBootableOK:
	invoke CreateFile, DrivePath1, 80000000h or 40000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; ALLOCATE  PAGE_READWRITE = 4, MEM_COMMIT = 1000h   ;
 ;----------------------------------------------------;
	invoke VirtualAlloc, IoBuffer, 512, 1000h, 4
 ;----------------------------------------------------;
 ; READ DISK                                          ;
 ;----------------------------------------------------;
	invoke ReadFile, [hFile], IoBuffer, 512, BytesRead, 0
 ;----------------------------------------------------;
 ; FREE                                               ;
 ;----------------------------------------------------;
	invoke VirtualFree, IoBuffer, 512, 4000h
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit
 ;----------------------------------------------------;
 ; Merge original boot sector with Dexhdd sector      ;
 ;----------------------------------------------------;
	mov   esi,HddImage
	mov   edi,IoBuffer
	add   edi,0x3e
	mov   ecx,450
	rep   movsb

	mov   al,byte[DrivePath1a]
	cmp   al,'C'
	je    ReadErrorExit
 ;----------------------------------------------------;
 ; GET HANDLE TO DISK TO WRITE TO                     ;
 ;----------------------------------------------------;
	invoke CreateFile, DrivePath1, 20000000h or 10000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; WRITE DISK                                         ;
 ;----------------------------------------------------;
      invoke WriteFile, [hFile], IoBuffer, 512, BytesRead, 0
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ;----------------------------------------------------;
 ; MoveFileYesNo                                      ;
 ;----------------------------------------------------;
MoveFileYesNo:
	invoke MessageBox,HWND_DESKTOP,MoveFileMess,caption,MB_ICONQUESTION+MB_YESNO
	cmp   eax,IDYES
	jne   ExitLoop
	call  CopyFolder
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	jmp   ExitLoop

 ;*******************************************************************************************;
 ; WriteBootSectorFdd                                                                        ;
 ;*******************************************************************************************;
WriteBootSectorFdd:
 ;----------------------------------------------------;
 ; GET HANDLE TO DISK TO COPY FROM                    ;
 ;----------------------------------------------------;
	mov   al,[drive]
	mov   byte[DrivePath1a],al
	invoke CreateFile, DrivePath1, 0,FILE_SHARE_READ + FILE_SHARE_WRITE, 0, 3, 0, 0
	mov   [hFile],EAX
 ;----------------------------------------------------;
 ; Fill the STORAGE_DEVICE_NUMBER var out             ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 02D1080h , 0, 0,STORAGE_DEVICE_NUMBER,3*4,cbReturned, 0
	invoke CloseHandle, [hFile]
 ;----------------------------------------------------;
 ; We need to check, right drive                      ;
 ;----------------------------------------------------;
	cmp  dword[DeviceType],7
	jne  ReadErrorExit
 ;****************************************************;
 ; GET HANDLE TO DISK TO WRITE TO                     ;
 ;****************************************************;
 ;----------------------------------------------------;
 ; OPEN_EXISTING = 3, GENERIC_READ = 0x80000000,      ;
 ; GENERIC_WRITE = 0x20000000, FILE_SHARE_READ = 1,   ;
 ; FILE_SHARE_READ = 1, FILE_SHARE_WRITE = 2          ;
 ;----------------------------------------------------;
	mov   eax, [DeviceNumber]
 ;----------------------------------------------------;
 ; ***** WE NEED TO CHECK IT NOT THE C:\ DRIVE *****  ;
 ;----------------------------------------------------;
	cmp   eax,0
	je    ReadErrorExit			  ; If people have problem with read error here could be the problem
	add   al,48
	mov   byte[DriveNumber],al

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	invoke CreateFile, DrivePath, 80000000h or 40000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; ALLOCATE  PAGE_READWRITE = 4, MEM_COMMIT = 1000h   ;
 ;----------------------------------------------------;
	invoke VirtualAlloc, IoBuffer, 512, 1000h, 4
 ;----------------------------------------------------;
 ; READ DISK                                          ;
 ;----------------------------------------------------;
; mov  [BytesRead],0
	invoke ReadFile, [hFile], IoBuffer, 512, BytesRead, 0
 ;----------------------------------------------------;
 ; FREE                                               ;
 ;----------------------------------------------------;
	invoke VirtualFree, IoBuffer, 512, 4000h
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit
 ;----------------------------------------------------;
 ; Test for FAT16.                                    ;
 ;----------------------------------------------------;
	mov   esi,IoBuffer
	add   esi,450
	mov   al,byte[ds:esi]	
	cmp   al,0x04					; Fat16
	je    YesFat16Fdd
	cmp   al,0x06					; Fat16
	je    YesFat16Fdd
	cmp   al,0x0E					; Fat16
	je    YesFat16Fdd
;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,IoBuffer
	add   esi,55
	mov   eax,dword[ds:esi]
	cmp   eax,'AT16'
	je    YesFat16Fdd
NoFat16ErrorFdd:
 ;----------------------------------------------------;
 ; NoFat16Error                                       ;
 ;----------------------------------------------------;
	 invoke  MessageBox,0,Fat16FddErrorMes,MsgBoxCaption,0
	 ;invoke  CloseHandle,[hFile]
	 ;invoke ExitProcess,0
	 ret
 ;----------------------------------------------------;
 ; YesFat16                                           ;
 ;----------------------------------------------------;
YesFat16Fdd:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	invoke CreateFile, DrivePath, 20000000h or 10000000h, 1 or 2, 0, 3, 0, 0
	mov   [hFile], EAX
 ;----------------------------------------------------;
 ; LOCK DISK              FSCTL_LOCK_VOLUME = 589848  ;
 ;----------------------------------------------------;
	invoke DeviceIoControl, [hFile], 589848, 0, 0, 0, 0, BytesRead, 0
 ;----------------------------------------------------;
 ; ALLOCATE  PAGE_READWRITE = 4, MEM_COMMIT = 1000h   ;
 ;----------------------------------------------------;
       ; invoke VirtualAlloc, IoBuffer, 512, 1000h, 4
 ;----------------------------------------------------;
 ; WRITE DISK                                         ;
 ;----------------------------------------------------;
      invoke WriteFile, [hFile], image, 512*2880, BytesRead, 0
 ;----------------------------------------------------;
 ; FREE                                               ;
 ;----------------------------------------------------;
      ;  invoke VirtualFree, IoBuffer, 512, 4000h
 ;----------------------------------------------------;
 ; CLOSE HANDLE                                       ;
 ;----------------------------------------------------;
	invoke CloseHandle, [hFile]
	cmp   [BytesRead],0
	je    ReadErrorExit
ExitLoop:
 ;----------------------------------------------------;
 ; Exit back to caller                                ;
 ;----------------------------------------------------;
	 ret

ReadErrorExit:
 ;----------------------------------------------------;
 ; Print read error message and Exit back to caller   ;
 ;----------------------------------------------------;
	 invoke  MessageBox,0,ErrorMes,MsgBoxCaption,0
	 ret

 ;----------------------------------------------------;
 ; write hex                                          ;
 ;----------------------------------------------------;
write_hex8:
	push  eax
	push  eax
	shr   al,4					   
	call  hexget					  
	call  Store2Buffer
	pop   eax
	call  hexget					   
	call  Store2Buffer
	pop   eax
	ret

 ;----------------------------------------------------;
 ; hexget         convers from ASCII to hexadecimal.  ;
 ;----------------------------------------------------;
hexget:
	and   eax,0x0000000f
	or    eax,0x00000030
	cmp   eax,0x39
	ja    add7
	ret
add7:	add   eax,7		
	ret

 ;----------------------------------------------------;
 ; Store2Buffer                                       ;
 ;----------------------------------------------------;
Store2Buffer:
	stosb
	ret

 proc CopyFolder
	mov eax,0
	mov [sfo.hwnd],eax
	mov [sfo.wFunc],FO_COPY
	mov eax,olddir
	mov [sfo.pFrom],eax
	mov eax,newdir
	mov [sfo.pTo],eax
	mov [sfo.fFlags],FOF_NOERRORUI ;FOF_FILESONLY
	lea eax,[sfoabort]
	mov [sfo.fAnyOperationsAborted],eax
	mov [sfo.hNameMappings],NULL
	mov [sfo.lpszProgressTitle],NULL
	invoke SHFileOperation,sfo
	ret
endp

section '.idata' import data readable writeable 
  library kernel32,'KERNEL32.DLL',user32,'USER32.DLL',shell32,'SHELL32.DLL'
  include '%include%\API\kernel32.inc'
  include '%include%\API\user32.inc'
  include '%include%\API\shell32.inc'

section '.rsrc' resource data readable
  directory RT_ICON,icons,RT_GROUP_ICON,group_icons
  resource icons,1,LANG_NEUTRAL,icon_data
  resource group_icons,1,LANG_NEUTRAL,icon
  icon icon,icon_data,'res\icon1.ico'
