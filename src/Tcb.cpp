//
// Created by os on 8/12/22.
//
#include "../h/Tcb.hpp"
#include "../h/syscall_c.hpp"
TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

uint64 TCB::timeToWakeUp = 0;

int TCB::generatorId = 0;
void* TCB::operator new(size_t bytes) {
    void* p = MemoryAllocator::getInstance()->memAlloc(bytes);
    return p;
}


void TCB::operator delete(void *ptr) {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}
void* TCB::operator new[](size_t bytes) {
    return MemoryAllocator::getInstance()->memAlloc(bytes);
}


void TCB::operator delete [](void* ptr) {
    MemoryAllocator::getInstance()->memDealloc((void*)ptr);
}


TCB *TCB::createThread(Body body, void* arg, uint64* stk, bool istrdCPP)
{
    if(istrdCPP){

        return new TCB(body, TIME_SLICE, arg, stk, istrdCPP);
    }
    return new TCB(body, TIME_SLICE, arg, stk);
}


void TCB::yield()
{


    TCB::dispatch();

}

void TCB::dispatch()
{

    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked() && !old->isAsleep()) { Scheduler::put(old); }
    /*else if (old->isFinished()) {
        TCB::operator delete(old);
    }*/
    running = Scheduler::get();



    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper()
{
    //printString("Usli smo u tred vreper\n");
    //Riscv::backToUser();
    if(running->body == &Riscv::outputBufConsumer){
        Riscv::popSppSpie();
    } else{
        Riscv::backToUser();
    }

    running->body(running->arguments);
    running->setFinished(true);
    //thread_dispatch();
    thread_exit();

}
int TCB::threadExit() {

    running->setFinished(true);
    thread_dispatch();

    return 0;
}

int TCB::threadSleep(time_t time) {
    running->sleepTime = time;
    Scheduler::putToSleep();
    return 0;
}
