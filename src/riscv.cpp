//
// Created by os on 8/8/22.
//

#include "../h/riscv.h"

KernelBuffer* Riscv::outputBuf;
KernelBuffer* Riscv::inputBuf;
void Riscv::backToUser(){
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap()
{
    __asm__ volatile ("addi sp,sp,-64");
    __asm__ volatile ("sd a0,0*8(sp)");
    __asm__ volatile ("sd a1,1*8(sp)");
    __asm__ volatile ("sd a2,2*8(sp)");
    __asm__ volatile ("sd a3,3*8(sp)");
    __asm__ volatile ("sd a4,4*8(sp)");
    __asm__ volatile ("sd a5,5*8(sp)");
    __asm__ volatile ("sd a6,6*8(sp)");
    __asm__ volatile ("sd a7,7*8(sp)");

    uint64 volatile scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
       uint64 volatile sepc = r_sepc() + 4;
       uint64 volatile sstatus = r_sstatus();

        uint64 volatile cause;
        __asm__ volatile ("ld %[cause], 0*8(sp)" : [cause] "=r" (cause));
        if (cause == 0x1) {

            uint64 size;
            __asm__ volatile ("ld %[size], 1*8(sp)" : [size] "=r" (size));

            void* mem = MemoryAllocator::getInstance()->memAlloc(size);

            __asm__ volatile ("mv a0, %[mem]":: [mem] "r"(mem));

        } else if (cause == 0x02) {

            uint64 ptr;
            __asm__ volatile ("ld %[ptr], 1*8(sp)" : [ptr] "=r" (ptr));

            int val = MemoryAllocator::getInstance()->memDealloc((void*)ptr);

            __asm__ volatile ("mv a0, %[val]":: [val] "r"(val));

        } else if (cause == 0x11) {
            uint64 handle;
            uint64 func;
            uint64 arg;
            uint64 stack;

            __asm__ volatile ("ld %[handle], 1*8(sp)" : [handle] "=r" (handle));
            __asm__ volatile ("ld %[func], 2*8(sp)" : [func] "=r" (func));
            __asm__ volatile ("ld %[arg], 3*8(sp)" : [arg] "=r" (arg));
            __asm__ volatile ("ld %[stack], 4*8(sp)" : [stack] "=r" (stack));



            TCB* hnd = TCB::createThread((TCB::Body)func, (void*)arg, (uint64*) stack, false);
            TCB** dpt = (TCB**) handle;
            *dpt = hnd;

            int ret = 0;
            if (!hnd){
                ret = -1;
            }
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));


        } else if (cause == 0x12) {
            int ret = TCB::threadExit();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
        }

        else if (cause == 0x13){
            TCB::yield();
        }

        else if (cause == 0x16){
            uint64 handle;
            uint64 func;
            uint64 arg;
            uint64 stack;

            __asm__ volatile ("ld %[handle], 1*8(sp)" : [handle] "=r" (handle));
            __asm__ volatile ("ld %[func], 2*8(sp)" : [func] "=r" (func));
            __asm__ volatile ("ld %[arg], 3*8(sp)" : [arg] "=r" (arg));
            __asm__ volatile ("ld %[stack], 4*8(sp)" : [stack] "=r" (stack));




            TCB* hnd = TCB::createThread((TCB::Body)func, (void*)arg, (uint64*) stack, true);
            TCB** dpt = (TCB**) handle;
            *dpt = hnd;


            int ret = 0;
            if (!hnd){
                ret = -1;
            }
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
        }

        else if (cause == 0x17){
            uint64 handle;
            __asm__ volatile ("ld %[handle], 1*8(sp)" : [handle] "=r" (handle));
            Scheduler::put((TCB*)handle);
        }
        else if (cause == 0x18){
            int ret = TCB::running->getId();
            TCB::yield();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
            //TCB::yield();

        }
        else if (cause == 0x21){
            //sem open
            uint64 handle;
            uint64 init;
            __asm__ volatile ("ld %[handle], 1*8(sp)" : [handle] "=r" (handle));
            __asm__ volatile ("ld %[init], 2*8(sp)" : [init] "=r" (init));
            SemCB* hnd = SemCB::semOpen(init);
            SemCB** dpt = (SemCB**) handle;
            *dpt = hnd;

            int ret = 0;
            if (!hnd){
                ret = -1;
            }
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));

        }

        else if (cause == 0x22) {
            uint64 id;
            __asm__ volatile ("ld %[id], 1*8(sp)" : [id] "=r" (id));
            SemCB* semCb = (SemCB*) id;
            int ret = semCb->semClose();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
        }
        else if (cause == 0x23){
            uint64 id;
            __asm__ volatile ("ld %[id], 1*8(sp)" : [id] "=r" (id));
            SemCB* semCb = (SemCB*) id;
            int ret = semCb->wait();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));

        }
        else if (cause == 0x24) {
            uint64 id;
            __asm__ volatile ("ld %[id], 1*8(sp)" : [id] "=r" (id));
            SemCB* semCb = (SemCB*) id;
            int ret = semCb->signal();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));

        }

        else if (cause == 0x31){
            time_t time;
            __asm__ volatile ("ld %[time], 1*8(sp)" : [time] "=r" (time));
            int ret = TCB::threadSleep(time);
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
        }
        else if (cause == 0x41) {
            char ret = inputBuf->get();
            __asm__ volatile ("mv a0, %[ret]":: [ret] "r"(ret));
        }

        else if(cause == 0x42){
            //putc
            char chr;
            __asm__ volatile ("ld %[chr], 1*8(sp)" : [chr] "=r" (chr));
            outputBuf->put(chr);
        }
        else if (cause == 0x43){
            while(true){
                while(*reinterpret_cast<char*>(CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT){
                    char chr = outputBuf->get();
                    *(char*) CONSOLE_TX_DATA = chr;
                }

            }
        }

        __asm__ volatile ("sd a0, 10*8(s0)");

        w_sepc(sepc);
        w_sstatus(sstatus);
    }
    else if (scause == 0x8000000000000001UL)
    {
        mc_sip(SIP_SSIP);
        //printString("asinhrona promena konteksta");
        TCB::timeSliceCounter++;
        TCB::timeToWakeUp++;
        TCB::timeToWakeUp = Scheduler::wakeUp(TCB::timeToWakeUp);
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {

        mc_sip(SIP_SEIP);
        int plic = plic_claim();
        if (plic == CONSOLE_IRQ) {
            while(*reinterpret_cast<char*>(CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT){
                uint64 volatile sepc = r_sepc();
                uint64 volatile sstatus = r_sstatus();
                char chr = *(char*) CONSOLE_RX_DATA;
                inputBuf->put(chr);
                w_sstatus(sstatus);
                w_sepc(sepc);
            }
        }

        plic_complete(plic);



        //console_handler();
        //printString("TAJMER");
    }
    else if (scause == 0x0000000000000005UL){

        printString("Nedozvoljena adresa citanja\n");
        printString("sepc: ");
        printInt(r_sepc());
        printString("\n");

    } else if (scause == 0x0000000000000007UL) {

        printString("Nedozvoljena adresa upisa\n");
        printString("sepc: ");
        printInt(r_sepc());
        printString("\n");

    } else if (scause == 0x0000000000000002UL) {

        printString("Nedozvoljena instrukcija\n");
        printString("sepc: ");
        printInt(r_sepc());
        printString("\n");

    }
    else
    {
        //printString("else grana \n");// unexpected trap cause
    }
    //Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile ("addi sp,sp,64");
}

void Riscv::outputBufConsumer(void *arg) {
    while(true){
        while(*reinterpret_cast<char*>(CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT){
            char chr = outputBuf->get();
            *(char*) CONSOLE_TX_DATA = chr;
        }

    }


}