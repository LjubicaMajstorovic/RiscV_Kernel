//
// Created by os on 9/2/23.
//

#ifndef PROJECT_BASE_BUDDYALLOCATOR_HPP
#define PROJECT_BASE_BUDDYALLOCATOR_HPP

#define MAX_BUCKET_SIZE 14
#define MAX_BLOCKS 8192

#include "../lib/hw.h"
class BuddyAllocator{
public:
    static BuddyAllocator* getInstance(){
        static BuddyAllocator instance;
        return &instance;
    }

    void* alloc(size_t bytes);
    void free(void* addr, size_t size);

private:
    BuddyAllocator();
    char* start;
    int bucket_size;
    int blocks;
    bool bucket[MAX_BUCKET_SIZE][MAX_BLOCKS];
    int numOfBlocks[MAX_BUCKET_SIZE];


    void* getBlockAddr(int size, int block) const;
    void setBlock(int size, int block, bool free) { bucket[size][block] = free; }
    int getFreeBlock(int size) const;
};
#endif //PROJECT_BASE_BUDDYALLOCATOR_HPP
