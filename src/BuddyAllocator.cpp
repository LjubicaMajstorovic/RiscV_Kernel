//
// Created by os on 9/2/23.
//
#include  "../h/BuddyAllocator.hpp"
#include "../h/printing.hpp"

BuddyAllocator::BuddyAllocator() {
    if((unsigned long long)HEAP_START_ADDR % BLOCK_SIZE){
        start = (char*)(((unsigned  long long)HEAP_START_ADDR) + ((unsigned long long) BLOCK_SIZE - ((unsigned  long long)HEAP_START_ADDR)% (unsigned long long)BLOCK_SIZE));
    } else {
        start = (char*)HEAP_START_ADDR;
    }

    int numOfBlocksInHeap = ((unsigned long long) HEAP_END_ADDR - (unsigned long long) start) / (unsigned long long) BLOCK_SIZE;
    int numOfBlockForBuddy = (numOfBlocksInHeap*125) / 1000;

    blocks = 1;
    bucket_size = 0;
    while (blocks <= (numOfBlockForBuddy >> 1)) {
        blocks <<= 1;
        bucket_size++;
    }

    printInt(bucket_size);
    printString("\n");

    for(int i = bucket_size, nblks = 1; i >= 0; i--, nblks *= 2){
        numOfBlocks[i] = nblks;
        for(int j = 0; j < nblks; j++){
            bucket[i][j] = false;
        }
    }
    bucket[bucket_size][0] = true;
}

inline int BuddyAllocator::getFreeBlock(int size) const {
    for(int i = 0; i < numOfBlocks[size]; i++){

        if(bucket[size][i]) return i;
    }
    return -1;
}

void* BuddyAllocator::alloc(size_t bytes) {
    int size = 0;
    size_t numBlk = bytes / BLOCK_SIZE + ((bytes % BLOCK_SIZE )? 1 : 0);
    size_t value = numBlk;
    while (value > 1) {
        value >>= 1;
        size++;
    }

    if ((1u << size) < numBlk) {
        size++;
    }



    int block = -1, current = size;

    for(; block < 0 && current <= bucket_size; current++){
        block = getFreeBlock(current);
    }

    if(block == -1) return 0;
    setBlock(--current, block, false);

    while(--current >= size){
        block *= 2;
        setBlock(current, block + 1, true);
    }


    return getBlockAddr(size, block);
}


void* BuddyAllocator::getBlockAddr(int size, int block) const {
    int pow2size = 1;
    for(int i = 0; i < size; i++){
        pow2size <<= 1;
    }

    int space = 1;
    /*for(int i = 0; i < size; i++){
        space <<= 1;
    }*/

    space = pow2size*BLOCK_SIZE*block;
    /*printInt(reinterpret_cast<size_t>(start));
    printString("evo\n");
    printInt(space);*/
    return (void*)(start + space);
}


void BuddyAllocator::free(void* addr, size_t bytes){
    int size = 0;
    int numBlk = bytes / BLOCK_SIZE + ((bytes % BLOCK_SIZE )? 1 : 0);
    size_t value = numBlk;
    while (value > 1) {
        value >>= 1;
        size++;
    }

    if ((1u << size) < value) {
        size++;
    }

    int block = ((char*)addr - start) / (BLOCK_SIZE << size);
    while(size < bucket_size){
        int buddy = block % 2 ? block - 1 : block + 1;
        if(!bucket[size][buddy]){
            setBlock(size, block, true);
            return;
        }

        setBlock(size, buddy, false);
        block /= 2;
        size++;
    }

    setBlock(bucket_size, 0, true);

}