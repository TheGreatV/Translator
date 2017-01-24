#include "Lexer.hpp"


Translator::Reference<const Translator::Token> Translator::Tokens::RequireToken(const TokensVector& tokens, Translator::TokensVector::const_iterator& it, Translator::Token::Type type)
{
	auto o = it;

	if(it != tokens.end())
	{
		auto &token = *it;

		if(type == Translator::Token::Type::None || token->GetTokenType() == type)
		{
			++it;

			return token;
		}
	}

	it = o;
	return nullptr;
}
Translator::Reference<const Translator::Tokens::Keyword> Translator::Tokens::RequireKeyword(const TokensVector& tokens, Translator::TokensVector::const_iterator& it, Translator::Tokens::Keyword::Type type)
{
	auto o = it;
	auto token = RequireToken(tokens, it, Translator::Token::Type::Keyword);

	if(token)
	{
		auto keyword = UpCast<Tokens::Keyword>(token);
		if(!keyword)
		{
			throw std::exception(); // TODO
		}

		if(type == Translator::Tokens::Keyword::Type::None || keyword->type == type)
		{
			return keyword;
		}
	}

	it = o;
	return nullptr;
}
Translator::Reference<const Translator::Tokens::Identifier> Translator::Tokens::RequireIdentifier(const TokensVector& tokens, Translator::TokensVector::const_iterator& it, std::string name)
{
	auto o = it;
	auto token = RequireToken(tokens, it, Translator::Token::Type::Identifier);

	if(token)
	{
		auto identifier = UpCast<Tokens::Identifier>(token);
		if(!identifier)
		{
			throw std::exception(); // TODO
		}

		if(name.length() == 0 || identifier->GetName() == name)
		{
			return identifier;
		}
	}

	it = o;
	return nullptr;
}
Translator::Reference<const Translator::Tokens::Brace> Translator::Tokens::RequireBrace(const TokensVector& tokens, Translator::TokensVector::const_iterator& it, Translator::Tokens::Brace::Type type, Translator::Tokens::Brace::Position position)
{
	auto o = it;
	auto token = RequireToken(tokens, it, Token::Type::Brace);

	if(token)
	{
		auto brace = UpCast<Tokens::Brace>(token);
		if(!brace)
		{
			throw std::exception(); // TODO
		}

		if(brace->type == type && brace->position == position)
		{
			return brace;
		}
	}

	it = o;
	return nullptr;
}
Translator::Reference<const Translator::Tokens::Special> Translator::Tokens::RequireSpecial(const TokensVector& tokens, Translator::TokensVector::const_iterator& it, Translator::Tokens::Special::Type type)
{
	auto o = it;
	auto token = RequireToken(tokens, it, Translator::Token::Type::Special);

	if(token)
	{
		auto special = UpCast<Tokens::Special>(token);
		if(!special)
		{
			throw std::exception(); // TODO
		}

		if(type == Translator::Tokens::Special::Type::None || type == special->type)
		{
			return special;
		}
	}

	it = o;
	return nullptr;
}


bool Translator::Lexer::IsWhitespace(Glyph c)
{
	return
		c == ' ' ||
		c == '\n' ||
		c == '\t';
}
bool Translator::Lexer::IsDigit(Glyph c)
{
	return
		c == '0' ||
		c == '1' ||
		c == '2' ||
		c == '3' ||
		c == '4' ||
		c == '5' ||
		c == '6' ||
		c == '7' ||
		c == '8' ||
		c == '9';
}
bool Translator::Lexer::IsBinary(Glyph c)
{
	return c == '0' || c == '1';
}
bool Translator::Lexer::IsOctal(Glyph c)
{
	return
		c == '0' ||
		c == '1' ||
		c == '2' ||
		c == '3' ||
		c == '4' ||
		c == '5' ||
		c == '6' ||
		c == '7';
}
bool Translator::Lexer::IsHexadecimal(Glyph c)
{
	return
		IsDigit(c) ||
		c == 'a' || c == 'A' ||
		c == 'b' || c == 'B' ||
		c == 'c' || c == 'C' ||
		c == 'd' || c == 'D' ||
		c == 'e' || c == 'E' ||
		c == 'f' || c == 'F';
}
bool Translator::Lexer::IsLetter(Glyph c)
{
	return //std::
		c == 'a' || c == 'A' ||
		c == 'b' || c == 'B' ||
		c == 'c' || c == 'C' ||
		c == 'd' || c == 'D' ||
		c == 'e' || c == 'E' ||
		c == 'f' || c == 'F' ||
		c == 'g' || c == 'G' ||
		c == 'h' || c == 'H' ||
		c == 'i' || c == 'I' ||
		c == 'j' || c == 'J' ||
		c == 'k' || c == 'K' ||
		c == 'l' || c == 'L' ||
		c == 'm' || c == 'M' ||
		c == 'n' || c == 'N' ||
		c == 'o' || c == 'O' ||
		c == 'p' || c == 'P' ||
		c == 'q' || c == 'Q' ||
		c == 'r' || c == 'R' ||
		c == 's' || c == 'S' ||
		c == 't' || c == 'T' ||
		c == 'u' || c == 'U' ||
		c == 'v' || c == 'V' ||
		c == 'w' || c == 'W' ||
		c == 'x' || c == 'X' ||
		c == 'y' || c == 'Y' ||
		c == 'z' || c == 'Z';
}
bool Translator::Lexer::IsSymbolicGroup(Glyph c)
{
	return IsDigit(c) || IsLetter(c);
}
bool Translator::Lexer::IsOperatorGroup(Glyph c)
{
	return
		c == '~' ||
		c == '!' ||
		c == '@' ||
		c == '#' ||
		c == '$' ||
		c == '%' ||
		c == '^' ||
		c == '&' ||
		c == '*' ||
		c == '-' ||
		c == '=' ||
		c == '+' ||
		c == '\\' ||
		c == '|' ||
		c == '/' ||
		c == '>' ||
		c == '<' ||
		c == '?';
}
bool Translator::Lexer::IsBrace(Glyph c)
{
	return
		c == '(' ||
		c == ')' ||
		c == '{' ||
		c == '}' ||
		c == '[' ||
		c == ']';
}
bool Translator::Lexer::IsSpecial(Glyph c)
{
	return
		c == '.' ||
		c == ',' ||
		c == ':' ||
		c == ';';
}

void Translator::Lexer::SkipComments(const Glyphs& glyphs_, Size& i)
{
	if(i < glyphs_.size())
	{
		if(glyphs_[i] == '`')
		{
			++i;
			if(i < glyphs_.size())
			{
				switch(glyphs_[i])
				{
					case '/': {
						while(i < glyphs_.size() && glyphs_[i] != '\n') ++i;
					} break;
					case '{': {
						while(true)
						{
							++i;
							
							SkipComments(glyphs_, i);
							
							if(i < glyphs_.size())
							{
								if(glyphs_[i] == '}')
								{
									++i;
									if(i < glyphs_.size())
									{
										if(glyphs_[i] == '`')
										{
											++i;
											break;
										}
									}
									else
									{
										throw std::exception("Invalid comment");
									}
								}
							}
							else
							{
								throw std::exception("Invalid comment");
							}
						}
					} break;
					default: throw std::exception("Invalid comment");
				}
			}
			else
			{
				throw std::exception("Invalid comment");
			}
		}
	}
}

Translator::TokensVector Translator::Lexer::Parse(const Glyphs& glyphs_)
{
	TokensVector tokens;

	size_t i = 0;
	while(i < glyphs_.size())
	{
		SkipComments(glyphs_, i);

		if(IsSymbolicGroup(glyphs_[i]))
		{
			auto j = i;
			while(j < glyphs_.size() && IsSymbolicGroup(glyphs_[j])) ++j;
			String identifierName = glyphs_.substr(i, j - i);
			auto token = MakeReference(new Tokens::Identifier(identifierName));
			tokens.push_back(std::move(token));
			i = j;
			continue;
		}
		if(IsOperatorGroup(glyphs_[i]))
		{
			auto j = i;
			while(j < glyphs_.size() && IsOperatorGroup(glyphs_[j])) ++j;
			auto identifierName = glyphs_.substr(i, j - i);
			auto token = MakeReference(new Tokens::Identifier(identifierName));
			tokens.push_back(std::move(token));
			i = j;
			continue;
		}

		switch(glyphs_[i])
		{
			case '\'': ([&]{
				auto j = i + 1;
				while(j < glyphs_.size() && glyphs_[j] != '\'') ++j;
				auto literalSource = glyphs_.substr(i + 1, j - (i + 1));

				([&]{
					auto ParseInteger = [&](const int bit, const String& literalBitSource, bool(*check)(const String&))
					{
						if(!check(literalBitSource)) throw std::exception(String("not " + std::to_string(bit) + "-decimal!").c_str());

						auto literalValue = std::strtol(literalSource.c_str(), nullptr, bit);
						auto literalInteger = MakeReference(new Tokens::Literals::Integer(literalValue));

						tokens.push_back(literalInteger);
					};
					auto ParseIntegerWithPostfix = [&](const int bit, List<char> postfixes, bool(*check)(const String&)) -> bool
					{
						for(auto postfix : postfixes)
						{
							if(literalSource[literalSource.size() - 1] == postfix)
							{
								auto literalBitSource = literalSource.substr(0, literalSource.size() - 1);
								ParseInteger(bit, literalBitSource, check);
								return true;
							}
						}
						return false;
					};
					auto ParseFloating = [&](const String& literalBitSource, bool(*check)(const String&))
					{
						size_t epos;
						if(((epos = literalBitSource.find('e')) != UINT_MAX) || ((epos = literalBitSource.find('E')) != UINT_MAX))
						{
							auto literalBaseSource = literalBitSource.substr(0, epos);
							auto literalExponentSource = literalBitSource.substr(epos + 1, literalBitSource.size() - epos - 1);

							for(auto i : literalExponentSource)
							{
								if(!(IsDigit(i) || (i == '-'))) throw std::exception("invalid exponent!");
							}

							auto literalBaseValue = std::strtof(literalBaseSource.c_str(), nullptr);
							auto literalExponentValue = std::strtol(literalExponentSource.c_str(), nullptr, 10);

							auto literalValue = literalBaseValue * powf(10.0, (float)literalExponentValue);
							auto literalFloating = MakeReference(new Tokens::Literals::Floating(literalValue));

							tokens.push_back(literalFloating);
						}
						else
						{
							if(!check(literalBitSource)) throw std::exception(String("not floating!").c_str());

							auto literalValue = std::strtof(literalSource.c_str(), nullptr);
							auto literalFloating = MakeReference(new Tokens::Literals::Floating(literalValue));

							tokens.push_back(literalFloating);
						}
					};
					auto ParseFloatingWithPostfix = [&](List<char> postfixes, bool(*check)(const String&)) -> bool
					{
						for(auto postfix : postfixes)
						{
							if(literalSource[literalSource.size() - 1] == postfix)
							{
								auto literalBitSource = literalSource.substr(0, literalSource.size() - 1);
								ParseFloating(literalBitSource, check);
								return true;
							}
						}
						return false;
					};

					auto CheckFloating = [](const String& t)
					{
						int dotCount = 0;
						for(auto i : t)
						{
							if(i == '.')
							{
								if(dotCount == 0) dotCount = 1; else return false;
							}
							else
							{
								if(!IsDigit(i)) return false;
							}
						}
						return true;
					};

					if(ParseFloatingWithPostfix({'f', 'F'}, CheckFloating)) return;

					if(ParseIntegerWithPostfix(16, {'h', 'H'}, [](const String& t){
						for(auto i : t) if(!IsHexadecimal(i)) return false;
						return true;
					})) return;

					if(ParseIntegerWithPostfix(10, {'d', 'D'}, [](const String& t){
						for(auto i : t) if(!IsDigit(i)) return false;
						return true;
					})) return;

					if(ParseIntegerWithPostfix(8, {'o', 'O'}, [](const String& t){
						for(auto i : t) if(!IsOctal(i)) return false;
						return true;
					})) return;

					if(ParseIntegerWithPostfix(2, {'b', 'B'}, [](const String& t){
						for(auto i : t) if(!IsBinary(i)) return false;
						return true;
					})) return;

					if((literalSource.find('.') != UINT_MAX) || (literalSource.find('e') != UINT_MAX) || (literalSource.find('E') != UINT_MAX))
					{
						ParseFloating(literalSource, CheckFloating);
						return;
					}

					ParseInteger(10, literalSource, [](const String& t){
						for(auto i : t) if(!IsDigit(i)) return false;
						return true;
					});
				})();

				i = j;
			})(); break;
			case '\"':
			{
				auto j = i + 1;
				while(j < glyphs_.size() && glyphs_[j] != '\"') ++j;
				auto literalSource = glyphs_.substr(i + 1, j - (i + 1));

				auto literalString = MakeReference(new Tokens::Literals::String(literalSource));
				tokens.push_back(literalString);

				i = j;
			} break;
			case '.':
			{
				auto j = i + 1;
				while(j < glyphs_.size() && (IsSymbolicGroup(glyphs_[j]) || IsOperatorGroup(glyphs_[j]))) ++j;

				auto keywordSource = glyphs_.substr(i, j - i);

				([&]{
					/*if(keywordSource == ".true")
					{
						auto literalTrue = new Tokens::Literals::Boolean(true);
						tokens.push_back(literalTrue);
						i = j - 1;
						return;
					}
					if(keywordSource == ".false")
					{
						auto literalTrue = new Tokens::Literals::Boolean(false);
						tokens.push_back(literalTrue);
						i = j - 1;
						return;
					}*/
					/*if(keywordSource == ".block")
					{
						auto keyword = new Tokens::Keyword(Tokens::Keyword::Type::Block);
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".space")
					{
						auto keyword = new Tokens::Keyword(Tokens::Keyword::Type::Space);
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".type")
					{
						auto keyword = new Tokens::Keyword(Tokens::Keyword::Type::Type);
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".make")
					{
						auto keyword = new Tokens::Keyword(Tokens::Keyword::Type::Make);
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".alias")
					{
						auto keyword = new Tokens::Keyword(Tokens::Keyword::Type::Alias);
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}*/
					if(keywordSource == ".schema")
					{
						auto keyword = MakeReference(new Tokens::Keyword(Tokens::Keyword::Type::Schema));
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".algorithm")
					{
						auto keyword = MakeReference(new Tokens::Keyword(Tokens::Keyword::Type::Algorithm));
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".body")
					{
						auto keyword = MakeReference(new Tokens::Keyword(Tokens::Keyword::Type::Body));
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}
					if(keywordSource == ".block")
					{
						auto keyword = MakeReference(new Tokens::Keyword(Tokens::Keyword::Type::Block));
						tokens.push_back(keyword);
						i = j - 1;
						return;
					}

					auto tokenDot = MakeReference(new Tokens::Special(Tokens::Special::Type::Dot));
					tokens.push_back(tokenDot);
				})();
			} break;
			case ',':
			{
				tokens.push_back(MakeReference(new Tokens::Special(Tokens::Special::Type::Comma)));
			} break;
			case ':':
			{
				tokens.push_back(MakeReference(new Tokens::Special(Tokens::Special::Type::Colon)));
			} break;
			case ';':
			{
				tokens.push_back(MakeReference(new Tokens::Special(Tokens::Special::Type::Semicolon)));
			} break;
			case '{':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin)));
			} break;
			case '}':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Figure, Tokens::Brace::Position::End)));
			} break;
			case '(':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Round, Tokens::Brace::Position::Begin)));
			} break;
			case ')':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Round, Tokens::Brace::Position::End)));
			} break;
			case '[':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Square, Tokens::Brace::Position::Begin)));
			} break;
			case ']':
			{
				tokens.push_back(MakeReference(new Tokens::Brace(Tokens::Brace::Type::Square, Tokens::Brace::Position::End)));
			} break;
			default: break;
		}

		++i;
	}
	return Move(tokens);
}


