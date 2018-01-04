// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Memory.h"
#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

// MemoryBlock Operations
bool32 MemoryBlock::Init(MemorySize blockSize, bool32 zero) {
    bool32 result = false;
    if (!initialized) {
        base = (uint8*)Allocate(blockSize);
    
        if (zero) {
            ZeroSize(blockSize, base);
        }
    
        size = blockSize;
        used = 0;
        initialized = true;
        result = true;
    }

    return result;
}

void MemoryBlock::Free() {
    if (initialized) {
        Deallocate(base);
    }
}

void* MemoryBlock::Push(MemorySize pushSize)
{
    void* result = NULL;
    if ((size - used) >= pushSize) {
        result = base + used;
        used += pushSize;
    }
    return result;
}

// MemoryArena Operations
bool32 MemoryArena::Init(MemorySize size, bool32 zero, real32 extend) {
    bool32 result = false;

    if (!initialized) {
        
        this->zero = zero;
        this->extend = extend;

        if (size > 0) {
            block = (MemoryBlock*) Allocate(sizeof(MemoryBlock));
            *block = {};
            block->Init(size, zero);

            count++;
        }
        initialized = true;
        result = true;
    }

    return result;
}

void MemoryArena::Free(MemoryBlock* freeBlock) {
    if (block) {
        MemoryBlock* iter = block;
        while (iter) {
            if (iter ==  freeBlock) {

                if (iter->next && iter->prev) {
                    iter->prev->next = iter->next;
                    iter->next->prev = iter->prev;
                }
                else if (iter->next) {
                    iter->next->prev = NULL;
                }
                else if (iter->prev) {
                    iter->prev->next = NULL;
                    block = iter->prev;
                }
                else {
                    block = NULL;
                }

                iter->Free();
                Deallocate(iter);
                iter = NULL;
                count--;
                break;
            }
            iter = iter->prev;
        }
    }
}

void MemoryArena::Free() {
    if (initialized) {
        while (block) {
            block->Free();

            MemoryBlock* delBlock  = block;
            block = block->prev;

            Deallocate(delBlock);
        }
        *this = {};
    }
}

bool32 MemoryArena::Extend(MemorySize size) {
    bool32 result = false;

    if (initialized) {
        // If size greater than current block size then use size as base for new block
        size = (block && (block->size > size)) ? block->size : size;
        size = (MemorySize)((real32)size* extend);

        MemoryBlock* newBlock = (MemoryBlock*)Allocate(sizeof(MemoryBlock));
        *newBlock = {};
        newBlock->Init(size, zero);

        if (block) {
            // Put new block in forward of chain
            newBlock->prev = block;
            newBlock->next = NULL;
            block->next = newBlock;
        }
        else {
            // First block
            newBlock->prev = NULL;
            newBlock->next = NULL;
        }

        block = newBlock;
        count++;

        result = true;
    }

    return result;        
}

bool32 MemoryArena::FitExtend(MemorySize size) 
{
    bool32 result = false;

    if (block && ((block->size - block->used) >= size)) {
        // Fit
        result = true;
    }
    else {
        result = Extend(size);
    }

    return result;
}

void* MemoryArena::Push(MemorySize size)
{
    if (!block || ((block->size - block->used) < size)) {
        if (!Extend(size)) {
            return NULL;
        }
    }

    void *result = block->base + block->used;
    block->used += size;
    
    return result;
}

MemoryBlockIndex MemoryArena::FindBlock(uint8* source) {
    MemoryBlockIndex result = {};

    MemoryBlock* searchBlock = block;
    uint32 index = count-1;
    while (searchBlock) {
        if(source >= searchBlock->base &&  source < (searchBlock->base + searchBlock->size)) {
            result.exist = true;
            result.index = index;
            result.block = searchBlock;
            break;
        }
        --index;
        searchBlock = searchBlock->prev;
    }

    return result;
}

bool32 MemoryArena::BlockContains(MemoryBlock* testBlock, uint8* source, MemorySize size) {
    bool32 result = false;
    
    if (source >= testBlock->base && 
        source < (testBlock->base + testBlock->size) &&
        (source + size) <= (testBlock->base + testBlock->size) 
    ) {
        result = true;
    } 
    
    return result;
}

// TODO(dainis): If current block is not used and its size is less than newSize 
// then dealocate new block and delete old one
void* MemoryArena::Resize(uint8* source, MemorySize size, MemorySize newSize) {
    void* result = NULL;

    if (!block) {
        // First block - just extend
        if (Extend(newSize)) {
            result = block->base;
        }
    } 
    else {
        MemoryBlockIndex blockIndex = FindBlock(source);

        if (blockIndex.exist && BlockContains(blockIndex.block, source, size)) {
        
            if ((blockIndex.block->size - blockIndex.block->used) >= newSize) {
                // Fits in same block
                result = blockIndex.block->Push(newSize);
                if (zero) {
                    ZeroSize(newSize, result);
                }
            }
        }

        // We need allocate new block
        if (!result && Extend(newSize)) {
            result = block->base;
        }

        // TODO(dainis): do some block chain remove or some kind of FreeList
        // For now allow some memory waste for old blocks or block parts

        if (result) {
            memmove(result, source, size);
        }
    }
    return result;
}

} // Namespace