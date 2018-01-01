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