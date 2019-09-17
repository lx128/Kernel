/*CLR All rights Reserved*/
//Controle overflow                                             =0
//Compatible 64 bits (long au lieu de int là où necessaire)     =0
//Controle unicode                                              =0
//Code d'erreur                                                 =0
//Sécurité                                                      =0

#include <archx86.h>
#include <mmu.h>
#include <cpux86.h>
#include <fonctions.h>
#include <error.h>

/*
Gestionnaire de mémoire pour PC
2 zones existent sur PC. La zone réelle 1 et 2 RZ1 = 0-1Mo RZ2>1Mo
2 allocations existe pour le monde Physique ou Virtuel
*/

static uchar *mem_map = (uchar*)MAP_POS; //1 bit = 1 PAGE_SIZE
static uint pde_map = 0;                         //pde start adress
static ulong page = 0, pagebitmap=0;   //Nombre de page total
static uchar error=0;

uint MmuInit (void)
{
    static uint nb=0;
    pagebitmap = ((page/PAGE_SIZE)/8); //Nombre d'octets pour carte des pages
    if ((page/8)%PAGE_SIZE)            //1 bit = 1 page
    {
	   pagebitmap ++;
    }
    
    nb=pagebitmap/PAGE_SIZE;           //Nombre de page pour la carte
    if (pagebitmap%PAGE_SIZE)
    {
	   nb ++;
    }

	MmuEraseBlock(MAP_POS_PAGE, nb);   //Efface ancienne carte
	MmuAddToBitmap(MAP_POS_PAGE, nb);	   //Alloc carte

    pde_map = (uint)((pagebitmap * PAGE_SIZE) + MAP_POS);	//Init pde position
    MmuAddToBitmap((MAP_POS_PAGE+pagebitmap), 1);		//Alloc in bloc map
    MmuEraseBlock(MAP_POS_PAGE+pagebitmap, 1);	//Erase old pde_map after reboot
while(1);

	AllocVM(0, 0, page);         //Alloc all blocks in pde pte
//    return 1;
while(1);
}

uint AllocPage(uint a, uint b)
{
	return 0;
}

//Allocate size block at position pos. Only aviable for driver
uint AllocVM(uint pos, uint virt_pos, uint pagenb)//pos in blocks of 4096 virt_pos too
{
    static uint comp = 0, comp2 = 0;
    static struct pde *pdep = 0;

    pdep = (struct pde *)(((virt_pos>>10)&0x3FF)*4 + pde_map);

    for (comp2 = 0; comp2 <pagenb; comp2++)
    {
    if (pdep->base==0)
    {
	comp = FindFreePage(pagenb);		//start address in block
	if (!comp)
	{
	    kprint("MMU.c ERROR: Memory's full. Can't allocate more.\n");
	    return 0;
	}
	AllocPage(comp, pagenb);
	MmuEraseBlock(comp, pagenb);		//Erase old page

	pdep->present = 1;
	pdep->rw = 1;
	pdep->us = 0;
	pdep->pwt = 0;
	pdep->cache = 0;
	pdep->accessed = 0;
	pdep->dirty = 0;	//reserved
	pdep->p_size = 0;
	pdep->global = 0;	//ignored
	pdep->no_used = 0;
	pdep->base = comp;
    }

    //Pointer to PTE whith pdep
    pdep = (struct pde *)(((virt_pos)&0x3FF)*4 + (pdep->base<<12));

    if (pdep->base)			//Virtual add already allocated
    {
	kprint("MMU.c ERROR: Vblock already allocated\n");
	return 0;
    }
    pdep->present = 1;
    pdep->rw = 1;
    pdep->us = 0;
    pdep->pwt = 0;
    pdep->cache = 0;
    pdep->accessed = 0;
    pdep->dirty = 0;
    pdep->p_size = 0;		//reserved
    pdep->global = 0;
    pdep->no_used = 0;
    pdep->base = pos;
    }

    return virt_pos;
}

/*Allocate size bytes of memory. Multiple of B_SIZE
void allocm2(int size)	//Size in bytes
{
    static struct mem_desc * desc = 0;
    static int comp = 0;

    (int)desc = MASTER_BLOC;

    aa:
    for (comp=0;comp<(B_SIZE-16);comp= comp+16)
    {
	switch(desc->pid==0)
	{
	case 0://free
	    (int) desc->pid = *p1;
	    (int) desc->start = *p2;
	    (int) desc->end = *p3;
	    comp = B_SIZE;
	break;
	case 1: //deleted
	    
	break;
	}
    }

    if (comp == B_SIZE)
    {
	desc = desc+ 16;
	(int)desc =  desc->pid;
	goto aa;
    }
//print("a");
//print("b");
//print("c");
//while(1);
}*/

/*static void allocated(int pos) //pos in page of B_SIZE
{
    static char * map = 0, comp = 0;

    pos = pos / B_SIZE;
    map = MAP_POS + (pos/8);

    for(comp = 0; comp == 8; comp++)
    {
	map = 
    }

    if (((map>>pos)%8)){return;}

}*/
/*static int free_b(int block)
{
    static int a = 0;
    a = block/8;

    if (block>page)	//Out of memory
    {
	return 0;
    }

    if (mem_map[a] & (1<<(block%8)))
    {
        mem_map[a] = mem_map[a] ^ (1<<(block%8));
	return block;
    }
    return 0;			//Already free
}*/

void MmuEraseBlock(uint block, uint size)
{
    static uint comp = 0;
    static uint* blockx = 0;

    blockx = ( uint*)(block * PAGE_SIZE);
    for (comp = 0;comp<((PAGE_SIZE/4)*size);comp++)
    {
	*(blockx+comp) = (uint)0;
    }
}

/*int CheckPage(int block_start, int size)
{
    static int comp = 0;

    if ((block_start>page) | (size>page))	//Out of memory
    {
	return 0;
    }

    for(comp = block_start; comp<size; comp++)
    {
        if (~mem_map[comp>>3] & (1<<(comp & 7)))
        {
           return 0;	//Bloc utilisé
	    }
    }
	    return (comp);  //Ok
}*/

uint FindFreePage(uint size)
{
       static uint add = 0;
       return add;
}

void MmuAddToBitmap(ulong block, ulong size)
{
    static ulong a = 0;
    a = block/8;

    if (mem_map[a] & (1<<(block%8)))
    {
        error=DEJA_ALLOUE;		//Already Allocated
    }
    mem_map[a] = mem_map[a] | (1<<(block%8));
    error=0;
}

void MmuInitMemorySize(void)
{
    static uchar aa = 'C', *ab = 0;
    page= 0x800;
    while ((aa=='C'))
    {
          page ++;
          ab = (uchar *) ((page * PAGE_SIZE) -1);
          *ab = aa;
          aa = *ab;
    }
    page --;
}

uint MmuGetMemorySize(void)
{
    return page;
}
