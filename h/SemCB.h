//
// Created by os on 8/17/22.
//

#ifndef SEMCB_H
#define SEMCB_H

#include "Tcb.hpp"

class SemCB{
public:
    static SemCB* semOpen(unsigned init);

    int semClose();

    int wait();

    int signal();

    void* operator new (size_t size);

    void* operator new [] (size_t size);

    void operator delete (void* ptr);

    void operator delete [] (void* ptr);

    ~SemCB();

private:
    TCB* headBlock;
    TCB* tailBlock;
    int value;

    SemCB(unsigned int val){
        value = val;
        headBlock = nullptr;
        tailBlock = nullptr;
    }
    int block();
    int deblock();




};
#endif
