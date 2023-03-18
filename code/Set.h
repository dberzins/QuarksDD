// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// NOTE: Based on https://www.geeksforgeeks.org/implementing-sets-without-c-stl-containers/ 
#if !defined(QSET_H)

#include "QuarksDD.h"
#include "Memory.h"
#include "Array.h"
#include "List.h"

#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

struct SetIterator {
    SetIterator* left;
    SetIterator* right;
};

struct SetNode 
{
    // Data 
    void* data;
    SetIterator iterator;
};


// struct SetNode 
// {
//     // Data 
//     void* data;

//     // Operations
//     SetNode& operator=(void* data) {
//         this->data = data;
        
//         return *this;
//     }

//     SetNode* left;
//     SetNode* right;
// };

struct Set
{
    // Data
    bool32 initialized;
    bool32 useLocalArena;
    uint32 size;
    MemorySize dataSize;
    MemoryArena* arena;
    MemoryArena* dataArena;
    SetIterator* root;
    SetIterator* firstFree;
    CompareFn Compare;
    
    // Operations
    bool32  Init(CompareFn Compare, uint32 setSize = 5, MemorySize dataSize = 0);
    bool32  Init(MemoryArena *arena, CompareFn Compare, MemorySize dataSize = 0);
    void Free();

    static SetNode* GetItem(SetIterator* iter) {
        SetNode* result = NULL;
        if (iter) {
            result = ContainerOf(iter, SetNode, iterator);
        }
        return result;
    }
    
    SetNode* AllocateNode();
    Set* Copy(Set* set, MemoryArena* copyArena = NULL);
    
    bool32 Contains(SetIterator* pos, void* data);
    bool32 Contains(void* data);
    SetNode* Insert(SetIterator* pos, void* data);
    void Add(void* data);

    Set* Union(Set* set, MemoryArena* copyArena = NULL);
    Set* Difference(Set* set, MemoryArena* copyArena = NULL);
    Set* Intersect(Set* set, MemoryArena* copyArena = NULL);
    Array* ToArray(MemoryArena* copyArena = NULL);
    List* ToList(MemoryArena* copyArena = NULL);

    // Remove
    // Clear
    // Equals
};

} // namespace
#define QSET_H
#endif