#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <Translator/Lexer.hpp>
using namespace Translator;

namespace LexerTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(EmptyTesting)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse(""));
			{
				Assert::IsTrue(tokens.size() == 0, L"Check that empty input generate no tokens");
			}
		}
		TEST_METHOD(LineCommentTesting)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse("`/"));
			{
				Assert::IsTrue(tokens.size() == 0, L"1");
			}

			tokens = Move(lexer.Parse("`/ vasya"));
			{
				Assert::IsTrue(tokens.size() == 0, L"2");
			}

			tokens = Move(lexer.Parse("`/\n"));
			{
				Assert::IsTrue(tokens.size() == 0, L"3");
			}

			tokens = Move(lexer.Parse("`/ vasya \n"));
			{
				Assert::IsTrue(tokens.size() == 0, L"4");
			}
		}
		TEST_METHOD(BlockCommentTesting)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse("`{}`"));
			{
				Assert::IsTrue(tokens.size() == 0, L"1");
			}

			tokens = Move(lexer.Parse("`{ vasya }`"));
			{
				Assert::IsTrue(tokens.size() == 0, L"2");
			}

			tokens = Move(lexer.Parse("`{`{vasya}`}`"));
			{
				Assert::IsTrue(tokens.size() == 0, L"3");
			}
		}
	};

	TEST_CLASS(KeywordTesting)
	{
	public:
		TEST_METHOD(Schema)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse(".schema"));
			{
				Assert::IsTrue(tokens.size() == 1, L"1");

				auto keyword = UpCast<Tokens::Keyword>(tokens[0]);
				Assert::IsTrue(keyword != nullptr, L"2");

				Assert::IsTrue(keyword->type == Tokens::Keyword::Type::Schema, L"3");
			}
		}
		TEST_METHOD(Algorithm)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse(".algorithm"));
			{
				Assert::IsTrue(tokens.size() == 1, L"1");

				auto keyword = UpCast<Tokens::Keyword>(tokens[0]);
				Assert::IsTrue(keyword != nullptr, L"2");

				Assert::IsTrue(keyword->type == Tokens::Keyword::Type::Algorithm, L"3");
			}
		}
		TEST_METHOD(Body)
		{
			Lexer lexer;
			TokensVector tokens;

			tokens = Move(lexer.Parse(".body"));
			{
				Assert::IsTrue(tokens.size() == 1, L"1");

				auto keyword = UpCast<Tokens::Keyword>(tokens[0]);
				Assert::IsTrue(keyword != nullptr, L"2");

				Assert::IsTrue(keyword->type == Tokens::Keyword::Type::Body, L"3");
			}
		}
	};
}