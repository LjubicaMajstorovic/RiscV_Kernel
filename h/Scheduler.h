//
// Created by os on 8/10/22.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "../lib/hw.h"

class TCB;

class Scheduler{


public:
    static Scheduler* getInstance();
    //static void initialize(TCB* idle);
    Scheduler(const Scheduler&) = delete;
    void operator=(const Scheduler&) = delete;
    static void put(TCB* tcb);
    static TCB* get();
    static void putToSleep();
    static int wakeUp(time_t timeToWakeUp);

private:

    Scheduler() = default;


    static TCB* head;
    static TCB* tail;
    static TCB* headSleep;



};
#endif