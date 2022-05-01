// Copyright (c) 2021 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Graph.h"
#include "Stack.h"
#include "Queue.h"

namespace QuarksDD {

int32 CompareValues(void* v1, void* v2)
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

void Graph::DepthSearch(uint32 start, Array* result) {
    Assert(result != NULL);
    Assert(result->initialized == true)
    result->sorted = true;
    result->order = SortOrder::Asc;
    result->Compare = CompareValues;

    Stack stack;
    stack = {};
    stack.Init(arena);
    stack.Push(&start);

    while (!stack.IsEmpty()) {
        uint32 node = *(uint32* )stack.Pop();
        int32 pos = result->Find(&node);
        if (pos < 0) {
            result->AddSorted(&node);

            List* list = (List*)adjencies->GetItem(node)->data;
            ListIterator* iter = list->head;
            while (iter)  {
                Edge* edge = (Edge*)list->GetItem(iter)->data;
                
                pos = result->Find(&edge->start);
                if (pos < 0)  {
                    stack.Push(&edge->start);
                }

                pos = result->Find(&edge->end);
                if (pos < 0)  {
                    stack.Push(&edge->end);
                }

                iter = iter->next;    
            }
        }
    }

    stack.Free();
}

void Graph::BreadthSearch(uint32 start, Array* result) {
    Assert(result != NULL);
    Assert(result->initialized == true)
    result->sorted = true;
    result->order = SortOrder::Asc;
    result->Compare = CompareValues;

    Queue queue;
    queue = {};
    queue.Init(arena);
    queue.Enque(&start);

    while (!queue.IsEmpty()) {
        uint32 node = *(uint32* )queue.Deque();
        int32 pos = result->Find(&node);
        if (pos < 0) {
            result->AddSorted(&node);

            List* list = (List*)adjencies->GetItem(node)->data;
            ListIterator* iter = list->head;
            while (iter)  {
                Edge* edge = (Edge*)list->GetItem(iter)->data;
                
                pos = result->Find(&edge->start);
                if (pos < 0)  {
                    queue.Enque(&edge->start);
                }

                pos = result->Find(&edge->end);
                if (pos < 0)  {
                    queue.Enque(&edge->end);
                }

                iter = iter->next;    
            }
        }
    }
    queue.Free();
}

void Graph::Print() {
    HashtableIterator* iter = adjencies->GetIterator();
    while (iter) {
        
        List* list = (List*)(adjencies->GetItem(iter)->data);
        printf("[%d] ", adjencies->GetItem(iter)->index);

        ListIterator* liter = list->head;
        while (liter) {
            Edge* edge = (Edge*)list->GetItem(liter)->data;
            printf(" %d, %d |", edge->start, edge->end);
            liter = liter->next;
        }
        printf("\n");
        
        iter = iter->next;
    }
}

void Graph::Build() {

    for (uint32 i = 0; i < edgeCount; i++) {
        printf("add edge %d\n", i);
        if (!adjencies->HasIndex(edges[i].start)) {
            
            List* list = ArenaPushStruct(arena, List);
            *list = {};
            list->Init(arena);
            list->Add(&edges[i]);

            adjencies->Add(edges[i].start,  list); 
            int32 x = 1;
        }
        else {
            List* list = (List*)adjencies->GetItem(edges[i].start)->data;
            list->Add(&edges[i]);
        }

        if (!adjencies->HasIndex(edges[i].end)) {
            
            List* list = ArenaPushStruct(arena, List);
            *list = {};
            list->Init(arena);
            list->Add(&edges[i]);

            adjencies->Add(edges[i].end, list); 
        }
        else {
            List* list = (List*)adjencies->GetItem(edges[i].end)->data;
            list->Add(&edges[i]);
        }
    }
}

bool32  Graph::Init(Edge* edges, uint32 edgeCount)
{
    bool32 result = false;

    if (!initialized) {
        initialized = true;
        result = true;

        this->edges = edges;
        this->edgeCount = edgeCount;
        this->nodeCount = edgeCount + 1;

        arena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *arena = {};
        arena->Init(sizeof(Hashtable) + RoundUpToPowerOf2(sizeof(HashItem) * nodeCount) + sizeof(List) * nodeCount + sizeof(ListItem) * edgeCount* nodeCount);


        adjencies = (Hashtable*)ArenaPushStruct(arena, Hashtable);
        *adjencies = {};
        adjencies->Init(arena, nodeCount);

        Build();
        Print();
    }
    
    return result;
}

bool32  Graph::Init(MemoryArena *arena, Edge* edges, uint32 edgeCount)
{
    bool32 result = false;

    if (!initialized) {
        initialized = true;
        result = true;
        
        useLocalArena = false;
        this->arena = arena;

        this->edges = edges;
        this->edgeCount = edgeCount;
        this->nodeCount = edgeCount + 1;

        adjencies = (Hashtable*)ArenaPushStruct(arena, Hashtable);
        *adjencies = {};
        adjencies->Init(arena, nodeCount);

        Build();
    }

    return result;
}



void Graph::Free() {
    if (initialized) {
        if (useLocalArena && arena) {
            arena->Free();
            Deallocate(arena);
            arena = NULL;
        }
        else if (arena) {
            // TODO(dainis): Upgrade MemoryArena to track allocations/deallocation positions
            // so memory can bee marked as free in middle of memory block/blocks.
            // Currently just assume that arena will be freed later by arena owner 
            // arena->used -= size * sizeof(ListItem);
        }
    }

    *this = {};
}

} // namespace 