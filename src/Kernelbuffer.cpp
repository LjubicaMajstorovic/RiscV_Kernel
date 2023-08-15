//
// Created by os on 8/23/22.
//
#include "../h/Kernelbuffer.hpp"

void* KernelBuffer::operator new(size_t n) {
    return MemoryAllocator::getInstance()->memAlloc(n);
}



void* KernelBuffer::operator new[](size_t n) {
    return MemoryAllocator::getInstance()->memAlloc(n);
}


void KernelBuffer::operator delete(void *ptr) {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}



void KernelBuffer::operator delete[](void *ptr) {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}
KernelBuffer::KernelBuffer() {
    capacity = 4096;
    head = 0;
    tail = 0;
    buffer = (char*)MemoryAllocator::getInstance()->memAlloc(sizeof(char)*capacity);
    itemAvailable = SemCB::semOpen(0);
    spaceAvailable = SemCB::semOpen(capacity);
    mutexHead = SemCB::semOpen(1);
    mutexTail = SemCB::semOpen(1);

}

void KernelBuffer::put(char c) {
    spaceAvailable->wait();
    mutexTail->wait();
    buffer[tail] = c;
    tail = (tail + 1) % capacity;
    mutexTail->signal();
    itemAvailable->signal();

}

char KernelBuffer::get() {
    itemAvailable->wait();
    mutexHead->wait();
    char ret = buffer[head];
    head = (head + 1) % capacity;
    mutexHead->signal();
    spaceAvailable->signal();

    return ret;

}

int KernelBuffer::size() {
    int ret;

    mutexHead->wait();
    mutexTail->wait();

    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = capacity - head + tail;
    }

    mutexTail->signal();
    mutexHead->signal();

    return ret;
}
KernelBuffer::~KernelBuffer() {
    KernelBuffer::operator delete[](buffer);
    delete mutexHead;
    delete mutexTail;
    delete spaceAvailable;
    delete itemAvailable;

}