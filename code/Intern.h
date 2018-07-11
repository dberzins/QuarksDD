// Derived implementation from Per Vognsen
//      Code From: https://github.com/pervognsen/bitwise/blob/master/ion/common.c
// NOTICE: This code is related to Bitwise and is released into the public domain.
//      If you have derived benefit from Bitwise or have incorporated parts of Bitwise directly into your own 
//      work and products,  Bitwise's author, Per Vognsen, would appreciate an email describing how and where it was used, 
//      but you are under no legal obligation to do so.

#if !defined(QINTERN_H)

#include "QuarksDD.h"
#include "Memory.h"
#include <stdlib.h>
#include <string.h>
#include "Map.h"

namespace QuarksDD {

#define DEAFAULT_INTERN_STR_LEN 128
#define DEAFAULT_INTERN_TABLE_SIZE 1024

struct Intern {
    SizeType len;
    Intern *next;
    char* str;
};

struct InternTable {
    // Data 
    bool32 initialized;
    MemoryArena* arena;
    MemoryArena* localArena;
    bool32 useHeapAlloc;
    Map interns;

    // Operations
    bool32 Init(uint32 size = DEAFAULT_INTERN_TABLE_SIZE) {
        bool32 result = false;

        if (!initialized) {
            useHeapAlloc = true;

            localArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
            *localArena = {};
            MemorySize memSize = sizeof(Intern) * size + DEAFAULT_INTERN_STR_LEN * size; 
            localArena->Init(memSize);
        }
        return result;
        
    }

    bool32 Init(MemoryArena* arena, uint32 size = DEAFAULT_INTERN_TABLE_SIZE) {
        bool32 result = false;

        if (!initialized && arena && arena->initialized) {
            useHeapAlloc = false;
            this->arena = arena;

            MemorySize memSize = sizeof(Intern) * size + DEAFAULT_INTERN_STR_LEN * size; 
            localArena =  arena->CreateChildArena(memSize);
        }
        return result;
        
    }

    void Free() {
        if (initialized) {
            if (useHeapAlloc) {
                if (localArena) {
                    localArena->Free();
                    Deallocate(localArena);
                }
            }
            else if (arena) {
                if (localArena) {
                    arena->FreeChildArena(localArena);
                    localArena = NULL;
                }
            }
            interns.Free();
        }

        *this = {};
    }

    const char *InternRange(const char* start, const char* end) {
        SizeType len = end - start;
        uint64 hash = HashBytes(start, len);
        uint64 key = hash ? hash : 1; // 0 key in Map assumed as empty slots
        Intern *intern = (Intern*)interns.Get(key);

        // Hnadle Map hashtable colisions externaly
        for (Intern *it = intern; it; it = it->next) {
            if (it->len == len && strncmp(it->str, start, len) == 0) {
                return it->str;
            }
        }

        Intern* newIntern = ArenaPushStruct(localArena, Intern);
        newIntern->str = (char*)ArenaPushSize(localArena, len + 1);

        newIntern->len = len;
        newIntern->next = intern;
        memcpy(newIntern->str, start, len);
        newIntern->str[len] = 0;
        interns.Put(key, newIntern);

        return newIntern->str;
    }

    const char *InternStr(const char *str) {
        return InternRange(str, str + strlen(str));
    }
};

} // namespace
#define QINTERN_H
#endif