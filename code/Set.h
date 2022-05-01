// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QSET_H)

#include "QuarksDD.h"
#include "Memory.h"

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
    MemoryArena *arena;
    SetIterator* root;
    SetIterator* firstFree;
    CompareFn Compare;
    
    // Operations
    bool32  Init(CompareFn Compare, uint32 setSize = 5);
    bool32  Init(MemoryArena *arena, CompareFn Compare);
    void Free();

    static SetNode* GetItem(SetIterator* iter) {
        SetNode* result = NULL;
        if (iter) {
            result = ContainerOf(iter, SetNode, iterator);
        }
        return result;
    }
    
    SetNode* AllocateNode();
    SetNode* Insert(SetIterator* pos, void* data);
    bool32 Contains(SetIterator* pos, void* data);
    void Add(void* data);

    // void Clear();
};

} // namespace
#define QSET_H
#endif