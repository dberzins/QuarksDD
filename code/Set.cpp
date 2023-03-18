// Copyright (c) 2022 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Set.h"
#include "Stack.h"

namespace QuarksDD {

bool32  Set::Init(CompareFn Compare, uint32 setSize, MemorySize dataSize)
{
    bool32 result = false;

    if (!initialized) {
        // if (dataSize > 0) {
        //     dataArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        //     *dataArena = {};
        //     dataArena->Init(setSize * dataSize);
        // }

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

bool32  Set::Init(MemoryArena* arena, CompareFn Compare, MemorySize dataSize)
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

Set* Set::Copy(Set* set, MemoryArena* copyArena) {
    Set* result = NULL;

    if (set != NULL) {
        if (copyArena != NULL) {
            result = (Set*)ArenaPushStruct(copyArena, Set);
        }
        else {
            result = (Set*)Allocate(sizeof(Set));
        }
        *result = {};
        result->Init(set->Compare, set->size, set->dataSize);

        Stack stack = {};
        stack.Init(set->size);
        stack.Push(set->root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            result->Add(node->data);

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }

    return result;
}

bool32 Set::Contains(SetIterator* pos, void* data) {
    bool32 result = false;
    if (pos != NULL) {

        SetNode* posNode= GetItem(pos);
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

bool32 Set::Contains(void* data) {
    bool32 result = false;
    if (root != NULL) {

        SetNode* node= GetItem(root);
        int32 cmp = Compare(data, node->data);
        if (cmp == 0) {
            result = true;
        }
        else if (cmp < 0) {
            if (node->iterator.left != NULL)
                result = Contains(node->iterator.left, data);
        }
        else if (cmp > 0) {
            if (node->iterator.right != NULL)
                result = Contains(node->iterator.right, data);
        }
    }

    return result;
}

SetNode* Set::Insert(SetIterator* pos, void* data) {
    SetNode* result = NULL;

    void* dataToStore = NULL;
    if ((dataSize > 0) && dataArena) {
        dataToStore = ArenaPushSize(dataArena, dataSize);
        ZeroSize(dataSize, dataToStore);
        if (data) {
            memcpy(dataToStore, data, dataSize);
        }
    }
    else {
        dataToStore = data;
    }

    if (pos == NULL) {
        result = AllocateNode();
        *result = {};
        result->data = dataToStore;
    }
    else {
        SetNode* posNode= GetItem(pos);
        int32 cmp = Compare(dataToStore, posNode->data);

        if (cmp < 0) {
            if (pos->left == NULL) {
                result = AllocateNode();
                *result = {};
                result->data = dataToStore;
                pos->left = &result->iterator;
            }
            else {
                result = Insert(pos->left, dataToStore);
            }            
        }
        else if (cmp > 0) {
            if (pos->right == NULL) {
                result = AllocateNode();
                *result = {};
                result->data = dataToStore;
                pos->right = &result->iterator;
            }
            else {
                result = Insert(pos->right, dataToStore);
            }            
        }
        else {
            result = posNode;
        }

    }

    return result;
}

void Set::Add(void* data) {
    SetNode* node = Insert(root, data);
    if (root == NULL)
        root = &node->iterator;
}

Set* Set::Union(Set* set, MemoryArena* copyArena) {
    Set* result = NULL;

    result = Copy(this, copyArena);

    if (set != NULL && set->root != NULL) {
        Assert(dataSize == set->dataSize);
        
        Stack stack = {};
        stack.Init(set->size);
        stack.Push(set->root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            result->Add(node->data);

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }
    
    return result;
}

Set* Set::Difference(Set* set, MemoryArena* copyArena) {
    Set* result;

    if (root != NULL) {

        if (copyArena != NULL) {
            result = (Set*)ArenaPushStruct(copyArena, Set);
        }
        else {
            result = (Set*)Allocate(sizeof(Set));
        }

        *result = {};
        result->Init(set->Compare, set->size, set->dataSize);

        Stack stack = {};
        stack.Init(size + set->size);
        stack.Push(root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            if (!set->Contains(node->data)) {
                result->Add(node->data);
            }

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }

    return result;
}

Set* Set::Intersect(Set* set, MemoryArena* copyArena) {
    Set* result;

    if (root != NULL) {

        if (copyArena != NULL) {
            result = (Set*)ArenaPushStruct(copyArena, Set);
        }
        else {
            result = (Set*)Allocate(sizeof(Set));
        }

        *result = {};
        result->Init(set->Compare, set->size, set->dataSize);

        Stack stack = {};
        stack.Init(size + set->size);
        stack.Push(root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            if (set->Contains(node->data)) {
                result->Add(node->data);
            }

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }

    return result;
}

Array* Set::ToArray(MemoryArena* copyArena) {
    Array* result;

    if (root != NULL) {

        if (copyArena != NULL) {
            result = (Array*)ArenaPushStruct(copyArena, Array);
        }
        else {
            result = (Array*)Allocate(sizeof(Array));
        }

        *result = {};
        result->Init(size, 0, true, Compare);

        Stack stack = {};
        stack.Init(size);
        stack.Push(root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            result->Add(node->data);

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }

    return result;
}

List* Set::ToList(MemoryArena* copyArena) {
    List* result;

    if (root != NULL) {
        if (copyArena != NULL) {
            result = (List*)ArenaPushStruct(copyArena, List);
        }
        else {
            result = (List*)Allocate(sizeof(List));
        }


        *result = {};
        result->Init(size, true, Compare);

        Stack stack = {};
        stack.Init(size);
        stack.Push(root);

        while (!stack.IsEmpty()) {
            SetIterator* iter = (SetIterator*)stack.Pop();
            SetNode* node = GetItem(iter);
            result->Add(node->data);

            if (iter->right) {
                stack.Push(iter->right);
            }
            if (iter->left) {
                stack.Push(iter->left);
            }
        }

        stack.Free();
    }

    return result;
}

} // namespace 