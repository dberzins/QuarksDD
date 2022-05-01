// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QCOMPONENT_H)

#include "../QuarksDD.h"

namespace QuarksDD {

struct Entity;

#define GET_COMPONENT(c, Type) (Type*) ContainerOf(c, Type, component)
#define GET_SCOMPONENT(c, Type) (Type*) GET_COMPONENT(&c->component, Type)
#define GET_ENTITY_COMPONENT(e, Type, CType) GET_COMPONENT(((Entity*)e)->GetComponent((u32)CType), Type)
#define GET_REL_COMPONENT(c, Type, CType) (Type*) GET_ENTITY_COMPONENT(((Component*)c)->entity, Type, CType)
#define GET_REL_SCOMPONENT(c, Type, CType) (Type*) GET_ENTITY_COMPONENT(c->component.entity, Type, CType)

#define COMPONENT_ID_UNINITIALIZED 0
#define COMPONENT_TYPE_UNINITIALIZED 0
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

