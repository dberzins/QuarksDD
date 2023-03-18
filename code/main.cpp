// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>

#include "QuarksDD.h"
#include "Memory.h"
#include "Buffer.h"
#include "Array.h"
#include "Graph.h"
#include "Set.h"
#include "FlatArray.h"
#include "Hashtable.h"
#include "Json\JsonParser.h"

#include "ECS\Sample\ECS.h"
#include "ECS\Entity.h"
#include "ECS\System.h"
#include "ECS\EntityManager.h"
#include "ECS\Sample\SampleComponent.h"
#include "ECS\Sample\SampleSystem.h"
#include "ECS\Sample\SampleRenderSystem.h"

#include "Intern.h"

#include <windows.h>
using namespace QuarksDD;

namespace QuarksDD {
    MemoryStats memStats = {};
}

#if 0
int32 CompareSetValuesInt32(void* v1, void* v2)
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
#endif


int32 main(int32 argc, char **argv)
{
    printf("Current dir: %s\n", argv[0]);

#if 0
    Set set1;
    set1 = {};
    set1.Init(CompareSetValuesInt32);
    int32 values1[] = {5, 3, 3, 8}; 

    set1.Add(&values1[0]);
    set1.Add(&values1[1]);
    set1.Add(&values1[2]);
    set1.Add(&values1[3]);

    Array* r1 = set1.ToArray();
    printf("Set1:");
    for(uint32 i=0; i < r1->count; i++) {
        int32 val = *(int32*)(*r1)[i].data;
        printf("%d, ", val);
    }
    printf("\n");


    Set set2;
    set2 = {};
    set2.Init(CompareSetValuesInt32);
    int32 values2[] = {1, 3, 10, 7}; 

    set2.Add(&values2[0]);
    set2.Add(&values2[1]);
    set2.Add(&values2[2]);
    set2.Add(&values2[3]);

    Array* r2 = set2.ToArray();
    printf("Set2:");
    for(uint32 i=0; i < r2->count; i++) {
        int32 val = *(int32*)(*r2)[i].data;
        printf("%d, ", val);
    }
    printf("\n");


    Set* set3 = set1.Union(&set2);

    List* result = set3->ToList();
    printf("List (set1 + set2) union values:");
    ListIterator* iter = result->head;
    uint32 i = 0;
    while (iter) {
        int32 val = *(int32*)result->GetItem(iter)->data;
        printf("%d, ", val);
        iter = iter->next;
    }
    printf("\n");

    Array* result2 = set3->ToArray();
    printf("Array (set1 + set2) union values:");
    for(uint32 i=0; i < result2->count; i++) {
        int32 val = *(int32*)(*result2)[i].data;
        printf("%d, ", val);
    }
    printf("\n");

    Set* set4 = set1.Difference(&set2);
    Array* result3 = set4->ToArray();
    printf("(set1 - set2) difference values:");
    for(uint32 i=0; i < result3->count; i++) {
        int32 val = *(int32*)(*result3)[i].data;
        printf("%d, ", val);
    }
    printf("\n");

    Set* set6 = set2.Difference(&set1);
    Array* result6 = set6->ToArray();
    printf("(set2 - set1) difference values:");
    for(uint32 i=0; i < result6->count; i++) {
        int32 val = *(int32*)(*result6)[i].data;
        printf("%d, ", val);
    }
    printf("\n");

    Set* set5 = set1.Intersect(&set2);
    Array* result4 = set5->ToArray();
    printf("set1, set2 intersect values:");
    for(uint32 i=0; i < result4->count; i++) {
        int32 val = *(int32*)(*result4)[i].data;
        printf("%d, ", val);
    }
    printf("\n");
#endif



#if 0
    Edge edges[] = {{0,1,2},{0,2,4},{1,4,3},{2,3,2},{3,1,4},{4,3,3}, {5, 6, 1}};
    Graph graph;
    graph = {};
    graph.Init((Edge*)edges, (int32)ArrayCount(edges));

    Array result;
    result = {};
    result.Init(graph.nodeCount, sizeof(int32));

    // graph.DepthSearch(4, &result);
    graph.BreadthSearch(0, &result);
    
    for (uint32 i = 0; i < result.count; i++) {
        int32* node = (int32*)result[i].data;
        printf("%d, ", *node);
    }

#endif
    // ListIterator* iter = list.head;
    // while (iter) {
    //     uint32* node = (uint32*)list.GetItem(iter)->data;
    //     printf("%d, ", *node);
    //     iter = iter->next;
    // }



    int32 a = 1;

#if 0
    InternTable table;
    table = {};
    table.Init();
    char str[] = "IcoSphere";
    const char* s1 = table.InternRange(str, str + strlen(str));
    const char* s2 = table.InternStr("IcoSphere");
    Assert(s1==s2);

    // InternTable table;
    // table = {};
    // table.Init();
    // const char* a;
    // const char* b;
    // const char* c;
    // const char* d;

    // a = table.InternStr("IcoSphere");
    // b = table.InternStr("Sphere");
    // c = table.InternStr("Circle");
    // d = table.InternStr("Cylinder");
#endif

#if 0
    // LARGE_INTEGER StartingTime, InitTime, ExecTime, ElapsedMicroseconds;
    // LARGE_INTEGER Frequency;

    // QueryPerformanceFrequency(&Frequency); 
    // QueryPerformanceCounter(&StartingTime);



    EntityManager ecs = {};
    EntityManager::maxComponents = MAX_COMPONENTS;
    EntityManager::maxEntities = MAX_ENTITIES;
    EntityManager::maxSystems = MAX_SYSTEMS;
    
    MemoryArena systemArena = {};
    systemArena.Init(Megabytes(10));
    
    MemoryArena entityArena = {};
    entityArena.Init(Megabytes(30));
    
    MemoryArena componentArena = {};
    componentArena.Init(Megabytes(20));

    ecs.Init(&systemArena, &entityArena, &componentArena);

    {
        SampleSystem* s = CREATE_SYSTEM(&ecs, SampleSystem);
        Assert(s);
        ADD_SYSTEM(&ecs, s);
    }
    {
        SampleRenderSystem* s = CREATE_SYSTEM(&ecs, SampleRenderSystem);
        Assert(s);
        ADD_SYSTEM(&ecs, s);
    }

    for (uint32 i = 0; i < EntityManager::maxEntities; i++) {
        Entity* e = ecs.CreateEntity("Entity");
        {
            // SampleComponent1* c = CREATE_COMPONENT(e, SampleComponent1);
            SampleComponent1* c = ArenaPushStruct(&ecs.componentArena, SampleComponent1);
            if (c) {
                *c = {};
                e->AddComponent((uint32)ComponentType::Component1, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component1, c);
            }
        }        
        {
            SampleComponent2* c = ArenaPushStruct(&ecs.componentArena, SampleComponent2);
            // SampleComponent2* c = CREATE_COMPONENT(e, SampleComponent2);
            if (c) {
                *c = {};
                e->AddComponent((u32)ComponentType::Component2, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component2, c);
            }
        }        
        {
            SampleComponent3* c = ArenaPushStruct(&ecs.componentArena, SampleComponent3);
            // SampleComponent3* c = CREATE_COMPONENT(e, SampleComponent3);
            if (c) {
                *c = {};
                e->AddComponent((u32)ComponentType::Component3, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component3, c);
            }
        }        
        {
            SampleComponent4* c = ArenaPushStruct(&ecs.componentArena, SampleComponent4);
            // SampleComponent4* c = CREATE_COMPONENT(e, SampleComponent4);
            if (c) {
                *c = {};
                e->AddComponent((u32)ComponentType::Component4, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component4, c);
            }
        }        
        {
            SampleComponent5* c = ArenaPushStruct(&ecs.componentArena, SampleComponent5);
            // SampleComponent5* c = CREATE_COMPONENT(e, SampleComponent5);
            if (c) {
                *c = {};
                e->AddComponent((u32)ComponentType::Component5, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component5, c);
            }
        }        
        {
            SampleComponent6* c = ArenaPushStruct(&ecs.componentArena, SampleComponent6);
            // SampleComponent6* c = CREATE_COMPONENT(e, SampleComponent6);
            if (c) {
                *c = {};
                e->AddComponent((u32)ComponentType::Component6, &c->component);
                // ADD_COMPONENT(e, ComponentType::Component6, c);
            }
        }        

        ecs.AddEntity(e);
    }

    // QueryPerformanceCounter(&InitTime);
    // ElapsedMicroseconds.QuadPart = InitTime.QuadPart - StartingTime.QuadPart;
    
    // ElapsedMicroseconds.QuadPart *= 1000000;
    // ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    // real32 elapsedSeconds = (real32)ElapsedMicroseconds.QuadPart/1000000.0f;

    // printf("ECS Init Elapsed: %f (s)\n", elapsedSeconds);
  

    RenderActionContext ctx = {};
    ctx.delatatime = 10.f;

    uint32 loopCount = 0;
    while (loopCount++ < 1) {
        ecs.Run(Flag32(SystemType::SampleSystem), NULL);
        ecs.Run(Flag32(SystemType::SampleRenderSystem), &ctx);
    }


    // QueryPerformanceCounter(&ExecTime);
    // ElapsedMicroseconds.QuadPart = ExecTime.QuadPart - InitTime.QuadPart;
    
    // ElapsedMicroseconds.QuadPart *= 1000000;
    // ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    // elapsedSeconds = (real32)ElapsedMicroseconds.QuadPart/1000000.0f;

    // printf("ECS Exec Elapsed: %f (s)\n", elapsedSeconds);

    ecs.Finish();
    ecs.Free();

    systemArena.Free();
    entityArena.Free();
    componentArena.Free();

#endif

#if 0
    JsonParser parser = {};
    parser.Init("../../tests/data/SampleData.json");
    JValue* root = parser.Parse();
    parser.Free();

#endif
    return 0;
}