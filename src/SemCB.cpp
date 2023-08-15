//
// Created by os on 8/17/22.
//
#include "../h/SemCB.h"
#include "../h/MemoryAllocator.h"
#include "../h/syscall_c.hpp"
SemCB* SemCB::semOpen(unsigned int init) {
    return new SemCB(init);
}

int SemCB::signal() {
    int ret = 0;
    if (++value <= 0){
        ret = deblock();
    }
    return ret;
}

int SemCB::wait() {
    int ret = 0;
    if (--value < 0){
        ret = block();
    }
    return ret;
}
int SemCB::block() {
    TCB* oldRunning = TCB::running;
    if (oldRunning == nullptr) return -1;
    if (!headBlock) {
        headBlock = oldRunning;
        tailBlock = oldRunning;
    } else {
        tailBlock->setNext(oldRunning);
        oldRunning->setPrev(tailBlock);
        tailBlock = oldRunning;
    }
    oldRunning->setBlocked(true);
    thread_dispatch();

    return 0;
}

int SemCB::deblock() {
    if(!headBlock){
        return -1;
    }
    TCB* deblocked = headBlock;
    if (deblocked->getNext())deblocked->getNext()->setPrev(nullptr);
    headBlock = deblocked->getNext();
    deblocked->setNext(nullptr);
    deblocked->setBlocked(false);
    Scheduler::put(deblocked);
    return 0;
}

void* SemCB::operator new(size_t size) {
    return MemoryAllocator::getInstance()->memAlloc(size);
}
void* SemCB::operator new [] (size_t size) {
    return MemoryAllocator::getInstance()->memAlloc(size);
}
void SemCB::operator delete(void *ptr)  {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}
void SemCB::operator delete [] (void *ptr)  {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}
int SemCB::semClose() {
    while(headBlock){

        TCB* next = headBlock->getNext();
        headBlock->setNext(nullptr);
        if (next) next->setPrev(nullptr);
        headBlock->setBlocked(false);
        Scheduler::put(headBlock);
        headBlock = next;
    }

    return 0;
}

SemCB::~SemCB() {
    semClose();
}