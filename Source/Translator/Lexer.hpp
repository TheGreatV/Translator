#include "Header.hpp"


namespace Translator
{
	class Lexer;

	class Token;
	namespace Tokens
	{
		class Identifier;		// abc123 !@#
		class Literal;
		namespace Literals
		{
			class Boolean;		// 'true' 'false'
			class Integer;		// '101b' 'A5h' '42'
			class Floating;		// '1.0' '10e5' '1.4e-5F' '1.0f'
			class String;		// "lol"
		};
		class Keyword;
		class Brace;
		class Special;
	}

	using TokensVector = Vector<Reference<const Token>>;


	class Lexer
	{
	public:
		using Glyph = String::value_type;
		using Glyphs = String;
	protected:
		static bool IsWhitespace(Glyph c);
		static bool IsDigit(Glyph c);
		static bool IsBinary(Glyph c);
		static bool IsOctal(Glyph c);
		static bool IsHexadecimal(Glyph c);
		static bool IsLetter(Glyph c);
		static bool IsSymbolicGroup(Glyph c);
		static bool IsOperatorGroup(Glyph c);
		static bool IsBrace(Glyph c);
		static bool IsSpecial(Glyph c);
	public:
		inline Lexer() = default;
		inline Lexer(const Lexer&) = delete;
		inline ~Lexer() = default;
		inline Lexer& operator = (const Lexer&) = delete;
	protected:
		void SkipComments(const Glyphs& glyphs_, Size& i);
	public:
		TokensVector Parse(const Glyphs& glyphs_);
	};

	class Token
	{
	public:
		enum class Type
		{
			None,
			Identifier,
			Literal,
			Keyword,
			Brace,
			Special
		};
	public:
		Token() = default;
		Token(const Token&) = default;
		virtual ~Token() = default;
		Token& operator = (const Token&) = default;
	public:
		virtual Token::Type GetTokenType() const = 0;
	public:
		virtual operator String() const = 0;
	};
	namespace Tokens
	{
		class Identifier:
			public Token,
			public Named
		{
		public:
			inline Identifier(const Name& name_):
				Named(name_)
			{
			}
			inline Identifier(const Identifier&) = default;
			virtual ~Identifier() override = default;
			Identifier& operator = (const Identifier&) = default;
		public:
			virtual operator String() const override
			{
				return GetName();
			}
		public:
			virtual Token::Type GetTokenType() const override
			{
				return Token::Type::Identifier;
			}
		};
		class Literal:
			public Token
		{
		public:
			enum class Type
			{
				Boolean,
				Integer,
				Floating,
				String
			};
		public:
			inline Literal() = default;
			inline Literal(const Literal&) = default;
			virtual ~Literal() override = default;
			Literal& operator = (const Literal&) = default;
		public:
			virtual Token::Type GetTokenType() const override
			{
				return Token::Type::Literal;
			}
			virtual Literal::Type GetLiteralType() const = 0;
		};
		namespace Literals
		{
			class Boolean:
				public Literal
			{
			public:
				using Value = bool;
			public:
				const Value value;
			public:
				inline Boolean() = delete;
				inline Boolean(const Value& value_):
					value(value_)
				{
				}
				inline Boolean(const Boolean&) = default;
				virtual ~Boolean() override = default;
				inline Boolean& operator = (const Boolean&) = delete;
			public:
				virtual operator Translator::String() const override
				{
					return value ? "true" : "false";
				}
			public:
				virtual Literal::Type GetLiteralType() const override
				{
					return Literal::Type::Boolean;
				}
			};
			class Integer:
				public Literal
			{
			public:
				using Value = int;
			public:
				const Value value;
			public:
				inline Integer() = delete;
				inline Integer(const Value& value_):
					value(value_)
				{
				}
				inline Integer(const Integer&) = default;
				virtual ~Integer() override = default;
				inline Integer& operator = (const Integer&) = delete;
			public:
				virtual operator Translator::String() const override
				{
					return "'" + std::to_string(value) + "'";
				}
			public:
				virtual Literal::Type GetLiteralType() const
				{
					return Literal::Type::Integer;
				}
			};
			class Floating:
				public Literal
			{
			public:
				const float value;
			public:
				inline Floating() = delete;
				inline Floating(float value_):
					value(value_)
				{
				}
				inline Floating(const Floating&) = default;
				virtual ~Floating() override = default;
				inline Floating& operator = (const Floating&) = delete;
			public:
				virtual operator Translator::String() const override
				{
					return "'" + std::to_string(value) + "'";
				}
			public:
				virtual Literal::Type GetLiteralType() const
				{
					return Literal::Type::Floating;
				}
			};
			class String:
				public Literal
			{
			public:
				using Value = Translator::String;
			public:
				const Value value;
			public:
				inline String() = delete;
				inline String(const Value& value_):
					value(value_)
				{
				}
				inline String(const String&) = default;
				virtual ~String() override = default;
				inline String& operator = (const String&) = delete;
			public:
				virtual operator Translator::String() const override
				{
					return "\"" + value + "\"";
				}
			public:
				virtual Literal::Type GetLiteralType() const override
				{
					return Literal::Type::String;
				}
			};
		}
		class Keyword:
			public Token
		{
		public:
			enum class Type
			{
				None,
			};
		public:
			const Keyword::Type type;
		public:
			inline Keyword() = delete;
			inline Keyword(Keyword::Type type_):
				type(type_)
			{
			}
			inline Keyword(const Keyword&) = default;
			virtual ~Keyword() override = default;
			inline Keyword& operator = (const Keyword&) = delete;
		public:
			virtual operator String() const override
			{
				switch(type)
				{
					// case Keyword::Type::None: return "None";
					default: throw std::exception("Unknown keyword type");
				}
			}
		public:
			virtual Token::Type GetTokenType() const override
			{
				return Token::Type::Keyword;
			}
		};
		class Brace:
			public Token
		{
		public:
			enum class Type
			{
				Round,
				Figure,
				Square
			};
			enum class Position
			{
				Begin,
				End
			};
		public:
			const Brace::Type type;
			const Brace::Position position;
		public:
			inline Brace() = delete;
			inline Brace(const Brace::Type& type_, const Brace::Position& position_):
				type(type_),
				position(position_)
			{
			}
			inline Brace(const Brace&) = default;
			virtual ~Brace() override = default;
			inline Brace& operator = (const Brace&) = delete;
		protected:
			// TokenVector::iterator SearchEnd(TokenVector& tokens_, TokenVector::iterator it_);
		public:
			virtual Token::Type GetTokenType() const override
			{
				return Token::Type::Brace;
			}
		public:
			virtual operator String() const override
			{
				switch(type)
				{
					case Brace::Type::Round: switch(position) {
						case Brace::Position::Begin: return "(";
						case Brace::Position::End: return ")";
						default: throw std::exception("Unknown brace position");
					} break;
					case Brace::Type::Figure: switch(position) {
						case Brace::Position::Begin: return "{";
						case Brace::Position::End: return "}";
						default: throw std::exception("Unknown brace position");
					} break;
					case Brace::Type::Square: switch(position) {
						case Brace::Position::Begin: return "[";
						case Brace::Position::End: return "]";
						default: throw std::exception("Unknown brace position");
					} break;
					default: throw std::exception("Unknown brace type");
				}
			}
		};
		class Special:
			public Token
		{
		public:
			enum class Type
			{
				None,
				Dot,
				Comma,
				Colon,
				Semicolon
			};
		public:
			const Type type;
		public:
			inline Special() = delete;
			inline Special(Special::Type type_):
				type(type_)
			{
			}
			inline Special(const Special&) = default;
			virtual ~Special() override = default;
			inline Special& operator = (const Special&) = delete;
		public:
			virtual operator String() const override
			{
				switch(type)
				{
					case Special::Type::Dot: return ".";
					case Special::Type::Comma: return ",";
					case Special::Type::Colon: return ":";
					case Special::Type::Semicolon: return ";";
					default: throw std::exception("Unknown special type");
				}
			}
		public:
			virtual Token::Type GetTokenType() const override
			{
				return Token::Type::Special;
			}
		};
	}
}




