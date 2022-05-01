// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Component.h"

namespace QuarksDD {

bool32 Component::Init(Entity* entity, ComponentId componentId, uint32 componentType) {
    bool32 result = false;

    id = componentId;
    type = componentType;
    this->entity = entity;
    initialized = true;
    result = true;

    return result;
}

} // namespace