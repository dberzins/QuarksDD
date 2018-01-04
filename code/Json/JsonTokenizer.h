// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QJSONTOKENIZER_H)

#include "JsonToken.h"

namespace QuarksDD {

	struct JsonTokenizer
	{
		// Data
		char* curr;
		bool32 initialized;
		
		// Operations
		bool32 Init(char* content);
		void Free();
		
		JsonToken NextToken();
		JsonToken PeekToken();

	private:
		bool32 IsAlpha(char c);
		bool32 IsDigit(char c);
		bool32 IsNewLine(char c);
		bool32 IsWhitespace(char c);
		void EatSpacesCommentsNewlines();
	};

} // namespace

#define QJSONTOKENIZER_H
#endif