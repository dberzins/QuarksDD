#if !defined(QTESTHASHTABLE_CPP)

#include <QuarksDD.h>
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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*2*sizeof(HashItem), "table.itemArena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");

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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*2*sizeof(HashItem), "table.itemArena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");
    
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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*2*sizeof(HashItem), "table.itemArena->block->size ==  table.size*2*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");
    
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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*sizeof(HashItem), "table.itemArena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");

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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*sizeof(HashItem), "table.itemArena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");

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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size ==  table.size*sizeof(HashItem), "table.itemArena->block->size ==  table.size*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used ==  table.size*sizeof(HashItem)");

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
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size > 0, "table.itemArena->block->size > 0");
    _Assert(table.itemArena->block->used ==  (table.size*sizeof(HashItem)), "table.itemArena->block->used ==  (table.size*sizeof(HashItem)");

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
    _Assert(arena.count == 2, "arena.count == 2");
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->block->size == table.size*sizeof(HashItem), "table.itemArena->block->size == table.size*sizeof(HashItem)");
    _Assert(table.itemArena->block->used ==  table.size*sizeof(HashItem), "table.itemArena->block->used == table.size*sizeof(HashItem)");

    table.Free();
    arena.Free();
    return true;
}

internal uint32 TestHashtableInitData()
{
    Hashtable table = {};
    table.Init(6, sizeof(uint32));

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->count == 1, "table.itemArena->count == 1");
    _Assert(table.itemArena->block->size == table.size*2*(sizeof(HashItem)), "table.itemArena->block->size == table.size*2*(sizeof(HashItem))");
    _Assert(table.itemArena->block->used == (table.itemArena->block->size/2), "table.itemArena->block->used == (table.itemArena->block->size/2)");

    table.Free();
    return true;
}

internal uint32 TestHashtableInitDataArena()
{
    MemoryArena arena = {};
    arena.Init();
    Hashtable table = {};
    table.Init(&arena, 6, sizeof(uint32), DEFAULT_ITEM_KEY_SIZE);

    _Assert(table.initialized == true, "table.initialized == true");
    _Assert(table.itemArena != NULL, "table.itemArena != NULL");
    _Assert(table.itemArena->block != NULL, "table.itemArena->block != NULL");
    _Assert(table.itemArena->count == 1, "table.itemArena->count == 1");
    _Assert(table.itemArena->block->size == (table.size * sizeof(HashItem)), "table.itemArena->block->size == (table.size * sizeof(HashItem))");
    _Assert(table.itemArena->block->used == (table.itemArena->block->size), "table.itemArena->block->used == (table.itemArena->block->size)");

    _Assert(table.keyArena != NULL, "table.keyArena != NULL");
    _Assert(table.keyArena->block != NULL, "table.keyArena->block != NULL");
    _Assert(table.keyArena->count == 1, "table.keyArena->count == 1");
    _Assert(table.keyArena->block->size == (table.size * DEFAULT_ITEM_KEY_SIZE), "table.keyArena->block->size == (table.size * DEFAULT_ITEM_KEY_SIZE)");
    _Assert(table.keyArena->block->used == (table.keyArena->block->size), "table.keyArena->block->used == (table.keyArena->block->size)");

    _Assert(table.dataArena != NULL, "table.dataArena != NULL");
    _Assert(table.dataArena->block != NULL, "table.dataArena->block != NULL");
    _Assert(table.dataArena->count == 1, "table.dataArena->count == 1");
    _Assert(table.dataArena->block->size == (table.size * sizeof(uint32)), "table.dataArena->block->size == (table.size * sizeof(uint32))");
    _Assert(table.dataArena->block->used == (table.dataArena->block->size), "table.dataArena->block->used == (table.dataArena->block->size)");


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
    _Assert(table.itemArena == NULL, "table.itemArena == NULL");

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
    _Assert(table.itemArena == NULL, "table.itemArena == NULL");
    _Assert(arena.freeCkunkCount == 1, "arena.freeCkunkCount == 1");
    _Assert(arena.firstFree != NULL, "arena.firstFree != NULL");
    _Assert(arena.firstFree->start == 0, "arena.firstFree->start == 0");
    _Assert(arena.block->size == arena.firstFree->end, "arena.block->size == arena.firstFree->end");
    _Assert(arena.block == arena.firstFree->block, "arena.block == arena.firstFree->block");

    arena.Free();
    return true;
}

internal uint32 TestHashtableClear()
{
    Hashtable table = {};
    table.Init();

    uint32 a = 1;
    table.Add("test", &a);
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

internal uint32 TestHashtableClearData()
{
    Hashtable table = {};
    table.Init(1, sizeof(uint32));

    uint32 a = 1;
    table.Add("test",  &a);
    _Assert(table.count == 1, "table.count == 1");
    void* data = table.GetItem("test")->data;

    _Assert(*(uint32*)data == a, "*(uint32*)data == a");
    _Assert(data != (void*)&a, "data!= (void*)&a");

    table.Clear();
    _Assert(table.count == 0, "table.count == 0");

    bool32 pass = true;
    for (uint32 i = 0; i < table.size; i++) {
        HashItem* item = (table.table + i);
        if (item->index != HASH_ITEM_UNINITIALIZED || item->next != NULL) {
            pass = false;
            break;
        }
        if (!item->data) {
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

    table.Add("a", &a);
    table.Add("b", &b);
    table.Add("c", &c);
    table.Add("d", &d);
    table.Add("e", &e);
    table.Add("f", &f);

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

    table.Add(1, &a);
    table.Add(2, &b);
    table.Add(3, &c);
    table.Add(4, &d);
    table.Add(5, &e);
    table.Add(6, &f);

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

    table.Add("a", &a);
    table.Add("b", &b);

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

    table.Add("aaa aaa", &a);
    table.Add("bbb bbb", &b);
    table.Add("ccc ccc", &c);
    table.Add("ddd ddd", &d);
    table.Add("eee eee", &e);
    table.Add("fff fff", &f);


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

    table.Add("aaa aaa", &a);
    table.Add("bbb bbb", &b);
    table.Add("ccc ccc", &c);
    table.Add("ddd ddd", &d);
    table.Add("eee eee", &e);
    table.Add("fff fff", &f);


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

    table.Add(2342, &a);
    table.Add(12, &b);
    table.Add(234, &c);
    table.Add(34234, &d);
    table.Add(0U, &e);
    table.Add(U32Max-1, &f);


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

    table.Add(2342, &a);
    table.Add(12, &b);
    table.Add(234, &c);
    table.Add(34234, &d);
    table.Add(0U, &e);
    table.Add(U32Max-1, &f);


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

    table.Add("aaa aaa", &a);
    table.Add("bbb bbb", &b);
    table.Add("ccc ccc", &c);


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

    table.Add(0U, &a);
    table.Add(4424, &b);
    table.Add(U32Max -1, &c);


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

    table.Add("aaa aaa", &a);
    table.Add("bbb bbb", &b);
    table.Add("ccc ccc", &c);


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

    table.Add(0U, &a);
    table.Add(234, &b);
    table.Add(U32Max - 1, &c);


    _Assert(table.HasIndex(0) == true, "table.HasKey(0U)) == true");
    _Assert(table.HasIndex(234) == true, "table.HasKey(234)) == true");
    _Assert(table.HasIndex(U32Max - 1) == true, "table.HasKey(U32Max - 1)) == true");

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

internal uint32 TestHashtableIteration() 
{
    Hashtable table = {};
    table.Init(10, sizeof(uint32), DEFAULT_ITEM_KEY_SIZE);
    uint32 q = 1;
    uint32 a = 2;
    uint32 c = 3;

    table.Add("Q", &q);
    table.Add("A", &a);
    table.Add("C", &c);


    bool32 passQ = false;
    bool32 passA = false;
    bool32 passC = false;
    HashItem* iter = table.GetFirstItem();
    while (iter) {
        switch (iter->key[0]) {
            case 'Q':
                passQ = *(uint32*)iter->data == q;
                break;
            case 'A':
                passA = *(uint32*)iter->data == a;
                break;
            case 'C':
                passC = *(uint32*)iter->data == c;
                break;
        }
        iter = table.GetNextItem(iter);
    }

    _Assert(passQ, "passQ");
    _Assert(passA, "passA");
    _Assert(passC, "passC");

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
    // _RunTest(TestHashtableInit1);
    // _RunTest(TestHashtableInit2);
    // _RunTest(TestHashtableInit3);
    // _RunTest(TestHashtableInitArena1);
    // _RunTest(TestHashtableInitArena2);
    // _RunTest(TestHashtableInitArena3);
    // _RunTest(TestHashtableInitArena4);
    // _RunTest(TestHashtableInitArena5);
    // _RunTest(TestHashtableInitData);
    // _RunTest(TestHashtableInitDataArena);
    // _RunTest(TestHashtableFree);
    // _RunTest(TestHashtableFreeArena);
    // _RunTest(TestHashtableClear);
    // _RunTest(TestHashtableClearData);
    // _RunTest(TestHashtableRemove1);
    // _RunTest(TestHashtableRemove2);
    // _RunTest(TestHashtableRemove3);
    // _RunTest(TestHashtableAdd1);
    // _RunTest(TestHashtableAdd2);
    // _RunTest(TestHashtableAdd3);
    // _RunTest(TestHashtableAdd4);
    // _RunTest(TestHashtableGetItemByKey1);
    // _RunTest(TestHashtableGetItemByKey2);
    // _RunTest(TestHashtableGetItemByIndex1);
    // _RunTest(TestHashtableGetItemByIndex2);
    // _RunTest(TestHashtableGetValueByKey);
    // _RunTest(TestHashtableGetValueByIndex);
    // _RunTest(TestHashtableHasKey);
    // _RunTest(TestHashtableHasIndex);
    // _RunTest(TestHashtableGetIterator);
    // _RunTest(HashtableSortedIterator);
    _RunTest(TestHashtableIteration);
}

} // namespace

#define QTESTHASHTABLE_CPP
#endif