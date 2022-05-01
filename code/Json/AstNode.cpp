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
                printf("\"%s\"", node->JsonString.value);
                break;
            }
            case AstType::Key: {
                printf("\"%s\" : ", node->JsonKey.name);
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
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
            
            result->type = JValueType::String;
            result->sValue.str = JsonString.value;
            return result;
        }
        case AstType::Object : {
            result = (JValue*)ArenaPushStruct(arena, JValue);
            *result = {};
            
            result->type = JValueType::Object;
            result->oValue.fields.Init(arena, JsonObject.count);

            AstNode* field = JsonObject.fields;
            while (field) {
                JValue* value = field->JsonField.value->Resolve(arena);
                result->oValue.fields.Add(field->JsonField.key->JsonKey.name, value);

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