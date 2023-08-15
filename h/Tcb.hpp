#ifndef TCB_HPP
#define TCB_HPP
#include "../lib/hw.h"
#include "Scheduler.h"



// Thread Control Block
class TCB
{
public:

    ~TCB(){
        //TCB::thread_exit();
        TCB::operator delete[](stack);

    }

    TCB* getNext(){
        return next;
    }

    TCB* getPrev(){
        return prev;
    }

    void setNext(TCB* nxt){
        next = nxt;
    }
    void setPrev(TCB* prv) {
        prev = prv;
    }

    int getId(){
        return myId;
    }

    static int threadExit();

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    bool isAsleep() { return asleep; }

    void setSleep(bool value) { asleep = value; }

    time_t getSleepTime() { return sleepTime; }

    void setSleepTime(time_t sleep) { sleepTime = sleep; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB* createThread(Body body, void* arg, uint64* stk, bool istrdCPP);

    static void yield();

    static int threadSleep(time_t time);

    static void* operator new(size_t bytes);

    static void operator delete(void* pointer);

    static void* operator new[] (size_t bytes);

    static void operator delete[] (void* ptr);

    static TCB *running;

    uint64 getStack(){
        return (uint64) stack;
    }
    uint64 getRA(){
        return context.ra;
    }
    uint64 getSP(){
        return context.sp;
    }

private:
    struct Context
    {
        uint64 ra;
        uint64 sp ;
    };
    TCB* prev = nullptr;
    TCB* next = nullptr;

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    bool asleep;
    time_t sleepTime;
    void* arguments;
    int myId;
    static int generatorId;

    TCB(Body bdy, uint64 tSlice, void* arg, uint64* stk) :
            body(bdy),
            stack(stk),
            context({(uint64) &threadWrapper,
                     stk ? (uint64) stk + DEFAULT_STACK_SIZE : 0,
                    }),
            timeSlice(tSlice),
            finished(false),
            blocked(false),
            asleep(false),
            sleepTime(0),
            arguments(arg),
            myId(++generatorId)
            {

        if (body != nullptr) {

            Scheduler::put(this); }
    }

    TCB(Body bdy, uint64 tSlice, void* arg, uint64* stk, bool istrdCPP) :
            body(bdy),
            stack(stk),
            context({(uint64) &threadWrapper,
                     stk ? (uint64) stk + DEFAULT_STACK_SIZE : 0,
                    }),
            timeSlice(tSlice),
            finished(false),
            blocked(false),
            asleep(false),
            sleepTime(0),
            arguments(arg),
            myId(++generatorId)
            {



    }



    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 timeToWakeUp;
    static uint64 constexpr TIME_SLICE = 2;
};
#endif