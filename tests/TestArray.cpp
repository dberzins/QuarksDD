#if !defined(QTESTARRAY_CPP)

#include <Common.h>
#include <Memory.h>
#include <Array.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestArray
{

internal uint32 ArrayInit()
{
    MemoryArena arena2 {};
    MemoryArena arena = {};
    uint32 testSize = 10;

    Array a = {};
    a.Init(testSize);

    _Assert(a.items != NULL, "a.items != NULL");
    _Assert(a.size - a.extend == testSize, "a.size - a.extend == %d", testSize);
    _Assert(a.count == 0, "a.count == 0");

    a.Free();
    return true;
}

internal uint32 ArrayInitArena()
{
    uint32 testSize = 10;
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena, testSize);

    _Assert(a.items != NULL, "a.items != NULL");
    _Assert(a.extend == 5.0f, "a.extend == 5.0f");
    _Assert(a.count == 0, "a.count == 0");
    _Assert(arena.block != 0, "arena.block != 0");
    _Assert(arena.block->size == (a.size * sizeof(ArrayItem)), "arena.block->size == (a.size * sizeof(ArrayItem)");

    a.Free();
    arena.Free();
    return true;
}

internal uint32 ArrayFree()
{
    uint32 testSize = 10;
    Array a = {};
    a.Init(testSize);
    a.Free();

    _Assert(a.items == NULL, "a.items == NULL");
    _Assert(a.size == 0, "a.size == 0");
    _Assert(a.count == 0, "a.count == 0");

    return true;
}

internal uint32 ArrayFreeArena()
{
    uint32 testSize = 10;

    MemoryArena arena = {};
    arena.Init(testSize);

    Array a = {};
    a.Init(&arena);

    a.Free();
    arena.Free();

    _Assert(a.items == NULL, "a.items == NULL");
    _Assert(a.size == 0, "a.size == 0");
    _Assert(a.count == 0, "a.count == 0");
    _Assert(arena.block == 0, "arena.block == 0");

    return true;
}

internal uint32 ArrayExtend()
{
    Array a = {};
    a.Init(1);

    uint32 startSize = a.size;
    for (uint32 i = 0; i <= startSize; i++) {
        a.Add(NULL);
    }

    _Assert(a.size > startSize, "a.size > startSize");

    a.Free();
    return true;
}

internal uint32 ArrayExtendArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena, 1);

    uint32 startSize = a.size;
    for (uint32 i = 0; i <= startSize; i++) {
        a.Add(NULL);
    }

    _Assert(a.size > startSize, "a.size > startSize");

    a.Free();
    arena.Free();
    return true;
}

internal uint32 ArrayAdd()
{
    Array a = {};
    a.Init();

    uint32 startCount = a.count;

    int32 value1 = 999;
    a.Add(&value1);
    _Assert(a.count == startCount + 1, "a.count == startCount + 1");
    int32 fetchValue = *(int32*)a[a.count-1].data;
    _Assert(fetchValue == value1, "fetchValue == %d", value1);

    int32 value2 = 555;
    a.Add(&value2);
    _Assert(a.count == startCount + 2, "a.count == startCount + 2");
    fetchValue = *(int32*)a[a.count-1].data;
    _Assert(fetchValue == value2, "fetchValue == %d", value2);

    a.Free();
    return true;
}

internal uint32 ArrayAddArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena);

    uint32 startCount = a.count;

    int32 value1 = 999;
    a.Add(&value1);
    _Assert(a.count == startCount + 1, "a.count == startCount + 1");
    int32 fetchValue = *(int32*)a[a.count-1].data;
    _Assert(fetchValue == value1, "fetchValue == %d", value1);

    int32 value2 = 555;
    a.Add(&value2);
    _Assert(a.count == startCount + 2, "a.count == startCount + 2");
    fetchValue = *(int32*)a[a.count-1].data;
    _Assert(fetchValue == value2, "fetchValue == %d", value2);

    a.Free();
    arena.Free();
    return true;
}

internal uint32 ArrayInsert()
{
    Array a = {};
    a.Init(10);

    int32 empty = 0;
    for (uint32 i = 0; i <10; i++) {
        a.Add(&empty);
    }
    _Assert(a.count == 10, "a.count == 10");

    uint32 count = a.count;

    int32 value1 = 999;
    uint32 pos1 = 5;
    a.Insert(pos1, &value1);
    _Assert(a.count == count +1 , "a.count == count +1");
    int32 fetchValue1 = *(int32*)a[pos1].data;
    _Assert(fetchValue1 == value1, "fetchValue1 == value1");

    int32 value2 = 555;
    uint32 pos2 = 2;
    count = a.count;
    a.Insert(pos2, &value2);
    _Assert(a.count == count +1 , "a.count == count +1");
    int32 fetchValue2 = *(int32*)a[pos2].data;
    _Assert(fetchValue2 == value2, "fetchValue2 == value2");

    int32 shiftedValuePos1 = *(int32*)a[pos1+1].data;
    _Assert(shiftedValuePos1 == value1, "shiftedValuePos1 == value1");

    a.Free();
    return true;
}

internal uint32 ArrayInsertArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena, 10);

    int32 empty = 0;
    for (uint32 i = 0; i <10; i++) {
        a.Add(&empty);
    }
    _Assert(a.count == 10, "a.count == 10");

    uint32 count = a.count;

    int32 value1 = 999;
    uint32 pos1 = 5;
    a.Insert(pos1, &value1);
    _Assert(a.count == count +1 , "a.count == count +1");
    int32 fetchValue1 = *(int32*)a[pos1].data;
    _Assert(fetchValue1 == value1, "fetchValue1 == value1");

    int32 value2 = 555;
    uint32 pos2 = 2;
    count = a.count;
    a.Insert(pos2, &value2);
    _Assert(a.count == count +1 , "a.count == count +1");
    int32 fetchValue2 = *(int32*)a[pos2].data;
    _Assert(fetchValue2 == value2, "fetchValue2 == value2");

    int32 shiftedValuePos1 = *(int32*)a[pos1+1].data;
    _Assert(shiftedValuePos1 == value1, "shiftedValuePos1 == value1");

    a.Free();
    arena.Free();
    return true;
}

internal uint32 ArrayInsertAdd()
{
    Array a = {};
    a.Init(1);

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8 , 9, 10};
    
    a.Add(&values1[0]); // 1
    a.Add(&values1[1]); // 2
    a.Add(&values1[2]); // 3
    a.Insert(1, &values1[2]); // 3
    a.Insert(1, &values1[3]); // 4
    a.Add(&values1[4]); // 5
    a.Insert(0, &values1[5]); // 6
    a.Add(&values1[6]); // 7
    a.Insert(a.count-1, &values1[7]); // 8
    a.Add(&values1[8]); // 9
    a.Insert(4, &values1[9]); //10

    int32 values2 [] = {6, 1, 4, 3, 10, 2, 3, 5, 8, 7, 9};

    char result[80];
    bool32 pass = true;
    int32 charsWritten  = 0;
    for(uint32 i=0; i < a.count; i++) {
        int32 val = *(int32*)a[i].data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{4, 1, 2, 5, 10, 6, 7, 9} = {%s}", result);

    a.Free();
    return true;
}

internal uint32 ArrayInsertAddArena()
{
    MemoryArena arena = {};
    Array a = {};
    a.Init(&arena, 1);

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8 , 9, 10};
    
    a.Add(&values1[0]); // 1
    a.Add(&values1[1]); // 2
    a.Add(&values1[2]); // 3
    a.Insert(1, &values1[2]); // 3
    a.Insert(1, &values1[3]); // 4
    a.Add(&values1[4]); // 5
    a.Insert(0, &values1[5]); // 6
    a.Add(&values1[6]); // 7
    a.Insert(a.count-1, &values1[7]); // 8
    a.Add(&values1[8]); // 9
    a.Insert(4, &values1[9]); //10

    int32 values2 [] = {6, 1, 4, 3, 10, 2, 3, 5, 8, 7, 9};

    char result[80];
    bool32 pass = true;
    int32 charsWritten  = 0;
    for(uint32 i=0; i < a.count; i++) {
        int32 val = *(int32*)a[i].data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{4, 1, 2, 5, 10, 6, 7, 9} = {%s}", result);

    a.Free();
    arena.Free();
    return true;
}

internal uint32 ArrayRemove()
{
    Array a = {};
    a.Init();

    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        a.Add(&values[i]);
    }

    a.Remove(5); // 6
    a.Remove(0); // 1
    a.Remove(a.count-1); // 10

    char result[20];
    bool32 pass = a.count == 7;
    int32 charsWritten  = 0;
    for(uint32 i=0; i<a.count; i++) {
        int32 val = *(int32*)a[i].data;
        if (val == 6 || val == 1 || val== 10) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{2, 3, 4, 5, 7, 8, 9} = {%s}", result);

    a.Free();
    return true;
}

internal uint32 ArrayRemoveArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena);

    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        a.Add(&values[i]);
    }

    a.Remove(5); // 6
    a.Remove(0); // 1
    a.Remove(a.count-1); // 10

    char result[20];
    bool32 pass = a.count == 7;
    int32 charsWritten  = 0;
    for(uint32 i=0; i<a.count; i++) {
        int32 val = *(int32*)a[i].data;
        if (val == 6 || val == 1 || val== 10) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{2, 3, 4, 5, 7, 8, 9} = {%s}", result);

    a.Free();
    arena.Free();

    return true;
}

internal uint32 ArrayClear()
{
    Array a = {};
    a.Init();

    uint32 startCount = a.count;

    a.Add(NULL);
    a.Add(NULL);
    a.Add(NULL);
    a.Add(NULL);

    a.Clear();
    _Assert(a.count == startCount, "a.count == startCount");

    a.Free();
    return true;
}

internal uint32 ArrayClearArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena);

    uint32 startCount = a.count;

    a.Add(NULL);
    a.Add(NULL);
    a.Add(NULL);
    a.Add(NULL);

    a.Clear();
    _Assert(a.count == startCount, "a.count == startCount");

    a.Free();
    arena.Free();

    return true;
}

internal uint32 ArrayItemValueAssigne()
{
    Array a = {};
    a.Init(1);

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    for (uint32 i = 0; i < ArrayCount(values1); i++) {
        a.Add(&values1[i]);
    }
    for (uint32 i = 0; i < ArrayCount(values2); i++) {
        a[i] = &values2[i];
    }


    char result[40];
    bool32 pass = true;
    int32 charsWritten  = 0;
    for(uint32 i=0; i < ArrayCount(values2); i++) {
        int32 val = *(int32*)a[i].data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } = {%s}", result);

    a.Free();
    return true;
}

internal uint32 ArrayItemValueAssigneArena()
{
    MemoryArena arena = {};
    arena.Init();

    Array a = {};
    a.Init(&arena, 1);

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    for (uint32 i = 0; i < ArrayCount(values1); i++) {
        a.Add(&values1[i]);
    }
    for (uint32 i = 0; i < ArrayCount(values2); i++) {
        a[i] = &values2[i];
    }


    char result[40];
    bool32 pass = true;
    int32 charsWritten  = 0;
    for(uint32 i=0; i < ArrayCount(values2); i++) {
        int32 val = *(int32*)a[i].data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }
    }

    _Assert(pass, "{11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } = {%s}", result);

    a.Free();
    arena.Free();

    return true;
}



internal void Run()
{
    printf("===========================================\n");
    printf("Start Array test group\n");
    printf("===========================================\n");
    _RunTest(ArrayInit);
    _RunTest(ArrayInitArena);
    _RunTest(ArrayFree);
    _RunTest(ArrayFreeArena);
    _RunTest(ArrayExtend);
    _RunTest(ArrayExtendArena);
    _RunTest(ArrayAdd);
    _RunTest(ArrayAddArena);
    _RunTest(ArrayInsert);
    _RunTest(ArrayInsertArena);
    _RunTest(ArrayInsertAdd);
    _RunTest(ArrayInsertAddArena);
    _RunTest(ArrayRemove);
    _RunTest(ArrayRemoveArena);
    _RunTest(ArrayClear);
    _RunTest(ArrayClearArena);
    _RunTest(ArrayItemValueAssigne);
    _RunTest(ArrayItemValueAssigneArena);
}

} // namespace

#define QTESTARRAY_CPP
#endif