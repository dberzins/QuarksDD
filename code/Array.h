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

    int32 GetSortedPos(void* data);
    bool32 AddSorted(void* data);

    bool32 Insert(uint32 index, void* data);
    bool32 Remove(uint32 index);
    void Clear();

    void Swap(ArrayItem* a, ArrayItem* b);

    void BubbleSort(CompareFn Compare, SortOrder order = SortOrder::Asc);

    int32 Partition(int32 lo, int32 hi, CompareFn Compare, SortOrder order = SortOrder::Asc);
    void QuickSort(CompareFn Compare, SortOrder order = SortOrder::Asc);

    void Sort(CompareFn Compare, SortOrder order = SortOrder::Asc, SortMethod method = SortMethod::QuickSort);
};

} // namespace
#define QARRAY_H
#endif