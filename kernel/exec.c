#include <pe.h>
#include <cpux86.h>
#include <textmode.h>

void exec(ulong ptr)
{
    static void (*pf)(void) = 0;
    static uint mem = 0, comp = 0, comp2 = 0;
    static uchar *pe_offset = 0;
    static struct Header *header = 0;
    static struct OptionalHeader *op_header = 0;
    static struct SectionHeadersEntry *sec_head_entry = 0;

    mem = (uint)(0x20210 + ptr);

    comp = mem;
    comp = comp +0x3c;
    pe_offset = (uchar *)(comp);
    header = (struct Header *)((*pe_offset & 0xFF) + mem);
    op_header = (struct OptionalHeader *)((uint)header + 24);
    sec_head_entry = (struct SectionHeadersEntry *)((uint)op_header + header->SizeOfOptionalHeader);
/*
hexl(header->NumberOfSection);
hexl(header->SizeOfOptionalHeader);
hexl(op_header->AddressOfEntryPoint);
hexl(op_header->ImageBase);
hexl(op_header->SizeOfHeaders);
print(sec_head_entry->name);TextModePutcar(10);
hexl(sec_head_entry->VirtualAddress);
hexl(sec_head_entry->SizeOfRawData);
hexl(sec_head_entry->PointerToRawData);
*/
    for(comp = 0; comp <header->NumberOfSection; comp++)
    {
	CpuCopyStringW(((uchar*)mem + sec_head_entry->PointerToRawData)
	,(uchar*)(op_header->ImageBase + sec_head_entry->VirtualAddress)
	,(uint)(sec_head_entry->SizeOfRawData/2));
	comp2 = (ulong)sec_head_entry;
	comp2 = comp2 + 40;
	sec_head_entry = (struct SectionHeadersEntry *)comp2;
//	hexl((long)sec_head_entry);
//    hex(comp);
    }
    pf = (void (*)(void))(op_header->AddressOfEntryPoint + op_header->ImageBase);
//hexl((int) pf);
    (*pf)(); //Jump to 401000

}
