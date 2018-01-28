// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QARRAY_H)

#include "Common.h"
#include "Memory.h"
#include <stdlib.h>
#include <string.h>

namespace QuarksDD {

struct ArrayItem 
{
    // Data 
    void* data;
    bool32 empty;

    // Operations
    ArrayItem& operator=(void* data) {
        this->data = data;
        
        return *this;
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
    uint32 extend;
    MemoryArena *arena;
    ArrayItem* items;
    
    // Operations
    CompareFn Compare;
    bool32  Init(uint32 arraySize = 5, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    bool32  Init(MemoryArena *arena, uint32 arraySize = 5, bool32 sorted = false, CompareFn Compare = NULL, SortOrder order = SortOrder::Asc);
    bool32 Extend();
    void Free();
    ArrayItem& operator[](uint32 index);
    ArrayItem* GetItem(uint32 index);
    bool32 Add(void* data);
    bool32 AddCopy(uint32 data);
    bool32 AddCopy(int32 data);
    bool32 AddCopy(real32 data);
    int32 GetSortedPos(void* data);
    bool32 AddSorted(void* data);
    bool32 Insert(uint32 index, void* data);
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
};

} // namespace
#define QARRAY_H
#endif