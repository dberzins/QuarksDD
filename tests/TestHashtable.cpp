#if !defined(QTESTHASHTABLE_CPP)

#include <Common.h>
#include <Hashtable.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestHashtable {

internal uint32 TestHashtableInit1()
{
    Hashtable table = {};
    table.Init();

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 4096, "tables.size = 4096");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == true, "useLocalArena == true");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*2*sizeof(HashItem), "table.arena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");

    table.Free();
    return true;
}

internal uint32 TestHashtableInit2()
{
    Hashtable table = {};
    table.Init(8);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == true, "useLocalArena == true");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*2*sizeof(HashItem), "table.arena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");
    
    table.Free();
    return true;
}

internal uint32 TestHashtableInit3()
{
    Hashtable table = {};
    table.Init(6);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == true, "useLocalArena == true");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*2*sizeof(HashItem), "table.arena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");
    
    table.Free();
    return true;
}

internal uint32 TestHashtableInitArena1()
{
    MemoryArena arena = {};
    arena.Init();

    Hashtable table = {};
    table.Init(&arena);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 4096, "tables.size = 4096");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == false, "useLocalArena == false");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*sizeof(HashItem), "table.arena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableInitArena2()
{
    MemoryArena arena = {};
    arena.Init();

    Hashtable table = {};
    table.Init(&arena, 8);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == false, "useLocalArena == false");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*sizeof(HashItem), "table.arena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableInitArena3()
{
    MemoryArena arena = {};
    arena.Init();

    Hashtable table = {};
    table.Init(&arena, 6);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == false, "useLocalArena == false");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  table.size*sizeof(HashItem), "table.arena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used ==  table.size*sizeof(HashItem)");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableInitArena4()
{
    MemoryArena arena = {};
    arena.Init(4096);
    ArenaPushSize(&arena, 10);

    Hashtable table = {};
    table.Init(&arena, 6);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == false, "useLocalArena == false");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->block->size ==  4096, "table.arena->block->size == 4096");
    _Assert(table.arena->block->used ==  (10 + table.size*sizeof(HashItem)), "table.arena->block->used == (10 + table.size*sizeof(HashItem))");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableInitArena5()
{
    MemoryArena arena = {};
    arena.Init(15);
    ArenaPushSize(&arena, 10);

    Hashtable table = {};
    table.Init(&arena, 6);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.size == 8, "tables.size = 8");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.useLocalArena == false, "useLocalArena == false");
    _Assert(table.arena != NULL, "table.arena != NULL");
    _Assert(table.arena->block != NULL, "table.arena->block != NULL");
    _Assert(table.arena->count == 2, "table.arena->count == 2");
    _Assert(table.arena->block->size == table.size*sizeof(HashItem), "table.arena->block->size == table.size*sizeof(HashItem)");
    _Assert(table.arena->block->used ==  table.size*sizeof(HashItem), "table.arena->block->used == table.size*sizeof(HashItem)");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableFree()
{
    Hashtable table = {};
    table.Init();
    table.Free();

    _Assert(table.size == 0, "tables.size = 0");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.arena == NULL, "table.arena == NULL");

    return true;
}

internal uint32 TestHashtableFreeArena()
{
    MemoryArena arena = {};
    arena.Init();

    Hashtable table = {};
    table.Init(&arena);
    table.Free();

    _Assert(table.size == 0, "tables.size = 0");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.arena == NULL, "table.arena == NULL");
    _Assert(arena.block != NULL, "arena.block != NULL");

    arena.Free();
    return true;
}

internal uint32 TestHashtableClear()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    table["test"] = &a;
    _Assert(table.count == 1, "table.count == 1");
    table.Clear();
    _Assert(table.count == 0, "table.count == 0");

    bool32 pass = true;
    for (uint32 i = 0; i < table.size; i++) {
        HashItem* item = (table.table + i);
        if (item->index != HASH_ITEM_UNINITIALIZED || item->next != NULL) {
            pass = false;
            break;
        }
    }

    _Assert(pass==true, "pass == true");

    table.Free();
    return true;
}

internal uint32 TestHashtableRemove1()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table["a"] = &a;
    table["b"] = &b;
    table["c"] = &c;
    table["d"] = &d;
    table["e"] = &e;
    table["f"] = &f;

    _Assert(table.count == 6, "table.count == 6");
    table.Remove("b");
    table.Remove("d");
    table.Remove("f");
    _Assert(table.count == 3, "table.count == 3");

    _Assert(*((uint32*)table["a"].data) == a, "*((uint32*)table[\"a\"].data) == a");
    _Assert(*((uint32*)table["c"].data) == c, "*((uint32*)table[\"c\"].data) == c");
    _Assert(*((uint32*)table["e"].data) == e, "*((uint32*)table[\"e\"].data) == e");

    table.Free();
    return true;
}

internal uint32 TestHashtableRemove2()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table[1] = &a;
    table[2] = &b;
    table[3] = &c;
    table[4] = &d;
    table[5] = &e;
    table[6] = &f;

    _Assert(table.count == 6, "table.count == 6");
    table.Remove(2);
    table.Remove(4);
    table.Remove(6);
    _Assert(table.count == 3, "table.count == 3");

    _Assert(*((uint32*)table[1].data) == a, "*((uint32*)table[1].data) == a");
    _Assert(*((uint32*)table[3].data) == c, "*((uint32*)table[3].data) == c");
    _Assert(*((uint32*)table[5].data) == e, "*((uint32*)table[5].data) == e");

    table.Free();
    return true;
}

internal uint32 TestHashtableRemove3()
{
    Hashtable table = {};
    table.Init(1);

    uint32 a = 1;
    uint32 b = 2;

    table["a"] = &a;
    table["b"] = &b;

    table.Remove("a");
    _Assert(table.firstFree == NULL, "table.firstFree == NULL");
    table.Remove("b");
    _Assert(table.count == 0, "table.count == 0");
    _Assert(table.firstFree != NULL, "table.firstFree != NULL");

    table.Free();
    return true;
}

internal uint32 TestHashtableAdd1()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table.Add("a", &a);
    table.Add("b", &b);
    table.Add("c", &c);
    table.Add("d", &d);
    table.Add("e", &e);
    table.Add("f", &f);

    _Assert(table.count == 6, "table.count == 6");

    _Assert(*((uint32*)table["a"].data) == a, "*((uint32*)table[\"a\"].data) == a");
    _Assert(*((uint32*)table["b"].data) == b, "*((uint32*)table[\"b\"].data) == b");
    _Assert(*((uint32*)table["c"].data) == c, "*((uint32*)table[\"c\"].data) == c");
    _Assert(*((uint32*)table["d"].data) == d, "*((uint32*)table[\"d\"].data) == d");
    _Assert(*((uint32*)table["e"].data) == e, "*((uint32*)table[\"e\"].data) == e");
    _Assert(*((uint32*)table["f"].data) == f, "*((uint32*)table[\"f\"].data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableAdd2()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table.Add(1, &a);
    table.Add(2, &b);
    table.Add(3, &c);
    table.Add(4, &d);
    table.Add(5, &e);
    table.Add(6, &f);

    _Assert(table.count == 6, "table.count == 6");

    _Assert(*((uint32*)table[1].data) == a, "*((uint32*)table[1].data) == a");
    _Assert(*((uint32*)table[2].data) == b, "*((uint32*)table[2].data) == b");
    _Assert(*((uint32*)table[3].data) == c, "*((uint32*)table[3].data) == c");
    _Assert(*((uint32*)table[4].data) == d, "*((uint32*)table[4].data) == d");
    _Assert(*((uint32*)table[5].data) == e, "*((uint32*)table[5].data) == e");
    _Assert(*((uint32*)table[6].data) == f, "*((uint32*)table[6].data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableAdd3()
{
    uint32 values[4096*10];

    Hashtable table = {};
    table.Init(4096);

    for (uint32 i = 0; i < 4096*10; i++) {
        values[i] = i*10;
        table.Add(i, &values[i]);
    }

    _Assert(table.count == 4096*10, "table.count == 4096*10");

    bool32 pass = true;
    for (uint32 i = 0; i < 4096*10; i++) {
        if (*((uint32*)table[i].data) != values[i]) {
            pass = false;
            break;
        }
    }
    _Assert(pass == true, "pass == true");

    table.Free();
    return true;
}

internal uint32 TestHashtableAdd4()
{
    uint32 values[4096];

    Hashtable table = {};
    table.Init(1);

    for (uint32 i = 0; i < 4096; i++) {
        values[i] = i*10;
        table.Add(i, &values[i]);
    }

    _Assert(table.count == 4096, "table.count == 4096");

    bool32 pass = true;
    for (uint32 i = 0; i < 4096; i++) {
        if (*((uint32*)table[i].data) != values[i]) {
            pass = false;
            break;
        }
    }
    _Assert(pass == true, "pass == true");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetItemByKey1()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table["aaa aaa"] = &a;
    table["bbb bbb"] = &b;
    table["ccc ccc"] = &c;
    table["ddd ddd"] = &d;
    table["eee eee"] = &e;
    table["fff fff"] = &f;


    HashItem* item = NULL;

    item = table.GetItem("aaa aaa");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == a, "*((uint32*)item->data) == a");
    item = table.GetItem("bbb bbb");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == b, "*((uint32*)item->data) == b");
    item = table.GetItem("ccc ccc");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == c, "*((uint32*)item->data) == c");
    item = table.GetItem("ddd ddd");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == d, "*((uint32*)item->data) == d");
    item = table.GetItem("eee eee");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == e, "*((uint32*)item->data) == e");
    item = table.GetItem("fff fff");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == f, "*((uint32*)item->data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetItemByKey2()
{

    Hashtable table = {};
    table.Init(1);

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table["aaa aaa"] = &a;
    table["bbb bbb"] = &b;
    table["ccc ccc"] = &c;
    table["ddd ddd"] = &d;
    table["eee eee"] = &e;
    table["fff fff"] = &f;


    HashItem* item = NULL;

    item = table.GetItem("aaa aaa");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == a, "*((uint32*)item->data) == a");
    item = table.GetItem("bbb bbb");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == b, "*((uint32*)item->data) == b");
    item = table.GetItem("ccc ccc");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == c, "*((uint32*)item->data) == c");
    item = table.GetItem("ddd ddd");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == d, "*((uint32*)item->data) == d");
    item = table.GetItem("eee eee");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == e, "*((uint32*)item->data) == e");
    item = table.GetItem("fff fff");
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == f, "*((uint32*)item->data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetItemByIndex1()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table[2342] = &a;
    table[12] = &b;
    table[234] = &c;
    table[34234] = &d;
    table[0U] = &e;
    table[U32Max-1] = &f;


    HashItem* item = NULL;

    item = table.GetItem(2342);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == a, "*((uint32*)item->data) == a");
    item = table.GetItem(12);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == b, "*((uint32*)item->data) == b");
    item = table.GetItem(234);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == c, "*((uint32*)item->data) == c");
    item = table.GetItem(34234);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == d, "*((uint32*)item->data) == d");
    item = table.GetItem(0U);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == e, "*((uint32*)item->data) == e");
    item = table.GetItem(U32Max-1);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == f, "*((uint32*)item->data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetItemByIndex2()
{

    Hashtable table = {};
    table.Init(1);

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;
    uint32 d = 4;
    uint32 e = 5;
    uint32 f = 6;

    table[2342] = &a;
    table[12] = &b;
    table[234] = &c;
    table[34234] = &d;
    table[0U] = &e;
    table[U32Max-1] = &f;


    HashItem* item = NULL;

    item = table.GetItem(2342);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == a, "*((uint32*)item->data) == a");
    item = table.GetItem(12);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == b, "*((uint32*)item->data) == b");
    item = table.GetItem(234);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == c, "*((uint32*)item->data) == c");
    item = table.GetItem(34234);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == d, "*((uint32*)item->data) == d");
    item = table.GetItem(0U);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == e, "*((uint32*)item->data) == e");
    item = table.GetItem(U32Max-1);
    _Assert(item != NULL, "item != NULL");
    _Assert(*((uint32*)item->data) == f, "*((uint32*)item->data) == f");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetValueByKey()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;

    table["aaa aaa"] = &a;
    table["bbb bbb"] = &b;
    table["ccc ccc"] = &c;


    _Assert(*((uint32*)table.GetValue("aaa aaa")) == a, "*((uint32*)table.GetValue(\"aaa aaa\")) == a");
    _Assert(*((uint32*)table.GetValue("bbb bbb")) == b, "*((uint32*)table.GetValue(\"bbb bbb\")) == b");
    _Assert(*((uint32*)table.GetValue("ccc ccc")) == c, "*((uint32*)table.GetValue(\"ccc ccc\")) == c");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetValueByIndex()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;

    table[0U] = &a;
    table[4424] = &b;
    table[U32Max -1] = &c;


    _Assert(*((uint32*)table.GetValue(0U)) == a, "*((uint32*)table.GetValue(0U)) == a");
    _Assert(*((uint32*)table.GetValue(4424)) == b, "*((uint32*)table.GetValue(4424)) == b");
    _Assert(*((uint32*)table.GetValue(U32Max - 1)) == c, "*((uint32*)table.GetValue(U32Max - 1)) == c");

    table.Free();
    return true;
}

internal uint32 TestHashtableHasKey()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;

    table["aaa aaa"] = &a;
    table["bbb bbb"] = &b;
    table["ccc ccc"] = &c;


    _Assert(table.HasKey("aaa aaa") == true, "table.HasKey(\"aaa aaa\")) == true");
    _Assert(table.HasKey("bbb bbb") == true, "table.HasKey(\"bbb bbb\")) == true");
    _Assert(table.HasKey("ccc ccc") == true, "table.HasKey(\"ccc ccc\")) == true");

    table.Free();
    return true;
}

internal uint32 TestHashtableHasIndex()
{

    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    uint32 b = 2;
    uint32 c = 3;

    table[0U] = &a;
    table[234] = &b;
    table[U32Max - 1] = &c;


    _Assert(table.HasKey(0U) == true, "table.HasKey(0U)) == true");
    _Assert(table.HasKey(234) == true, "table.HasKey(234)) == true");
    _Assert(table.HasKey(U32Max - 1) == true, "table.HasKey(U32Max - 1)) == true");

    table.Free();
    return true;
}

internal uint32 TestHashtableGetIterator() 
{
    Hashtable table = {};
    table.Init(2);

    uint32 values[10];

    for (uint32 i=0; i<10; i++) {
        values[i] = i;
        table[i].data = &values[i]; 
    }

    HashtableIterator* iter;

    iter = table.GetIterator();

    bool32 pass = true;
    uint32 count = 0;
    while(iter) {
        
        HashItem* item = table.GetItem(iter);
        uint32 value = *(uint32*)item->data;

        if (value != item->index) {
            pass = false;
            break;
        }

        count++;
        iter = iter->next;
    }

    _Assert(pass == true, "pass == true");
    _Assert(count == 10, "count == 10");

    table.Free();

    return true;
}

internal uint32 HashtableSortedIterator() {
    Hashtable table = {};
    table.Init(4);

    uint32 values[5];

    for (uint32 i=0; i < 5; i++) {
        values[i] = i;
        table[i].data = &values[i]; 
    }

    HashtableIterator* iter;
    iter = table.GetIteratorSorted();

    bool32 pass = true;
    while (iter && iter->next) {
        uint32 val1 = *(uint32*)table.GetItem(iter)->data;
        uint32 val2 = *(uint32*)table.GetItem(iter->next)->data;

        if (val1 > val2) {
            pass = false;
            break;
        }

        iter = iter->next;
    }

    _Assert(pass == true, "pass == true");

    table.Free();
    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Hashtable test group\n");
    printf("===========================================\n");
    _RunTest(TestHashtableInit1);
    _RunTest(TestHashtableInit2);
    _RunTest(TestHashtableInit3);
    _RunTest(TestHashtableInitArena1);
    _RunTest(TestHashtableInitArena2);
    _RunTest(TestHashtableInitArena3);
    _RunTest(TestHashtableInitArena4);
    _RunTest(TestHashtableInitArena5);
    _RunTest(TestHashtableFree);
    _RunTest(TestHashtableFreeArena);
    _RunTest(TestHashtableClear);
    _RunTest(TestHashtableRemove1);
    _RunTest(TestHashtableRemove2);
    _RunTest(TestHashtableRemove3);
    _RunTest(TestHashtableAdd1);
    _RunTest(TestHashtableAdd2);
    _RunTest(TestHashtableAdd3);
    _RunTest(TestHashtableAdd4);
    _RunTest(TestHashtableGetItemByKey1);
    _RunTest(TestHashtableGetItemByKey2);
    _RunTest(TestHashtableGetItemByIndex1);
    _RunTest(TestHashtableGetItemByIndex2);
    _RunTest(TestHashtableGetValueByKey);
    _RunTest(TestHashtableGetValueByIndex);
    _RunTest(TestHashtableHasKey);
    _RunTest(TestHashtableHasIndex);
    _RunTest(TestHashtableGetIterator);
    _RunTest(HashtableSortedIterator);
}

} // namespace

#define QTESTHASHTABLE_CPP
#endif