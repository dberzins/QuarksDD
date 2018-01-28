// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QESAMPLESYSTEM_H)

#include "ECS.h"
#include "../System.h"
#include "../../Array.h"

namespace QuarksDD {

struct SampleSystem {
    System system;    
    bool32 initialized;

    SampleSystem* Init() {
        SampleSystem* result = NULL;

        if (!initialized) {

            // Defined ECS system
            SYSTEM_DEF(this, SystemType::SampleSystem, 
                Flag64(ComponentType::Component1) |
                Flag64(ComponentType::Component2) 
            );
            
            initialized = true;
            result = this;
        }
        return result;
    }

    // Actions
    bool32 OnStartup(void* context) {
        printf("\nSampleSystem OnStartup \n");
        return false;
    }

    bool32 OnAction(void* context) {
        printf("\nSampleSystem OnAction \n");
  
        Array* components1 = system.GetComponents((u32)ComponentType::Component1);
        for (u32 i = 0; components1 && i < components1->count; i++) {
            Component* c = (Component*)components1->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n", c->entity->name, c->entity->id, c->id, c->type);
        }

        Array* components2 = system.GetComponents((u32)ComponentType::Component2);
        for (u32 i = 0; components2 && i < components2->count; i++) {
            Component* c = (Component*)components2->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n", c->entity->name, c->entity->id, c->id, c->type);
        }

        Array* components3 = system.GetComponents((u32)ComponentType::Component3);
        for (u32 i = 0; components3 && i < components3->count; i++) {
            Component* c = (Component*)components3->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n", c->entity->name, c->entity->id, c->id, c->type);
        }

        return false;
    }

    bool32 OnFinish(void* context) {
        printf("\nSampleSystem OnFinish \n");
        return false;
    }

    // Action invokers
    static bool32 Startup(System* s, void* context) {
        SampleSystem* sys = GET_SYSTEM(s, SampleSystem);
        return sys->OnStartup(context);
    }

    static bool32 Action(System* s, void* context) {
        SampleSystem* sys = GET_SYSTEM(s, SampleSystem);
        return sys->OnAction(context);
    }

    static bool32 Finish(System* s, void* context) {
        SampleSystem* sys = GET_SYSTEM(s, SampleSystem);
        return sys->OnFinish(context);
    }
};


} // namespace
#define QESAMPLESYSTEM_H
#endif
