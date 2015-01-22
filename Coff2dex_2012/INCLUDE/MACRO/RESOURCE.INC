
; macroinstructions for making resource section

macro directory [type,label]
 { common
    resource_count = 0
    resource_max = 0
   forward
    resource_count = resource_count + 1
    if type > resource_max
    resource_max = type
    end if
   common
    resource_root dd 0,0,0,resource_count shl 16
    repeat resource_max
   forward
    if % = type
    dd type,80000000h+label-resource_root
    end if
   common
    end repeat
    all_id equ }

macro resource [id,lang,label]
 { forward
    resource_min = id
    resource_max = id
   common
    resource_count = 0
   forward
    local resource_label
    resource_count = resource_count + 1
    if id < resource_min
    resource_min = id
    else if id > resource_max
    resource_max = id
    end if
   common
    dd 0,0,0,resource_count shl 16
    repeat resource_max-resource_min+1
   forward
    if resource_min+%-1 = id
    dd id,80000000h+resource_label-resource_root
    end if
   common
    end repeat
   forward
    all_id equ all_id+id*(RVA label/filter_id)*(filter_id/RVA label)
    resource_label dd 0,0,0,10000h,lang,label-resource_root }

macro bitmap label,bitmap_file
 { local data,size
   label dd RVA data,size,0,0
   data file bitmap_file:0Eh
   size = $ - data
   align 4 }

macro icon group,label,icon_file
 { local header,data,size,position
   label dd RVA data,size,0,0
   load size dword from icon_file:0Eh
   load position dword from icon_file:12h
   data file icon_file:position,size
   align 4
   group dd RVA header,14h,0,0
   header file icon_file,12h
   filter_id = RVA label
   dw all_id }

macro menu label
 { local data,size
   label dd RVA data,size
   data dw 1,4,0,0
   menu_size equ size = $ - data
   menu_level = 1 }

macro menuitem string,id,resinfo,status
 { dd MFT_STRING,status+0,id
   dw resinfo
   du string,0
   align 4
   if resinfo and MFR_END <> 0
   menu_level = menu_level - 1
   end if
   if resinfo and MFR_POPUP <> 0
   menu_level = menu_level + 1
   dd 0
   end if
   if menu_level = 0
   menu_size
   end if }

macro menuseparator resinfo,status
 { dd MFT_SEPARATOR,status+0,0
   dw resinfo+0,0
   if resinfo+0 and MFR_END <> 0
   menu_level = menu_level - 1
   end if
   if menu_level = 0
   menu_size
   end if }

macro dialog label,itemscount,title,x,y,cx,cy,style,menu
 { local data,size
   label dd RVA data,size
   data dd style or DS_SETFONT,0
   dw itemscount,x,y,cx,cy
   if menu+0 <> 0
   dw 0FFFFh
   end if
   du menu+0,0,title,0
   du 8,'MS Sans Serif',0
   align 4
   dialog_items = itemscount
   dialog_size equ size = $ - data
   if dialog_items = 0
   dialog_size
   end if }

macro dialogitem class,title,id,x,y,cx,cy,style
 { dd style or WS_CHILD,0
   dw x,y,cx,cy,id
   if class eq 'BUTTON'
   dw 0FFFFh,80h
   else if class eq 'EDIT'
   dw 0FFFFh,81h
   else if class eq 'STATIC'
   dw 0FFFFh,82h
   else if class eq 'LISTBOX'
   dw 0FFFFh,83h
   else if class eq 'SCROLLBAR'
   dw 0FFFFh,84h
   else if class eq 'COMBOBOX'
   dw 0FFFFh,85h
   else
   du class,0
   end if
   if +title eq title
   dw 0FFFFh,title
   else
   du title,0
   end if
   dw 0
   align 4
   dialog_items = dialog_items - 1
   if dialog_items = 0
   dialog_size
   end if }

macro accelerator label,[fvirt,key,cmd]
 { common
    local data,size
    label dd RVA data,size,0,0
    data:
    accel_count = 0
   forward
    accel_count = accel_count + 1
   common
    size = accel_count * 8
   forward
    accel_count = accel_count - 1
    if accel_count = 0
    dw fvirt or 80h,key
    else
    dw fvirt,key
    end if
    dd cmd }
