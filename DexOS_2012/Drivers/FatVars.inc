;=======================================================  ;
;  DATA.                                                  ;
;=======================================================  ;
DefaultDriveNumber      db      0                         ; 
HddFileName		dd	0			  ;
ModExt			db	0			  ;
ModLoadLow		db	0			  ;
FileLoadAddress 	dd	0			  ;
FddSectorsLoadedCount	dd	0			  ;
RootStart		dw	0			  ;
RootStartDx		dw	0			  ;
DataStart		dw	0			  ;
cluster 		dw	0			  ;
clusterSPC		dw	0			  ;
absoluteSector		db	0			  ;
absoluteHead		db	0			  ;
absoluteTrack		db	0			  ;
FddErrCount             db      0                         ;
RootSize		dw	0			  ;
FileLoadCount		dd	0			  ;
CxCount 		dd	0			  ;
FloppyTest		db	0			  ;
UsedSectorBytes 	dd	0			  ;
UsedSectorCount 	dw	0			  ;
RdirFileCount		dd	0			  ;
FddFileListCount	dd	0			  ;
Fdd_FileListCount	dd	0			  ;
FileNumberCount 	dd	0			  ;
RdirCount		dd	0			  ;
BpbCount		dd	0			  ;
number			db	0			  ;
FileExt 		dw	0			  ;
errorloop		db	0			  ;
FatStartDx		dw	0			  ;
FatStart		dw	0			  ;
FatSize2		dw	0			  ;
FatSize1		dw	0			  ;
buff			dd	0			  ;
count			db	0			  ;
Drive			db	0			  ;
align 4
;=======================================================  ;
;  DATA.                                                  ;
;=======================================================  ; 
FileAttrVar                     db        0               ; File Attr Var
ErrorCodeFat16                  db        0               ; Error code var.
UsedClusterCountFat16           dw        0               ; Var to store number of used clusters.
DataSizeInClusters              dw        0               ; Store size of date area, in clusters.
FreeClusterCountFat16           dw        0               ; Tmp var to store free clusters 16bit. 
ClusterCount                    dw        0               ; Tmp var to store cluster
StartCluster                    dw        0               ; First cluster
ClusterOne                      dw        0               ; First cluster od a pair
ClusterTwo                      dw        0               ; Second cluster od a pair
MaxClusters                     dw        0               ; Max number od clusters
Clusterloop                     dw        0               ; Times to loop
Cluster                         dw        0               ; Tmp cluster store
UpdateValue                     dw        0               ; What to put at a cluster address.
ClustOrigValue                  dw        0               ; Save what was at the cluster address.
HddSectorsWriteCountFat16       dd        0               ; Store the number of Sectors written to Hdd.
FileLoadCountFat16              dd        0               ; Number of sectors to add to file load address.
NameOfFileToMakeFat16           dd        0               ; Store name of file to write.
LoadFileFromAddressFat16        dd        0               ; Address of place to load file to write from.
FileSizeInBytesFat16            dd        0               ; Size of file to write in bytes.
FreeClustersFat16               dd        0               ; Stores number of free clusters on Hdd.
FileSizeInClustersFat16         dd        0               ; File to write size, in clusters.
FileSizeInSectorsFat16          dd        0               ; File to write size, in sectors.
FreeSectorsFat16                dd        0               ; Number of free sectors on Hdd.
RootDirEntryAddressFat16        dd        0               ; Store the address of free root entry.
RootFileAddressFat16            dd        0               ; Store the address in root entry, file by same name.
NameOfFileToLoadFat16           dd        0               ; Tmp Store name of file.
align 4                                                   ;
;=======================================================  ;
;  Directory Entry format file                            ;
;=======================================================  ; 
FileEntry:		                                  ;
FileName		rb	8		          ; 0x00
FileExten		rb	3		          ; 0x08
FileAttr		rb	1		          ; 0x0B
FileReserved		rb	1		          ; 0x0C
FileCreate10MS		rb	1		          ; 0x0D
FileCreateTime		rw	1		          ; 0x0E
FileCreateDate		rw	1		          ; 0x10
FileAccessDate		rw	1		          ; 0x12
FileClusterHigh 	rw	1		          ; 0x14
FileTime		rw	1		          ; 0x16
FileDate		rw	1		          ; 0x18
FileCluster		rw	1		          ; 0x1A
FileSize		rd	1		          ; 0x1C
align 4                                                   ;
;=======================================================  ;
;  MBR.                                                   ;
;=======================================================  ; 
MbrBuffer		rb	512			  ;
align 4                                                   ;
;=======================================================  ;
;  BPB.                                                   ;
;=======================================================  ; 
BpbFat: 						  ;
bsJmp			rb	3			  ; 0x00
bsOemName		rb	8			  ; 0x03
bsFat12:						  ;
bpbBytesPerSector	rw	1			  ; 0x0B
bpbSectorsPerCluster	rb	1			  ; 0x0D
bpbReservedSectors	rw	1			  ; 0x0E
bpbNumberOfFATs 	rb	1			  ; 0x10
bpbRootEntries		rw	1			  ; 0x11
bpbTotalSectors 	rw	1			  ; 0x13
bpbMedia		rb	1			  ; 0x15
bpbSectorsPerFAT	rw	1			  ; 0x16
bpbSectorsPerTrack	rw	1			  ; 0x18
bpbHeadsPerCylinder	rw	1			  ; 0x1A
bpbHiddenSectors	rd	1			  ; 0x1C
bpbTotalSectorsBig	rd	1			  ; 0x20
bsDriveNumber		rb	1			  ; 0x24
bsUnused		rb	1			  ; 0x25
bsExtBootSignature	rb	1			  ; 0x26
bsSerialNumber		rd	1			  ; 0x27
bsVolumeLabel		rb	11			  ; 0x2B   
bsFileSystem		rb	8			  ; 0x36   
bsBootCode		rb	450			  ; 0x3E
;=======================================================  ;
;   Fat.                                                  ;
;=======================================================  ; 
Fat			rb	512*256 		  ;
;=======================================================  ;
;  Root Dir.                                              ;
;=======================================================  ; 
RootDirBuffer		rb	512*32			  ;
