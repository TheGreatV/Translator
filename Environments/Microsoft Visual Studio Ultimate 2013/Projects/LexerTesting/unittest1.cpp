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
}