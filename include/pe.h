#ifndef PE_H
#define PE_H

#include<clr.h>

#define IMAGE_REL_BASED_ABSOLUTE 0
#define IMAGE_REL_BASED_HIGH 1
#define IMAGE_REL_BASED_LOW 2
#define IMAGE_REL_BASED_HIGHLOW 3
#define IMAGE_REL_BASED_HIGHADJ 4
#define IMAGE_REL_BASED_MIPS_JMPADDR 5
#define IMAGE_REL_BASED_ARM_MOV32A 6
#define IMAGE_REL_BASED_ARM_MOV32T 7
#define IMAGE_REL_BASED_MIPS_JMPADDR16 9
#define IMAGE_REL_BASED_DIR64 10

struct IMAGE_DOS_HEADER      // DOS .EXE header
{
    short   e_magic;                     // Magic number
    short   e_cblp;                      // Bytes on last page of file
    short   e_cp;                        // Pages in file
    short   e_crlc;                      // Relocations
    short   e_cparhdr;                   // Size of header in paragraphs
    short   e_minalloc;                  // Minimum extra paragraphs needed
    short   e_maxalloc;                  // Maximum extra paragraphs needed
    short   e_ss;                        // Initial (relative) SS value
    short   e_sp;                        // Initial SP value
    short   e_csum;                      // Checksum
    short   e_ip;                        // Initial IP value
    short   e_cs;                        // Initial (relative) CS value
    short   e_lfarlc;                    // File address of relocation table
    short   e_ovno;                      // Overlay number
    short   e_res[4];                    // Reserved chars
    short   e_oemid;                     // OEM identifier (for e_oeminfo)
    short   e_oeminfo;                   // OEM information; e_oemid specific
    short   e_res2[10];                  // Reserved chars
    short   e_lfanew;                    // File address of new exe header
}__attribute__ ((packed));

struct Header
{
    int Magic;
    short Machine;		//0x14c=x86 0x8664=AMD64
    ushort NumberOfSection;
    int TimeDateStamp;
    int PointerToSymbolTable;
    int NumberOfSymbols;
    short SizeOfOptionalHeader;
    short Characteristics;
}__attribute__ ((packed));

struct OptionalHeader
{
    short Magic;
    char MajorLinkerVersion;
    char MinorLinkerVersion;
    int SizeOfCode;
    int SizeOfInitializedData;
    int SizeOfUninitializedData;
    int AddressOfEntryPoint;
    int BaseOfCode;
    int BaseOfData;
    
    int ImageBase;
    int SectionAlignement;
    int FileAlignement;
    short MajorOperatingSystemVersion;
    short MinorOperatingSystemVersion;
    short MajorImageVersion;
    short MinorImageVersion;
    short MajorSubsystemVersion;
    short MinorSubsystemVersion;
    int Reserved;
    int SizeOfImage;
    int SizeOfHeaders;
    int CheckSum;
    short Subsystem;
    short DllCharacteristics;
    int SizeOfStackReserve;
    int SizeOfStackCommit;
    int SizeOfHeapReserve;
    int SizeOfHeapCommit;
    int LoaderFlags;			/*Obsolete*/
    int NumbersOfRvaAndSIZES;
}__attribute__ ((packed));

struct OptionalHeader64
{
    short Magic;
    char MajorLinkerVersion;
    char MinorLinkerVersion;
    int SizeOfCode;
    int SizeOfInitializedData;
    int SizeOfUninitializedData;
    int AddressOfEntryPoint;
    int BaseOfCode;
//    int BaseOfData;
    
    long ImageBase;
    int SectionAlignement;
    int FileAlignement;
    short MajorOperatingSystemVersion;
    short MinorOperatingSystemVersion;
    short MajorImageVersion;
    short MinorImageVersion;
    short MajorSubsystemVersion;
    short MinorSubsystemVersion;
    int Reserved;
    int SizeOfImage;
    int SizeOfHeaders;
    int CheckSum;
    short Subsystem;
    short DllCharacteristics;
    long SizeOfStackReserve;
    long SizeOfStackCommit;
    long SizeOfHeapReserve;
    long SizeOfHeapCommit;
    int LoaderFlags;			/*Obsolete*/
    int NumbersOfRvaAndSIZES;
}__attribute__ ((packed));

struct DataDirectory
{
    int ExportTableRva;
    int ExportTableSize;
    int ImportTableRva;
    int ImportTableSize;
    int ResourceTableRva;
    int ResourceTableSize;
    int ExceptionTableRva;
    int ExceptionTableSize;
    int CertificateTableRva;
    int CertificateTableSize;
    int BaseRelocationTableRva;
    int BaseRelocationTableSize;
    int DebugRva;
    int DebugSize;
    int ArchitectureRva;
    int ArchitectureSize;
    int GlobalPtrRva;
    int GlobalPtrSize;
    int TlsTableRva;
    int TlsTableSize;
    int LoadConfigTableRva;
    int LoadConfigTableSize;
    int BoundImportRva;
    int BoundImportSize;
    int IATRva;
    int IATSize;
    int DelayImportDescriptorRva;
    int DelayImportDescriptorSize;
    int ComPlusHeaderRva;
    int ComPlusHeaderSize;
    int _ReservedRva;
    int _ReservedSize;
}__attribute__ ((packed));

struct SectionHeadersEntry
{
    char name[8];
    int VirtualSize;
    int VirtualAddress;
    int SizeOfRawData;
    int PointerToRawData;
    int PointerToRelocations;
    int PointerToLinenumbers;
    short NumberOfRelocations;
    short NumberOfLinenumbers;
    int Characteristics;
}__attribute__ ((packed));

struct BaseRelocationBlock
{
	int page_rva;
	int block_size;
}__attribute__ ((packed));

struct BaseRelocationEntry
{
	short offset:12;
	short type:4;
}__attribute__ ((packed));

struct ExportDirectoryTable
{
	int ExportFlags;
	int TimeDateStamp;
	short MajorVersion;
	short MinorVersion;
	int NameRVA;
	int OrdinalBase;
	int AddressTableEntries;
	int NumberofNamePointers;
	int ExportAddressTableRVA;
	int NamePointerRVA;
	int OrdinalTableRVA;
}__attribute__ ((packed));

#endif
