// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QESAMPLERENDERSYSTEM_H)

#include "ECS.h"

namespace QuarksDD {

struct SampleRenderSystem {
    System system;    
    bool32 initialized;

    static bool32 Init(System* s) {
        bool32 result = false;

        SampleRenderSystem* sys = GET_SYSTEM(s, SampleRenderSystem);
        Assert(sys->system.initialized);

        if (!sys->initialized && sys->system.initialized) {
            sys->system.AddComponentType((uint32)ComponentType::Component1);
            sys->system.AddComponentType((uint32)ComponentType::Component2);
            sys->system.AddComponentType((uint32)ComponentType::Component3);

            sys->initialized = true;
            result = true;

            printf("\nSampleRenderSystem INIT!!!\n");
        }
        return result;
    }
    // bool32 Init() {
    //     bool32 result = false;
    //     Assert(system.initialized);

    //     if (!initialized && system.initialized) {
    //         system.AddComponentType((uint32)ComponentType::Component1);
    //         system.AddComponentType((uint32)ComponentType::Component2);
    //         system.AddComponentType((uint32)ComponentType::Component3);

    //         initialized = true;
    //         result = true;
    //     }
    //     return result;
    // }

    void Update() {
        // Do some update staff on entities 
    }

    void Render() {
        // Do some update staff on entities 
    }

};

} // namespace
#define QESAMPLERENDERSYSTEM_H
#endif
