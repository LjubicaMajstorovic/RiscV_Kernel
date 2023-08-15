//
// Created by os on 8/6/22.
//

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include "../lib/hw.h"

class MemoryAllocator{
public:
    struct FreeMem{
        struct FreeMem* next;
        struct FreeMem* prev;
        size_t size;


    };

    //static void init();
    static FreeMem* head;
    static  void* heapEnd;
    void* memAlloc(size_t bytes);
    int memDealloc(void* ptr);
    static MemoryAllocator* getInstance(){
        static MemoryAllocator instance;
        return &instance;

    }

private:
    MemoryAllocator();





};

#endif
