
#include "JsonTokenizer.h"

namespace QuarksDD
{

bool32 JsonTokenizer::Init(char *content)
{
	bool32 result = false;

	if (!initialized && content) {
		// IMPORTANT: content must be null terminated char buffer
		curr = content;
		initialized = true;
		result = true; 
	}
	return result;
}

void JsonTokenizer::Free() {
	*this = {};
}

bool32 JsonTokenizer::IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool32 JsonTokenizer::IsDigit(char c)
{
	return (c >= '0' && c <= '9');
}

bool32 JsonTokenizer::IsNewLine(char c)
{
	switch (c)
	{
	case '\r': // carriage return
	case '\n': // new line
		return true;
	default:
		return false;
	}
	return false;
}

bool32 JsonTokenizer::IsWhitespace(char c)
{
	switch (c)
	{
	case '\t': // horrizontal tab
	case '\v': // vertical tab
	case '\f': // form feed - new page
	case ' ':  // space
		return true;
	default:
		return false;
	}
	return false;
}

void JsonTokenizer::EatSpacesCommentsNewlines()
{
	for (;;)
	{
		if (IsNewLine(curr[0])) {
			++curr;
		}
		else if (IsWhitespace(curr[0]))
		{
			++curr;
		}
		else if ((curr[0] == '/') && (curr[1] == '/'))
		{
			curr += 2;
			while (curr[0] && !IsNewLine(curr[0]))
			{
				++curr;
			}
		}
		else if ((curr[0] == '/') && (curr[1] == '*'))
		{
			curr += 2;
			while (curr[0] && !((curr[0] == '*') && (curr[1] == '/')))
			{
				++curr;
			}

			if (curr[0] == '*')
			{
				curr += 2;
			}
		}
		else
		{
			break;
		}
	}
}

JsonToken JsonTokenizer::PeekToken()
{
	JsonTokenizer tokenizer2;
	tokenizer2 = *this;
	return tokenizer2.NextToken();
}

JsonToken JsonTokenizer::NextToken()
{
	// Skip spaces, tabs, comments, newline
	EatSpacesCommentsNewlines();

	JsonToken token = {};
	token.text = curr;
	token.len = 1;
	
	char c = curr[0];
	++curr;

	switch (c)
	{
		case '\0':
		{
			token.type = JsonTokenType::EndOfStream;
			curr--;
			break;
		}
		case '.':
			token.type = JsonTokenType::Dot;
			break;
		case ',':
			token.type = JsonTokenType::Comma;
			break;
		case ':':
			token.type = JsonTokenType::Colon;
			break;
		case ';':
			token.type = JsonTokenType::Semicolon;
			break;
		case '*':
			token.type = JsonTokenType::Asterisk;
			break;
		case '=':
			token.type = JsonTokenType::Equals;
			break;
		case '(':
			token.type = JsonTokenType::OpenParen;
			break;
		case ')':
			token.type = JsonTokenType::CloseParen;
			break;
		case '[':
			token.type = JsonTokenType::OpenBracket;
			break;
		case ']':
			token.type = JsonTokenType::CloseBracket;
			break;
		case '{':
			token.type = JsonTokenType::OpenBrace;
			break;
		case '}':
			token.type = JsonTokenType::CloseBrace;
			break;
		case '"':
		{
			token.type = JsonTokenType::String;
			token.text = curr;
						
			while(curr[0] && curr[0] != '"')
			{
				if((curr[0] == '\\') &&	curr[1])
				{
					++curr;
				}
				++curr;
			}

			token.len = curr - token.text;
			if(curr[0] == '"') {
				++curr;
			}

			break;
		}
		default:
		{
			if (IsAlpha(c))
			{
				token.type = JsonTokenType::Identifier;
				while (IsAlpha(curr[0]) || IsDigit(curr[0])) {
					curr++;
				}
				token.len = curr - token.text;

				break;
			}
			else if (IsDigit(c) || (IsDigit(curr[0]) && (c == '-') || c == '+'))
			{
				token.type = JsonTokenType::Integer;
				while(IsDigit(curr[0]) || curr[0] == '.')
				{
					if (curr[0] == '.') {
						token.type = JsonTokenType::Float;
					}
					++curr;
					token.len = curr - token.text;
				}
			}
			else {
				token.type = JsonTokenType::Unknown;
			}
		}
	}
	
	return token;
}

} // namespace