#include "../h/Scheduler.h"
#include "../h/Tcb.hpp"
TCB* Scheduler::head = nullptr;
TCB* Scheduler::tail = nullptr;
TCB* Scheduler::headSleep = nullptr;

Scheduler* Scheduler::getInstance() {
    static Scheduler scheduler;
    return &scheduler;
}

void Scheduler:: put(TCB* tcb){


    if(!tail){
        tail = tcb;
        head = tcb;
        return;
    }
    if(!tail->getPrev()){
        head = tcb;
        head->setPrev(nullptr);
        head->setNext(tail);
        tail->setPrev(head);
        return;
    }
    tail->getPrev()->setNext(tcb);
    tcb->setPrev(tail->getPrev());
    tcb->setNext(tail);
    tail->setPrev(tcb);

}

TCB* Scheduler::get() {
    if (head == nullptr) {
        //ovde treba da se vrati pokazivac na idle;
        return nullptr;
    }
    TCB* ret = head;
    if (head->getNext()) {
        head->getNext()->setPrev(nullptr);
        head = head->getNext();
        ret->setNext(nullptr);
        return ret;
    }
    return ret;
}

void Scheduler::putToSleep() {
    TCB* tcb = TCB::running;

    if(!headSleep){

        headSleep = tcb;
        tcb->setSleep(true);
        TCB::yield();

        return;
    }
    int dif = tcb->getSleepTime() - headSleep->getSleepTime();
    if (dif >= 0) {
        tcb->setSleepTime(dif);

        if(!headSleep->getNext() ){
            headSleep->setNext(tcb);
            tcb->setPrev(headSleep);
        }
        else{
            TCB* cur = headSleep->getNext();
            TCB* prev = nullptr;
            while(cur){
                if (cur->getSleepTime() > tcb->getSleepTime()){
                    cur->getPrev()->setNext(tcb);
                    tcb->setPrev(cur->getPrev());
                    cur->setPrev(tcb);
                    tcb->setNext(cur);

                    break;
                }
                tcb->setSleepTime(tcb->getSleepTime()-cur->getSleepTime());
                prev = cur;
                cur = cur->getNext();
            }
            if(!cur) {
                prev->setNext(tcb);
                tcb->setPrev(prev);
            }
        }

    }
    else{
        headSleep->setSleepTime(-dif);
        tcb->setNext(headSleep);
        headSleep->setPrev(tcb);
        headSleep = tcb;
    }
    tcb->setSleep(true);
    TCB::yield();


}

int Scheduler::wakeUp(time_t twp) {
    time_t timeToWakeUp = twp;
    if(!headSleep) return 0;
    time_t headTime = headSleep->getSleepTime();
    if (headTime <= timeToWakeUp) {
        timeToWakeUp = 0;

        if (headSleep->getNext()) headSleep->getNext()->setPrev(nullptr);
        TCB *old = headSleep;
        headSleep = headSleep->getNext();
        old->setNext(nullptr);
        old->setSleep(false);
        Scheduler::put(old);

        while (true) {
            if (!headSleep) return timeToWakeUp;
            if (headSleep->getSleepTime() == 0) {
                if (headSleep->getNext())headSleep->getNext()->setPrev(nullptr);
                TCB *old = headSleep;
                headSleep = headSleep->getNext();
                old->setNext(nullptr);
                old->setSleep(false);
                Scheduler::put(old);
            } else {
                break;
            }

        }
    }

    else {
        headSleep->setSleepTime(--headTime);
    }



    return timeToWakeUp;
}

