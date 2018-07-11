// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QARRAY_H)

#include "QuarksDD.h"
#include "Memory.h"
#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

struct ArrayItem 
{
    // Data 
    void* data;
    // bool32 empty;

    // Operations
    void Reset(MemorySize dataSize) {

        if (dataSize > 0 && data) {
            ZeroSize(dataSize, data);
        }
        else {
            data = NULL;
        }

    }
};

struct Array 
{
    // Data
    bool32 initialized;
    bool32 sorted;
    SortOrder order;
    bool32 useHeapAlloc;
    uint32 count;
    uint32 size;
    MemorySize dataSize;
    uint32 extend;
    MemoryArena *arena;
    MemoryArena *itemArena;
    MemoryArena *dataArena;
    ArrayItem* items;
    
    // Operations
    CompareFn Compare;
    bool32 Init(uint32 arraySize = 5, MemorySize dataSize = 0, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    bool32 Init(MemoryArena *arena, uint32 arraySize = 5, MemorySize dataSize = 0, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    bool32 Extend();
    void Free();
    ArrayItem& operator[](uint32 index);
    ArrayItem* GetItem(uint32 index);
    void* GetValue(uint32 index);
    ArrayItem* Add(void* data);
    ArrayItem* Set(uint32 index, void* data, bool32 fillCeheck = true);
    int32 GetSortedPos(void* data);
    ArrayItem* AddSorted(void* data);
    ArrayItem* Insert(uint32 index, void* data);
    bool32 Remove(uint32 index);
    bool32 Remove(void* matchData, MatchFn Match);
    void Clear();
    void Swap(ArrayItem* a, ArrayItem* b);
    void BubbleSort(CompareFn Compare, SortOrder order = SortOrder::Asc);
    int32 Partition(int32 lo, int32 hi, CompareFn Compare, SortOrder order = SortOrder::Asc);
    void QuickSort(CompareFn Compare, SortOrder order = SortOrder::Asc);
    void Sort(CompareFn Compare, SortOrder order = SortOrder::Asc, SortMethod method = SortMethod::QuickSort);
    bool32 SearchData(Array* searchResults, void* matchData, MatchFn Match);
    bool32 SearchPos(Array* searchPositions, void* matchData, MatchFn Match);
    int32 FindSorted(void* matchData);
    int32 Find(void* matchData, CompareFn FindCompare = NULL);
    int32 FindMatch(void* matchData, MatchFn Match);
    // TODO(dainis): FlatArray* Flatten(MemoryArena arena);
};

} // namespace
#define QARRAY_H
#endif