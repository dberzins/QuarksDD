// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QSTACK_H)

#include "Common.h"
#include "Memory.h"

#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

struct StackItem 
{
    // Data 
    void* data;

    // Operations
    StackItem& operator=(void* data) {
        this->data = data;
        
        return *this;
    }
};

struct Stack
{
    // Data
    bool32 initialized;
    bool32 useHeapAlloc;
    int32 top;
    int32 size;
    MemoryArena *arena;
    StackItem* items;
    
    // Operations
    bool32  Init(int32 stackSize = 4096);
    bool32  Init(MemoryArena *arena, int32 stackSize = 4096);
    void Free();

    StackItem* PopItem();
    void* Pop();
    bool32 Push(void* data);
    void Clear();
    bool32 IsEmpty();
};

} // namespace
#define QSTACK_H
#endif