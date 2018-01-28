// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QCOMPONENT_H)

#include "../Common.h"

namespace QuarksDD {

struct Entity;

#define GET_COMPONENT(c, Type) (Type*) ContainerOf(c, Type, component)
typedef uint64 ComponentId;

enum class ComponentStatus {
    Ghost = 0,
    Alive,
    Dead
};

struct Component {
    ComponentId id;
    uint32 type;
    ComponentStatus status;
    Entity* entity;
    bool32 initialized;

    bool32 Init(Entity* entity, ComponentId componentId, uint32 componentType);
};


} // namespace
#define QCOMPONENT_H
#endif

