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
#include "ECS\Sample\SampleComponent.h"
#include "ECS\Sample\SampleSystem.h"
#include "ECS\Sample\SampleRenderSystem.h"

using namespace QuarksDD;

namespace QuarksDD {
    MemoryStats memStats = {};
}

int32 main(int32 argc, char **argv)
{
    printf("Current dir: %s\n", argv[0]);

    EntityManager ecs = {};
    ecs.Init();

    // Add systems
    SampleSystem* system1 = NULL;
    ADD_SYSTEM(ecs, system1, SampleSystem, SystemType::SampleSystem);

    SampleRenderSystem* system2 = NULL;
    ADD_SYSTEM(ecs, system2, SampleRenderSystem, SystemType::SampleRenderSystem);

    ecs.InitSystems();

    // Add entities
    {
        Entity* e = ecs.CreateEntity();
        if (e) {
            // Add components
            {
                SampleComponent1* c1 = NULL;
                CREATE_COMPONENT(ecs, c1, SampleComponent1, ComponentType::Component1);
                c1->prop1 = 1;
                c1->prop2 = 2;
                c1->prop3 = 3;
                ADD_COMPONENT(e, c1);

                // e->AddComponent(&c1->component);

                SampleComponent2* c2 = NULL;
                CREATE_COMPONENT(ecs, c2, SampleComponent2, ComponentType::Component2);
                c2->attr1 = 1;
                c2->attr2 = 2;
                c2->attr3 = 3;
                e->AddComponent(&c2->component);
                
                SampleComponent3* c3 = NULL;
                CREATE_COMPONENT(ecs, c3, SampleComponent3, ComponentType::Component3);
                c3->field1 = 1;
                c3->field2 = 2;
                c3->field3 = 3;
                e->AddComponent(&c3->component);

                // SampleComponent2* c2 = NULL;
                // ADD_COMPONENT(ecs, e, c2, SampleComponent2, ComponentType::Component2);
                // SampleComponent3* c3 = NULL;
                // ADD_COMPONENT(ecs, e, c3, SampleComponent3, ComponentType::Component3);
                e->RemoveComponent(&c3->component);

                SampleComponent2* cc2 = GET_COMPONENT(&c2->component, SampleComponent2);

                bool32 hasC1 = e->HasComponent((uint32)ComponentType::Component1);
                bool32 hasC2 = e->HasComponent((uint32)ComponentType::Component2);
                bool32 hasC3 = e->HasComponent((uint32)ComponentType::Component3);
                bool32 hasX = e->HasComponent((uint32)ComponentType::Component_Count);
                bool32 hasY = e->HasComponent((uint32)ComponentType::Component_Count);
                bool32 hasZ = e->HasComponent((uint32)ComponentType::Component_Count);

            }
            ecs.AddEntity(e);
        }
        // ADD_ENTITY(ecs, e, Entity);
    }
    
    // {
    //     Entity* e = NULL;
    //     AddEntity(ecs, e, Entity);

    //     Add components
    //     {
    //         SampleComponent1* c1 = NULL;
    //         AddComponent(ecs, e, c1, SampleComponent1);
    //         SampleComponent3* c3 = NULL;
    //         AddComponent(ecs, e, c3, SampleComponent3);
    //     }
    // }

    // uint32 loopCount = 0;
    // while (loopCount++ < 10) {
    //     ecs.Update();
    // }
    // Do staff with ecs

    ecs.Free();




    // JsonParser parser = {};
    // parser.Init("../../tests/data/SampleData.json");
    // JValue* root = parser.Parse();
    // parser.Free();

    return 0;
}