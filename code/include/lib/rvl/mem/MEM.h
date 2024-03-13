#pragma once
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *mpPrev;
    void *mpNext;
} MEMLink;

typedef struct {
    void *mpHead;
    void *mpTail;
    u16 mCount;
    u16 mOffset;
} MEMList;

struct OSMutex; // TODO: fill in struct

typedef struct {
    u32 mTag;
    MEMLink mpLink;
    MEMList mpList;
    void *mpHeapStart;
    void *mpHeapEnd;
    OSMutex *mpMutex;
} MEMiHeapHeader;

typedef struct MEMAllocator MEMAllocator;
typedef void* (*MEMFuncAllocatorAlloc)(MEMAllocator* pAllocator, u32 size);
typedef void (*MEMFuncAllocatorFree)(MEMAllocator* pAllocator, void* memBlock);

typedef struct {
    MEMFuncAllocatorAlloc alloc;
    MEMFuncAllocatorFree free;
} MEMAllocatorFunc;

struct MEMAllocator {
    MEMAllocatorFunc* funcs;
    void* heap;
    u32 param1;
    u32 param2;
};

#ifdef __cplusplus
}
#endif
