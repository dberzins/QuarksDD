// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QMEMORY_H)

#include "Common.h"

namespace QuarksDD {
    #define MEMORY_STATS

    // Defines from: Handmade Hero
    #define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

    #define Kilobytes(value) (value)*1024LL)
    #define Megabytes(value) (Kilobytes(value)*1024LL)
    #define Gigabytes(value) (Megabytes(value)*1024LL)
    #define Terabytes(value) (Gigabytes(value)*1024LL)

    typedef size_t MemorySize;
    

    #define ZeroStruct(instance) ZeroSize(sizeof(instance), &(instance))
    #define ZeroArray(count, pointer) ZeroSize(count*sizeof((pointer)[0]), pointer)
	internal void ZeroSize(MemorySize size, void *ptr)
    {
        uint8 *byte = (uint8 *)ptr;
        while(size--) {
            *byte++ = 0;
        }
    }

    struct MemoryBlock {
        // Data
        bool32 initialized;

        MemorySize size;
        MemorySize used;
        uint8* base;

        MemoryBlock *prev;
        MemoryBlock *next;

        // Operations
        bool32 Init(MemorySize blockSize, bool32 zero = true);
        void Free();
        void* Push(MemorySize pushSize);
    };

    struct MemoryBlockIndex {
        bool32 exist;
        uint32 index;
        MemoryBlock* block;
    };

    #define ArenaPushStruct(arena, Type) (Type*) ArenaPushSize_(arena, sizeof(Type))
    #define ArenaPushArray(arena, count, Type) (Type*) ArenaPushSize_(arena, count * sizeof(Type))
    #define ArenaPushSize(arena, size) (void*) ArenaPushSize_(((MemoryArena*)arena), size)
    #define ArenaPushSize_(arena, size) (void*) arena->Push(size)
    
    struct MemoryArena {
        // Data
        bool32 initialized;
        real32 extend;
        uint32 count;
        bool32 zero;
        MemoryBlock* block;

        // Operations
        bool32 Init(MemorySize size = 0, bool32 zero = true, real32 extend = 1.0f);
        void Free(MemoryBlock* freeBlock);
        void Free();
        bool32 Extend(MemorySize size);
        bool32 FitExtend(MemorySize size);

        void* Push(MemorySize size);
        MemoryBlockIndex FindBlock(uint8* source);
        bool32 BlockContains(MemoryBlock* testBlock, uint8* source, MemorySize size);
        void* Resize(uint8* source, MemorySize size, MemorySize newSize);
    };


 struct StatsMemoryBlock {
        void* memory;
        MemorySize size;
        StatsMemoryBlock* next;
    };

    struct MemoryStats {
        uint32 alocated;
        uint32 deallocated;
        MemorySize totalSize;

        StatsMemoryBlock* aBlocks;
        StatsMemoryBlock* dBlocks;

        void Reslove() {
            StatsMemoryBlock* add;
            StatsMemoryBlock* del;

            add = aBlocks;
            del = dBlocks;

            bool32 match = false;
            while (add) {
                while (del) {
                    if (&add->memory == &del->memory) {
                        match = true;
                        // TODO(dainis): Implement stats tracking for:
                        //  * Blocks in add list but not in del list
                        //  * Blocks in del list but not in add list
                        //  * Multiple blocks in add list
                        //  * Multiple blocks in del list
                    }   
                    del = del->next;
                }
                add = add->next;
            }
        }

        void TrackAllocate(void* p, MemorySize size) {
            alocated++;
            totalSize += size;
        
            StatsMemoryBlock* newBlock = (StatsMemoryBlock*)malloc(sizeof(StatsMemoryBlock));
            newBlock->memory = p;
            newBlock->size = size;

            if (aBlocks) {
                newBlock->next = aBlocks;
                aBlocks = newBlock;
            }
            else {
                aBlocks = newBlock;
            }
        }

        void TrackDeallocate(void* p, MemorySize size) {
            StatsMemoryBlock* newBlock = (StatsMemoryBlock*)malloc(sizeof(StatsMemoryBlock));
            newBlock->memory = p;
            newBlock->size = 0;

            if (dBlocks) {
                newBlock->next = dBlocks;
                dBlocks = newBlock;
            }
            else {
                dBlocks = newBlock;
            }

        }

        void Free() {
            while (aBlocks) {
                free(aBlocks);
                aBlocks = aBlocks->next;
            }

            aBlocks = NULL;

            while (dBlocks) {
                free(dBlocks);
                dBlocks = dBlocks->next;
            }

            dBlocks = NULL;

        }
    };
    extern MemoryStats memStats; 


    #define Allocate(size) Allocate_(size, __FILE__, __LINE__, __FUNCTION__)
    internal void* Allocate_(MemorySize size, const char *file, int line, const char *func)
    {
        void *ptr = malloc(size);
        
        #ifdef MEMORY_STATS
        // printf("Allocated = %s, %i, %s, %p[%li]\n", file, line, func, ptr, size);
        memStats.alocated++;
        memStats.totalSize += size;
        #endif
     
        return ptr;
    }

    #define Reallocate(pointer, size) Reallocate_(pointer, size, __FILE__, __LINE__, __FUNCTION__)
    internal void* Reallocate_(void* ptr, MemorySize size, const char *file, int line, const char *func)
    {
        // NOTE: http://en.cppreference.com/w/c/memory/realloc
        // * On success, returns the pointer to the beginning of newly allocated memory. To avoid a memory leak, 
        // the returned pointer must be deallocated with free() or realloc(). The original pointer ptr is invalidated 
        // and any access to it is undefined behavior (even if reallocation was in-place). 
        // * On failure, returns a null pointer. The original pointer ptr remains valid and may need 
        // to be deallocated with free() or realloc().
        void *newPtr = realloc(ptr, size);
        if (newPtr) {

            #ifdef MEMORY_STATS
            // printf("Deallocated = %s, %i, %s, %p\n", file, line, func, ptr);
            // printf("Reallocated = %s, %i, %s, %p[%li]\n", file, line, func, newPtr, size);
            memStats.deallocated++; // ols ptr memory is freed by realloc call
            memStats.alocated++;
            memStats.totalSize += size;
            #endif
        }

        return newPtr;
    }


    #define Deallocate(pointer) Deallocate_(pointer, __FILE__, __LINE__, __FUNCTION__)
    internal void Deallocate_(void* ptr, const char *file, int line, const char *func)
    {
        #ifdef MEMORY_STATS
        // printf("Deallocated = %s, %i, %s, %p\n", file, line, func, ptr);
        memStats.deallocated++;
        #endif

        free(ptr);
        ptr = NULL;
    }
} // namespace
#define QMEMORY_H
#endif