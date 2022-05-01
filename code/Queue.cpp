// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Queue.h"

namespace QuarksDD {

bool32  Queue::Init(int32 queueSize)
{
    bool32 result = false;

    if (!initialized) {
        // TODO(dainis): Upgrade to local MemoryArena
        // NOTE: For now we must realay on Allocate calls
        useHeapAlloc = true;
        size = queueSize;

        front = 0;
        used = 0;
        end = size - 1;

        // Allocate array memory
        MemorySize memSize = size * sizeof(QueueItem);
        
        items = (QueueItem*) Allocate(memSize);
        if (items) {
            ZeroArray(size, items);
            
            initialized = true;
            result = true;
        }
    }
    return result;
}

bool32  Queue::Init(MemoryArena* arena, int32 queueSize)
{
    bool32 result = false;

    if (!initialized && arena && arena->initialized) {
        
        useHeapAlloc = false;
        size = queueSize;

        front = 0;
        used = 0;
        end = size - 1;

        this->arena = arena;

        items = (QueueItem*)ArenaPushArray(arena, size, QueueItem);
        if (items) {
            if (!arena->zero) {
                // If arena is not zeroing memory then do it by ourselves
                ZeroArray(size, items);
            }
            
            initialized = true;
            result = true;
        }
    }
    return result;
}

void Queue::Free() {
    if (initialized) {
        if (useHeapAlloc) {
            if (items) {
                Deallocate(items);
                items = NULL;
                size = 0;
                
                front = 0;
                used = 0;
                end = 0;
            }
            arena = NULL;
        }
        else if (arena) {
            // TODO(dainis): Implement memory arena Pop, For now owner of arene should take care of mem managment
        }
    }

    *this = {};
}

QueueItem* Queue::DequeItem() {
    QueueItem* result = NULL;

    if (used > 0) {
        result = (items + front);
        front = (front + 1)  % size;
        --used;
    }

    return result;
}

void* Queue::Deque() {
    void* result = NULL;

    if (used > 0) {
        QueueItem* item = (items + front);
        result = item->data;
        front = (front + 1)  % size;
        --used;
    }

    return result;
}

bool32 Queue::Enque(void* data) {
    bool32 result = false;

    if (size > (int32)used) {
        end = (end + 1) % size;
        (items + end)->data = data;
        ++used;
        result = true;
    }
    return result;
}

void Queue::Clear() {
    if (end >= 0) {
        ZeroArray(end, items);
    }
    end = -1;
}

bool32 Queue::IsEmpty() {
    return used == 0;
}

bool32 Queue::IsFull() {
    return size == used;
}

} // namespace 