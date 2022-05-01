// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QFLATARRAY_H)

#include "QuarksDD.h"
#include "Memory.h"
#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

#define FLAT_ARRAY_POS_UNINITIALIZED U32Max

#define FlatArrayInit(array, Type) ((FlatArray*)array)->Init(sizeof(Type))
#define FlatArrayGetItem(array, index, Type) (Type*) ((FlatArray*)array)->GetItem(index)

struct FlatArray 
{
    // Data
    bool32 initialized;
    bool32 useHeapAlloc;
    uint32 size;
    MemoryArena *arena;
    void* items;
    MemorySize itemSize;
    uint32 count;
    uint32* removedPos;
    uint32 removedCount;

    // Operations
    bool32 Init(uint32 arraySize, MemorySize itemSize, bool32 trackRemoves = true) {
        bool32 result = false;

        if (!initialized && arraySize < FLAT_ARRAY_POS_UNINITIALIZED) {
            useHeapAlloc = true;
            size = arraySize;
            this->itemSize = itemSize;
            
            // Allocate array memory
            MemorySize memSize = size * itemSize;
            
            items = (void*) Allocate(memSize);
            if (items) {
                ZeroSize(memSize, items);
                
                initialized = true;
                result = true;
            }
            if (trackRemoves) {
                removedPos = (uint32*) Allocate(arraySize* sizeof(uint32));
                for (uint32 i = 0; i < arraySize; ++i) {
                    *(removedPos + i) = FLAT_ARRAY_POS_UNINITIALIZED;
                }
            }
        }
        return result;
    }

    bool32 Init(MemoryArena *arena, uint32 arraySize, MemorySize itemSize, bool32 trackRemoves = true) {
        bool32 result = false;

        if (!initialized && arena && arena->initialized && arraySize < FLAT_ARRAY_POS_UNINITIALIZED) {
            
            useHeapAlloc = false;
            size = arraySize;
            this->itemSize = itemSize;
            MemorySize memSize = size * itemSize;

            this->arena = arena;
            items = (void*)ArenaPushSize(arena, memSize);
            if (items) {
                ZeroSize(memSize, items);
                
                initialized = true;
                result = true;
            }

            if (trackRemoves) {
                removedPos = (uint32*) ArenaPushSize(arena, arraySize* sizeof(uint32));
                for (uint32 i = 0; i < arraySize; ++i) {
                    *(removedPos + i) = FLAT_ARRAY_POS_UNINITIALIZED;
                }
            }
            
        }
        return result;
    }

    void Free() {
        if (initialized) {
            if (useHeapAlloc) {
                if (items) {
                    Deallocate(items);
                    items = NULL;
                    size = 0;
                }
                if (removedPos) {
                    Deallocate(removedPos);
                    removedPos = NULL;
                }
                // if (arena) {
                //     arena->Free();
                // }
                // arena = NULL;
            }
            else if (arena) {
                // TODO(dainis): Upgrade MemoryArena to track allocations/deallocation positions
                // so memory can bee marked as free in middle of memory block/blocks.
                // Currently just assume that arena will be freed later by arena owner 
                // arena->used -= size * sizeof(ArrayItem);
            }
        }

        *this = {};
    }

    void* GetItem(uint32 index) {
        void* result = NULL;

        if (index < count) {
            result = (void*)((uint8*)items + (index * itemSize));
        }
        return result;
    }

    void* operator[](uint32 index)
    {
        void* result = GetItem(index);
        return result;
    }

    uint32 AddToLastRemoved(void* data) {
        uint32 result = FLAT_ARRAY_POS_UNINITIALIZED;

        if (removedPos && removedCount > 0) {
            uint32 index = *(removedPos + (removedCount-1));
            memcpy(((uint8*)items + (index * itemSize)), (uint8*)data, itemSize);

            *(removedPos + (removedCount-1)) = FLAT_ARRAY_POS_UNINITIALIZED;
            --removedCount;
            result = index;
        }
        return result;
    }

    bool32 Add(void* data) {
        bool32 result = false;

        if (count < size)  {
            memcpy(((uint8*)items + (count * itemSize)), (uint8*)data, itemSize);
            count++;
            result = true;
        }
        return result;
    }

    bool32 Set(uint32 index, void* data, bool32 fillCheck = true) {
        bool32 result = false;

        if (index < count && fillCheck)  {
            memcpy(((uint8*)items + (count * itemSize)), (uint8*)data, itemSize);
            result = true;
        }
        else if (index < size) {
            memcpy(((uint8*)items + (count * itemSize)), (uint8*)data, itemSize);
            count = index + 1;
            result = true;
        }
        return result;
    }

    void Remove(uint32 index) {
        // Mybe set to 0xCD - newly allocated memory (0xCD) is Clean memory.
        ZeroSize(itemSize, (uint8*)items + (index * itemSize));
        
        if (removedPos && removedCount < count){
            *(removedPos + removedCount) = index;
            ++removedCount;    
        }
    }

    bool32 Remove(void* matchData, MatchFn Match) {
        bool32 result = false;

        for (uint32 i = 0; i < count; i++) {
            bool32 match = Match(matchData, ((uint8*)items + (i * itemSize)));
            if (match) {
                Remove(i);
                result = true;
                break;
            }
        }
        return result;    
    }

    void Clear() {
        ZeroSize(count * itemSize, items);
        count = 0;

        if (removedPos && removedCount > 0) {
            for (uint32 i = 0; i < count; ++i) {
                *(removedPos + i) = FLAT_ARRAY_POS_UNINITIALIZED;
            }
            removedCount = 0;
        }
    }
};

} // namespace
#define QFLATARRAY_H
#endif