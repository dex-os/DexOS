CPUlogo:
db 0xd2,0x02, '   ________  __  _________  ',10,13
db	      '  / ___/ _ \/ / / /  _/ _ \ ',10,13
db	      ' / /__/ ___/ /_/ // // // / ',10,13
db	      ' \___/_/   \____/___/____/  ', 0xd2,0x07,10,13,13,0

Wait4keyPressMess: db 0xd2,0x04,10,13,'Press anykey to continue', 0xd2,0x07,10,13,10,13,0

GenuineIntel:
    CompY0	   db 0xd2,0x02, "  FPU     YES! ", 0xd2,0x07,'Floating Point Unit',10,13,0
    CompY1	   db 0xd2,0x02, "  VME     YES! ", 0xd2,0x07,'Virtual Mode Extension',10,13,0
    CompY2	   db 0xd2,0x02, "  DE      YES! ", 0xd2,0x07,'Debugging Extension',10,13,0
    CompY3	   db 0xd2,0x02, "  PSE     YES! ", 0xd2,0x07,'Page Size Extension',10,13,0
    CompY4	   db 0xd2,0x02, "  TSC     YES! ", 0xd2,0x07,'Time Stamp Counter',10,13,0
    CompY5	   db 0xd2,0x02, "  MSR     YES! ", 0xd2,0x07,'Model Specific Registers',10,13,0
    CompY6	   db 0xd2,0x02, "  PAE     YES! ", 0xd2,0x07,'Physical Address Extesnion',10,13,0
    CompY7	   db 0xd2,0x02, "  MCE     YES! ", 0xd2,0x07,'Machine Check Extension',10,13,0
    CompY8	   db 0xd2,0x02, "  CX8     YES! ", 0xd2,0x07,'CMPXCHG8 Instruction',10,13,0
    CompY9	   db 0xd2,0x02, "  APIC    YES! ", 0xd2,0x07,'On-chip APIC Hardware',10,13,0
    CompY10	   db 0xd2,0x02, "  Reserved     ", 0xd2,0x07,'            ',10,13,0
    CompY11	   db 0xd2,0x02, "  SEP     YES! ", 0xd2,0x07,'SYSENTER SYSEXIT',10,13,0
    CompY12	   db 0xd2,0x02, "  MTRR    YES! ", 0xd2,0x07,'Machine Type Range Registers',10,13,0
    CompY13	   db 0xd2,0x02, "  PGE     YES! ", 0xd2,0x07,'Global Paging Extension',10,13,0
    CompY14	   db 0xd2,0x02, "  MCA     YES! ", 0xd2,0x07,'Machine Check Architecture',10,13,0
    CompY15	   db 0xd2,0x02, "  CMOV    YES! ", 0xd2,0x07,'Conditional Move Instrction',10,13,0
    CompY16	   db 0xd2,0x02, "  PAT     YES! ", 0xd2,0x07,'Page Attribute Table',10,13,0
    CompY17	   db 0xd2,0x02, "  PSE-36  YES! ", 0xd2,0x07,'36-bit Page Size Extension',10,13,0
    CompY18	   db 0xd2,0x02, "  PSN     YES! ", 0xd2,0x07,'96-bit Processor Serial Number',10,13,0
    CompY19	   db 0xd2,0x02, "  CLFSH   YES! ", 0xd2,0x07,'CLFLUSH Instruction',10,13,0
    CompY20	   db 0xd2,0x02, "  Reserved     ", 0xd2,0x07,'             ',10,13,0
    CompY21	   db 0xd2,0x02, "  DS      YES! ", 0xd2,0x07,'Debug Trace Store',10,13,0
    CompY22	   db 0xd2,0x02, "  ACPI    YES! ", 0xd2,0x07,'ACPI Support',10,13,0
    CompY23	   db 0xd2,0x02, "  MMX     YES! ", 0xd2,0x07,'MMX Technology',10,13,0
    CompY24	   db 0xd2,0x02, "  FXSR    YES! ", 0xd2,0x07,'FXSAVE FXRSTOR (Fast save and restore)',10,13,0
    CompY25	   db 0xd2,0x02, "  SSE     YES! ", 0xd2,0x07,'Streaming SIMD Extensions',10,13,0
    CompY26	   db 0xd2,0x02, "  SSE2    YES! ", 0xd2,0x07,'Streaming SIMD Extensions 2',10,13,0
    CompY27	   db 0xd2,0x02, "  SS      YES! ", 0xd2,0x07,'Self-Snoop',10,13,0
    CompY28	   db 0xd2,0x02, "  HTT     YES! ", 0xd2,0x07,'Hyper-Threading Technology',10,13,0
    CompY29	   db 0xd2,0x02, "  TM      YES! ", 0xd2,0x07,'Thermal Monitor Supported',10,13,0
    CompY30	   db 0xd2,0x02, "  IA-64   YES! ", 0xd2,0x07,'IA-64 capable',10,13,0
    CompY31	   db 0xd2,0x02, "  Reserved     ", 0xd2,0x07,'             ',10,13,0
   
    CompN0	   db 0xd2,0x02, "  FPU      NO! ", 0xd2,0x07,'Floating Point Unit',10,13,0
    CompN1	   db 0xd2,0x09, "  VME      NO! ", 0xd2,0x07,'Virtual Mode Extension',10,13,0
    CompN2	   db 0xd2,0x09, "  DE       NO! ", 0xd2,0x07,'Debugging Extension',10,13,0
    CompN3	   db 0xd2,0x09, "  PSE      NO! ", 0xd2,0x07,'Page Size Extension',10,13,0
    CompN4	   db 0xd2,0x09, "  TSC      NO! ", 0xd2,0x07,'Time Stamp Counter',10,13,0
    CompN5	   db 0xd2,0x09, "  MSR      NO! ", 0xd2,0x07,'Model Specific Registers',10,13,0
    CompN6	   db 0xd2,0x09, "  PAE      NO! ", 0xd2,0x07,'Physical Address Extesnion',10,13,0
    CompN7	   db 0xd2,0x09, "  MCE      NO! ", 0xd2,0x07,'Machine Check Extension',10,13,0
    CompN8	   db 0xd2,0x09, "  CX8      NO! ", 0xd2,0x07,'CMPXCHG8 Instruction',10,13,0
    CompN9	   db 0xd2,0x09, "  APIC     NO! ", 0xd2,0x07,'On-chip APIC Hardware',10,13,0
    CompN10	   db 0xd2,0x09, "  Reserved     ", 0xd2,0x07,'            ',10,13,0
    CompN11	   db 0xd2,0x09, "  SEP      NO! ", 0xd2,0x07,'SYSENTER SYSEXIT',10,13,0
    CompN12	   db 0xd2,0x09, "  MTRR     NO! ", 0xd2,0x07,'Machine Type Range Registers',10,13,0
    CompN13	   db 0xd2,0x09, "  PGE      NO! ", 0xd2,0x07,'Global Paging Extension',10,13,0
    CompN14	   db 0xd2,0x09, "  MCA      NO! ", 0xd2,0x07,'Machine Check Architecture',10,13,0
    CompN15	   db 0xd2,0x09, "  CMOV     NO! ", 0xd2,0x07,'Conditional Move Instrction',10,13,0
    CompN16	   db 0xd2,0x09, "  PAT      NO! ", 0xd2,0x07,'Page Attribute Table',10,13,0
    CompN17	   db 0xd2,0x09, "  PSE-36   NO! ", 0xd2,0x07,'36-bit Page Size Extension',10,13,0
    CompN18	   db 0xd2,0x09, "  PSN      NO! ", 0xd2,0x07,'96-bit Processor Serial Number',10,13,0
    CompN19	   db 0xd2,0x09, "  CLFSH    NO! ", 0xd2,0x07,'CLFLUSH Instruction',10,13,0
    CompN20	   db 0xd2,0x09, "  Reserved     ", 0xd2,0x07,'            ',10,13,0
    CompN21	   db 0xd2,0x09, "  DS       NO! ", 0xd2,0x07,'Debug Trace Store',10,13,0
    CompN22	   db 0xd2,0x09, "  ACPI     NO! ", 0xd2,0x07,'ACPI Support',10,13,0
    CompN23	   db 0xd2,0x09, "  MMX      NO! ", 0xd2,0x07,'MMX Technology',10,13,0
    CompN24	   db 0xd2,0x09, "  FXSR     NO! ", 0xd2,0x07,'FXSAVE FXRSTOR (Fast save and restore)',10,13,0
    CompN25	   db 0xd2,0x09, "  SSE      NO! ", 0xd2,0x07,'Streaming SIMD Extensions',10,13,0
    CompN26	   db 0xd2,0x09, "  SSE2     NO! ", 0xd2,0x07,'Streaming SIMD Extensions 2',10,13,0
    CompN27	   db 0xd2,0x09, "  SS       NO! ", 0xd2,0x07,'Self-Snoop',10,13,0
    CompN28	   db 0xd2,0x09, "  HTT      NO! ", 0xd2,0x07,'Hyper-Threading Technology',10,13,0
    CompN29	   db 0xd2,0x09, "  TM       NO! ", 0xd2,0x07,'Thermal Monitor Supported',10,13,0
    CompN30	   db 0xd2,0x09, "  IA-64    NO! ", 0xd2,0x07,'IA-64 capable',10,13,0
    CompN31	   db 0xd2,0x09, "  Reserved     ", 0xd2,0x07,'             ',10,13,0

AuthenticAMD:
    CompY19AMD	   db 0xd2,0x02, "  MPC      YES! ", 0xd2,0x07,'MultiProcessing Capable',10,13,0
    CompY22AMD	   db 0xd2,0x02, "  MIE      YES! ", 0xd2,0x07,'AMD Multimedia Instruction Extensions ',10,13,0
    CompY30AMD	   db 0xd2,0x02, "  3DNowExt YES! ", 0xd2,0x07,'3DNow Instruction Extensions',10,13,0
    CompY31AMD	   db 0xd2,0x02, "  3DNow    YES! ", 0xd2,0x07,'3DNow Instructions  ',10,13,0

    CompN19AMD	   db 0xd2,0x09, "  MPC      NO! ", 0xd2,0x07,'MultiProcessing Capable',10,13,0
    CompN22AMD	   db 0xd2,0x09, "  MIE      NO! ", 0xd2,0x07,'AMD Multimedia Instruction Extensions ',10,13,0
    CompN30AMD	   db 0xd2,0x09, "  3DNowExt NO! ", 0xd2,0x07,'3DNow Instruction Extensions',10,13,0
    CompN31AMD	   db 0xd2,0x09, "  3DNow    NO! ", 0xd2,0x07,'3DNow Instructions  ',10,13,0