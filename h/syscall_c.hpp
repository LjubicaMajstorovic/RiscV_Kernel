//
// Created by os on 8/8/22.
//

#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP
#include "../lib/hw.h"
#include "riscv.h"


void* mem_alloc (size_t size);
int mem_free (void*);
struct Arguments{
    uint64 ptr1;
    uint64 ptr2;
    uint64 ptr3;
    uint64 ptr4;
    uint64 ptr5;
};
class _thread;
typedef _thread* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_create_CPPI(thread_t* handle, void(*start_routine)(void*), void* arg);
void thread_dispatch();
int thread_exit();
int time_sleep(time_t time);
int getThreadId();
void putInScheduler(thread_t trd);

class _sem;
typedef _sem* sem_t;
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);
void callAbi(Arguments arg);


void putc(char c);
char getc();

#endif