// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Array.h"

namespace QuarksDD {

bool32  Array::Init(uint32 arraySize, MemorySize dataSize, bool32 sorted, CompareFn Compare, SortOrder order)
{
    bool32 result = false;

    if (!initialized) {
        this->dataSize = dataSize;
        this->sorted = sorted;
        this->Compare = Compare;
        this->order = order;


        useHeapAlloc = true;
        extend = 5;
        size = arraySize + extend;
        
        if (dataSize > 0) {
            dataArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
            *dataArena = {};
            dataArena->Init(size * dataSize);
        }

        // Allocate array memory
        MemorySize memSize = size * sizeof(ArrayItem);
        
        items = (ArrayItem*) Allocate(memSize);
        if (items) {
            ZeroArray(size, items);
            
            initialized = true;
            result = true;
        }
    }
    return result;
}

bool32  Array::Init(MemoryArena* arena, uint32 arraySize, MemorySize dataSize,  bool32 sorted, CompareFn Compare, SortOrder order)
{
    bool32 result = false;

    if (!initialized && arena && arena->initialized) {
        this->dataSize = dataSize;
        this->sorted = sorted;
        this->Compare = Compare;
        this->order = order;
        
        useHeapAlloc = false;
        extend = 5;
        size = arraySize + extend;

        this->arena = arena;
       
        itemArena = arena->CreateChildArena(size * sizeof(ArrayItem));
        items = (ArrayItem*)ArenaPushArray(itemArena, size, ArrayItem);
        
        if (dataSize > 0) {
            dataArena = arena->CreateChildArena(size * dataSize);
        }

        if (items) {
            initialized = true;
            result = true;
        }
    }
    return result;
}

void Array::Free() {
    if (initialized) {
        if (useHeapAlloc) {
            if (items) {
                Deallocate(items);
                items = NULL;
                size = 0;
            }
            if (dataArena) {
                dataArena->Free();
                Deallocate(dataArena);
                dataArena = NULL;
            }
        }
        else if (arena) {
            if (itemArena) {
                arena->FreeChildArena(itemArena);
                itemArena = NULL;
            }
            if (dataArena) {
                arena->FreeChildArena(dataArena);
                dataArena = NULL;
            }
        }
    }

    *this = {};
}

ArrayItem& Array::operator[](uint32 index)
{
    ArrayItem* result = GetItem(index);
    return *result;
}

ArrayItem* Array::GetItem(uint32 index) {
    ArrayItem* result = NULL;

    if (index < count) {
        result = (items + index);
    }
    return result;
}

void* Array::GetValue(uint32 index) {
    void* result = NULL;

    if (index < count) {
        ArrayItem* item = items + index;
        if (item) {
            result = item->data;
        }
    }
    return result;
}

bool32 Array::Extend() {
    bool32 result = false;
    ArrayItem* moreItems = NULL;

    // If not initialized then init to heap alloc memory managment
    if (!initialized) {
        Init(1);
    }

    MemorySize newMemSize = (size + extend) * sizeof(ArrayItem);

    if (useHeapAlloc) {
        // Realoceate more from heap space
        moreItems = (ArrayItem*)Reallocate(items, newMemSize);
    }
    else {
        // Try to resize arena block
        MemorySize memSize = size * sizeof(ArrayItem);
        moreItems = (ArrayItem*)itemArena->Resize((uint8*)items, memSize, newMemSize);
    }

    if (moreItems) {
        items = moreItems;
        size += extend;
        result = true;
    }

    return result;
}

ArrayItem* Array::Add(void* data) {
    ArrayItem* result = NULL;
    ArrayItem* item = NULL;

    Assert(count <= size);

    if (count >= size)  {
        if (!Extend()) {
            return NULL;
        }
    }

    item = (items + count);

    if ((dataSize > 0) && dataArena) {
        if (data) {
            if (!item->data) {
                item->data = ArenaPushSize(dataArena, dataSize);
            }
            memcpy(item->data, data, dataSize);
            result = item;
        }
    }
    else {
        item->data = data;
        result = item;
    }
    
    count++;

    return result;
}

ArrayItem* Array::Set(uint32 index, void* data, bool32 fillCheck) {
    ArrayItem* result = NULL;
    ArrayItem* item = NULL;

    if (index < count && fillCheck) {

        item = (items + index);

        if ((dataSize > 0) && dataArena) {
            // At this point data memory must be already allocated
            if (item->data && data) {
                memcpy(item->data, data, dataSize);
                result = item;
            }
        }
        else {
            item->data = data;
            result = item;
        }
    }
    else if (index < size) {

        item = (items + index);

        if ((dataSize > 0) && dataArena) {
            // At this point data memory must be already allocated
            if (item->data && data) {
                memcpy(item->data, data, dataSize);
                result = item;
            }
        }
        else {
            item->data = data;
            result = item;
        }
        count = index + 1;
    }

    return result;
}

int32 Array::GetSortedPos(void* data) {
    Assert(sorted == true);
    Assert(Compare != NULL);

    uint32 lo = 0;
    uint32 hi = count;
    
    if (sorted == true && Compare != NULL) {
        while (hi > lo) {
            int32 half = (hi + lo) >> 1;
            ArrayItem* item = (items + half);

            int32 cmp = Compare(data, item->data);

            // Inverse compare result?
            cmp = (order == SortOrder::Asc) ? cmp : (cmp * -1);

            if (cmp == 0) {
                return -half - 1;
            }

            if (cmp < 0) {
                hi = half;
                continue;
            }
            lo = half + 1;
        }
    }
    return lo;
}

ArrayItem* Array::AddSorted(void* data) {
    ArrayItem* result = NULL;

    Assert(sorted == true);
    Assert(Compare != NULL);

    if (sorted == true && Compare != NULL) {

        int32 pos = GetSortedPos(data);

        // NOTE: If pos is less than zero then its compare collision 
        if (pos < 0) {
            // Restore colision position
            pos = -pos-1;
        }
        result = Insert(pos, data);
    }

    return result;
}

ArrayItem* Array::Insert(uint32 index, void* data) {
    ArrayItem* result = NULL;
    ArrayItem* item = NULL;
    
    if (index > count) {
        return NULL;
    }
    else if (index == count) {
        return Add(data);
    }
    else {
        if (count == size) {
            if (!Extend()) {
                return NULL;
            }
        }

        void* source = (void*)(items + index);
        void* dest = (void*)(items + index + 1);
        MemorySize moveSize = (count - index) * sizeof(ArrayItem);
        memmove(dest, source, moveSize);

        item = (items + index);

        if ((dataSize > 0) && dataArena) {
            if (data) {
                if (!item->data) {
                    item->data = ArenaPushSize(dataArena, dataSize);
                }
                memcpy(item->data, data, dataSize);
                result = item;
            }
        }
        else {
            item->data = data;
            result = item;
        }

        count++;
    }

    return result;
}

bool32 Array::Remove(uint32 index) {
    bool32 result = false;

    if (index >= count) {
        result = false;
    }
    else if (index == (count - 1)) {
        // *(items + index) = {};
        (items + index)->Reset(dataSize); 
        count--;

        result = true;
    }
    else {

        void* source = (void*)(items + index + 1);
        void* dest = (void*)(items + index);
        MemorySize moveSize = (count - index) * sizeof(ArrayItem);
        memmove(dest, source, moveSize);

        // *(items + count - 1) = {}; 
        (items + count - 1)->Reset(dataSize); 

        count--;
        result = true;
    }

    return result;
}

bool32 Array::Remove(void* matchData, MatchFn Match) {
    bool32 result = false;
    Assert(sorted == true);
    if (sorted && matchData && Match) {
        uint32 match = 0;

        uint32 lo = 0;
        uint32 hi = count - 1;
        // Swap match elements to non-match elemtes at the end of array
        while (lo <= hi) {
            bool32 matchLo = Match(matchData, (items + lo)->data);
            if (matchLo) {
                ++match;

                while (lo < hi) {
                    bool32 matchHi = Match(matchData, (items + hi)->data);
                    if (!matchHi) {
                        Swap(items + lo, items + hi);
                        --hi;
                        break;
                    }
                    else {
                        ++match;
                    }
                    --hi;
                } 
            }
            ++lo;
        }

        // Shrink used array from end by empty elemts count
        Assert(match <= count);
        if (match > 0 && match <= count) {
            for (uint32 i = (count - match); i < count; i++) {
                (items + i)->Reset(dataSize);
            }

            count -= match;
            result = true;
        }
    }

    return result;
}

void Array::Clear() {
    for (uint32 i = 0; i < count; i++) {
        (items + i)->Reset(dataSize);
    }
    count = 0;
}

void Array::Swap(ArrayItem* a, ArrayItem* b)
{
    ArrayItem temp = {};

    memmove(&temp, a, sizeof(ArrayItem));
    memmove(a, b, sizeof(ArrayItem));
    memmove(b, &temp, sizeof(ArrayItem));
}

// NOTE: Bubble Sort
// Code form: https://www.geeksforgeeks.org/bubble-sort/
void Array::BubbleSort(CompareFn Compare, SortOrder order) 
{
    ArrayItem temp = {};
    bool32 swaped = false;

    for (uint32 i = 0; i < count - 1; i++) {
        swaped = false;
        for (uint32 j = 0; j < count - i - 1; j++) {

            int32 cmp = Compare((items + j)->data, (items + j + 1)->data);

            // Inverse compare result?
            cmp = (order == SortOrder::Asc) ? cmp : (cmp * -1);

            if (cmp > 0) {
                Swap((items + j), (items + j + 1));
                swaped = true;
            }
        }
        if (swaped == false) {
            break;
        }
    }
}

// NOTE: Iterative Quick Sort
// Code form: https://www.geeksforgeeks.org/iterative-quick-sort/
int32 Array::Partition (int32 lo, int32 hi, CompareFn Compare, SortOrder order)
{
    ArrayItem* pivot = (items + hi);
    
    int32 i = (lo - 1);
 
    for (int32 j = lo; j <= hi - 1; j++)
    {
        int32 cmp = Compare((items + j)->data, pivot->data);

        // Inverse compare result?
        cmp = (order == SortOrder::Asc) ? cmp : (cmp * -1);

        if (cmp <= 0)
        {
            i++;
            Swap((items + i), (items + j));
        }
    }
    
    Swap((items + i + 1), pivot);

    return (i + 1);
}
 

void Array::QuickSort(CompareFn Compare, SortOrder order) 
{
    int32 lo = 0;
    int32 hi = count-1;
    // Create an auxiliary stack
    // TODO(dainis): Use tempory memory here?
    int32* stack = (int32*)Allocate((hi - lo + 1) * sizeof(int32));
 
    // initialize top of stack
    int32 top = -1;
 
    // push initial values of lo and hi to stack
    stack[++top] = lo;
    stack[++top] = hi;
 
    // Keep popping from stack while is not empty
    while (top >= 0)
    {
        // Pop hi and lo
        hi = stack[top--];
        lo = stack[top--];
 
        // Set pivot element at its correct position
        // in sorted array
        int32 p = Partition(lo, hi, Compare, order);
 
        // If there are elements on left side of pivot,
        // then push left side to stack
        if (p - 1 > lo )
        {
            stack[++top] = lo;
            stack[++top] = p - 1;
        }
 
        // If there are elements on right side of pivot,
        // then push right side to stack
        if ( p + 1 < hi )
        {
            stack[++top] = p + 1;
            stack[++top] = hi;
        }
    }

    Deallocate(stack);
}

void Array::Sort(CompareFn Compare, SortOrder order, SortMethod method) 
{
    switch (method) {
        case SortMethod::QuickSort : {
            QuickSort(Compare, order);
            break;
        }
        case SortMethod::BubbleSort : {
            BubbleSort(Compare, order);
            break;
        }
    }

    if (sorted) {
        // NOTE: If sorted array then we need update these for AddSorted to work correctly!
        this->Compare = Compare;
        this->order = order;
    }
}

bool32 Array::SearchData(Array* searchResults, void* matchData, MatchFn Match) {
    bool32 result = false; 
    if (searchResults && matchData && Match) {
        for (uint32 i = 0; i < count; i++) {
            bool32 match = Match(matchData, (items + i)->data);

            if (match) {
                searchResults->Add((items + i)->data);
                result = true;
            }
        }
    }
    return result; 
}

bool32 Array::SearchPos(Array* searchPositions, void* matchData, MatchFn Match) {
    bool32 result = false; 
    if (searchPositions && matchData && Match && searchPositions->dataSize == sizeof(uint32)) {
        for (uint32 i = 0; i < count; i++) {
            bool32 match = Match(matchData, (items + i)->data);

            if (match) {
                searchPositions->Add(&i);
                result = true;
            }
        }
    }
    return result; 
}

int32 Array::FindSorted(void* matchData) {
    int32 result = -1; 
    if (matchData && Compare) {
        int32 pos = GetSortedPos(matchData);

        // NOTE: If pos is less than zero then its compare collision 
        if (pos < 0) {
            // Restore colision position
            result = -pos - 1;
        }
    }
    return result; 
}

int32 Array::Find(void* matchData, CompareFn FindCompare) {
    int32 result = -1; 
    if (matchData) {
        if (sorted && !FindCompare && Compare) {
            result = FindSorted(matchData);
        }
        else if (FindCompare) {
            for (uint32 i = 0; i < count; i++) {
                int32 cmp = FindCompare(matchData, (items + i)->data);

                if (cmp == 0) {
                    result = i;
                    return result;
                }
            }
        }
    }
    return result; 
}

int32 Array::FindMatch(void* matchData, MatchFn Match) {
    int32 result = -1; 
    if (matchData && Match) {
        for (uint32 i = 0; i < count; i++) {
            bool32 match = Match(matchData, (items + i)->data);

            if (match) {
                result = i;
                return result;
            }
        }
    }
    return result; 
}

} // namespace 