#if !defined(QTESTLIST_CPP)

#include <QuarksDD.h>
#include <Memory.h>
#include <List.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestList
{

internal uint32 ListInit()
{
    List l = {};
    l.Init();

    _Assert(l.head == NULL, "l.head == NULL");
    _Assert(l.tail == NULL, "l.tail == NULL");
    _Assert(l.count == 0, "l.count == 0");

    l.Free();
    return true;
}

internal uint32 ListInitArena()
{
    MemoryArena arena = {};
    arena.Init();

    List l = {};
    l.Init(&arena);

    _Assert(l.head == NULL, "l.head == NULL");
    _Assert(l.tail == NULL, "l.tail == NULL");
    _Assert(l.count == 0, "l.count == 0");
    _Assert(arena.block == NULL, "arena.block == NULL");

    l.Free();
    arena.Free();
    return true;
}


internal uint32 ListFree()
{
    List l = {};
    l.Init();
    l.Add(NULL);
    l.Free();

    _Assert(l.head == NULL, "l.head == NULL");
    _Assert(l.tail == NULL, "l.tail == NULL");
    _Assert(l.count == 0, "l.count == 0");
    _Assert(l.firstFree == NULL, "l.firstFree == NULL");

    return true;
}

internal uint32 ListFreeArena()
{
    uint32 testSize = 10;

    MemoryArena arena = {};
    arena.Init(testSize);

    List l = {};
    l.Init(&arena);
    l.Add(NULL);
    l.Free();
    arena.Free();

    _Assert(l.head == NULL, "l.head == NULL");
    _Assert(l.tail == NULL, "l.tail == NULL");
    _Assert(l.count == 0, "l.count == 0");
    _Assert(arena.block == NULL, "arena.block == NULL");
    _Assert(l.firstFree == NULL, "l.firstFree == NULL");

    return true;
}


internal uint32 ListAdd()
{
    List l = {};
    l.Init();

    uint32 startCount = l.count;

    int32 value1 = 999;
    l.Add(&value1);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    int32 fetchValue = *(int32*)l.GetItem(l.tail)->data;
    _Assert(fetchValue == value1, "fetchValue == %d", value1);

    int32 value2 = 555;
    l.Add(&value2);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    fetchValue = *(int32*)l.GetItem(l.tail)->data;
    _Assert(fetchValue == value2, "fetchValue == %d", value2);
    
    int32 value3 = 666;
    ListItem* item3  = l.Add(&value3);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    fetchValue = *(int32*)l.GetItem(&item3->iterator)->data;
    _Assert(fetchValue == value3, "fetchValue == %d", value3);

    l.Free();
    return true;
}

internal uint32 ListAddArena()
{
    MemoryArena arena = {};
    arena.Init();

    List l = {};
    l.Init(&arena);

    uint32 startCount = l.count;

    int32 value1 = 999;
    l.Add(&value1);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    int32 fetchValue = *(int32*)l.GetItem(l.tail)->data;
    _Assert(fetchValue == value1, "fetchValue == %d", value1);

    int32 value2 = 555;
    l.Add(&value2);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    fetchValue = *(int32*)l.GetItem(l.tail)->data;
    _Assert(fetchValue == value2, "fetchValue == %d", value2);

    int32 value3 = 666;
    ListItem* item3  = l.Add(&value3);
    _Assert(l.count == ++startCount, "l.count == ++startCount");
    fetchValue = *(int32*)l.GetItem(&item3->iterator)->data;
    _Assert(fetchValue == value3, "fetchValue == %d", value3);

    l.Free();
    arena.Free();
    return true;
}


internal uint32 ListInsert()
{
    List l = {};
    l.Init();

    int32 value = 0;
    for (uint32 i = 0; i <10; i++, value++) {
        l.Add(&value);
    }
    _Assert(l.count == 10, "l.count == 10");

    uint32 count = l.count;

    int32 value1 = 999;
    // 0 -> 1 -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* pos1 = l.head->next->next->next->next;
    l.Insert(pos1, &value1);
    _Assert(l.count == ++count , "l.count == count +1");
    int32 fetchValue1 = *(int32*)l.GetItem(pos1->next)->data;
    _Assert(fetchValue1 == value1, "fetchValue1 == value1");

    int32 value2 = 555;

    // 0 -> 1 -> [555] -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* pos2 = l.head->next;
    l.Insert(pos2, &value2);
    _Assert(l.count == ++count , "l.count == count +1");
    int32 fetchValue2 = *(int32*)l.GetItem(pos2->next)->data;
    _Assert(fetchValue2 == value2, "fetchValue2 == value2");


    // 0 -> 1 -> [555] -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* shiftedPos = pos2->next->next->next->next->next;
    int32 shiftedPosValue = *(int32*)l.GetItem(shiftedPos)->data;
    _Assert(shiftedPosValue == value1, "shiftedPosValue == value1");

    l.Free();
    return true;
}

internal uint32 ListInsertArena()
{
    MemoryArena arena = {};
    arena.Init(sizeof(ListItem)*10);

    List l = {};
    l.Init(&arena);

    int32 value = 0;
    for (uint32 i = 0; i <10; i++, value++) {
        l.Add(&value);
    }
    _Assert(l.count == 10, "l.count == 10");

    uint32 count = l.count;

    int32 value1 = 999;
    // 0 -> 1 -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* pos1 = l.head->next->next->next->next;
    l.Insert(pos1, &value1);
    _Assert(l.count == ++count , "l.count == count +1");
    int32 fetchValue1 = *(int32*)l.GetItem(pos1->next)->data;
    _Assert(fetchValue1 == value1, "fetchValue1 == value1");

    int32 value2 = 555;

    // 0 -> 1 -> [555] -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* pos2 = l.head->next;
    l.Insert(pos2, &value2);
    _Assert(l.count == ++count , "l.count == count +1");
    int32 fetchValue2 = *(int32*)l.GetItem(pos2->next)->data;
    _Assert(fetchValue2 == value2, "fetchValue2 == value2");


    // 0 -> 1 -> [555] -> 2 -> 3 -> 4 -> [999] -> 5 -> ... 
    ListIterator* shiftedPos = pos2->next->next->next->next->next;
    int32 shiftedPosValue = *(int32*)l.GetItem(shiftedPos)->data;
    _Assert(shiftedPosValue == value1, "shiftedPosValue == value1");

    l.Free();
    arena.Free();
    return true;
}

internal uint32 ListInsertAdd()
{
    List l = {};
    l.Init();
    ListIterator* iter;

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8 , 9, 10};
    
    l.Add(&values1[0]); // 1
    l.Add(&values1[1]); // 2
    l.Add(&values1[2]); // 3
    l.Insert(l.head, &values1[2]); // 3
    l.Insert(l.head, &values1[3]); // 4
    l.Add(&values1[4]); // 5
    l.Insert(NULL, &values1[5]); // 6
    l.Add(&values1[6]); // 7
    l.Insert(l.tail, &values1[7]); // 8
    l.Add(&values1[8]); // 9
    l.Insert(l.head->next->next->next->next, &values1[9]); //10

    int32 values2 [] = {6, 1, 4, 3, 2, 10, 3, 5, 7, 8, 9};

    char result[100];
    bool32 pass = true;
    int32 charsWritten  = 0;
    iter = l.head;
    uint32 i = 0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        ++i;
        iter = iter->next;
    }

    _Assert(pass, " {6, 1, 4, 3, 2, 10, 3, 5, 7, 8, 9} = {%s}", result);

    l.Free();
    return true;
}

internal int32 ListAddSortedCompareValues(void* v1, void* v2)
{
   int32 result = 0;

   int32 value1 = *(int32*)v1;
   int32 value2 = *(int32*)v2;

   if (value1 == value2)
       result = 0;
   if (value1 < value2)
       result = -1;
   if (value1 > value2)
       result = 1;

   return result;
}

internal uint32 ListInsertAddSorted()
{

    List l = {};
    l.Init(10, true, ListAddSortedCompareValues, SortOrder::Asc);

    // Unsorted values
    int32 values[] = {3, 5, 2, 1, 4};

    for (uint32 i = 0; i < 5; i++) {
        l.AddSorted(&values[i]);
    }


    // Print list content
    ListIterator* iter = l.head;
    int32 values2 [] = {1, 2, 3, 4, 5};
    uint32 i = 0;
    bool32 pass = true;
    char result[100];
    int32 charsWritten  = 0;

    while (iter) {
        int32* val = (int32*)l.GetItem(iter)->data;

        if (*val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", *val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", *val);
        }


        iter = iter->next;
        i++;
    }
    _Assert(pass, "{1, 2, 3, 4, 5} = {%s}", result);

    l.Free();

    return true;
}
internal uint32 ListInsertAddSortedDesc()
{

    List l = {};
    l.Init(10, true, ListAddSortedCompareValues, SortOrder::Desc);

    // Unsorted values
    int32 values[] = {3, 5, 2, 1, 4};

    for (uint32 i = 0; i < 5; i++) {
        l.AddSorted(&values[i]);
    }


    // Print list content
    ListIterator* iter = l.head;
    int32 values2 [] = {5, 4, 3, 2, 1};
    uint32 i = 0;
    bool32 pass = true;
    char result[100];
    int32 charsWritten  = 0;

    while (iter) {
        int32* val = (int32*)l.GetItem(iter)->data;

        if (*val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", *val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", *val);
        }


        iter = iter->next;
        i++;
    }
    _Assert(pass, "{5, 4, 3, 2, 1} = {%s}", result);

    l.Free();

    return true;
}
internal uint32 ListInsertAddArena()
{
    MemoryArena arena = {};
    arena.Init(sizeof(int32)*20);

    List l = {};
    l.Init(&arena);
    ListIterator* iter;

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8 , 9, 10};
    
    l.Add(&values1[0]); // 1
    l.Add(&values1[1]); // 2
    l.Add(&values1[2]); // 3
    l.Insert(l.head, &values1[2]); // 3
    l.Insert(l.head, &values1[3]); // 4
    l.Add(&values1[4]); // 5
    l.Insert(NULL, &values1[5]); // 6
    l.Add(&values1[6]); // 7
    l.Insert(l.tail, &values1[7]); // 8
    l.Add(&values1[8]); // 9
    l.Insert(l.head->next->next->next->next, &values1[9]); //10

    int32 values2 [] = {6, 1, 4, 3, 2, 10, 3, 5, 7, 8, 9};

    char result[100];
    bool32 pass = true;
    int32 charsWritten  = 0;
    iter = l.head;
    uint32 i = 0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        ++i;
        iter = iter->next;
    }

    _Assert(pass, " {6, 1, 4, 3, 2, 10, 3, 5, 7, 8, 9} = {%s}", result);

    l.Free();
    arena.Free();
    return true;
}

internal uint32 ListRemove()
{
    List l = {};
    l.Init();
    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Remove(l.head->next->next->next->next->next); // 6
    l.Remove(l.head); // 1
    l.Remove(l.tail); // 10
    char result[100];
    bool32 pass = l.count == 7;
    int32 charsWritten  = 0;

    iter = l.head;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val == 6 || val == 1 || val== 10) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{2, 3, 4, 5, 7, 8, 9} = {%s}", result);
    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    l.Free();
    return true;
}

internal uint32 ListRemoveArena()
{
    MemoryArena arena = {};
    arena.Init();

    List l = {};
    l.Init(&arena);

    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Remove(l.head->next->next->next->next->next); // 6
    l.Remove(l.head); // 1
    l.Remove(l.tail); // 10
    char result[100];
    bool32 pass = l.count == 7;
    int32 charsWritten  = 0;

    iter = l.head;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val == 6 || val == 1 || val== 10) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{2, 3, 4, 5, 7, 8, 9} = {%s}", result);
    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    l.Free();
    arena.Free();
    return true;
}

internal uint32 ListRemoveAll()
{
    List l = {};
    l.Init();
    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    iter = l.head;
    while (iter) {
        ListIterator* removeIter = iter;
        iter = iter->next;
        l.Remove(removeIter);
    }
    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    char result[100];
    bool32 pass = l.count == 0;
    int32 charsWritten  = 0;

    iter = l.firstFree;
    uint32 i=0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i++]) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{10, 9, 8, 7, 6, 5, 4, 3, 2, 1} = {%s}", result);

    l.Free();
    return true;
}

internal uint32 ListRemoveAllArena()
{
    MemoryArena arena = {};
    arena.Init(sizeof(int32)*10);

    List l = {};
    l.Init(&arena);
    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    iter = l.head;
    while (iter) {
        ListIterator* removeIter = iter;
        iter = iter->next;
        l.Remove(removeIter);
    }
    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    char result[100];
    bool32 pass = l.count == 0;
    int32 charsWritten  = 0;

    iter = l.firstFree;
    uint32 i=0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i++]) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{10, 9, 8, 7, 6, 5, 4, 3, 2, 1} = {%s}", result);

    l.Free();
    arena.Free();
    return true;
}

internal uint32 ListClear()
{
    List l = {};
    l.Init();
    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {3, 4, 5, 6, 7, 8, 2, 1, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Remove(l.tail);
    l.Remove(l.tail);
    l.Remove(l.head);
    l.Remove(l.head);
    l.Clear();

    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    char result[100];
    bool32 pass = l.count == 0;
    int32 charsWritten  = 0;

    iter = l.firstFree;
    uint32 i=0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i++]) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{3, 4, 5, 6, 7, 8, 2, 1, 9, 10 } = {%s}", result);

    l.Free();
    return true;
}

internal uint32 ListClearArena()
{
    MemoryArena arena = {};
    arena.Init(sizeof(int32)*10);

    List l = {};
    l.Init(&arena);
    ListIterator* iter;
    int32 values [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {3, 4, 5, 6, 7, 8, 2, 1, 9, 10 };

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Remove(l.tail);
    l.Remove(l.tail);
    l.Remove(l.head);
    l.Remove(l.head);
    l.Clear();

    _Assert(l.firstFree != NULL, "l.firstFree != NULL");

    char result[100];
    bool32 pass = l.count == 0;
    int32 charsWritten  = 0;

    iter = l.firstFree;
    uint32 i=0;
    while (iter) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i++]) {
            pass = false;
        }

        if (iter == l.head) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{3, 4, 5, 6, 7, 8, 2, 1, 9, 10 } = {%s}", result);

    l.Free();
    arena.Free();
    return true;
}


internal uint32 ListItemValueAssigne()
{
    List l = {};
    l.Init();

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    for (uint32 i = 0; i < ArrayCount(values1); i++) {
        l.Add(&values1[i]);
    }

    ListIterator* iter = l.head;
    for (uint32 i = 0; i < ArrayCount(values2); i++) {
        l.GetItem(iter)->data = &values2[i];
        iter = iter->next;
    }


    char result[40];
    bool32 pass = true;
    int32 charsWritten  = 0;
    iter = l.head;
    for(uint32 i=0; i < ArrayCount(values2); i++) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } = {%s}", result);

    l.Free();
    return true;
}

internal uint32 ListItemValueAssigneArena()
{
    MemoryArena arena = {};
    arena.Init(sizeof(int32)*10);

    List l = {};
    l.Init(&arena);

    int32 values1 [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int32 values2 [] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    for (uint32 i = 0; i < ArrayCount(values1); i++) {
        l.Add(&values1[i]);
    }

    ListIterator* iter = l.head;
    for (uint32 i = 0; i < ArrayCount(values2); i++) {
        l.GetItem(iter)->data = &values2[i];
        iter = iter->next;
    }


    char result[40];
    bool32 pass = true;
    int32 charsWritten  = 0;
    iter = l.head;
    for(uint32 i=0; i < ArrayCount(values2); i++) {
        int32 val = *(int32*)l.GetItem(iter)->data;
        if (val != values2[i]) {
            pass = false;
        }

        if (i == 0) {
            charsWritten += sprintf((result + charsWritten), "%d", val);
        }
        else {
            charsWritten += sprintf((result + charsWritten), ", %d", val);
        }

        iter = iter->next;
    }

    _Assert(pass, "{11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } = {%s}", result);

    l.Free();
    arena.Free();
    return true;
}

internal int32 CompareListItemsValue(void* item1, void* item2)
{
   int32 result = 0;

   int32 val1 = *(int32*)item1;
   int32 val2 = *(int32*)item2;

   if (val1 == val2)
       result = 0;
   if (val1 < val2)
       result = -1;
   if (val1 > val2)
       result = 1;

   return result;
}

internal uint32 ListBubleSort()
{
    List l = {};
    l.Init();
    ListIterator* iter;
    int32 values [] = { 
        6, 58, 80, 96, 53, 45, 76, 98, 78, 31, 43, 87, 57, 86, 2, 33, 35, 40, 45, 5, 52, 24, 76, 85, 100, 
        16, 85, 43, 83, 6, 74, 18, 59, 59, 10, 61, 88, 83, 74, 70, 67, 38, 69, 48, 39, 4, 71, 49, 73, 7, 35, 
        28, 48, 8, 51, 5, 79, 14, 58, 47, 91, 17, 64, 45, 52, 52, 45, 71, 58, 64, 79, 64, 3, 51, 65, 99, 34, 
        54, 87, 96, 81, 36, 13, 6, 2, 12, 26, 85, 65, 53, 44, 91, 34, 40, 63, 4, 32, 50, 71, 98, 55, 17, 33, 
        23, 69, 77, 91, 60, 81, 35, 22, 44, 6, 82, 25, 9, 9, 69, 40, 67, 42, 8, 18, 3, 10, 66, 15, 59, 45, 12, 
        10, 3, 85, 87, 94, 29, 8, 93, 96, 42, 71, 5, 74, 39, 26, 80, 12, 91, 4, 89, 91, 74, 84, 58, 37, 91, 63, 
        53, 94, 28, 64, 94, 50, 79, 83, 93, 54, 78, 1, 9, 65, 66, 53, 95, 25, 29, 19, 83, 100, 58, 84, 3, 89, 81, 
        54, 45, 79, 94, 100, 72, 45, 15, 47, 21, 60, 17, 99, 68, 44, 48, 43, 74, 8, 49, 6, 98, 7, 33, 80, 3, 40, 
        73, 20, 40, 79, 21, 90, 36, 93, 63, 84, 67, 68, 55, 19, 52, 85, 73, 57, 100, 95, 85, 93, 46, 44, 44, 70, 
        33, 89, 38, 46, 36, 44, 16, 31, 18, 34, 68, 72, 52, 88, 52, 41, 60, 43, 35, 76, 84, 2, 57, 86, 65, 25, 19, 
        89, 72, 11, 15, 82, 19, 33, 85, 63, 21, 13, 54, 82, 2, 57, 8, 87, 31, 84, 32, 25, 46, 70, 95, 27, 97, 91, 
        97, 33, 70, 56, 2, 36, 16, 23, 51, 14, 99, 97, 94, 6, 36, 19, 8, 80, 70, 96, 31, 15, 11, 28, 85, 41, 64, 7, 
        40, 1, 47, 99, 72, 54, 31, 31, 7, 98, 90, 86, 19, 3, 89, 14, 39, 8, 73, 81, 97, 70, 82, 5, 24, 34, 83, 36, 85, 
        73, 92, 44, 67, 59, 50, 16, 39, 37, 75, 46, 41, 5, 23, 58, 41, 46, 32, 53, 88, 9, 5, 4, 51, 31, 67, 45, 23, 23, 
        28, 5, 91, 43, 63, 94, 98, 67, 36, 41, 36, 95, 61, 22, 3, 36, 35, 67, 48, 12, 54, 45, 89, 52, 20, 66, 32, 18, 61, 
        1, 46, 81, 4, 74, 27, 73, 73, 48, 20, 55, 96, 4, 63, 10, 77, 49, 100, 91, 12, 29, 64, 80, 3, 20, 100, 31, 94, 7, 
        35, 71, 61, 90, 63, 93, 32, 9, 83, 83, 40, 27, 29, 44, 48, 7, 82, 70, 23, 54, 52, 78, 68, 70, 38, 88, 89, 2, 81, 
        36, 86, 62, 21, 85, 64, 87, 27, 23, 25, 91, 27, 49, 14, 48, 23, 11, 17, 60, 77, 45, 21, 100, 93, 54, 66, 19, 79, 75, 
        23, 100, 42, 87, 99, 41, 21, 62, 12, 77, 41, 75, 54, 56, 59, 16, 73, 11, 77, 93, 62, 64, 43, 28, 74, 96, 66, 14, 13, 
        25, 21, 55, 63, 12, 9, 41, 38, 43, 82, 44, 83, 74, 52, 59, 9, 36, 2, 30, 31, 91, 50, 99, 27, 13, 26, 85, 78, 7, 72, 
        73, 95, 61, 59, 28, 81, 47, 67, 14, 38, 25, 47, 84, 63, 75, 14, 49, 15, 33, 29, 89, 56, 56, 62, 31, 9, 85, 32, 29, 
        25, 44, 71, 49, 79, 76, 25, 96, 18, 21, 17, 22, 74, 81, 21, 7, 69, 49, 43, 71, 44, 33, 81, 45, 94, 16, 11, 79, 62, 
        13, 61, 13, 79, 19, 28, 97, 60, 30, 26, 81, 67, 84, 31, 24, 42, 46, 88, 5, 74, 31, 87, 17, 46, 73, 36, 85, 73, 23, 
        87, 33, 54, 34, 86, 59, 36, 82, 21, 56, 19, 69, 81, 93, 40, 99, 67, 57, 31, 45, 50, 82, 99, 65, 85, 22, 62, 97, 58, 
        47, 3, 8, 72, 56, 65, 35, 100, 99, 44, 58, 68, 25, 50, 34, 3, 79, 28, 65, 84, 12, 56, 21, 75, 13, 31, 14, 89, 14, 20, 
        93, 40, 60, 26, 33, 77, 89, 67, 83, 57, 38, 30, 37, 35, 42, 49, 22, 89, 2, 89, 4, 2, 56, 58, 35, 84, 1, 9, 82, 7, 68, 
        78, 13, 66, 80, 82, 6, 67, 18, 63, 98, 55, 7, 47, 41, 21, 18, 17, 59, 4, 13, 68, 1, 66, 77, 70, 56, 10, 22, 17, 98, 78, 
        91, 77, 4, 71, 22, 81, 26, 80, 15, 41, 73, 11, 74, 49, 16, 55, 72, 28, 14, 100, 76, 26, 83, 70, 74, 53, 65, 66, 13, 21, 
        92, 42, 61, 46, 27, 17, 67, 5, 66, 81, 53, 73, 97, 75, 95, 51, 76, 15, 12, 98, 90, 96, 24, 90, 75, 95, 25, 20, 4, 17, 79, 
        80, 29, 44, 4, 74, 89, 4, 30, 52, 3, 10, 95, 87, 90, 31, 82, 3, 44, 85, 60, 63, 83, 63, 100, 62, 25, 34, 1, 49, 80, 40, 
        18, 14, 3, 55, 73, 61, 3, 5, 71, 15, 51, 70, 48, 3, 83, 68, 18, 47, 59, 2, 30, 44, 84, 94, 23, 11, 52, 84, 15, 99, 33, 23, 
        50, 49, 33, 50, 55, 15, 88, 79, 15, 26, 3, 90, 72, 79, 24, 29, 87, 97, 79, 76, 58, 80, 67, 7, 92, 46, 36, 31, 12, 72, 25, 
        54, 72, 33, 52, 21, 78, 21, 45, 91, 64, 50, 50, 76, 38, 7, 69, 6, 41, 12, 39, 96, 22, 49, 3, 75, 19, 42, 53, 47, 99, 33, 64, 
        22, 59, 46, 67, 87, 64, 94, 12, 9, 14, 39, 71, 51, 97, 1, 92, 85, 28, 88, 96, 28, 59, 48, 43, 7, 74, 20, 80, 89, 58, 61, 90, 
        38, 94, 76, 6, 65, 17, 57, 4, 4, 12, 30, 61, 73, 3, 63, 52, 45, 33, 29, 88, 67
    };
    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Sort(CompareListItemsValue, SortOrder::Asc, SortMethod::BubbleSort);

    bool32 pass = true;

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 > val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }

    _Assert(pass == true, "pass == true");
    l.Sort(CompareListItemsValue, SortOrder::Desc, SortMethod::BubbleSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 < val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Clear();

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }
    l.Sort(CompareListItemsValue, SortOrder::Desc, SortMethod::BubbleSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 < val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Sort(CompareListItemsValue, SortOrder::Asc, SortMethod::QuickSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 > val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Free();
    return true;
}

internal uint32 ListQuickSort()
{
    List l = {};
    l.Init();
    ListIterator* iter;
    int32 values [] = { 
        6, 58, 80, 96, 53, 45, 76, 98, 78, 31, 43, 87, 57, 86, 2, 33, 35, 40, 45, 5, 52, 24, 76, 85, 100, 
        16, 85, 43, 83, 6, 74, 18, 59, 59, 10, 61, 88, 83, 74, 70, 67, 38, 69, 48, 39, 4, 71, 49, 73, 7, 35, 
        28, 48, 8, 51, 5, 79, 14, 58, 47, 91, 17, 64, 45, 52, 52, 45, 71, 58, 64, 79, 64, 3, 51, 65, 99, 34, 
        54, 87, 96, 81, 36, 13, 6, 2, 12, 26, 85, 65, 53, 44, 91, 34, 40, 63, 4, 32, 50, 71, 98, 55, 17, 33, 
        23, 69, 77, 91, 60, 81, 35, 22, 44, 6, 82, 25, 9, 9, 69, 40, 67, 42, 8, 18, 3, 10, 66, 15, 59, 45, 12, 
        10, 3, 85, 87, 94, 29, 8, 93, 96, 42, 71, 5, 74, 39, 26, 80, 12, 91, 4, 89, 91, 74, 84, 58, 37, 91, 63, 
        53, 94, 28, 64, 94, 50, 79, 83, 93, 54, 78, 1, 9, 65, 66, 53, 95, 25, 29, 19, 83, 100, 58, 84, 3, 89, 81, 
        54, 45, 79, 94, 100, 72, 45, 15, 47, 21, 60, 17, 99, 68, 44, 48, 43, 74, 8, 49, 6, 98, 7, 33, 80, 3, 40, 
        73, 20, 40, 79, 21, 90, 36, 93, 63, 84, 67, 68, 55, 19, 52, 85, 73, 57, 100, 95, 85, 93, 46, 44, 44, 70, 
        33, 89, 38, 46, 36, 44, 16, 31, 18, 34, 68, 72, 52, 88, 52, 41, 60, 43, 35, 76, 84, 2, 57, 86, 65, 25, 19, 
        89, 72, 11, 15, 82, 19, 33, 85, 63, 21, 13, 54, 82, 2, 57, 8, 87, 31, 84, 32, 25, 46, 70, 95, 27, 97, 91, 
        97, 33, 70, 56, 2, 36, 16, 23, 51, 14, 99, 97, 94, 6, 36, 19, 8, 80, 70, 96, 31, 15, 11, 28, 85, 41, 64, 7, 
        40, 1, 47, 99, 72, 54, 31, 31, 7, 98, 90, 86, 19, 3, 89, 14, 39, 8, 73, 81, 97, 70, 82, 5, 24, 34, 83, 36, 85, 
        73, 92, 44, 67, 59, 50, 16, 39, 37, 75, 46, 41, 5, 23, 58, 41, 46, 32, 53, 88, 9, 5, 4, 51, 31, 67, 45, 23, 23, 
        28, 5, 91, 43, 63, 94, 98, 67, 36, 41, 36, 95, 61, 22, 3, 36, 35, 67, 48, 12, 54, 45, 89, 52, 20, 66, 32, 18, 61, 
        1, 46, 81, 4, 74, 27, 73, 73, 48, 20, 55, 96, 4, 63, 10, 77, 49, 100, 91, 12, 29, 64, 80, 3, 20, 100, 31, 94, 7, 
        35, 71, 61, 90, 63, 93, 32, 9, 83, 83, 40, 27, 29, 44, 48, 7, 82, 70, 23, 54, 52, 78, 68, 70, 38, 88, 89, 2, 81, 
        36, 86, 62, 21, 85, 64, 87, 27, 23, 25, 91, 27, 49, 14, 48, 23, 11, 17, 60, 77, 45, 21, 100, 93, 54, 66, 19, 79, 75, 
        23, 100, 42, 87, 99, 41, 21, 62, 12, 77, 41, 75, 54, 56, 59, 16, 73, 11, 77, 93, 62, 64, 43, 28, 74, 96, 66, 14, 13, 
        25, 21, 55, 63, 12, 9, 41, 38, 43, 82, 44, 83, 74, 52, 59, 9, 36, 2, 30, 31, 91, 50, 99, 27, 13, 26, 85, 78, 7, 72, 
        73, 95, 61, 59, 28, 81, 47, 67, 14, 38, 25, 47, 84, 63, 75, 14, 49, 15, 33, 29, 89, 56, 56, 62, 31, 9, 85, 32, 29, 
        25, 44, 71, 49, 79, 76, 25, 96, 18, 21, 17, 22, 74, 81, 21, 7, 69, 49, 43, 71, 44, 33, 81, 45, 94, 16, 11, 79, 62, 
        13, 61, 13, 79, 19, 28, 97, 60, 30, 26, 81, 67, 84, 31, 24, 42, 46, 88, 5, 74, 31, 87, 17, 46, 73, 36, 85, 73, 23, 
        87, 33, 54, 34, 86, 59, 36, 82, 21, 56, 19, 69, 81, 93, 40, 99, 67, 57, 31, 45, 50, 82, 99, 65, 85, 22, 62, 97, 58, 
        47, 3, 8, 72, 56, 65, 35, 100, 99, 44, 58, 68, 25, 50, 34, 3, 79, 28, 65, 84, 12, 56, 21, 75, 13, 31, 14, 89, 14, 20, 
        93, 40, 60, 26, 33, 77, 89, 67, 83, 57, 38, 30, 37, 35, 42, 49, 22, 89, 2, 89, 4, 2, 56, 58, 35, 84, 1, 9, 82, 7, 68, 
        78, 13, 66, 80, 82, 6, 67, 18, 63, 98, 55, 7, 47, 41, 21, 18, 17, 59, 4, 13, 68, 1, 66, 77, 70, 56, 10, 22, 17, 98, 78, 
        91, 77, 4, 71, 22, 81, 26, 80, 15, 41, 73, 11, 74, 49, 16, 55, 72, 28, 14, 100, 76, 26, 83, 70, 74, 53, 65, 66, 13, 21, 
        92, 42, 61, 46, 27, 17, 67, 5, 66, 81, 53, 73, 97, 75, 95, 51, 76, 15, 12, 98, 90, 96, 24, 90, 75, 95, 25, 20, 4, 17, 79, 
        80, 29, 44, 4, 74, 89, 4, 30, 52, 3, 10, 95, 87, 90, 31, 82, 3, 44, 85, 60, 63, 83, 63, 100, 62, 25, 34, 1, 49, 80, 40, 
        18, 14, 3, 55, 73, 61, 3, 5, 71, 15, 51, 70, 48, 3, 83, 68, 18, 47, 59, 2, 30, 44, 84, 94, 23, 11, 52, 84, 15, 99, 33, 23, 
        50, 49, 33, 50, 55, 15, 88, 79, 15, 26, 3, 90, 72, 79, 24, 29, 87, 97, 79, 76, 58, 80, 67, 7, 92, 46, 36, 31, 12, 72, 25, 
        54, 72, 33, 52, 21, 78, 21, 45, 91, 64, 50, 50, 76, 38, 7, 69, 6, 41, 12, 39, 96, 22, 49, 3, 75, 19, 42, 53, 47, 99, 33, 64, 
        22, 59, 46, 67, 87, 64, 94, 12, 9, 14, 39, 71, 51, 97, 1, 92, 85, 28, 88, 96, 28, 59, 48, 43, 7, 74, 20, 80, 89, 58, 61, 90, 
        38, 94, 76, 6, 65, 17, 57, 4, 4, 12, 30, 61, 73, 3, 63, 52, 45, 33, 29, 88, 67
    };
    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }
    l.Sort(CompareListItemsValue, SortOrder::Asc, SortMethod::QuickSort);

    bool32 pass = true;

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 > val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }

    _Assert(pass == true, "pass == true");

    l.Sort(CompareListItemsValue, SortOrder::Desc, SortMethod::QuickSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 < val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Clear();

    for (uint32 i = 0; i<ArrayCount(values); i++) {
        l.Add(&values[i]);
    }

    l.Sort(CompareListItemsValue, SortOrder::Desc, SortMethod::QuickSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 < val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Sort(CompareListItemsValue, SortOrder::Asc, SortMethod::QuickSort);

    iter = l.head;
    while (iter && iter->next) {

        int32 val1 = *(int32*)l.GetItem(iter)->data;
        int32 val2 = *(int32*)l.GetItem(iter->next)->data;
        if (val1 > val2) {
            pass = false;
            break;
        }
        iter = iter->next;
    }
    _Assert(pass == true, "pass == true");

    l.Free();
    return true;
}


internal void Run()
{
    printf("\n===========================================\n");
    printf("Start List test group\n");
    printf("===========================================\n");
    _RunTest(ListInit);
    _RunTest(ListInitArena);
    _RunTest(ListFree);
    _RunTest(ListFreeArena);
    _RunTest(ListAdd);
    _RunTest(ListAddArena);
    _RunTest(ListInsert);
    _RunTest(ListInsertArena);
    _RunTest(ListInsertAdd);
    _RunTest(ListInsertAddSorted);
    _RunTest(ListInsertAddSortedDesc);
    _RunTest(ListInsertAddArena);
    _RunTest(ListRemove);
    _RunTest(ListRemoveArena);
    _RunTest(ListRemoveAll);
    _RunTest(ListRemoveAllArena);
    _RunTest(ListClear);
    _RunTest(ListClearArena);
    _RunTest(ListItemValueAssigne);
    _RunTest(ListItemValueAssigneArena);
    _RunTest(ListBubleSort);
    _RunTest(ListQuickSort);
}

} // namespace

#define QTESTLSIT_CPP
#endif