#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size){
    static Arguments arg;
    arg.ptr1 = 0x01;
    arg.ptr2 = size;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    callAbi(arg);
    uint64 ptr;
    __asm__ ("mv %[ptr], a0" : [ptr] "=r" (ptr) );
    return (void*) ptr;




}

int mem_free(void* ptr){
    static Arguments arg;
    arg.ptr1 = 0x02;
    arg.ptr2 = (uint64) ptr;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int value;
    __asm__ ("mv %[value], a0" : [value] "=r" (value) );
    return (int) value;
}

void callAbi(Arguments arg){
    __asm__ volatile ("mv a1, %[ptr2]":: [ptr2] "r"(arg.ptr2));
    __asm__ volatile ("mv a2, %[ptr3]":: [ptr3] "r"(arg.ptr3));
    __asm__ volatile ("mv a3, %[ptr4]":: [ptr4] "r"(arg.ptr4));
    __asm__ volatile ("mv a4, %[ptr5]":: [ptr5] "r" (arg.ptr5));
    __asm__ volatile ("mv a0, %[ptr1]":: [ptr1] "r"(arg.ptr1));
    __asm__ volatile ("ecall");
}

void thread_dispatch(){
    static Arguments arg;
    arg.ptr1 = 0x13;
    arg.ptr2 = 0;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* argum) {
    static Arguments arg;
    arg.ptr1 = 0x11;
    arg.ptr2 = (uint64) handle ;
    arg.ptr3 = (uint64) start_routine;
    arg.ptr4 = (uint64) argum;
    void* stack;
    if(!start_routine) {
        stack = nullptr;
    } else{
        stack = MemoryAllocator::getInstance()->memAlloc(DEFAULT_STACK_SIZE);
        //printString("Adresa steka (syscall_c.cpp): ");
        //printInteger((uint64)stack);
        //printString("\n");
        if (!stack) return -1;
    }
    /*printString("alocirani stek u tred kriejtu");
    printInteger((uint64)stack);
    printString("\n");*/

    arg.ptr5 = (uint64)stack;
    callAbi(arg);

    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}

int time_sleep(time_t time){
    if (time == 0) return 0;
    static Arguments arg;
    arg.ptr1 = 0x31;
    arg.ptr2 = time;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}

int thread_exit(){
    static Arguments arg;
    arg.ptr1 = 0x12;
    arg.ptr2 = 0;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;

}



int sem_open(sem_t* handle, unsigned init){
    static Arguments arg;
    arg.ptr1 = 0x21;
    arg.ptr2 = (uint64) handle;
    arg.ptr3 = (uint64) init;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;

}
int sem_close(sem_t id){
    static Arguments arg;
    arg.ptr1 = 0x22;
    arg.ptr2 = (uint64) id;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}


int sem_wait(sem_t id){
    static Arguments arg;
    arg.ptr1 = 0x23;
    arg.ptr2 = (uint64) id;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;

}

int sem_signal(sem_t id){
    static Arguments arg;
    arg.ptr1 = 0x24;
    arg.ptr2 = (uint64) id;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}

void putc(char c){
    static Arguments arg;
    arg.ptr1 = 0x42;
    arg.ptr2 = c;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
}

char getc(){
    static Arguments arg;
    arg.ptr1 = 0x41;
    arg.ptr2 = 0;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}

int getThreadId(){
    static Arguments arg;
    arg.ptr1 = 0x18;
    arg.ptr2 = 0;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);
    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;

}
int thread_create_CPPI(thread_t* handle, void(*start_routine)(void*), void* argum){
    static Arguments arg;
    arg.ptr1 = 0x16;
    arg.ptr2 = (uint64) handle ;
    arg.ptr3 = (uint64) start_routine;
    arg.ptr4 = (uint64) argum;
    void* stack;
    if(!start_routine) {
        stack = nullptr;
    } else{
        stack =  MemoryAllocator::getInstance()->memAlloc(DEFAULT_STACK_SIZE);
        //printString("Adresa steka (syscall_c.cpp): ");
        //printInteger((uint64)stack);
        //printString("\n");
        if (!stack) return -1;
    }
    /*printString("alocirani stek u tred kriejtu");
    printInteger((uint64)stack);
    printString("\n");*/

    arg.ptr5 = (uint64)stack;
    callAbi(arg);

    int val;
    __asm__ ("mv %[val], a0" : [val] "=r" (val) );
    return (int) val;
}

void putInScheduler(thread_t trd){
    static Arguments arg;
    arg.ptr1 = 0x17;
    arg.ptr2 = (uint64) trd;
    arg.ptr3 = 0;
    arg.ptr4 = 0;
    arg.ptr5 = 0;
    callAbi(arg);

}