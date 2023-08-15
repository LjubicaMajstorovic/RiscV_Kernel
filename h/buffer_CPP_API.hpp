#ifndef BUFFER_CPP_API_HPP
#define BUFFER_CPP_API_HPP

#include "../h/syscall_cpp.hpp"
#include "printing.hpp"

class BufferCPP {
private:
    int cap;
    int *buffer;
    int head, tail;

    Semaphore* spaceAvailable;
    Semaphore* itemAvailable;
    Semaphore* mutexHead;
    Semaphore* mutexTail;

public:
    BufferCPP(int _cap);
    ~BufferCPP();

    void put(int val);
    int get();

    int getCnt();
};


#endif
