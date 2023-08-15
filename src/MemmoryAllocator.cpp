//
// Created by os on 8/6/22.
//

#include  "../h/MemoryAllocator.h"
#include "../lib/hw.h"

MemoryAllocator::FreeMem* MemoryAllocator::head = nullptr;
void* MemoryAllocator::heapEnd = nullptr;
MemoryAllocator::MemoryAllocator(){
    //poravnati adresu pocetka hipa, na onu deljivu sa 4
    if ((unsigned  long long)HEAP_START_ADDR%4){
        head = (FreeMem*)(((unsigned  long long)HEAP_START_ADDR) + (4 - ((unsigned  long long)HEAP_START_ADDR)%4));

    } else{
        head = (FreeMem*)HEAP_START_ADDR;
    }
    if ((unsigned  long long)HEAP_END_ADDR%4){
        heapEnd = (FreeMem*)(((unsigned  long long)HEAP_END_ADDR) - ((unsigned  long long)HEAP_END_ADDR)%4);

    } else{
        heapEnd =(FreeMem*) HEAP_END_ADDR;
    }

    MemoryAllocator::head->next = nullptr;
    MemoryAllocator::head->size = (unsigned long long)heapEnd - (unsigned long long)MemoryAllocator::head  - sizeof(FreeMem) - 40;
    //MemoryAllocator::head->size = (unsigned long long)heapEnd - (unsigned long long)MemoryAllocator::head

}
 /*void MemoryAllocator::init(){
    //poravnati adresu pocetka hipa, na onu deljivu sa 4
    if ((unsigned  long long)HEAP_START_ADDR%4){
        head = (FreeMem*)(((unsigned  long long)HEAP_START_ADDR) + (4 - ((unsigned  long long)HEAP_START_ADDR)%4));

    } else{
        head = (FreeMem*)HEAP_START_ADDR;
    }
    if ((unsigned  long long)HEAP_END_ADDR%4){
        heapEnd = (FreeMem*)(((unsigned  long long)HEAP_END_ADDR) - ((unsigned  long long)HEAP_END_ADDR)%4);

    } else{
        heapEnd =(FreeMem*) HEAP_END_ADDR;
    }

    MemoryAllocator::head->next = nullptr;
    MemoryAllocator::head->size = (unsigned long long)heapEnd - (unsigned long long)MemoryAllocator::head + 1 - sizeof(FreeMem);


}*/
void * MemoryAllocator::memAlloc(size_t bytes)
{
    if((bytes % MEM_BLOCK_SIZE)){
        bytes = bytes + (MEM_BLOCK_SIZE - (bytes % MEM_BLOCK_SIZE));
    }
    FreeMem* curBlk;
    FreeMem* bestBlk;
    uint64 bestBlkSize;

    curBlk = head;

    bestBlk = nullptr;
    bestBlkSize =(unsigned long long) heapEnd - (unsigned long long) head - sizeof(FreeMem) -40;
    while (curBlk){
        if ( (curBlk->size >= (bytes+ sizeof(FreeMem) + 40)) && (curBlk->size <= bestBlkSize) ){
            bestBlk = curBlk;
            bestBlkSize = curBlk->size;
        }
        curBlk = curBlk->next;
    }
    if (bestBlk != nullptr){
        FreeMem* alloc;
        bestBlk->size = bestBlk->size - bytes - sizeof(FreeMem) -40;

        alloc = (FreeMem*)(((unsigned char*)bestBlk) + sizeof(FreeMem) + bestBlk->size + 40);
        alloc->size = bytes;

        return (void*)((unsigned char*)alloc + sizeof(FreeMem) + 40);
    }

    return nullptr;
}

int MemoryAllocator::memDealloc(void *ptr) {
    FreeMem* prevBlock;
    FreeMem* nextBlock;
    if (ptr == nullptr){
        return -1;
    }
    FreeMem* dealloc = (FreeMem*)((unsigned char*)ptr - sizeof(FreeMem)-40);
    if (dealloc == nullptr){
        return -1;
    }
    prevBlock = nullptr;
    nextBlock = head;
    while ((nextBlock != nullptr) && (nextBlock < dealloc)){
        prevBlock = nextBlock;
        nextBlock = nextBlock->next;
    }
    dealloc->next = nextBlock;
    if (prevBlock != nullptr){
        prevBlock->next = dealloc;
    }
    if ( (nextBlock != nullptr) && ( ((unsigned char*)dealloc + dealloc->size + sizeof(FreeMem) + 40) == (unsigned char*)nextBlock) ){
        dealloc->size += nextBlock->size + sizeof(FreeMem) +40;
        dealloc->next = nextBlock->next;
    }
    if ( (prevBlock != nullptr) && ( ((unsigned char*)prevBlock + prevBlock->size + sizeof(FreeMem) + 40) == (unsigned char*)dealloc) ){
        prevBlock->size += dealloc->size + sizeof(FreeMem) + 40;
        prevBlock->next = dealloc->next;
    }
    return 0;



}