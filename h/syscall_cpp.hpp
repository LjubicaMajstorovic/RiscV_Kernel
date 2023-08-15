#ifndef SYSCALL_CPP_HPP
#define SYSCALL_CPP_HPP
#include "../lib/hw.h"
#include "syscall_c.hpp"
#include "Tcb.hpp"
void* operator new (size_t size);
void* operator new[](size_t size);



void operator delete (void* ptr);
void operator delete[](void* ptr);

class Thread {
public:
    Thread(void (*body)(void *), void *arg);
    virtual ~Thread();
    int start();
    static void dispatch();
    static int sleep(time_t time);
    int getTrdId(){
        return getThreadId();
    }
protected:
    Thread();
    virtual void run() {}
private:
    static void callRun(void *ptr);
    thread_t myHandle;
};

class Semaphore {
public:
    explicit Semaphore(unsigned init = 1);
    virtual ~Semaphore();

    int wait();
    int signal();

private:
    sem_t myHandle;
};


class PeriodicThread : public Thread{
public:
    void run() override;
    void stopThread();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {};

private:
    time_t time;
};

class Console {
public:
    static char getc();
    static void putc(char c);
};




#endif