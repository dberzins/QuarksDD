// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Set.h"
#include "Stack.h"

namespace QuarksDD {

bool32  Set::Init(CompareFn Compare, uint32 setSize)
{
    bool32 result = false;

    if (!initialized) {
        arena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *arena = {};
        arena->Init(sizeof(SetNode) * setSize);
        this->Compare = Compare;

        useLocalArena = true;
        root = NULL;
        size = 0;

        initialized = true;
        result = true;
    }
    return result;
}

bool32  Set::Init(MemoryArena* arena, CompareFn Compare)
{
    bool32 result = false;

    if (!initialized) {
        this->arena = arena;
        useLocalArena = false;
        root = NULL;
        size = 0;
        this->Compare = Compare;
        initialized = true;
        result = true;
    }
    return result;
}

void Set::Free() {
    if (initialized) {
        if (useLocalArena && arena) {
            arena->Free();
            Deallocate(arena);
            arena = NULL;
        }
        else if (arena) {
            // TODO(dainis): Upgrade MemoryArena to track allocations/deallocation positions
            // so memory can bee marked as free in middle of memory block/blocks.
            // Currently just assume that arena will be freed later by arena owner 
            // arena->used -= size * sizeof(ListItem);
        }
    }

    *this = {};
}

SetNode* Set::AllocateNode() {
    SetNode* result = NULL;

    if (firstFree != NULL) {
        // Reuse free list
        result = GetItem(firstFree);
        firstFree = firstFree->right;
    }
    else if (arena) {   
        result = ArenaPushStruct(arena, SetNode);
        size++;
    }
    return result;
}

bool32 Set::Contains(SetIterator* pos, void* data) {
    bool32 result = false;
    if (pos != NULL) {

        SetNode* posNode= Set::GetItem(pos);
        int32 cmp = Compare(data, posNode->data);
        if (cmp == 0) {
            result = true;
        }
        else if (cmp < 0) {
            if (posNode->iterator.left != NULL)
                result = Contains(posNode->iterator.left, data);
        }
        else if (cmp > 0) {
            if (posNode->iterator.right != NULL)
                result = Contains(posNode->iterator.right, data);
        }
    }

    return result;
}

void Set::Add(void* data) {
    SetNode* node = Insert(root, data);
    if (root == NULL)
        root = &node->iterator;
}

SetNode* Set::Insert(SetIterator* pos, void* data) {
    SetNode* result = NULL;

    if (pos == NULL) {
        result = AllocateNode();
        *result = {};
        result->data = data;
    }
    else {
        SetNode* posNode= Set::GetItem(pos);
        int32 cmp = Compare(data, posNode->data);

        if (cmp < 0) {
            if (pos->left == NULL) {
                result = AllocateNode();
                *result = {};
                result->data = data;
                pos->left = &result->iterator;
            }
            else {
                result = Insert(pos->left, data);
            }            
        }
        else if (cmp > 0) {
            if (pos->right == NULL) {
                result = AllocateNode();
                *result = {};
                result->data = data;
                pos->right = &result->iterator;
            }
            else {
                result = Insert(pos->right, data);
            }            
        }
        else {
            result = posNode;
        }

    }

    return result;
}

} // namespace 