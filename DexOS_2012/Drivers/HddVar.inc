;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LbaFirstSecOfPartition		dd 0			    ; Lba of First Sector Of Partition
NumBlocksInPartition		dd 0			    ; Number of Blocks In Partition
FileSysType			db 0			    ; File sys type
ActiveByte			db 0			    ; Is this partition bootable
FS_GetPartionNumber		db 0			    ; Partion number
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
align 4
BaseTable			dd 0x000, 0x1f0, 0x1f0, 0x170, 0x170, 0x1e8, 0x1e8, 0x168, 0x168
				dd 0x1e0, 0x1e0, 0x160, 0x160

align 4
HddBaseFS			dd 0			    ; First device =0x1f0 for primary, second =0x170 for secondary device.
MasterSlave			db 0			    ; Master = 0x00 Slave = 0x10.
FS_RW_SectorErrorNum		db 0			    ; Store the error number in.
HddScratchBuffer		dd 0			    ; Place to store sector buffer address.
FS_RW_Sector			dd 0			    ; Sector size var.
FS_RW_Count			dd 0			    ; Count, var for sectors to read/write.
