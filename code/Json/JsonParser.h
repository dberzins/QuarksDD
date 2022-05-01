// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QJSONPARSER_H)

#include "../QuarksDD.h"
#include "../Memory.h"

#include "AstNode.h"
#include "Json.h"
#include "JsonToken.h"
#include "JsonTokenizer.h"
#include "../Intern.h"

namespace QuarksDD {
    struct JsonParser
    {
        // Data
        bool32 initialized;
        InternTable symbols;
        
        JValue* root;
        AstNode* rootNode;
        const char* filePath;
        char* source;

        MemoryArena sourceArena;
        MemoryArena nodesArena;
        MemoryArena valuesArena;
        
        // Operations
        bool32 Init(char* filePath);
        void Free();

        bool32 IsJsonFileExt(char* filePath);
        MemorySize GetFileSize(const char *filePath);

        JValue* Parse();

    private:
        AstNode* AddNode(MemoryArena* arena, AstType type, AstNode* parent, uint32 level);
        bool32 ReadFile(MemoryArena* arena, const char *filePath, char** buffer);

        AstNode* ParseValue(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent, uint32 level);
        AstNode* ParseObject(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent, uint32 level);
        AstNode* ParseArray(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent, uint32 level);
        AstNode* ParseField(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent, uint32 level);
    };

} // namespace

#define QJSONPARSER_H
#endif