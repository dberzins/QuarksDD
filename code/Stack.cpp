#include "Stack.h"

namespace QuarksDD {

bool32  Stack::Init(int32 stackSize)
{
    bool32 result = false;

    if (!initialized) {
        // TODO(dainis): Upgrade to local MemoryArena
        // NOTE: For now we must realay on Allocate calls
        useHeapAlloc = true;
        size = stackSize;
        top = -1;
                
        // Allocate array memory
        MemorySize memSize = size * sizeof(StackItem);
        
        items = (StackItem*) Allocate(memSize);
        if (items) {
            ZeroArray(size, items);
            
            initialized = true;
            result = true;
        }
    }
    return result;
}

bool32  Stack::Init(MemoryArena* arena, int32 stackSize)
{
    bool32 result = false;

    if (!initialized && arena && arena->initialized) {
        
        useHeapAlloc = false;
        size = stackSize;
        top = -1;

        this->arena = arena;

        items = (StackItem*)ArenaPushArray(arena, size, StackItem);
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

void Stack::Free() {
    if (initialized) {
        if (useHeapAlloc) {
            if (items) {
                Deallocate(items);
                items = NULL;
                size = 0;
            }
            arena = NULL;
        }
        else if (arena) {
            // TODO(dainis): Implement memory arena Pop, For now owner of arene should take care of mem managment
        }
    }

    *this = {};
}

StackItem* Stack::PopItem() {
    StackItem* result = NULL;

    if (top >= 0) {
        result = (items + top);
        --top;
    }
    return result;
}

void* Stack::Pop() {
    void* result = NULL;

    if (top >= 0) {
        StackItem* item = (items + top);
        if (item) {
            result = item->data;
        }
        --top;
    }
    return result;
}

bool32 Stack::Push(void* data) {
    bool32 result = false;

    if (top < size) {
        // Push item to the top 
        ++top;
        (items + top)->data = data;

        result =  true;
    }
    return result;
}

void Stack::Clear() {
    if (top >= 0) {
        ZeroArray(top, items);
    }
    top = -1;
}

bool32 Stack::IsEmpty() {
    return top < 0;
}

} // namespace 