// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QLIST_H)

#include "QuarksDD.h"
#include "Memory.h"

#include <stdlib.h>

namespace QuarksDD {

struct ListIterator {
    ListIterator* next;
    ListIterator* prev;
};

struct ListItem 
{
    // Data 
    void* data;
    ListIterator iterator;
};

struct PartitionResult {
    ListIterator* pivot;
    ListIterator* lo;
    ListIterator* hi;
};

struct List
{
    // Data
    bool32 initialized;
    bool32 sorted;
    SortOrder order;
    bool32 useLocalArena;
    MemoryArena *arena;

    uint32 count;
    ListIterator* head;
    ListIterator* tail;

    ListIterator* firstFree;

    // Operations
    CompareFn Compare;
    bool32  Init(uint32 listSize = 5, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    bool32  Init(MemoryArena *arena, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    void Free();

    ListItem* AllocateItem();
    ListItem* Add(void* data);

    ListIterator* GetSortedPos(void* data);
    ListItem* AddSorted(void* data);
    
    static ListItem* GetItem(ListIterator* iter) {
        ListItem* result = NULL;
        if (iter) {
            result = ContainerOf(iter, ListItem, iterator);
        }
        return result;
    }

    static void* GetValue(ListIterator* iter) {
        void* result = NULL;
        if (iter) {
            ListItem* item = ContainerOf(iter, ListItem, iterator);
            if (item) {
                result = item->data;
            }
        }
        return result;
    }

    bool32 Attach(ListIterator* pos, ListIterator* iter);
    bool32 Detach(ListIterator* iter);
    bool32 Move(ListIterator* pos, ListIterator* iter);
    bool32 Swap(ListIterator* a, ListIterator* b);
    
    ListItem* Insert(ListIterator* pos, void* data);
    bool32 Remove(ListIterator* iter);
    bool32 Remove(ListItem* item);

    void Clear();
    
    void BubbleSort(CompareFn Compare, SortOrder order);
    PartitionResult Partition(ListIterator* lo, ListIterator* hi, CompareFn Compare, SortOrder order = SortOrder::Asc);
    void QuickSort(CompareFn Compare, SortOrder order = SortOrder::Asc);
    void Sort(CompareFn Compare, SortOrder order = SortOrder::Asc, SortMethod method = SortMethod::QuickSort);
};

} // namespace
#define QLIST_H
#endif