// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QJSONTOKEN_H)
#include  "../QuarksDD.h"
namespace QuarksDD {
    enum class JsonTokenType {
        Unknown,
        Hash,			// #
        NewLineCRLF,	// /r/n 
        NewLineLF,		// /n
        Tab,			// /t
        Space,			// ' ' 
        CarriageReturn,	// /r
        Dot,			// .
        Comma,			// ,
        Colon,			// :
        Semicolon,		// ;	
        Asterisk,		// *
        Equals,			// =
        OpenParen,		// (
        CloseParen,		// )
        OpenBracket,	// [
        CloseBracket,	// ]
        OpenBrace,		// {
        CloseBrace,		// }
        Comment,		// // or /* */
        String,			// "..."
        Integer,		// 1 ...
        Float,			// 1.0 ...
        Identifier,		// <Identifier>	
        EndOfStream		// \0
    };

    // TODO(dainis): Add intern string support!!!
    struct JsonToken
    {
        JsonTokenType type;
        char* text;
        uint32 len;
    };
} // namespace

#define QJSONTOKEN_H
#endif