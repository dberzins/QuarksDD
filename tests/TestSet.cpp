#if !defined(QTESTSet_CPP)

#include <QuarksDD.h>
#include <Memory.h>
#include <Set.h>
#include <List.h>
#include <Array.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestSet
{

// TODO(dainis): create Set tests

// int32 CompareSetValuesInt32(void* v1, void* v2)
// {
//    int32 result = 0;

//    int32 value1 = *(int32*)v1;
//    int32 value2 = *(int32*)v2;

//    if (value1 == value2)
//        result = 0;
//    if (value1 < value2)
//        result = -1;
//    if (value1 > value2)
//        result = 1;

//    return result;
// }
// Set set1;
// set1 = {};
// set1.Init(CompareSetValuesInt32);
// int32 values1[] = {5, 3, 3, 8}; 

// set1.Add(&values1[0]);
// set1.Add(&values1[1]);
// set1.Add(&values1[2]);
// set1.Add(&values1[3]);

// Array* r1 = set1.ToArray();
// printf("Set1:");
// for(uint32 i=0; i < r1->count; i++) {
//     int32 val = *(int32*)(*r1)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");


// Set set2;
// set2 = {};
// set2.Init(CompareSetValuesInt32);
// int32 values2[] = {1, 3, 10, 7}; 

// set2.Add(&values2[0]);
// set2.Add(&values2[1]);
// set2.Add(&values2[2]);
// set2.Add(&values2[3]);

// Array* r2 = set2.ToArray();
// printf("Set2:");
// for(uint32 i=0; i < r2->count; i++) {
//     int32 val = *(int32*)(*r2)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");


// Set* set3 = set1.Union(&set2);

// List* result = set3->ToList();
// printf("List (set1 + set2) union values:");
// ListIterator* iter = result->head;
// uint32 i = 0;
// while (iter) {
//     int32 val = *(int32*)result->GetItem(iter)->data;
//     printf("%d, ", val);
//     iter = iter->next;
// }
// printf("\n");

// Array* result2 = set3->ToArray();
// printf("Array (set1 + set2) union values:");
// for(uint32 i=0; i < result2->count; i++) {
//     int32 val = *(int32*)(*result2)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");

// Set* set4 = set1.Difference(&set2);
// Array* result3 = set4->ToArray();
// printf("(set1 - set2) difference values:");
// for(uint32 i=0; i < result3->count; i++) {
//     int32 val = *(int32*)(*result3)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");

// Set* set6 = set2.Difference(&set1);
// Array* result6 = set6->ToArray();
// printf("(set2 - set1) difference values:");
// for(uint32 i=0; i < result6->count; i++) {
//     int32 val = *(int32*)(*result6)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");

// Set* set5 = set1.Intersect(&set2);
// Array* result4 = set5->ToArray();
// printf("set1, set2 intersect values:");
// for(uint32 i=0; i < result4->count; i++) {
//     int32 val = *(int32*)(*result4)[i].data;
//     printf("%d, ", val);
// }
// printf("\n");


internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Set test group\n");
    printf("===========================================\n");
    // _RunTest(SetInit);
    // _RunTest(SetInitArena);
    // _RunTest(SetFree);
    // _RunTest(SetFreeArena);
    // _RunTest(SetAdd);
    // _RunTest(SetAddArena);
    // _RunTest(SetInsert);
    // _RunTest(SetInsertArena);
    // _RunTest(SetInsertAdd);
    // _RunTest(SetInsertAddSorted);
    // _RunTest(SetInsertAddSortedDesc);
    // _RunTest(SetInsertAddArena);
    // _RunTest(SetRemove);
    // _RunTest(SetRemoveArena);
    // _RunTest(SetRemoveAll);
    // _RunTest(SetRemoveAllArena);
    // _RunTest(SetClear);
    // _RunTest(SetClearArena);
    // _RunTest(SetItemValueAssigne);
    // _RunTest(SetItemValueAssigneArena);
    // _RunTest(SetBubleSort);
    // _RunTest(SetQuickSort);
}

} // namespace

#define QTESTSET_CPP
#endif