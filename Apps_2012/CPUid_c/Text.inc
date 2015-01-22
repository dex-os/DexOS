 ;====================================================;
 ; Print Veatures                                     ;
 ;====================================================;
PrintVeatures: 
       mov   esi,NextLine
       call  [PrintString_0]
       mov   edx,[Veatures]
       mov   [Features],0
       mov   [Features],1
 ;====================================================;
 ; Floating Point Unit.                               ;
 ;====================================================;
       test  edx,[Features]
       jz    No1
       mov   esi,CompY0 
       call  [PrintString_0]
       jmp   @f
No1:
       mov   esi,CompN0 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Virtual Mode Extension.                            ;
 ;====================================================;
       test  edx,[Features]
       jz    No2
       mov   esi,CompY1 
       call  [PrintString_0]
       jmp   @f
No2:
       mov   esi,CompN1 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Debugging Extension.                               ;
 ;====================================================;
       test  edx,[Features]
       jz    No3
       mov   esi,CompY2 
       call  [PrintString_0]
       jmp   @f
No3:
       mov   esi,CompN2 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Page Size Extension.                               ;
 ;====================================================;
       test  edx,[Features]
       jz    No4
       mov   esi,CompY3 
       call  [PrintString_0]
       jmp   @f
No4:
       mov   esi,CompN3 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Time Stamp Counter.                                ;
 ;====================================================;
       test  edx,[Features]
       jz    No5
       mov   esi,CompY4 
       call  [PrintString_0]
       jmp   @f
No5:
       mov   esi,CompN4 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Model Specific Registers.                          ;
 ;====================================================;
       test  edx,[Features]
       jz    No6
       mov   esi,CompY5 
       call  [PrintString_0]
       jmp   @f
No6:
       mov   esi,CompN5 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Physical Address Extesnion.                        ;
 ;====================================================;
       test  edx,[Features]
       jz    No7
       mov   esi,CompY6 
       call  [PrintString_0]
       jmp   @f
No7:
       mov   esi,CompN6 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Machine Check Extension.                           ;
 ;====================================================;
       test  edx,[Features]
       jz    No8
       mov   esi,CompY7 
       call  [PrintString_0]
       jmp   @f
No8:
       mov   esi,CompN7 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; CMPXCHG8 Instruction.                              ;
 ;====================================================;
       test  edx,[Features]
       jz    No9
       mov   esi,CompY8 
       call  [PrintString_0]
       jmp   @f
No9:
       mov   esi,CompN8
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; On-chip APIC Hardware.                             ;
 ;====================================================;
       test  edx,[Features]
       jz    No10
       mov   esi,CompY9 
       call  [PrintString_0]
       jmp   @f
No10:
       mov   esi,CompN9 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Reserved.                                          ;
 ;====================================================;
       test  edx,[Features]
       jz    No11
       mov   esi,CompY10 
       call  [PrintString_0]
       jmp   @f
No11:
       mov   esi,CompN10 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; SYSENTER SYSEXIT.                                  ;
 ;====================================================;
       test  edx,[Features]
       jz    No12
       mov   esi,CompY11 
       call  [PrintString_0]
       jmp   @f
No12:
       mov   esi,CompN11 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Machine Type Range Registers.                      ;
 ;====================================================;
       test  edx,[Features]
       jz    No13
       mov   esi,CompY12 
       call  [PrintString_0]
       jmp   @f
No13:
       mov   esi,CompN12 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Global Paging Extension.                           ;
 ;====================================================;
       test  edx,[Features]
       jz    No14
       mov   esi,CompY13 
       call  [PrintString_0]
       jmp   @f
No14:
       mov   esi,CompN13 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Machine Check Architecture.                        ;
 ;====================================================;
       test  edx,[Features]
       jz    No15
       mov   esi,CompY14 
       call  [PrintString_0]
       jmp   @f
No15:
       mov   esi,CompN14 
       call  [PrintString_0]
@@:
;;;;;;;;
       pushad
       mov   esi,Wait4keyPressMess
       call  [PrintString_0]
       call  [WaitForKeyPress]
       popad
;;;;;;;;
       shl   [Features],1
 ;====================================================;
 ; Conditional Move Instrction.                       ;
 ;====================================================;
       test  edx,[Features]
       jz    No16
       mov   esi,CompY15 
       call  [PrintString_0]
       jmp   @f
No16:
       mov   esi,CompN15 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Page Attribute Table.                              ;
 ;====================================================;
       test  edx,[Features]
       jz    No17
       mov   esi,CompY16 
       call  [PrintString_0]
       jmp   @f
No17:
       mov   esi,CompN16 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; 36-bit Page Size Extension.                        ;
 ;====================================================;
       test  edx,[Features]
       jz    No18
       mov   esi,CompY17 
       call  [PrintString_0]
       jmp   @f
No18:
       mov   esi,CompN17 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; 96-bit Processor Serial Number.                    ;
 ;====================================================;
       test  edx,[Features]
       jz    No19
       mov   esi,CompY18 
       call  [PrintString_0]
       jmp   @f
No19:
       mov   esi,CompN18 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; CLFLUSH Instruction.                               ;
 ;====================================================;
       cmp   dword[VendorId+8],'cAMD'
       je    ItsAMD1
       test  edx,[Features]
       jz    No20
       mov   esi,CompY19 
       call  [PrintString_0]
       jmp   @f
No20:
       mov   esi,CompN19 
       call  [PrintString_0]
       jmp   @f
 ;====================================================;
 ; MultiProcessing Capable.                           ;
 ;====================================================;
ItsAMD1:
       test  edx,[Features]
       jz    No20AMD
       mov   esi,CompY19AMD 
       call  [PrintString_0]
       jmp   @f
No20AMD:
       mov   esi,CompN19AMD 
       call  [PrintString_0]

@@:
       shl   [Features],1
 ;====================================================;
 ; Reserved.                                          ;
 ;====================================================;
       test  edx,[Features]
       jz    No21
       mov   esi,CompY20 
       call  [PrintString_0]
       jmp   @f
No21:
       mov   esi,CompN20 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Debug Trace Store.                                 ;
 ;====================================================;
       test  edx,[Features]
       jz    No22
       mov   esi,CompY21 
       call  [PrintString_0]
       jmp   @f
No22:
       mov   esi,CompN21 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; ACPI Support.                                      ;
 ;====================================================;
       cmp   dword[VendorId+8],'cAMD'
       je    ItsAMD2

       test  edx,[Features]
       jz    No23
       mov   esi,CompY22 
       call  [PrintString_0]
       jmp   @f
No23:
       mov   esi,CompN22 
       call  [PrintString_0]
       jmp   @f
 ;====================================================;
 ; AMD Multimedia Instruction Extensions.             ;
 ;====================================================;
ItsAMD2:
       test  edx,[Features]
       jz    No23AMD
       mov   esi,CompY22AMD 
       call  [PrintString_0]
       jmp   @f
No23AMD:
       mov   esi,CompN22AMD 
       call  [PrintString_0]

@@:
       shl   [Features],1
 ;====================================================;
 ; MMX Technology.                                    ;
 ;====================================================;
       test  edx,[Features]
       jz    No24
       mov   esi,CompY23 
       call  [PrintString_0]
       jmp   @f
No24:
       mov   esi,CompN23 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; FXSAVE FXRSTOR.                                    ;
 ;====================================================;
       test  edx,[Features]
       jz    No25
       mov   esi,CompY24 
       call  [PrintString_0]
       jmp   @f
No25:
       mov   esi,CompN24 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Streaming SIMD Extensions.                         ;
 ;====================================================;
       test  edx,[Features]
       jz    No26
       mov   esi,CompY25 
       call  [PrintString_0]
       jmp   @f
No26:
       mov   esi,CompN25 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Streaming SIMD Extensions 2.                       ;
 ;====================================================;
       test  edx,[Features]
       jz    No27
       mov   esi,CompY26 
       call  [PrintString_0]
       jmp   @f
No27:
       mov   esi,CompN26 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Self-Snoop.                                        ;
 ;====================================================;
       test  edx,[Features]
       jz    No28
       mov   esi,CompY27 
       call  [PrintString_0]
       jmp   @f
No28:
       mov   esi,CompN27 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Hyper-Threading Technology.                        ;
 ;====================================================;
       test  edx,[Features]
       jz    No29
       mov   esi,CompY28 
       call  [PrintString_0]
       jmp   @f
No29:
       mov   esi,CompN28 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Thermal Monitor Supported.                         ;
 ;====================================================;
       test  edx,[Features]
       jz    No30
       mov   esi,CompY29 
       call  [PrintString_0]
       jmp   @f
No30:
       mov   esi,CompN29 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; IA-64 capable.                                     ;
 ;====================================================;
       cmp   dword[VendorId+8],'cAMD'
       je    ItsAMD3
       test  edx,[Features]
       jz    No31
       mov   esi,CompY30 
       call  [PrintString_0]
       jmp   @f
No31:
       mov   esi,CompN30 
       call  [PrintString_0]
       jmp   @f
 ;====================================================;
 ; 3DNow Instruction Extensions.                      ;
 ;====================================================;
ItsAMD3:
       test  edx,[Features]
       jz    No31AMD
       mov   esi,CompY30AMD 
       call  [PrintString_0]
       jmp   @f
No31AMD:
       mov   esi,CompN30AMD 
       call  [PrintString_0]
@@:
       shl   [Features],1
 ;====================================================;
 ; Reserved.                                          ;
 ;====================================================;
       cmp   dword[VendorId+8],'cAMD'
       je    ItsAMD4
       test  edx,[Features]
       jz    No32
       mov   esi,CompY31 
       call  [PrintString_0]
       jmp   @f
No32:
       mov   esi,CompN31 
       call  [PrintString_0]
       jmp   @f
 ;====================================================;
 ; 3DNow Instructions.                                ;
 ;====================================================;
ItsAMD4:
       test  edx,[Features]
       jz    No32AMD
       mov   esi,CompY31AMD 
       call  [PrintString_0]
       jmp   @f
No32AMD:
       mov   esi,CompN31AMD 
       call  [PrintString_0]
 ;====================================================;
 ; Exit.                                              ;
 ;====================================================;
@@:
       ret

Features   dd 0