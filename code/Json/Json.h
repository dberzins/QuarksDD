// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QJSON_H)
#include "../Hashtable.h"
#include "../Array.h"

namespace QuarksDD {

    struct JValue;
    struct JArray;

    enum class JValueType {
        Integer,
        Float,
        String,
        Object,
        Array
    };

    struct JString {
        // Data
        char* str;
        uint32 len;
    };

    struct JObject {
        // Data 
        Hashtable fields;

        // Operations
        JValue* operator[](char* key) {
            JValue* value = NULL;
            HashItem* item = fields.GetItem(key);
            if (item) {
                value = (JValue*)item->data;
            }

            return value;
        }
    };

    struct JArray {
        // Data
        Array values;
    };

    struct JValue 
    {
        // Data 
        bool32 initialized;
        JValueType type;
        union {
            int32 iValue;
            real32 fValue;
            JString sValue;
            JObject oValue;
            JArray	 aValue;
        };

        // Operations
        bool32 Init();

        void* GetValue();
        bool32 GetValue(int32** value);
        bool32 GetValue(int32& value);
        bool32 GetValue(real32** value);
        bool32 GetValue(real32& value);
        bool32 GetValue(JString** value);
        bool32 GetValue(JObject** value);
        bool32 GetValue(JArray** value);
    };
} // namespace

#define QJSON_H
#endif