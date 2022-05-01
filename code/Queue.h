// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QQUEUE_H)

#include "QuarksDD.h"
#include "Memory.h"

#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

struct QueueItem 
{
    // Data 
    void* data;

    // Operations
    QueueItem& operator=(void* data) {
        this->data = data;
        
        return *this;
    }
};

struct Queue
{
    // Data
    bool32 initialized;
    bool32 useHeapAlloc;
    int32 front;
    int32 end;
    uint32 used;
    int32 size;
    MemoryArena *arena;
    QueueItem* items;
    
    // Operations
    bool32  Init(int32 queueSize = 4096);
    bool32  Init(MemoryArena *arena, int32 queueSize = 4096);
    void Free();

    QueueItem* DequeItem();
    void* Deque();
    bool32 Enque(void* data);
    void Clear();
    bool32 IsEmpty();
    bool32 IsFull();
};

} // namespace
#define QUEUE_H
#endif