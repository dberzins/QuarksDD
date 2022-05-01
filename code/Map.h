// Derived implementation from Per Vognsen
//      Code From: https://github.com/pervognsen/bitwise/blob/master/ion/common.c
//      Video: https://bitwise.handmade.network/episode/bitwise/bitwise012/#1066
// NOTICE: This code is related to Bitwise and is released into the public domain.
//      If you have derived benefit from Bitwise or have incorporated parts of Bitwise directly into your own 
//      work and products,  Bitwise's author, Per Vognsen, would appreciate an email describing how and where it was used, 
//      but you are under no legal obligation to do so.

#if !defined(QMAP_H)

#include "QuarksDD.h"
#include "Memory.h"
#include <stdlib.h>

namespace QuarksDD {

#define MAP_ITEM_UNINITIALIZED 0

internal uint64 HashUint64(uint64 x) {
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 32;
    return x;
}

internal uint64 HashPtr(const void *ptr) {
    return HashUint64((uintptr)ptr);
}

internal uint64 HashMix(uint64 x, uint64 y) {
    x ^= y;
    x *= 0xff51afd7ed558ccd;
    x ^= x >> 32;
    return x;
}

internal uint64 HashBytes(const void *ptr, SizeType count) {
    uint64 x = 0xcbf29ce484222325;
    const char *buf = (const char *)ptr;
    for (SizeType i = 0; i < count; i++) {
        x ^= buf[i];
        x *= 0x100000001b3;
        x ^= x >> 32;
    }
    return x;
}

struct Map {
    // Data
    uint64* keys;
    uint64* items;
    SizeType count;
    SizeType size;

    // Operations
    void Free() {
        if (keys) {
            Deallocate(keys);
        }
        if (items) {
            Deallocate(items);
        }
        *this = {};
    }

    void Extend(SizeType newSize) {
        newSize = ClampMin(newSize, 16);
        
        Map newMap = {};
        newMap.keys = (uint64*)XCallocate(newSize, sizeof(uint64));
        newMap.items = (uint64*)XAllocate(newSize * sizeof(uint64));
        newMap.size = newSize;

        for (SizeType i = 0; i < size; i++) {
            if (keys[i]) {
                newMap.PutUint64(keys[i], items[i]);
            }
        }
        Deallocate((void *)keys);
        Deallocate(items);
        *this = newMap;
    }
   
    uint64 GetUint64(uint64 key) {
        if (count == 0) {
            return 0;
        }
        Assert(IsPowOf2(size));
        SizeType hash = (SizeType)HashUint64(key);
        SizeType slot = hash;
        Assert(count < size);
        for (;;) {
            slot &= (size - 1);
            if (keys[slot] == key) {
                return items[slot];
            } else if (!keys[slot]) {
                return 0;
            }
            slot++;
        }
        return 0;
    }

    uint64 Get(void* key) {
        return GetUint64((uint64)(uintptr)key);
    }

    void* Get(uint64 key) {
        return (void*)(uintptr)GetUint64(key);
    }

    void* Get(const void* key) {
        return (void*)(uintptr)GetUint64((uint64)(uintptr)key);
    }

    void PutUint64(uint64 key, uint64 item) {
        Assert(key);
        if (!item) {
            return;
        }
        if (2 * count >= size) {
            Extend(2 * size);
        }
        Assert(2 * count < size);
        Assert(IsPowOf2(size));
        SizeType hash = (SizeType)HashUint64(key);
        SizeType slot = hash;
        for (;;) {
            slot &= (size - 1);
            if (!keys[slot]) {
                count++;
                keys[slot] = key;
                items[slot] = item;
                return;
            } else if (keys[slot] == key) {
                items[slot] = item;
                return;
            }
            slot++;
        }
    }

    void Put(void* key, uint64 item) {
        PutUint64((uint64)(uintptr)key, item);
    }

    void Put(const void* key, void* item) {
        PutUint64((uint64)(uintptr)key, (uint64)(uintptr)item);
    }

    void Put(uint64 key, void* item) {
        PutUint64(key, (uint64)(uintptr)item);
    }
};

} // namespace
#define QMAP_H
#endif