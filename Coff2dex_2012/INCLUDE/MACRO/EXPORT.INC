
; macroinstruction for making export section

macro export dllname,[label,string]	; strings must be sorted
 { common
    local module,addresses,names,ordinal,count
    count = 0
   forward
    count = count+1
   common
    dd 0,0,0,RVA module,1
    dd count,count,RVA addresses,RVA names,RVA ordinal
    addresses:
   forward
    dd RVA label
   common
    names:
   forward
    local name
    dd RVA name
   common
    ordinal: count = 0
   forward
    dw count
    count = count+1
   common
    module db dllname,0
   forward
    name db string,0 }
