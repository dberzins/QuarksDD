// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "List.h"

namespace QuarksDD {
bool32  List::Init(uint32 listSize, bool32 sorted, CompareFn Compare, SortOrder order)
{
    bool32 result = false;

    if (!initialized) {
        this->sorted = sorted;
        this->Compare = Compare;
        this->order = order;

        arena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *arena = {};
        arena->Init(sizeof(ListItem) * listSize);

        useLocalArena = true;
        initialized = true;
        result = true;
    }
    return result;
}

bool32  List::Init(MemoryArena* arena, bool32 sorted, CompareFn Compare, SortOrder order)
{
    bool32 result = false;

    if (!initialized) {
        this->sorted = sorted;
        this->Compare = Compare;
        this->order = order;

        useLocalArena = false;
        this->arena = arena;

        initialized = true;
        result = true;
    }
    return result;
}

void List::Free() {
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

ListItem* List::AllocateItem() {
    ListItem* result = NULL;

    if (firstFree != NULL) {
        // Reuse free list
        result = GetItem(firstFree);
        firstFree = firstFree->next;
    }
    else if (arena) {   
        result = ArenaPushStruct(arena, ListItem);
    }
    return result;
}

ListItem* List::Add(void* data) {
    ListItem* result = NULL;

    ListItem* item = AllocateItem();
    *item = {};

    if (item) {
        item->data = data;
        
        if (head == NULL) {
            // First item
            head = &item->iterator;
            tail = &item->iterator;
        }
        else {
            item->iterator.prev = tail;
            tail->next = &item->iterator;
            tail = &item->iterator;
        }
        ++count;

        result = item;
    }
    return result;
}


bool32 List::Attach(ListIterator* pos, ListIterator* iter) {
    bool32 result = false;

    if (pos && iter) {
        iter->prev = pos;
        iter->next = pos->next;

        if (pos->next)
            pos->next->prev = iter;
        
        pos->next = iter;
        
        if (pos==tail)
            tail = iter;

        result = true;
    }
    else if (pos == NULL) {
        // if pos NULL then attach as head
        iter->next = head;
        iter->prev = NULL;
        head->prev = iter;
        head = iter;
    }

    return result;
}

bool32 List::Detach(ListIterator* iter) {
    bool32 result = false;

    if (iter) {
        if (iter->prev)
            iter->prev->next = iter->next;

        if (iter->next)
            iter->next->prev = iter->prev;
        
        if (iter == head) 
            head = iter->next;
        
        if (iter == tail) 
            tail = iter->prev;
            
        // iter->next = NULL;
        // iter->prev = NULL;
        
        result = true;
    }

    return result;
}

bool32 List::Move(ListIterator* pos, ListIterator* iter) {
    bool32 result = false;

    if (iter) {

        Detach(iter);
        Attach(pos, iter);

        result = true;
    }

    return result;
}

bool32 List::Swap(ListIterator* a, ListIterator* b) {
    bool32 result = false;
    
    if (a && b) {
        ListIterator* posa = a->prev;
        Detach(a);
        ListIterator* posb = b->prev;
        Attach(posb, a);
        Detach(b);
        Attach(posa, b);

        result = true;
    }

    return result;
}

ListItem* List::Insert(ListIterator* pos, void* data) {
    ListItem* result = NULL;

    ListItem* item = AllocateItem();
    *item = {};

    if (item) {
        item->data = data;

        Attach(pos, &item->iterator);
        ++count;

        result = item;
    }
    
    return result;
}

bool32 List::Remove(ListIterator* iter) {
    bool32 result = false;
    
    if (iter) {
        Detach(iter);

        // Add to free list
        iter->next = firstFree;
        iter->prev = NULL;
        firstFree = iter;

        --count;
    }
    return result;
}

bool32 List::Remove(ListItem* item) {
    bool32 result = Remove(&item->iterator);
    return result;
}

void List::Clear() {
    if (tail) {
        tail->next = firstFree;
        
        Assert(head != NULL);
        firstFree = head;
    }
    count = 0;
    head = NULL;
    tail = NULL;
}

void List::BubbleSort(CompareFn Compare, SortOrder order) 
{
    for (uint32 i = 0; i < (count - 1); i++) {
        ListIterator* iter2 = head;
        bool32 swaped = false;

        for (uint32 j = 0; j < (count - i - 1); j++) {
            int32 cmp = Compare(GetItem(iter2)->data, GetItem(iter2->next)->data);
            // Inverse compare result?
            cmp = (order == SortOrder::Asc) ? cmp : (cmp * -1);

            if (cmp > 0) {  
                // NOTE: Swap will advance iter2 to next iterator!
                Swap(iter2, iter2->next);
                swaped = true;
            }
            else {
                iter2 = iter2->next;
            }
        }
        if (swaped == false) {
            break;
        }
     }
}

PartitionResult List::Partition(ListIterator* lo, ListIterator* hi, CompareFn Compare, SortOrder order)
{
    PartitionResult result = {};

    ListIterator* pivot = hi;
    ListIterator* iter1 = lo; // Next of last swaped iterator
    ListIterator* iter2 = lo; // Current iterrator

    while (iter2 && iter2 != pivot) {
        int32 cmp = Compare(GetItem(iter2)->data, GetItem(pivot)->data);
        // Inverse compare result?
        cmp = (order == SortOrder::Asc) ? cmp : (cmp * -1);
        
        // Save next in case of swap will happen
        ListIterator* next = iter2->next;

        if (cmp <= 0) {
            Swap(iter1, iter2);
            iter1 = iter2->next;

            // Save lo position
            if (!result.lo) {
                result.lo = iter2;
            }
        }

        iter2 = next; 
    }

    Swap(iter1, pivot);

    result.pivot = pivot;
    result.hi = iter1;

    // if result.lo is still empty then everything is sorted! 
    if (!result.lo) {
        result.lo = pivot;
    }

    return result;
}
 
void List::QuickSort(CompareFn Compare, SortOrder order) 
{
    ListIterator* lo = head;
    ListIterator* hi = tail;
    int32 top = -1;
    ListIterator** stack = (ListIterator**)Allocate(count * sizeof(ListIterator*));
 
    stack[++top] = lo;
    stack[++top] = hi;
 
    while (top >= 0) {
        // Pop hi and lo
        hi = stack[top--];
        lo = stack[top--];
 
        PartitionResult r = Partition(lo, hi, Compare, order);
        
        // If r.pivot = r.lo than everything is sorted
        if (r.pivot && r.lo && r.pivot != r.lo) {
            // Push left side of partition
            // if r.lo == r.pivot->prev then there is no need to sort 1 element partition
            if (r.pivot->prev && r.lo != r.pivot->prev)
            {
                stack[++top] = r.lo;
                stack[++top] = r.pivot->prev;
            }
    
            // Push right side of partition
            // if r.pivot == r.hi ther is no more elements on right side
            if (r.pivot->next && r.pivot != r.hi)
            {
                stack[++top] = r.pivot->next;
                stack[++top] = r.hi;
            }
        }
    }

    Deallocate(stack);
}

void List::Sort(CompareFn Compare, SortOrder order, SortMethod method) 
{
    switch (method) {
        case SortMethod::QuickSort : {
            QuickSort(Compare, order);
            break;
        }
        case SortMethod::BubbleSort : {
            BubbleSort(Compare, order);
        }
    }

    if (sorted) {
        // NOTE: If sorted List then we need update these for AddSorted to work correctly!
        // TODO(dainis): Implement AddSorted
        this->Compare = Compare;
        this->order = order;
    }
}

} // namespace