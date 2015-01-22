format PE GUI 4.0
entry start

include '%include%\win32a.inc'

section '.data' data readable writeable
  _title	 db 'Dex4usb',0
  _class	 db 'FASMWIN32',0
  _class_cb	 db 'COMBOBOX',0
  mainhwnd	 dd ?
  hinstance	 dd ?
  hwnd_cb	 dd ?
  msg MSG
  wc WNDCLASS

;========== BUTTON ===========;
  lpStop db "Install DexOS on usb fob",0
  ButtonClassName db "BUTTON",0

;========= BMP file ==========;
  bmp1H 	dd ?						   ; handle for BMP file
  bmp1Ps	PAINTSTRUCT
  bmp1Rect	RECT
  dcH		dd ?
  dcMemH	dd ?
  insH		dd ?						   ; handle for instance

;========== CHECKBOX ===========;
  textg  db 'Emulate As',0
  textc  db 'Harddrive',0
  textD  db 'Device',0
  textc1 db 'Floppy',0
  classb db 'BUTTON',0
  classS db 'STATIC',0
  hwndc  dd 0
  hwndc1 dd 0
  hwndc2 dd 0
;=========== FONTS ============;
  hFont 	 dd 0
  font_name	 db "Courier",0
;========= COMBO LIST ==========;
  USBDriveNO	 db ' No Removable Drives Found',0
  DriveLetter	 db 0
  drive2	 dd 0
  drive 	 db '?:\',0					   ; drive name "template"
  USBDrive	 db ' Use this USB Drive ?:\',0
  USBDriveSize:
  USBDrive1	 db '                       ',0
  USBDrive2	 db '                       ',0
  USBDrive3	 db '                       ',0
  USBDrive4	 db '                       ',0
  caption	 db 'Removable Drives',0			   ; rb 40h
  IndexCount	 db 0
  letter	 dd  ?						   ; to hold the current drive letter
;========= DRIVE TYPES ==========;
  UNKNOWN = 0							   ; the drive type is unknown
  DOES_NOT_EXIST = 1						   ; the drive does not exist
  DRIVE_REMOVABLE = 2						   ; floppy
  DRIVE_FIXED = 3						   ; harddrives
  DRIVE_REMOTE = 4						   ; USB?
  DRIVE_CDROM = 5						   ; cdrom units
  DRIVE_RAMDISK = 6						   ; ramdisks

section '.code' code readable executable

;======== GET USB DRIVE ========;
  start:
	mov [IndexCount],0
	mov [letter],'D'
  theLoop:							   ; loop through letters
	mov ecx,[letter]
	mov [drive],cl						   ; add current letter to drive name (example: 'A:\')
	call Get						   ; get info for current drive letter
	inc [letter]						   ; next letter
	cmp [letter],'Z'					   ; compare the current letter to 'Z'...
	jg ComboStart						   ; ... and jump if current letter is greater than 'Z'
	jmp theLoop						   ; from the top

  Get:
	push ebx esi edi
	invoke GetDriveType,drive
	cmp eax, DRIVE_REMOVABLE				    ; is it a Removable drive ?
	jnz GetExit						    ; jump if the result from GetDriveType equals DRIVE_FIXED
	call foundONE
  GetExit:
	pop edi esi ebx
	ret

;====== FOUND USB DRIVE ======;
  foundONE:
	push ebx esi edi
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
	add edi,20
	stosb
	jmp PrintItOut
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
	add edi,20
	stosb
	jmp PrintItOut
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
	add edi,20
	stosb
	jmp PrintItOut
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
	add edi,20
	stosb
	jmp PrintItOut
PrintItOut:
	inc [IndexCount]
	;invoke MessageBox,HWND_DESKTOP,[drive2],caption,MB_ICONQUESTION+MB_YESNO  ;MB_YESNO  ;0 ;display drive name
PrintItOutExit:
	 pop edi esi ebx
	 ret

;======== CREATE WINDOW ========;
  ComboStart:
	invoke GetModuleHandle,0
	mov [hinstance],eax

	invoke	LoadIcon,eax,17
	mov	[wc.hIcon],eax
      ;  invoke LoadIcon,0,IDI_APPLICATION
      ;  mov [wc.hIcon],eax
	invoke LoadCursor,0,IDC_ARROW
	mov [wc.hCursor],eax
	mov [wc.style],CS_HREDRAW + CS_VREDRAW	;0
	mov [wc.lpfnWndProc],WindowProc
	mov [wc.cbClsExtra],0
	mov [wc.cbWndExtra],0
	mov eax,[hinstance]
	mov [wc.hInstance],eax
	mov [wc.hbrBackground],COLOR_BTNFACE+1
	mov [wc.lpszMenuName],0
	mov [wc.lpszClassName],_class
	invoke RegisterClass,wc

;======== CREATE FONTS ========;
	invoke CreateFont,15,0,0,0,0,0,0,0,204,0,0,0,0,font_name
	mov [hFont],eax

;======== CREATE WINDOW ========;
	invoke CreateWindowEx,0,_class,_title,WS_VISIBLE+WS_DLGFRAME+WS_SYSMENU ,128,128,265,308,NULL,NULL,[hinstance],NULL
	mov [mainhwnd],eax

;========== MAIN LOOP ==========;
  msg_loop:
	invoke GetMessage,msg,NULL,0,0
	or eax,eax
	jz end_loop
	invoke TranslateMessage,msg
	invoke DispatchMessage,msg
	jmp msg_loop
  end_loop:
	invoke DeleteObject,[hFont]
	invoke ExitProcess,[msg.wParam]

;========= WINDOWPROC =========;
proc WindowProc, hwnd,wmsg,wparam,lparam
	push ebx esi edi
	cmp [wmsg],WM_CREATE
	je wmcreate
	cmp [wmsg],WM_COMMAND
	je wmcommand
	cmp  [wmsg],WM_PAINT
	je   wmPAINT
	cmp [wmsg],WM_DESTROY
	je wmdestroy
  defwndproc:
	invoke DefWindowProc,[hwnd],[wmsg],[wparam],[lparam]
	jmp finish

;========= wmPAINT =========;
  wmPAINT:
	invoke	BeginPaint,[hwnd],bmp1Ps
	mov  [dcH],eax
	invoke	CreateCompatibleDC,[dcH]
	mov  [dcMemH],eax
	invoke	SelectObject,[dcMemH],[bmp1H]
	invoke	GetClientRect,[hwnd],bmp1Rect
	invoke	BitBlt,[dcH],19,14,[bmp1Rect.right],[bmp1Rect.bottom],[dcMemH],0,0,SRCCOPY
	invoke	DeleteDC,[dcMemH]
	invoke	EndPaint,[hwnd],bmp1Ps
	jmp finish
;=========  wmcreate =========;
  wmcreate:
	invoke	LoadBitmap,[hinstance],300
	mov  [bmp1H],eax

;========== CHECKBOX ===========;
;========== ADD STRING =========;
	invoke CreateWindowEx,0,classS,textD,WS_CHILD + WS_VISIBLE,16,12+90,150,20,[hwnd],1000,[wc.hInstance],0
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	invoke CreateWindowEx,0,classb,textg,WS_VISIBLE+WS_CHILD+BS_GROUPBOX,16,66+90,226,70,[hwnd],1001,[wc.hInstance],0
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	invoke CreateWindowEx,0,classb,textc,WS_VISIBLE+WS_CHILD+BS_AUTOCHECKBOX+WS_TABSTOP,28,88+90,150,20,[hwnd],1002,[wc.hInstance],0
	mov [hwndc],eax
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	invoke CreateWindowEx,0,classb,textc1,WS_VISIBLE+WS_CHILD+BS_AUTOCHECKBOX+WS_TABSTOP,28,110+90,150,20,[hwnd],1003,[wc.hInstance],0
	mov [hwndc1],eax
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	invoke CreateWindowEx,NULL,ButtonClassName,lpStop,\WS_CHILD or WS_VISIBLE or BS_DEFPUSHBUTTON,\30,150+90,200,20,[hwnd],1003,[wc.hInstance],0
	;invoke CreateWindowEx,0,classb,textc1,WS_VISIBLE+WS_CHILD+BS_AUTOCHECKBOX+WS_TABSTOP,28,110+90,150,20,[hwnd],1003,[wc.hInstance],0
	mov [hwndc2],eax
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	;invoke  SetFocus,[hwndc]
;========= ADD DRIVES ==========;
	stdcall create_control,hwnd_cb,_class_cb,0,WS_CHILD+WS_VISIBLE+CBS_DROPDOWNLIST+CBS_SORT,[hwnd],14,32+90,230,100,1005
	cmp [IndexCount],0
	ja NextMessage1
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDriveNO
	jmp Selection
  NextMessage1:
	cmp [IndexCount],1
	ja NextMessage2
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive1
	jmp Selection
  NextMessage2:
	cmp [IndexCount],2
	ja NextMessage3
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive1
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive2
	jmp Selection
  NextMessage3:
	cmp [IndexCount],3
	ja NextMessage4
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive1
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive2
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive3
	jmp Selection
  NextMessage4:
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive1
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive2
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive3
	invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,USBDrive4
  Selection:
	invoke SendMessage,[hwnd_cb],CB_SETCURSEL,0,0
	invoke SetFocus,[hwnd_cb]				     ; This is from the forum setfocue, seems to work
	invoke SendMessage,[hwndc],BM_SETCHECK,BST_CHECKED,0	     ; Set the Hdd checkbox as set on startup
       ;invoke SendMessage,[hwnd_cb],CB_GETCURSEL  ,0,0              ; this code test the  get selected string in combo box
       ;cmp eax,0                                                    ; it returns the index starting at 0
       ;jne @f
       ;invoke SendMessage,[hwnd_cb],CB_ADDSTRING,0,_class_cb1
@@:
	xor eax,eax
	jmp finish

;========= CHECKBOX MES ==========;
  wmcommand:
	cmp [wparam],1002
	je CheckBox1
	cmp [wparam],1003
	je CheckBox2
	jmp finish
  CheckBox1:
	invoke SendMessage,[hwndc],BM_GETCHECK,0,0
	cmp eax,BST_CHECKED
	jne finish
	invoke SendMessage,[hwndc1],BM_SETCHECK,BST_UNCHECKED,0
	jmp finish
  CheckBox2:
	invoke SendMessage,[hwndc1],BM_GETCHECK,0,0
	cmp eax,BST_CHECKED
	jne finish
	invoke SendMessage,[hwndc],BM_SETCHECK,BST_UNCHECKED,0
	jmp finish
  wmdestroy:
	invoke	DeleteObject,[bmp1H]
	invoke PostQuitMessage,0
	xor eax,eax
  finish:
	pop edi esi ebx
	ret
endp

;======= CREATE CONTROL =======;
proc create_control,handle,_class_name,_caption,style,parent,x,y,w,h,control_id
	push ebx esi edi
	invoke CreateWindowEx,0,[_class_name],[_caption],[style],[x],[y],[w],[h],[parent],0,[hinstance],[control_id]
	mov ecx,[handle]
	mov [ecx],eax
	invoke SendMessage,eax,WM_SETFONT,[hFont],TRUE
	pop edi esi ebx
	ret
endp

section '.idata' import data readable writeable

   library kernel32,'KERNEL32.DLL',\
	  user32,'USER32.DLL',\
	  gdi32,'GDI32.DLL'

   include '\api\kernel32.inc'
   include '\api\user32.inc'
   include '\api\gdi32.inc'

;========== VISTA STYLE ==========;
section '.rsrc' resource data readable

   directory RT_MANIFEST, manifest,\
	     RT_ICON,icons,\
	     RT_GROUP_ICON,group_icons,\
	     RT_BITMAP, appBmp

   resource  appBmp,\
	     300,LANG_NEUTRAL,bmp1

   resource icons,\
	    1,LANG_NEUTRAL,icon_data

   resource group_icons,\
	    17,LANG_NEUTRAL,main_icon

   icon      main_icon,icon_data,'res\icon1.ico'
	
   bitmap    bmp1, "res\Logo3ab.bmp"

   resource manifest, 1, LANG_NEUTRAL, xpstyle

   resdata xpstyle
	    file 'res\winxpstyle32.xml'
   endres