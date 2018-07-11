// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QASTNODE_H)

#include "../QuarksDD.h"
#include "../Memory.h"
#include "JsonToken.h"
#include "Json.h"

namespace QuarksDD {

    enum class AstType
    {
        Key,
        Field,
        String,
        Integer,
        Float,
        Object,
        Array
    };

    struct AstNode
    {
        // Data      
        AstType type;
        AstNode* left;
        AstNode* right;
        AstNode* parent;
        uint32 level;

        union {
            struct
            {
                JsonToken token;
                char* name;
                uint32 len;
            } JsonKey;

            struct {
                JsonToken token;
                char* value;
                uint32 len;
            } JsonString;

            struct {
                JsonToken token;
                int32 value;
            } JsonInt;
            
            struct {
                JsonToken token;
                real32 value;
            } JsonFloat;

            struct
            {
                JsonToken token;
                AstNode* key;
                AstNode* value;
            } JsonField;

            struct
            {
                JsonToken open;
                JsonToken close;
                
                AstNode* fields;
                AstNode* last;
                uint32 count;
            } JsonObject;
            struct
            {
                JsonToken open;
                JsonToken close;
                
                AstNode* values;
                uint32 count;
                AstNode* last;
            } JsonArray;
        };

        // Operations
        void Print();
        JValue* Resolve(MemoryArena* arena);
    private:
        void PrintSpaces(uint32 level);
    };

} // namespace

#define QASTNODE_H
#endif