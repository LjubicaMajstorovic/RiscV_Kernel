#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/riscv.h"
//#include "../h/print.h"
#include "../h/syscall_c.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/workers.h"
//#include "../h/ThreadSleep_C_API_test.hpp"
//#include "../h/ConsumerProducer_C_API_test.h"
//#include "../h/Threads_CPP_API_test.hpp"
//#include "../h/ConsumerProducer_CPP_API_test.hpp"
//#include "../h/ConsumerProducer_CPP_Sync_API_test.hpp"
#include "../h/Threads_C_API_test.hpp"
#include "../h/BuddyAllocator.hpp"
Semaphore* sem;
class WorkerA: public PeriodicThread {
public:
    WorkerA(time_t t):PeriodicThread(t) {}

    void periodicActivation() override {
        putc('A');
        putc('\n');
    }
};
class WorkerB: public PeriodicThread{
public:
    WorkerB(time_t t):PeriodicThread(t) {}

    void periodicActivation() override {
        putc('B');
        putc('\n');
    }
};

class WorkerC: public PeriodicThread {
public:
    WorkerC(time_t t):PeriodicThread(t) {}

    void periodicActivation() override {
        putc('C');
        putc('\n');
        }

};

void userMain(void* arg){
    //Threads_C_API_test();
    //Threads_CPP_API_test();
    //testSleeping();
    //producerConsumer_C_API();
    //producerConsumer_CPP_Sync_API();
    //ConsumerProducerCPP::testConsumerProducer();
    //producerConsumer_C_API();
    /*sem = new Semaphore();
    WorkerA* a = new WorkerA(5);
    WorkerB* b = new WorkerB(5);
    WorkerC* c = new WorkerC(5);

    a->start();
    b->start();
    c->start();

    while(getc()!= 'k'){

    }
    a->stopThread();
    while(getc()!= 'k'){


    }
    b->stopThread();
    while(getc()!= 'k'){

    }
    c->stopThread();*/

    BuddyAllocator* ba = BuddyAllocator::getInstance();
    void* a = ba->alloc(3);
    printString("a\n");
    printInt((unsigned long long)a);
    void* b = ba->alloc(5);
    printString("\nb\n");
    printInt((unsigned long long ) b);
    void* c = ba->alloc(8);
    printString("\nc\n");
    printInt((unsigned long long )c);
    printString("\n");
    void* d = ba->alloc(10000);
    printString("\nd\n");
    printInt((unsigned long long) d);
    printString("\n");
    void* e = ba->alloc(10000);
    printString("\ne\n");
    printInt((unsigned long long) e);
    printString("\n");
    ba->free(a, 3);
    ba->free(b, 5);
    void* f = ba->alloc(3);
    printString("\nf\n");
    printInt((unsigned long long) f);
    printString("\n");




}
void main(){


    Riscv::outputBuf = new KernelBuffer();
    Riscv::inputBuf = new KernelBuffer();


    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);





    thread_t mejnptr;
    thread_create(&mejnptr, nullptr, nullptr);
    TCB::running = (TCB*)mejnptr;

    thread_t ajdlp;
    thread_create(&ajdlp,  &idle, nullptr);

    thread_t outputcons;
    thread_create(&outputcons, &Riscv::outputBufConsumer, nullptr);

    thread_t usermain;
    thread_create(&usermain, &userMain, nullptr);


    /*thread_t a;
    thread_create(&a, &A, nullptr);
    thread_t b;
    thread_create(&b, &B, nullptr);

    thread_t c;
    thread_create(&c, &C, nullptr);*/




    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    while(!((TCB*)usermain)->isFinished() || Riscv::outputBuf->size() > 0){
        thread_dispatch();
    }










    delete Riscv::inputBuf;




}

