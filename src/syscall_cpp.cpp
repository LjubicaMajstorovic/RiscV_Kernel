//
// Created by os on 8/11/22.
//
#include "../h/syscall_cpp.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}

void* operator new[](size_t size){
    return mem_alloc(size);
}

void operator delete (void* ptr){
    mem_free(ptr);
}

void operator delete[](void* ptr){
    mem_free(ptr);
}

void PeriodicThread::run(){
    while (true && time != 0){
        periodicActivation();
        time_sleep(time);
    }
}
PeriodicThread::PeriodicThread(time_t period) : Thread(){
    time = period;
}

void PeriodicThread::stopThread()  {
    time = 0;
}
Thread::Thread(void (*body)(void*), void *arg) {
    thread_create_CPPI(&myHandle, body, arg);
}



Thread::~Thread() {
    delete (TCB*)(myHandle);
}



int Thread::start() {
    putInScheduler(myHandle);
    return 0;
}



void Thread::dispatch() {
    thread_dispatch();
}



int Thread::sleep(time_t time) {
    return time_sleep(time);
}



Thread::Thread() {
    thread_create_CPPI(&myHandle, &callRun, this);
}



void Thread::callRun(void* ptr) {
    ((Thread*)(ptr))->run();
}



Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}



Semaphore::~Semaphore() {
    delete (SemCB*)myHandle;
}



int Semaphore::wait() {
    return sem_wait(myHandle);
}



int Semaphore::signal() {
    return sem_signal(myHandle);
}


char Console::getc() {
    return ::getc();
}



void Console::putc(char c) {
    ::putc(c);
}
