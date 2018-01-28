// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>

#include "Common.h"
#include "Memory.h"
#include "Array.h"
#include "Hashtable.h"
#include "Json\JsonParser.h"

#include "ECS\Sample\ECS.h"
#include "ECS\Entity.h"
#include "ECS\System.h"
#include "ECS\EntityManager.h"
#include "ECS\Sample\SampleComponent.h"
#include "ECS\Sample\SampleSystem.h"
#include "ECS\Sample\SampleRenderSystem.h"

// #include <windows.h>
using namespace QuarksDD;

namespace QuarksDD {
    MemoryStats memStats = {};
}
int32 main(int32 argc, char **argv)
{
    printf("Current dir: %s\n", argv[0]);

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
            SampleComponent1* c = CREATE_COMPONENT(e, SampleComponent1);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component1, c);
            }
        }        
        {
            SampleComponent2* c = CREATE_COMPONENT(e, SampleComponent2);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component2, c);
            }
        }        
        {
            SampleComponent3* c = CREATE_COMPONENT(e, SampleComponent3);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component3, c);
            }
        }        
        {
            SampleComponent4* c = CREATE_COMPONENT(e, SampleComponent4);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component4, c);
            }
        }        
        {
            SampleComponent5* c = CREATE_COMPONENT(e, SampleComponent5);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component5, c);
            }
        }        
        {
            SampleComponent6* c = CREATE_COMPONENT(e, SampleComponent6);
            if (c) {
                *c = {};
                ADD_COMPONENT(e, ComponentType::Component6, c);
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



    // JsonParser parser = {};
    // parser.Init("../../tests/data/SampleData.json");
    // JValue* root = parser.Parse();
    // parser.Free();

    return 0;
}