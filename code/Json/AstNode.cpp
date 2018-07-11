// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "AstNode.h"

#include <stdio.h>
#include <string.h>

namespace QuarksDD {

void AstNode::PrintSpaces(uint32 level) {
    for (uint32 t=0; t<level; t++) {
        printf("    ");
    }
}

void AstNode::Print() {

    AstNode* node;
    
    node = this;
    while (node) {
        switch(node->type) {
            case AstType::Integer : {
                printf("%d", node->JsonInt.value);
                break;
            }
            case AstType::Float : {
                printf("%f", node->JsonFloat.value);
                break;
            }
            case AstType::String : {
                char* value = (char*)Allocate(node->JsonString.len + 1);
                memcpy(value, node->JsonString.value, node->JsonString.len);
                value[node->JsonString.len] = 0;
                printf("\"%s\"", value);
                Deallocate(value);

                break;
            }
            case AstType::Key: {
                char* key = (char*)Allocate(node->JsonKey.len + 1);
                memcpy(key, node->JsonKey.name, node->JsonKey.len);
                key[node->JsonKey.len] = 0;
                printf("\"%s\" : ", key);
                Deallocate(key);

                break;
            }
            case AstType::Field : {
                PrintSpaces(node->level);
                node->JsonField.key->Print();
                node->JsonField.value->Print();
                if (node->right) {
                    printf(",\n");
                }
                else {
                    printf("\n");
                }
                break;
            }
            case AstType::Object : {
                printf("{\n");
                node->JsonObject.fields->Print();
                PrintSpaces(node->level);
                printf("}");
                break;
            }
            case AstType::Array : {
                printf("[");
                node->JsonArray.values->Print();
                printf("]");
                break;
            }
        }
        
        // Handle array values comma separator
        if (node->parent && node->parent->type == AstType::Array) {
            if (node->right) {
                printf(", ");
            }
        }

        node = node->right;
    }
}

JValue* AstNode::Resolve(MemoryArena* arena) {
    JValue* result = NULL;

    switch(type) {
        case AstType::Integer : {
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};

            result->type = JValueType::Integer;
            result->iValue = JsonInt.value; 
            return result;
        }
        case AstType::Float : {
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
            
            result->type = JValueType::Float;
            result->fValue = JsonFloat.value;
            return result;
        }
        case AstType::String : {
            char* value = (char*)ArenaPushSize(arena, JsonString.len + 1);
            memcpy(value, JsonString.value, JsonString.len);
            value[JsonString.len] = 0;

            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
            
            result->type = JValueType::String;
            result->sValue.str = value;
            result->sValue.len = JsonString.len + 1;
            return result;
        }
        case AstType::Object : {
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
            
            result->type = JValueType::Object;
            result->oValue.fields.Init(arena, JsonObject.count);

            AstNode* field = JsonObject.fields;
            while (field) {
                char* key = (char*)ArenaPushSize(arena, field->JsonField.key->JsonKey.len+1);
                memcpy(key, field->JsonField.key->JsonKey.name, field->JsonField.key->JsonKey.len + 1);
                key[field->JsonField.key->JsonKey.len] = 0;

                JValue* value = field->JsonField.value->Resolve(arena);
                result->oValue.fields.Add(key, value);

                field = field->right;
            }

            break;
        }
        case AstType::Array : {
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
           
            result->type = JValueType::Array;
            result->aValue.values.Init(arena, JsonArray.count);

            AstNode* value = JsonArray.values;
            for (uint32 i=0; value; i++) {
                result->aValue.values.Add(value->Resolve(arena));
                value = value->right;
            }

            break;
        }
    }
    
    return result;
}

} // namsepace