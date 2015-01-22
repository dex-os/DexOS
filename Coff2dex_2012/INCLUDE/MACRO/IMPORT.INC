
; macroinstructions for making import section

macro library [label,string]
 { forward
    local _label
    dd 0,0,0,RVA _label,RVA label
   common
    dd 0,0,0,0,0
   forward
    _label db string,0 }

macro import [label,string]
 { forward
    local _label
    label dd RVA _label
   common
    dd 0
   forward
    _label dw 0
	   db string,0 }
