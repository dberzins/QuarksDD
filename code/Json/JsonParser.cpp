#include "JsonParser.h"
#include "JsonTokenizer.h"

namespace QuarksDD
{

bool32 JsonParser::Init(char* filePath) {
    bool32 result = false;
        
    if (!initialized && IsJsonFileExt(filePath))
    {
        MemorySize size = GetFileSize(filePath);
        if (size <= 0) {
            printf("Failed to load %s file.\n", filePath);
            return false;
        }

        sourceArena = {};
        sourceArena.Init(++size, true);

        nodesArena = {};
        nodesArena.Init(size, false);

        valuesArena = {};
        valuesArena.Init(size, false);


        if (ReadFile(&sourceArena, filePath, &source) == false)
        {
            printf("Failed to load %s file.\n", filePath);
            return false;
        }

        this->filePath = filePath;
        initialized = true;

        result = true;
    }

    return result;
}

void JsonParser::Free() {
    if (initialized) {
        nodesArena.Free();
        sourceArena.Free();
        valuesArena.Free();
    }
    *this = {};
}

MemorySize JsonParser::GetFileSize(const char *filePath)
{
    MemorySize result = 0;
    FILE* file = NULL;

    if(!fopen_s(&file, filePath, "rb"))
    {
        fseek(file, 0, SEEK_END);
        result = ftell(file);
        fclose(file);

        return result;
    }

    return result;
}

bool32 JsonParser::ReadFile(MemoryArena* arena, const char *filePath, char** buffer)
{
    char* result = NULL;
    FILE* file = NULL;
    if(!fopen_s(&file, filePath, "rb"))
    {
        fseek(file, 0, SEEK_END);
        MemorySize fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        result = (char*)ArenaPushSize(arena, fileSize + 1);

        fread(result, fileSize, 1, file);
        result[fileSize] = 0;
        fclose(file);

        *buffer = result;
        return true;
    }

    return false;
}

AstNode * JsonParser::AddNode(MemoryArena* arena, AstType type, AstNode* parent, uint32 level = 0)
{
    AstNode *node = ArenaPushStruct(arena, AstNode);
    *node = {};

    node->type = type;
    node->parent = parent;
    node->level = level;

    return node;
}

AstNode* JsonParser::ParseValue(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent = NULL, uint32 level = 0)
{
    AstNode* result = NULL;
    JsonToken token = {};
    token = tokenizer->PeekToken();
    switch(token.type) {
        case JsonTokenType::Integer : {
            result = AddNode(arena, AstType::Integer, parent, level);
            token = tokenizer->NextToken(); // Eat Integer token
            result->JsonInt.token = token;
            result->JsonInt.value = atoi(token.text);
            break;
        }
        case JsonTokenType::Float : {
            result = AddNode(arena, AstType::Float, parent, level);
            token = tokenizer->NextToken(); // Eat Float token
            result->JsonFloat.token = token;
            result->JsonFloat.value = (float)atof(token.text);

            break;
        }
        case JsonTokenType::String : {
            result = AddNode(arena, AstType::String, parent, level);
            token = tokenizer->NextToken(); // Eat String token
            result->JsonString.token = token;
            // NOTE: Trying to avoid mem allocation at this stage
            result->JsonString.value = token.text;
            result->JsonString.len = token.len;
            
            break;
        }
        case JsonTokenType::OpenBrace : {
            result = ParseObject(arena, tokenizer, parent, level);
            break;
        }
        case JsonTokenType::OpenBracket : {
            result = ParseArray(arena, tokenizer, parent, level);
            break;
        }
    }
    
    return result;
}

AstNode* JsonParser::ParseObject(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent = NULL, uint32 level = 0)
{
    AstNode* result = NULL;
    
    JsonToken token = {};
    token = tokenizer->PeekToken();
    if (token.type == JsonTokenType::OpenBrace) {
        result = AddNode(arena, AstType::Object, parent, level);
        token = tokenizer->NextToken(); // Eat open brace
        result->JsonObject.open = token;
        
        for (;;) {
            AstNode* field = ParseField(arena, tokenizer, result, level+1);

            if (field) {
                if (result->JsonObject.fields == NULL) {
                    result->JsonObject.fields = field;
                    result->JsonObject.last = field;
                }
                else {
                    field->left = result->JsonObject.last;
                    result->JsonObject.last->right = field;
                    result->JsonObject.last = field;					
                }

                result->JsonObject.count++;
            }
            
            token = tokenizer->PeekToken();
            if (token.type == JsonTokenType::Comma) {
                token = tokenizer->NextToken(); // Eat comma token
            }
            else if (token.type == JsonTokenType::CloseBrace) {
                token = tokenizer->NextToken(); // Eat close brace
                result->JsonObject.close = token;
                break;
            }
            else if (token.type == JsonTokenType::EndOfStream) {
                break;
            }
            else if (!field) {
                token = tokenizer->NextToken(); // Eat unknown token
            }
        }
    } 


    return result;
}

AstNode* JsonParser::ParseArray(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent = NULL, uint32 level = 0)
{
    AstNode* result = NULL;
    
    JsonToken token = {};
    token = tokenizer->PeekToken();
    if (token.type == JsonTokenType::OpenBracket) {
        result = AddNode(arena, AstType::Array, parent, level);
        token = tokenizer->NextToken(); // Eat open bracket
        result->JsonArray.open = token;

        
        for (;;) {
            AstNode* value = ParseValue(arena, tokenizer, result, level + 1);
            if (value) {
                if (result->JsonArray.values == NULL) {
                    result->JsonArray.values = value;
                    result->JsonArray.last = value;
                }
                else {
                    value->left = result->JsonArray.last;
                    result->JsonArray.last->right = value;
                    result->JsonArray.last = value;					
                }

                result->JsonArray.count++;
            }

            token = tokenizer->PeekToken();
            if (token.type == JsonTokenType::Comma) {
                token = tokenizer->NextToken(); // Eat comma token
            }
            else if (token.type == JsonTokenType::CloseBracket) {
                token = tokenizer->NextToken(); // Eat close bracket
                result->JsonArray.close = token;
                break;
            }
            else if (token.type == JsonTokenType::EndOfStream) {
                break;
            }
            else if (!value) {
                token = tokenizer->NextToken(); // Eat unknown token
            }
        }
    } 

    return result;
}

AstNode* JsonParser::ParseField(MemoryArena* arena, JsonTokenizer* tokenizer, AstNode* parent = NULL, uint32 level = 0) 
{
    AstNode* result = NULL;
    
    JsonToken token = {};
    token = tokenizer->PeekToken();

    switch(token.type) {
        case JsonTokenType::String : {
            tokenizer->NextToken(); // Eat string token

            JsonToken next = tokenizer->PeekToken();
            if (next.type == JsonTokenType::Colon){
                
                result = AddNode(arena, AstType::Field, parent, level);
                result->JsonField.key = AddNode(arena, AstType::Key, result, level); 
                result->JsonField.key->JsonKey.token = token;
                result->JsonField.key->JsonKey.name = token.text;
                result->JsonField.key->JsonKey.len = token.len;

                tokenizer->NextToken(); // Eat colon token
                
                result->JsonField.value  =  ParseValue(arena, tokenizer, result, level);
                
                next = tokenizer->PeekToken();
                if (next.type == JsonTokenType::Comma) {
                    tokenizer->NextToken(); // Eat comma token
                }
            }
            
            break;
        }
        case JsonTokenType::OpenBrace : {
            result = ParseObject(arena, tokenizer, parent, level);
            break;
        }
        case JsonTokenType::OpenBracket :{
            result = ParseArray(arena, tokenizer, parent, level);
            break;
        }

    }

    return result;
}

bool32 JsonParser::IsJsonFileExt(char *filePath)
{
    char* ext = ".json";
    uint32 len = strlen(ext);

    char *fileExt = strrchr(filePath, '.');

    if (strlen(fileExt) != len)
        return false;

    for (uint32 i = 0; i < len; i++) {
        char c = fileExt[i];
        if (c >= 'A' && c <= 'Z')
            c = 'a' + (c - 'A');
        
        if (c != ext[i]) 
            return false;
    }
    return true;
}



JValue* JsonParser::Parse()
{
    JValue* result = NULL;

    if (initialized)
    {
        JsonTokenizer tokenizer = {};
        tokenizer.Init(source);

        rootNode = ParseField(&nodesArena, &tokenizer);
        //rootNode->Print();

        root = rootNode->Resolve(&valuesArena);
        return root;
    }

    return result;
}

} // namespace
