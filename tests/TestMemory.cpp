#if !defined(QTESTMEMORY_CPP)
#include <Common.h>
#include <Memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"
using namespace QuarksDD;
namespace TestMemory {

internal uint32 TestMemoryInit()
{

    MemoryArena arena = {};
    arena.Init(10);
    
    _Assert(arena.initialized == true, "arena.initialized == true");
    _Assert(arena.count == 1, "arena.size == 1");
    _Assert(arena.block != NULL, "arena.block != NULL");
    _Assert(arena.block->size == 10, "arena.block->size == 10");
    
    arena.Free();
    return true;
}

internal uint32 TestMemoryInitExtend()
{

    MemoryArena arena = {};
    arena.Init(10, true, 2.0f);
    
    _Assert(arena.initialized == true, "arena.initialized == true");
    _Assert(arena.block->size == 10, "arena.block->size == 10");
    _Assert(arena.count == 1, "arena.size == 1");
    _Assert(arena.block != NULL, "arena.block != NULL");

    void* m = ArenaPushSize(&arena, 20);

    _Assert(arena.count == 2, "arena.size == 2");
    _Assert(arena.block->size == 40, "arena.block->size == 40");
    _Assert(arena.block->used == 20, "arena.block->used == 20");

    
    arena.Free();
    return true;
}

internal uint32 TestMemoryInitZeroSize()
{

    MemoryArena arena = {};
    arena.Init();
    
    _Assert(arena.initialized == true, "arena.initialized == true");
    _Assert(arena.count == 0, "arena.size == 0");
    _Assert(arena.block == NULL, "arena.block == NULL");
    
    ArenaPushSize(&arena, 10);
    
    _Assert(arena.block != NULL, "arena.block != NULL");
    _Assert(arena.count == 1, "arena.count == 1");
    _Assert(arena.block->size == 10, "arena.block->size == 10");


    arena.Free();
    return true;
}

internal uint32 TestMemoryInitZero()
{

    MemoryArena arena = {};
    arena.Init(5);
    
    bool32 pass = true;
    for (uint32 i = 0; i < 5; i++) {
        if (*(arena.block->base + i) != 0) {
            pass = false;
            break;
        }
    }   

    _Assert(arena.initialized == true, "arena.initialized == true");
    _Assert(pass == true, "pass == true");
    
    arena.Free();
    return true;
}

internal uint32 TestMemoryFree()
{

    MemoryArena arena = {};
    arena.Init(5);
    arena.Free();

    _Assert(arena.initialized == false, "arena.initialized == false");
    _Assert(arena.block == NULL, "arena.block == NULL");
    _Assert(arena.count == 0, "arena.count == 0");
    
    return true;
}

internal uint32 TestMemoryFreeBlock()
{
    // BLOCK
    //   V 
    // [LAST]-[MID3]-[MID2]-[MID1]-[FIRST]
    //

    MemoryArena arena = {};
    arena.Init(5);
    ArenaPushSize(&arena, 5);
    MemoryBlock* first = arena.block;

    ArenaPushSize(&arena, 5);
    MemoryBlock* mid1 = arena.block;

    ArenaPushSize(&arena, 5);
    MemoryBlock* mid2 = arena.block;

    ArenaPushSize(&arena, 5);
    MemoryBlock* mid3 = arena.block;

    ArenaPushSize(&arena, 5);
    MemoryBlock* last = arena.block;

    _Assert(arena.count == 5, "arena.count == 5");
    
    // Delete [MID1]
    arena.Free(mid1);

    // BLOCK
    //   V 
    // [LAST]-[MID3]-[MID2]-[FIRST]
    //

    _Assert(arena.count == 4, "arena.count == 4");
    _Assert(first->next  == mid2, "first->next == mid2");
    _Assert(mid2->prev  == first, "mid2->prev == first");


    // Delete [FIRST]
    arena.Free(first);

    // BLOCK
    //   V 
    // [LAST]-[MID3]-[MID2]
    //

    _Assert(arena.count == 3, "arena.count == 3");
    _Assert(mid2->next  == mid3, "mid2->next == mid3");
    _Assert(mid3->prev  == mid2, "mid3->prev == mid2");

    // Delete [LAST]
    arena.Free(last);

    // BLOCK
    //   V 
    // [MID3]-[MID2]
    //

    _Assert(arena.count == 2, "arena.count == 2");
    _Assert(mid3  == arena.block, "mid3 == arena.block");
    _Assert(mid3->next  == NULL, "mid3->next == NULL");
    _Assert(mid3->prev  == mid2, "mid3->prev == mid2");

    // Delete all left over blocks [MID3] and [MID2]
    arena.Free(mid3);
    arena.Free(mid2);

    _Assert(arena.initialized == true, "arena.initialized == true");
    _Assert(arena.block == NULL, "arena.block == NULL");
    _Assert(arena.count == 0, "arena.count == 0");
    
    return true;
}

internal uint32 TestMemoryExtend()
{
    bool32 extended = false;
    MemoryArena arena = {};
    arena.Init();
    
    extended = arena.Extend(5);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 1, "arena.count == 1"); 
    _Assert(arena.block != NULL, "arena.block != NULL");
    _Assert(arena.block->size == 5, "arena.block->size == 5");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    extended = arena.Extend(10);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 2, "arena.count == 2"); 
    _Assert(arena.block->size == 10, "arena.block->size == 10");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    extended = arena.Extend(15);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 3, "arena.count == 3"); 
    _Assert(arena.block->size == 15, "arena.block->size == 15");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    arena.Free();
    return true;
}

internal uint32 TestMemoryExtendWithExtendFactor()
{
    bool32 extended = false;
    MemoryArena arena = {};
    arena.Init(5, true, 2.0f);
    
    extended = arena.Extend(5);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 2, "arena.count == 2"); 
    _Assert(arena.block != NULL, "arena.block != NULL");
    _Assert(arena.block->size == 10, "arena.block->size == 5");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    extended = arena.Extend(10);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 3, "arena.count == 3"); 
    _Assert(arena.block->size == 20, "arena.block->size == 20");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    extended = arena.Extend(15);
    _Assert(extended == true, "extended == true");
    _Assert(arena.count == 4, "arena.count == 4"); 
    // last bolck size is used (20) if its greater than extend size (15) 
    _Assert(arena.block->size == 40, "arena.block->size == 40");
    _Assert(arena.block->base != NULL, "arena.block->base != NULL");

    arena.Free();
    return true;
}

internal uint32 TestMemoryPush()
{
    void* result = NULL;
    MemoryArena arena = {};
    arena.Init(5, true);
    
    result = arena.Push(5);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 1, "arena.count == 1");
    _Assert(arena.block->used == arena.block->size, "arena.block->used == arena.block->size");

    result = arena.Push(10);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 2, "arena.count == 2");
    _Assert(arena.block->used == arena.block->size, "arena.block->used == arena.block->size");

    arena.Extend(15);
    result = arena.Push(20);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 4, "arena.count == 4");
    _Assert(arena.block->used == arena.block->size, "arena.block->used == arena.block->size");

    arena.Extend(30);
    result = arena.Push(20);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 5, "arena.count == 5");
    _Assert(arena.block->used == 20, "arena.block->used == 20");

    result = arena.Push(5);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 5, "arena.count == 5");
    _Assert(arena.block->used == 25, "arena.block->used == 25");

    result = arena.Push(6);
    _Assert(result != NULL, "result != NULL");
    _Assert(arena.count == 6, "arena.count == 6");
    _Assert(arena.block->used == 6, "arena.block->used == 6");
    _Assert(arena.block->size == 30, "arena.block->size == 30");

    arena.Free();
    return true;
}

internal uint32 TestMemoryFindBlock()
{
    MemoryArena arena = {};
    arena.Init();
    
    // Index = 0
    ArenaPushSize(&arena, 10);
    // Index = 1
    ArenaPushSize(&arena, 10);
    // Index = 2
    ArenaPushSize(&arena, 10);
    // Index = 3
    ArenaPushSize(&arena, 10);
    MemoryBlock* search1 = arena.block;
    // Index = 4
    ArenaPushSize(&arena, 10);
    // Index = 5
    ArenaPushSize(&arena, 10);
    // Index = 6
    ArenaPushSize(&arena, 10);
    MemoryBlock* search2 = arena.block;
    // Index = 7
    ArenaPushSize(&arena, 10);
    // Index = 8
    ArenaPushSize(&arena, 10);
    MemoryBlock* search3 = arena.block;
    // Index = 9
    ArenaPushSize(&arena, 10);
    // Index = 10
    ArenaPushSize(&arena, 10);
    // Index = 11
    ArenaPushSize(&arena, 10);
    MemoryBlock* search4 = arena.block;
    // Index = 12
    ArenaPushSize(&arena, 10);

    uint8* source1 = search1->base + 2;
    uint8* source2 = search2->base;
    uint8* source3 = search3->base;
    uint8* source4 = search4->base + 5;
    MemoryBlockIndex result;

    result = arena.FindBlock(source1);
    _Assert(result.exist == true, "result.exist == true");
    _Assert(result.index == 3, "result.index == 3");

    result = arena.FindBlock(source2);
    _Assert(result.exist == true, "result.exist == true");
    _Assert(result.index == 6, "result.index == 6");

    result = arena.FindBlock(source3);
    _Assert(result.exist == true, "result.exist == true");
    _Assert(result.index == 8, "result.index == 8");

    result = arena.FindBlock(source4);
    _Assert(result.exist == true, "result.exist == true");
    _Assert(result.index == 11, "result.index == 11");

    arena.Free();
    return true;
}

internal uint32 TestMemoryBlockContains()
{
    MemoryArena arena = {};
    arena.Init();
    bool32  result = false;
    
    ArenaPushSize(&arena, 10);

    result = arena.BlockContains(arena.block, arena.block->base, 5);
    _Assert(result == true, "result == true");

    result = arena.BlockContains(arena.block, arena.block->base + 2, 5);
    _Assert(result == true, "result == true");

    result = arena.BlockContains(arena.block, arena.block->base + 5, 5);
    _Assert(result == true, "result == true");

    result = arena.BlockContains(arena.block, arena.block->base, 10);
    _Assert(result == true, "result == true");

    result = arena.BlockContains(arena.block, arena.block->base, 11);
    _Assert(result == false, "result == false");

    result = arena.BlockContains(arena.block, arena.block->base + 6, 5);
    _Assert(result == false, "result == false");

    MemoryBlock someBlock = {};
    someBlock.Init(20);
    result = arena.BlockContains(arena.block, someBlock.base, 5);
    _Assert(result == false, "result == false");
    someBlock.Free();

    arena.Free();
    return true;
}

internal uint32 TestMemoryResize1()
{
    MemoryArena arena = {};
    arena.Init();
    bool32  result = false;
    
    ArenaPushSize(&arena, 10);

    ArenaPushSize(&arena, 10);
    uint8* source = arena.block->base + 2;
    MemorySize size = 5;

    for (uint32 i = 0; i < size; i++) {
        *(source +i) = 1;
    }

    ArenaPushSize(&arena, 10);

    uint8* newSource = (uint8*)arena.Resize(source, size, 20);
    _Assert(arena.count == 4, "arena.count == 4");
    _Assert(source != newSource, "source != newSource");
    _Assert(arena.block->size == 20, "arena.block->size == 20");
    _Assert(arena.BlockContains(arena.block, newSource, size) == true, "arena.BlockContains(arena.block, newSource, size) == true");
    _Assert(arena.BlockContains(arena.block, source, size) == false, "arena.BlockContains(arena.block, source, size) == false");

    bool32 pass = true; 
    for (uint32 i = 0; i < size; i++) {
        if (*(newSource +i) != 1)  {
            pass = false;
            break;
        }
    }

    _Assert(pass == true, "pass == true");

    arena.Free();
    return true;
}

internal uint32 TestMemoryResize2()
{
    MemoryArena arena = {};
    arena.Init(1000);
    bool32  result = false;
    
    ArenaPushSize(&arena, 10);

    ArenaPushSize(&arena, 10);
    uint8* source = arena.block->base + 2;
    MemorySize size = 5;

    for (uint32 i = 0; i < size; i++) {
        *(source +i) = 1;
    }

    ArenaPushSize(&arena, 10);

    uint8* newSource = (uint8*)arena.Resize(source, size, 20);
    ArenaPushSize(&arena, 10);

    _Assert(arena.count == 1, "arena.count == 1");
    _Assert(source != newSource, "source != newSource");
    _Assert(arena.block->size == 1000, "arena.block->size == 1000");
    _Assert(arena.BlockContains(arena.block, newSource, size) == true, "arena.BlockContains(arena.block, newSource, size) == true");
    _Assert(arena.BlockContains(arena.block, source, size) == true, "arena.BlockContains(arena.block, source, size) == true");

    bool32 pass = true; 
    for (uint32 i = 0; i < size; i++) {
        if (*(newSource +i) != 1)  {
            pass = false;
            break;
        }
    }

    _Assert(pass == true, "pass == true");

    arena.Free();
    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Memory test group\n");
    printf("===========================================\n");
    _RunTest(TestMemoryInit);
    _RunTest(TestMemoryInitExtend);
    _RunTest(TestMemoryInitZeroSize);
    _RunTest(TestMemoryInitZero);
    _RunTest(TestMemoryFree);
    _RunTest(TestMemoryFreeBlock);
    _RunTest(TestMemoryExtend);
    _RunTest(TestMemoryExtendWithExtendFactor);
    _RunTest(TestMemoryPush);
    _RunTest(TestMemoryFindBlock);
    _RunTest(TestMemoryBlockContains);
    _RunTest(TestMemoryResize1);
    _RunTest(TestMemoryResize2);
}

} // namespace

#define QTESTMEMORY_CPP
#endif