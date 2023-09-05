#ifndef SLAB_HPP
#define SLAB_HPP


#include "../lib/hw.h"



class SlabAllocator{
public:
    static SlabAllocator& getInstance();

    SlabAllocator(const SlabAllocator&) = delete;
    SlabAllocator(SlabAllocator&) = delete;
    void operator=(const SlabAllocator&) = delete;
    void operator=(const SlabAllocator) = delete;

private:
    SlabAllocator();

    class
};



#endif