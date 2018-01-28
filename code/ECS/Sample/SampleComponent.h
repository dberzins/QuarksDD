// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QESAMPLECOMPONET_H)

#include "ECS.h"
#include "../Component.h"
namespace QuarksDD {

struct SampleComponent1 {
    uint32 prop1;
    uint32 prop2;
    uint32 prop3;
    Component component;
};

struct SampleComponent2 {
    real32 attr1;
    real32 attr2;
    real32 attr3;
    Component component;
};

struct SampleComponent3 {
    int8 field1;
    int8 field2;
    int8 field3;
    Component component;
};

struct SampleComponent4 {
    int8 field1;
    int8 field2;
    int8 field3;
    Component component;
};

struct SampleComponent5 {
    int8 field1;
    int8 field2;
    int8 field3;
    Component component;
};

struct SampleComponent6 {
    int8 field1;
    int8 field2;
    int8 field3;
    Component component;
};

} // namespace
#define QESAMPLECOMPONET_H
#endif
