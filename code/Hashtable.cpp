// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Hashtable.h"
#include "Array.h"
#include "Murmur.h"

namespace QuarksDD {

bool32  Hashtable::Init(uint32 tableSize, MemorySize dataSize, MemorySize keySize)
{
    bool32 result = false;

    if (!initialized) {
        // Enforce power of 2 size for table
        size = RoundUpToPowerOf2(tableSize);

        Assert(keySize == 0 || ( keySize > 1));
        this->keySize = keySize;
        this->dataSize = dataSize;
        
        // Allocate hashtable bolcks memory (1 primmary slot + 1 colision slot = 2)
        itemArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *itemArena = {};
        itemArena->Init(size * 2 * sizeof(HashItem), true);
        table = ArenaPushArray(itemArena, size, HashItem);

        if (keySize) {
            keyArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
            *keyArena = {};
            keyArena->Init(size * 2 * keySize, true);
        }

        if (dataSize > 0) {
            dataArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
            *dataArena = {};
            dataArena->Init(size * 2 * dataSize, true);
        }

        if (table) {
            for (uint32 i = 0; i < size; ++i) {
                *(table + i) = {};
                (table + i)->index = HASH_ITEM_UNINITIALIZED;

                if (keySize > 0 && keyArena) {
                    (table + i)->key = (char*)ArenaPushSize(keyArena, keySize);
                }

                if (dataSize > 0 && dataArena) {
                    (table + i)->data = (void*)ArenaPushSize(dataArena, dataSize);
                }
            }

            useLocalArena = true;
            initialized = true;
            result = true;
        }
    }
    return result;
}

bool32  Hashtable::Init(MemoryArena *arena, uint32 tableSize, MemorySize dataSize, MemorySize keySize)
{
    bool32 result = false;

    if (!initialized) {
        useLocalArena = false;
        this->arena = arena;

        Assert(arena);
        Assert(keySize == 0 || ( keySize > 1));
        this->keySize = keySize;
        this->dataSize = dataSize;

        // Enforce power of 2 size for table
        count = 0;            
        size = RoundUpToPowerOf2(tableSize);
        
        // Allocate hashtable bolcks memory (1 primmary slot + 1 colision slot = 2)
        itemArena = arena->CreateChildArena(size * 2 * sizeof(HashItem));
        table = (HashItem*)ArenaPushArray(itemArena, size, HashItem);
        
        if (keySize > 0) {
            keyArena = arena->CreateChildArena(size * 2 * keySize);
        }

        if (dataSize > 0) {
            dataArena = arena->CreateChildArena(size * 2 * dataSize);
        }
        
        if (table) {
            for (uint32 i = 0; i < size; ++i) {
                *(table + i) = {};
                (table + i)->index = HASH_ITEM_UNINITIALIZED;

                if (keySize > 0 && keyArena) {
                    (table + i)->key = (char*)ArenaPushSize(keyArena, keySize);
                }

                if (dataSize > 0 && dataArena) {
                    (table + i)->data = (void*)ArenaPushSize(dataArena, dataSize);
                }
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
            if (itemArena) {
                itemArena->Free();
                Deallocate(itemArena);
                itemArena = NULL;
            }
            
            if (keyArena) {
                keyArena->Free();
                Deallocate(keyArena);
                keyArena = NULL;
            }

            if (dataArena) {
                dataArena->Free();
                Deallocate(dataArena);
                dataArena = NULL;
            }
        }
        else {
            if (arena) {
                if (itemArena) {
                    arena->FreeChildArena(itemArena);
                    itemArena = NULL;
                }
                if (keyArena) {
                    arena->FreeChildArena(keyArena);
                    keyArena = NULL;
                }
                if (dataArena) {
                    arena->FreeChildArena(dataArena);
                    dataArena = NULL;
                }
            }
        }
    }

    table = NULL;
    firstFree = NULL;
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
                item->Reset(dataSize, keySize);

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
    uint32 index = ToIndex(key);
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
    uint32 index = ToIndex(key);
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    if (result) {
        if (keySize > 0) {
            if (key) {
                memcpy(result->key, key, keySize -1);
                result->key[keySize - 1] = 0;
            }
        }
        else {
            result->key = key;
        }

        if (dataSize > 0) {
            if (data) {
                memcpy(result->data, data, dataSize);
            }
        }
        else {
            result->data = data;
        }
    }
    return result;
}

HashItem* Hashtable::Add(uint32 index, void* data = NULL) {
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
   
    if (result) {
        if (dataSize > 0) {
            if (data) {
                memcpy(result->data, data, dataSize);
            }
        }
        else {
            result->data = data;
        }
    }
    return result;
}

HashItem& Hashtable::operator[](char* key)
{
    uint32 index = ToIndex(key);
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    if (result) {
        if (keySize > 0) {
            if (key) {
                memcpy(result->key, key, keySize - 1);
                result->key[keySize - 1] = 0;
            }
        }
        else {
            result->key = key;
        }
    }
    return *result;
};

HashItem& Hashtable::operator[](uint32 index)
{
    HashItem* result = GetHashItem(index, HashAccess::ReadWrite);
    return *result;
};

HashItem* Hashtable::GetItem(char* key) {
    uint32 index = ToIndex(key);
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

bool32  Hashtable::HasIndex(uint32 index) {
    HashItem* item = GetItem(index);
    return item != NULL;
}

bool32  Hashtable::IsEmpty() { 
        return size <= 0;
}

HashItem* Hashtable::GetFirstItem() {
    HashItem* result = NULL;

    for (uint32 slot = 0; slot < size; slot++) {
        HashItem* item = table + slot;
        if (item && item->index != HASH_ITEM_UNINITIALIZED) {
            result = item; 
            return result;
        }
    }

    return result;
}

HashItem* Hashtable::GetNextItem(HashItem* item) {
    HashItem* result = NULL;
    if (item) {
        bool32 found = false;
        for (uint32 slot = item->slot; slot < size; slot++) {
            HashItem* testItem = table + slot;
            
            if (item == testItem) {
                testItem = item->next;
                found = true;
            }
            else if (found && testItem && testItem->index != HASH_ITEM_UNINITIALIZED) {
                result = testItem;
                return result;
            }

            while (testItem) {
                if (item == testItem) {
                    found = true;
                }
                else if (found && testItem->index != HASH_ITEM_UNINITIALIZED) {
                    result = testItem;
                    return result;
                }
                testItem = testItem->next;
            };
        }
    }
    return result;
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
   items.Init(count, 0, true, CompareHashItemsPos, SortOrder::Asc);

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
        result = ArenaPushStruct(itemArena, HashItem);
        if (keySize > 0) {
            result->key = (char*)ArenaPushSize(keyArena, keySize);
        }

        if (dataSize > 0) {
            result->data = ArenaPushSize(dataArena, dataSize);
        }
    }

    if (result) {
        result->Init();
        // result->Reset(dataSize, keySize);
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
            // item->next = NULL; // IMPORTANT: this will brake bucket chain if first bucket was removed!!!
            item->pos = maxPos;
            item->slot = slot;
           
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

    // Reset item
    item->Reset(dataSize, keySize);
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

uint32 Hashtable::ToIndex(char* key) {
    uint32 result =  Murmur3x32(key, (SizeType)strlen(key), 0);
    return result;
}

} // namespace
