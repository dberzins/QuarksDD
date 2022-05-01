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
        Assert(base);
        if (base) {
            allocated = true;

            if (zero) {
                ZeroSize(blockSize, base);
            }
        
            size = blockSize;
            used = 0;
            initialized = true;
            result = true;
        }
    }

    return result;
}

bool32 MemoryBlock::Init(uint8* memory, MemorySize blockSize, bool32 zero) {
    bool32 result = false;
    if (!initialized) {
        base = memory;
        allocated = false;

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
    
        MemoryArena* iter = child;
        while (iter) {
            MemoryArena* childArena = iter;
            iter= iter->next;
            childArena->Free();
        }
        
        if (allocated) {
            Deallocate(base);
        }
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
            Assert(block);
            if (block) {
                *block = {};
                block->Init(size, zero);

                count++;
            }
            else {
                return false;
            }
        }
        initialized = true;
        result = true;
    }

    return result;
}

bool32 MemoryArena::Init(MemoryBlock* arenaBlock, bool32 zero, real32 extend) {
    bool32 result = false;

    if (!initialized) {
        if (arenaBlock && arenaBlock->initialized) {
            this->zero = zero;
            this->extend = extend;

            block = arenaBlock;

            count++;
            initialized = true;
            result = true;
        }
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

            if (delBlock->allocated) {
                Deallocate(delBlock);
            }
        }
        *this = {};
    }
}

bool32 MemoryArena::Extend(MemorySize size) {
    bool32 result = false;

    if (initialized) {
        // If size greater than current block size then use size as base for new block
        size = (block && (block->size > size)) ? block->size : size;
        size = (MemorySize)((real64)size* (real64)extend);

        MemoryBlock* newBlock = (MemoryBlock*)Allocate(sizeof(MemoryBlock));
        Assert(newBlock);

        if (newBlock) {
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

void* MemoryArena::Resize(uint8* source, MemorySize size, MemorySize newSize) {
    void* result = NULL;

    if (!block) {
        // First block - just extend
        if (Extend(newSize)) {
            result = block->base;
        }
    }
    else if (block->size < newSize && !block->allocated && !block->used) {
        // If current block is not used and its size is less than newSize 
        // then alocate new block and delete old one
        MemoryBlock* prev = block->prev;
        block->Free();
        block = NULL;
        if (Extend(newSize)) {
            block->prev = prev;
            result = block->base;
        }
    } 
    else {
        ++resizeCount;
        MemoryBlockIndex blockIndex = FindBlock(source);
        MemoryBlock* sourceBlock = blockIndex.block;

        if (blockIndex.exist && BlockContains(sourceBlock, source, size)) {
        
            if ((sourceBlock->size - sourceBlock->used) >= newSize) {
                // Check if new size is at the end of block then reuse old space
                if (sourceBlock->used == ((source + size) - sourceBlock->base))  {
                    // Last block chunk
                    MemorySize delta = newSize - size;
                    if (sourceBlock->size >=  (sourceBlock->used + delta)) {
                        // Resize delta fits in current block
                        
                        // Push size of delta
                        sourceBlock->Push(delta);
                        result = source;
                        ++simpleResizeCount;
                        return result;
                    }
                }

                // Fits in same block
                result = sourceBlock->Push(newSize);
                if (zero) {
                    ZeroSize(newSize, result);
                }
            }
        }

        // We need allocate new block
        if (!result && Extend(newSize)) {
            result = block->Push(newSize);
        }


        if (result) {
            memmove(result, source, size);
            
            if (blockIndex.exist) {
                // TODO(dainis): do some block chain remove or some kind of FreeList reuse
                FreeChunk(sourceBlock, source, size);
            }
        }
    }
    return result;
}

MemorySnapshot MemoryArena::CreateSnapshot() {
    MemorySnapshot snapshot = {};
    snapshot.arena = this;
    snapshot.block = block;
    snapshot.used = block->used;
    snapshot.lastChild = block->child;
    snapshot.lastChildCount = block->childCount;

    ++snapshots;

    return snapshot;
}

void MemoryArena::Rollback(MemorySnapshot snapshot) {
    Assert(snapshots > 0)
    Assert(snapshot.arena && snapshot.arena == this && snapshot.block);
    Assert(block)

    if (snapshots > 0 && snapshot.arena && snapshot.arena == this && snapshot.block && block) {
        // Remove all blocks till snapshot block
        MemoryBlock* iter = block;
        while (iter && iter != snapshot.block) {
            MemoryBlock* delBlock = iter;
            iter = block->prev;
            Free(delBlock);
        }

        // Rollback block child arenas
        if (iter->childCount > snapshot.lastChildCount) {
            
            while (iter->child && iter->child != snapshot.lastChild) {
                MemoryArena* delArena = iter->child;
                iter->child = iter->child->next;
                delArena->Free();        
            } 

            iter->child = snapshot.lastChild;
            iter->childCount = snapshot.lastChildCount;
        }

        Assert(iter == snapshot.block);
        Assert(snapshot.used <= iter->used);
        if (snapshot.used <= iter->used) {
            iter->used  = snapshot.used;
            --snapshots;
        }

    }
}

MemoryArena* MemoryArena::CreateChildArena(MemorySize memSize, bool32 zero) {
    MemoryArena* result = NULL;
    
    if (initialized && memSize > 0) {

        if (!block) {
            Extend(sizeof(MemoryChunk) + sizeof(MemoryBlock) + sizeof(MemoryArena) + memSize);
        }
        
        // Take snapshot of child arean base memory so it it can be palced in feree chunk list on child arena free.
        MemoryChunk snapshotChunk = {};
        snapshotChunk.block = block;
        snapshotChunk.start = block->used;

        // All child arena base memory should be allocated in one block!
        void* childBase = ArenaPushSize(this, sizeof(MemoryChunk) + sizeof(MemoryBlock) + sizeof(MemoryArena) + memSize);

        if (snapshotChunk.block != block) {
            // Didn't fit current block - new block was alocated
            snapshotChunk.block = block;
            snapshotChunk.start = 0;
        }
        snapshotChunk.end = block->used;

        MemoryChunk* childBaseChunk = (MemoryChunk*) childBase;
        *childBaseChunk = snapshotChunk;
        MemoryBlock* childBlock = (MemoryBlock*)((uint8*)childBase + sizeof(MemoryChunk));
        MemoryArena* childArena = (MemoryArena*)((uint8*)childBase + sizeof(MemoryChunk) + sizeof(MemoryBlock));
        uint8* childMemory = (uint8*)childBase + sizeof(MemoryChunk) + sizeof(MemoryBlock) + sizeof(MemoryArena);

        // MemoryBlock* childBlock = ArenaPushStruct(this, MemoryBlock);
        // MemoryArena* childArena = ArenaPushStruct(this, MemoryArena);
        // uint8* childMemory = (uint8*)ArenaPushSize(this, memSize);
        
        *childBlock = {};
        childBlock->Init(childMemory, memSize, zero);

        *childArena = {};
        childArena->Init(childBlock, zero);
        childArena->baseChunk = childBaseChunk;

        if (block->child) {
            childArena->next = block->child;
        }
        block->child = childArena;
        ++block->childCount;
        
        result = childArena;
    }

    return result;
}

bool32 MemoryArena::FreeChildArena(MemoryArena* childArena) {
    bool32 result = false;
    if (childArena) {
        Assert(childArena->baseChunk);
        FreeChunk(childArena->baseChunk);
        childArena->Free();
        result = true;
    }

    return result;
}

// TODO(dainis): Check block child arenas states
bool32 MemoryArena::FreeChunk(MemoryBlock* chunkBlock, uint8* source, MemorySize size) {
    bool32 result = false;

    // NOTE: Add to free list only chunks greater than 
    //  MemoryChunk size to avoid allocations!
    if (size >= sizeof(MemoryChunk)) {
        MemoryChunk* chunk = (MemoryChunk*) source;
        *chunk = {};
        chunk->block = chunkBlock;
        
        chunk->start = source - chunkBlock->base;
        chunk->end = chunk->start + size;

        if (firstFree) {
            chunk->next = firstFree;
            firstFree = chunk;
        }
        else {
            firstFree = chunk;
        }

        ++freeCkunkCount;
        result = true;
    }
    return result;
}

bool32 MemoryArena::FreeChunk(MemoryChunk* chunk) {
    bool32 result = false;

    if (chunk) {
        if (firstFree) {
            chunk->next = firstFree;
            firstFree = chunk;
        }
        else {
            firstFree = chunk;
        }

        ++freeCkunkCount;
        result = true;
    }
    return result;
}



} // Namespace