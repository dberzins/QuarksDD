// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QESAMPLERENDERSYSTEM_H)

#include "ECS.h"
#include "../System.h"

namespace QuarksDD {

struct RenderActionContext {
    real32 delatatime;
};

struct SampleRenderSystem {
    System system;    
    bool32 initialized;

    SampleRenderSystem* Init() {
        SampleRenderSystem* result = NULL;

        if (!initialized) {
            
            // Defined ECS system
            SYSTEM_DEF(this, SystemType::SampleRenderSystem, 
                Flag64(ComponentType::Component1) |
                Flag64(ComponentType::Component2) |
                Flag64(ComponentType::Component3) 
            );

            initialized = true;
            result = this;
        }
        return result;
    }

    // Actions
    bool32 OnStartup(void* context) {
        printf("\nSampleRenderSystem OnStartup \n");
        return false;
    }

    bool32 OnAction(RenderActionContext* context) {
        printf("\nSampleRenderSystem OnAction \n");

        Array* components1 = system.GetComponents((u32)ComponentType::Component1);
        for (u32 i = 0; components1 && i < components1->count; i++) {
            Component* c = (Component*)components1->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n", c->entity->name, c->entity->id, c->id, c->type);
        }

        Array* components2 = system.GetComponents((u32)ComponentType::Component2);
        for (u32 i = 0; components2 && i < components2->count; i++) {
            Component* c = (Component*)components2->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n",c->entity->name, c->entity->id, c->id, c->type);
        }

        Array* components3 = system.GetComponents((u32)ComponentType::Component3);
        for (u32 i = 0; components3 && i < components3->count; i++) {
            Component* c = (Component*)components3->GetItem(i)->data;
            printf("Name: \"%s\" EnityId: %llu : ComponentId: %llu  type: %d\n",c->entity->name, c->entity->id, c->id, c->type);
        }

        return false;
    }

    bool32 OnFinish(void* context) {
        printf("\nSampleRenderSystem OnFinish \n");
        return false;
    }

    // Action invokers
    static bool32 Startup(System* s, void* context) {
        SampleRenderSystem* sys = GET_SYSTEM(s, SampleRenderSystem);
        return sys->OnStartup(context);
    }

    static bool32 Action(System* s, void* context) {
        SampleRenderSystem* sys = GET_SYSTEM(s, SampleRenderSystem);
        return sys->OnAction((RenderActionContext*)context);
    }
    static bool32 Finish(System* s, void* context) {
        SampleRenderSystem* sys = GET_SYSTEM(s, SampleRenderSystem);
        return sys->OnFinish(context);
    }
};

} // namespace
#define QESAMPLERENDERSYSTEM_H
#endif
