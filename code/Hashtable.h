// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QHASHTABLE_H)

#include "Common.h"
#include "Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


namespace QuarksDD {

#define HASH_ITEM_UNINITIALIZED U32Max

    enum class HashAccess {
        ReadOnly,
        ReadWrite
    };

    struct HashItem;
    struct HashtableIterator 
    {
        // Data
        HashtableIterator* next;
        HashtableIterator* prev;
    };

    struct HashItem
    {
        // Data 
        uint32 pos;
        uint32 index;
        char* key;
        void* data;
        HashItem *next;
        HashItem *prev;
        // IMPORTANT: Initialized by Hashtable.GetIterator calls!
        HashtableIterator iterator;

        // Operations
        HashItem& operator=(void* data) {
            this->data = data;
            return *this;
        }
    };

    struct Hashtable
    {
        // Data
        bool32 initialized;
        bool32 useLocalArena;
        uint32 count;
        uint32 size;
        uint32 maxPos;
        MemoryArena *arena;
        HashItem *firstFree;
        HashItem* table;

        // Operations
        bool32 Init(uint32 tableSize = 4096);
        bool32 Init(MemoryArena* arena, uint32 tableSize = 4096);
        void Free();
        void Clear();
        void Remove(char* key);
        void Remove(uint32 index);
        HashItem* Add(char* key, void* data);
        HashItem* Add(uint32 index, void* data);
        
        HashItem& operator[](char* key);
        HashItem& operator[](uint32 index);

        HashItem* GetItem(char* key);
        HashItem* GetItem(uint32 index);
        
        static HashItem* GetItem(HashtableIterator* iter) {
            HashItem* result = NULL;
            if (iter) {
                result = ContainerOf(iter, HashItem, iterator);
            }
            return result;
        }

        void* GetValue(char* key);
        void* GetValue(uint32 index);

        bool32 HasKey(char* key);
        bool32 HasIndex(uint32 index);

        bool32 IsEmpty();

        HashtableIterator* GetIterator();
        HashtableIterator* GetIteratorSorted(SortOrder order = SortOrder::Asc);

        

    private:
        HashItem* AllocateHashItem();
        HashItem* GetHashItem(uint32 index, HashAccess access = HashAccess::ReadOnly);
        void RemoveHashItem(HashItem* item);
    };

    extern int32 CompareHashItemsPos(void* item1, void* item2);

} // namespace

#define QHASHTABLE_H
#endif