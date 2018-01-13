// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Hashtable.h"
#include "Array.h"

namespace QuarksDD {

bool32  Hashtable::Init(uint32 tableSize)
{
    bool32 result = false;

    if (!initialized) {
        // Enforce power of 2 size for table
        size = RoundUpToPowerOf2(tableSize);
        
        // Allocate hashtable bolcks memory (1 primmary slot + 1 colision slot = 2)
        MemorySize memSize = size * 2 * sizeof(HashItem);
        
        arena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *arena = {};
        arena->Init(memSize, true);

        table = ArenaPushArray(arena, size, HashItem);
        if (table) {
            for (uint32 i = 0; i < size; ++i) {
                *(table + i) = {};
                (table + i)->index = HASH_ITEM_UNINITIALIZED;
            }

            useLocalArena = true;
            initialized = true;
            result = true;
        }
    }
    return result;
}

bool32  Hashtable::Init(MemoryArena *arena, uint32 tableSize)
{
    bool32 result = false;

    if (!initialized) {
        useLocalArena = false;
        this->arena = arena;
        
        // Enforce power of 2 size for table
        count = 0;            
        size = RoundUpToPowerOf2(tableSize);

        // NOTE(dainis): We are not reserving additional space for collisions, 
        // as we dont know how arena will be used by other code paths
        // MemorySize memSize = size * 2 * sizeof(HashItem);
        
        table = ArenaPushArray(arena, size, HashItem);
        if (table) {
            for (uint32 i = 0; i < size; ++i) {
                *(table + i) = {};
                (table + i)->index = HASH_ITEM_UNINITIALIZED;
            }
            
            initialized = true;

            result = true;
        }
    }
    return result;
}

void Hashtable::Free() {
    if (initialized) {
        if (useLocalArena) {
            if (arena) {
                arena->Free();
                Deallocate(arena);
                arena = NULL;
            }

            table = NULL;
            firstFree = NULL;
        }
    }

    *this = {};
}

void Hashtable::Clear() {
    for (uint32 i = 0; i < size; i++) {
        HashItem* item = (table + i);
        if (item->index != HASH_ITEM_UNINITIALIZED) {
            while (item) {
                HashItem* next = item->next;
                bool32  isFirstItem = item->prev == NULL;

                // Reset block
                *item = {};
                item->index = HASH_ITEM_UNINITIALIZED;

                // Add item to free list for memory reuse if ts not first item in table
                if (!isFirstItem) {
                    if (!firstFree) {
                        firstFree = item;
                    }
                    else {
                        item->next = firstFree;
                        firstFree = item;
                    }
                }

                item = next;
            }
        }
    }
    count = 0;
}

void Hashtable::Remove(char* key) {
    uint32 index = Murmur3x32(key, (size_t)strlen(key), 0);
    HashItem* result = GetHashItem(index);
    if (result) {
        RemoveHashItem(result);
    }
}

void Hashtable::Remove(uint32 index) {
    HashItem* result = GetHashItem(index);
    if (result) {
        RemoveHashItem(result);
    }
}

// NOTE: Hash functions
// http://burtleburtle.net/bob/hash/doobs.html
// https://stackoverflow.com/questions/7666509/hash-function-for-string
HashItem* Hashtable::Add(char* key, void* data = NULL) {
    uint32 index = Murmur3x32(key, (size_t)strlen(key), 0);
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    result->key = key;
    result->data = data;

    return result;
}

HashItem* Hashtable::Add(uint32 index, void* data = NULL) {
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    result->data = data;
    return result;
}

HashItem& Hashtable::operator[](char* key)
{
    uint32 index = Murmur3x32(key, (size_t)strlen(key), 0);
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    result->key = key;
    return *result;
};

HashItem& Hashtable::operator[](uint32 index)
{
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    return *result;
};

HashItem* Hashtable::GetItem(char* key) {
    uint32 index = Murmur3x32(key, (size_t)strlen(key), 0);
    HashItem* result = GetHashItem(index, HashAccess::ReadOnly);
    return result;
}

HashItem* Hashtable::GetItem(uint32 index) {
    HashItem* result = GetHashItem(index, HashAccess::ReadOnly);
    return result;
}

void* Hashtable::GetValue(char* key) {
    void* result = NULL;
    HashItem* item = GetItem(key);
    if (item) {
        result = item->data;
    }
    return result;
}

void* Hashtable::GetValue(uint32 index) {
    void* result = NULL;
    HashItem* item = GetItem(index);
    if (item) {
        result = item->data;
    }
    return result;
}

bool32  Hashtable::HasKey(char* key) {
    HashItem* item = GetItem(key);
    return item != NULL;
}

bool32  Hashtable::HasKey(uint32 index) {
    HashItem* item = GetItem(index);
    return item != NULL;
}

bool32  Hashtable::IsEmpty() { 
        return size <= 0;
}

// NOTE: This retuns hastable items in undefined order!
HashtableIterator* Hashtable::GetIterator() {

   HashItem* item = NULL;
   HashtableIterator* first = NULL;
   HashtableIterator* last = NULL;

   for (uint32 slot = 0; slot < size; slot++) {
       item = table + slot;

       while (item && item->index != HASH_ITEM_UNINITIALIZED) {
           if (!first) {
               // First iterator item
               first = &item->iterator;
               first->prev = NULL;
               first->next = NULL;
               last = first;
           }
           else {
               // Chain iterator
               last->next = &item->iterator;
               item->iterator.prev = last;
               item->iterator.next = NULL;
               last = &item->iterator;
           }
               
           item = item->next;
       };
   }

   return first;
}

int32 CompareHashItemsPos(void* item1, void* item2)
{
   int32 result = 0;

   uint32 pos1 = ((HashItem*)item1)->pos;
   uint32 pos2 = ((HashItem*)item2)->pos;

   if (pos1 == pos2)
       result = 0;
   if (pos1 < pos2)
       result = -1;
   if (pos1 > pos2)
       result = 1;

   return result;
}

// NOTE: Retuns hastable items in sorted order!
HashtableIterator* Hashtable::GetIteratorSorted(SortOrder order) 
{

   HashItem* item = NULL;
   HashtableIterator* first = NULL;
   HashtableIterator* last = NULL;
   
   Array items = {};
   items.Init(count, true, CompareHashItemsPos, SortOrder::Asc);

   for (uint32 slot = 0; slot < size; slot++) {
       item = table + slot;

       while (item && item->index != HASH_ITEM_UNINITIALIZED) {
           items.AddSorted(item);
           item = item->next;
       };
   }

   // First iterator item
   item = (HashItem*)items[0].data;
   first = &item->iterator;
   first->prev = NULL;
   first->next = NULL;
   last = first;
   
   for (uint32 i = 1; i < items.count; i++) {
        item = (HashItem*)items[i].data;
       // Chain iterator
       last->next = &item->iterator;
       item->iterator.prev = last;
       item->iterator.next = NULL;
       last = &item->iterator;
   }

   items.Free();
   return first;
}

HashItem* Hashtable::AllocateHashItem()
{
    HashItem* result = NULL;

    // Try to reuse alocated memory
    if (firstFree != NULL) {
        result = firstFree;
        firstFree = firstFree->next;
    }
    else {
        result = ArenaPushStruct(arena, HashItem);
    }

    if (result) {
        *result = {};
    }

    return result;
}

HashItem *Hashtable::GetHashItem(uint32 index, HashAccess access)
{
    HashItem *item = NULL;

    // Calculate index hash value
    // TODO(dainis): Look for hash generation functions to achieve even distribution
    // https://stackoverflow.com/questions/3613102/why-use-a-prime-number-in-hashcode/3613423#3613423
    uint32 hashValue = 19 * index + 7 * index + 3 * index;

    // Calculate index hash slot
    uint32 slot = hashValue & (size - 1);
    // slot = 0; // chaining debug
    Assert(slot < size);

    item = table + slot;

    do {
        if (index == item->index) {
            // Found
            break;
        }

        // Push new item if all slot items does not match index
        if (access == HashAccess::ReadWrite && item->index != HASH_ITEM_UNINITIALIZED && !item->next)
        {
            item->next = AllocateHashItem();
            item->next->prev = item;
            item = item->next;
            item->index = HASH_ITEM_UNINITIALIZED;
        }

        // Initialize first slot item or new alocated collided item
        if (access == HashAccess::ReadWrite && item->index == HASH_ITEM_UNINITIALIZED)
        {
            item->index = index;
            item->next = NULL;
            item->pos = maxPos;
           
            maxPos++;
            count++;
            break;
        }

        item = item->next;
    } while (item);

    return item;
}

void Hashtable::RemoveHashItem(HashItem* item)
{
    Assert(item);
    Assert(count > 0);

    bool32  isFirstItem = true;

    // Do some item chain cleanup
    if (item->prev != NULL) {
        isFirstItem = false;

        if (item->next != NULL) {
            item->prev->next = item->next;
        }
        else {
            item->prev->next = NULL;
        }
    }

    // Reset block
    *item = {};
    item->index = HASH_ITEM_UNINITIALIZED;
    count--;

    // Add item to free list for memory reuse if ts not first item in table
    if (!isFirstItem) {
        if (!firstFree) {
            firstFree = item;
        }
        else {
            item->next = firstFree;
            firstFree = item;
        }
    }
}

} // namespace
