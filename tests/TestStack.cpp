#if !defined(QTESTSTACK_CPP)

#include <QuarksDD.h>
#include <Memory.h>
#include <Stack.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestStack
{

internal uint32 StackInit()
{
    MemoryArena arena = {};
    uint32 testSize = 10;

    Stack s = {};
    s.Init(testSize);

    _Assert(s.items != NULL, "s.items != NULL");
    _Assert(s.size == testSize, "s.size == %d", testSize);
    _Assert(s.top == -1, "s.top == -1");

    s.Free();
    return true;
}

internal uint32 StackInitArena()
{
    uint32 testSize = 10;
    MemoryArena arena = {};
    arena.Init();

    Stack s = {};
    s.Init(&arena, testSize);

    _Assert(s.items != NULL, "s.items != NULL");
    _Assert(s.top == -1, "s.top == -1");
    _Assert(arena.block != 0, "arena.block != 0");
    _Assert(arena.block->size == (s.size * sizeof(StackItem)), "arena.block->size == (s.size * sizeof(StackItem)");

    s.Free();
    arena.Free();
    return true;
}

internal uint32 StackFree()
{
    uint32 testSize = 10;
    Stack s = {};
    s.Init(testSize);
    s.Free();

    _Assert(s.items == NULL, "s.items == NULL");
    _Assert(s.size == 0, "s.size == 0");
    _Assert(s.top == 0, "s.top == 0");
    _Assert(s.initialized == 0, "s.initilized == 0");

    return true;
}

internal uint32 StackFreeArena()
{
    uint32 testSize = 10;

    MemoryArena arena = {};
    arena.Init(testSize);

    Stack s = {};
    s.Init(&arena);

    s.Free();
    arena.Free();

    _Assert(s.items == NULL, "s.items == NULL");
    _Assert(s.size == 0, "s.size == 0");
    _Assert(s.top == 0, "s.top == 0");
    _Assert(arena.block == 0, "arena.block == 0");

    return true;
}

internal uint32 StackPushPop()
{
    int32 values[] = {-1, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Stack s = {};
    s.Init(10);

    for(uint32 i = 0; i < 10; i++) {
        s.Push(&values[i]);
    }

    _Assert(s.size == 10, "s.size == 10");
    _Assert(s.top == (s.size -1), "s.top == (s.size-1)");

    bool32 pass = true;
    bool32 index = 9;
    while(!s.IsEmpty() && index >= 0) {
        int32* value = (int32*)s.Pop();
        printf("%d, ", *value);
        int32 testValue = values[index--];
    	if (value == NULL || *value != testValue) {
            pass = false;
            break;
        }
    }
    _Assert(pass == true, "pass == true");

    s.Free();
    return true;
}

internal uint32 StackClear()
{
    Stack s = {};
    s.Init(10);

    s.Push(NULL);
    s.Push(NULL);
    s.Push(NULL);
    s.Push(NULL);

    s.Clear();
    _Assert(s.top == -1, "s.top == -1");

    s.Free();
    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Stack test group\n");
    printf("===========================================\n");
    _RunTest(StackInit);
    _RunTest(StackInitArena);
    _RunTest(StackFree);
    _RunTest(StackFreeArena);
    _RunTest(StackPushPop);
    _RunTest(StackClear);
}

} // namespace

#define QTESTSTACK_CPP
#endif