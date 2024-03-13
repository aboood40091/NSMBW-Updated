#pragma once
#include <types.h>
#include <lib/egg/heap/eggHeap.hpp>
#include <lib/rvl/mem/MEM.h>

namespace EGG {

class Allocator : public MEMAllocator {
    public:
        void* vtable;
        Heap* mHeap;
        s32 mAlign;
};

} // namespace EGG
