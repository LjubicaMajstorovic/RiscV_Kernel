//
// Created by os on 8/23/22.
//

#ifndef KERNELBUFFER_HPP
#define KERNELBUFFER_HPP
#include "SemCB.h"
#include "MemoryAllocator.h"
class SemCB;
class MemoryAllocator;
class KernelBuffer{
public:
    KernelBuffer();
    ~KernelBuffer();
    void put(char c);
    char get();
    int size();

    static void* operator new(size_t n);
    static void* operator new[](size_t n);
    static void operator delete(void* ptr);
    static void operator delete[](void* ptr);

private:
    SemCB* itemAvailable;
    SemCB*  spaceAvailable;
    SemCB* mutexHead;
    SemCB* mutexTail;

    int capacity;
    char* buffer;
    int head, tail;

};

#endif
