// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QESAMPLESYSTEM_H)

#include "ECS.h"

namespace QuarksDD {

struct SampleSystem {
    System system;    
    bool32 initialized;


    static bool32 Init(System* s) {
        bool32 result = false;

        SampleSystem* sys = GET_SYSTEM(s, SampleSystem);
        Assert(sys->system.initialized);

        if (!sys->initialized && sys->system.initialized) {
            sys->system.AddComponentType((uint32)ComponentType::Component1);
            sys->system.AddComponentType((uint32)ComponentType::Component2);

            sys->initialized = true;
            result = true;

            printf("\nSampleSystem INIT!!!\n");
        }
        return result;
    }
    // bool32 Init() {
    //     bool32 result = false;
    //     Assert(system.initialized);

    //     if (!initialized && system.initialized) {
    //         system.AddComponentType((uint32)ComponentType::Component1);
    //         system.AddComponentType((uint32)ComponentType::Component2);

    //         initialized = true;
    //         result = true;
    //     }
    //     return result;
    // }

    void Update() {
        // Do some update staff on entities 
    }
};


} // namespace
#define QESAMPLESYSTEM_H
#endif
