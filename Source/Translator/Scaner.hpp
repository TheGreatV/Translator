#pragma once


#include "Header.hpp"
#include "Lexer.hpp"


namespace Translator
{
	class Marker;
	using Markers = Vector<Reference<Marker>>;

	namespace Schematic
	{
		class Marker;

		class Token;

		class Unit;
		class Scope;

		class Schema;

		class Algorithm;


		class Scaner;
	}

	namespace Algorithmic
	{
		class Marker;

		class Token;

		class Unit;
		class Scope;

		class Schema;

		class AlgorithmScope;
		class Algorithm;
		namespace Algorithms
		{
			class Bare;
		}


		class Scaner;
	}


	class Marker:
		public This<Marker>
	{
	protected:
		inline Marker(const Reference<Marker>& this_);
	public:
		virtual ~Marker() = default;
	};

	namespace Schematic
	{
		class Marker:
			public Translator::Marker
		{
		protected:
			inline Marker(const Reference<Marker>& this_);
		public:
			virtual ~Marker() override = default;
		};

		class Token:
			public Marker
		{
		public:
			using Source = const Translator::Token;
		protected:
			const Reference<Source> source;
		public:
			inline Token(const Reference<Token>& this_, const Reference<Source>& source_);
			virtual ~Token() override = default;
		public:
			inline const Reference<Source> GetSource() const;
		};
		class Contentable
		{
		public:
			using Content = Vector<Reference<Marker>>;
		protected:
			Content content;
		protected:
			inline Contentable() = default;
		public:
			inline ~Contentable() = default;
		public:
			inline void Add(const Reference<Marker>& marker_);
		};

		class Unit:
			public Marker
		{
		protected:
			const Link<Scope> scope;
		protected:
			inline Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			virtual ~Unit() override = default;
		public:
			inline Reference<Scope> GetScope() const;
		};
		class Scope:
			public Unit
		{
		public:
			using Name = String;
			using Units = Dictionary<Name, Reference<Unit>>;
		protected:
			class Namer;
		protected:
			const Pointer<Namer> namer = Move(MakePointer<Namer>());
			Units units;
		protected:
			friend Scaner;
			template<class Type, class...Arguments> friend Reference<Type> Translator::Make(Arguments&&...arguments_);
			inline Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			virtual ~Scope() override = default;
		public:
			inline void Add(const Name& name_, const Reference<Unit>& unit_);
			inline Reference<Unit> GetOwned(const Name& name_) const;
			inline const Units& GetUnits() const;
			virtual void ResetNaming();
			inline Name GenerateName();
		};
		class Scope::Namer
		{
		protected:
			Size index = 0;
		public:
			inline void Reset();
			inline Name Generate();
		};

		class Schema:
			public Scope
		{
		public:
			using Index = Size;
			using Algorithms = Dictionary<Index, Reference<Algorithm>>;
		protected:
			class Indexer;
		protected:
			const Pointer<Indexer> indexer = Move(MakePointer<Indexer>());
			Algorithms algorithms;
		public:
			inline Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			inline void Add(const Index& index_, const Reference<Algorithm>& algorithm_);
			inline Reference<Algorithm> GetOwned(const Index& index_) const;
			inline const Algorithms& GetAlgorithms() const;
			virtual void ResetNaming() override;
			inline Index GenerateIndex();
		};
		class Schema::Indexer
		{
		protected:
			Index index = 0;
		public:
			inline void Reset();
			inline Index Generate();
		};

		class Algorithm:
			public Scope,
			public Contentable
		{
		protected:
			const Link<Schema> schema;
			const Reference<Marker> resultSchema; // in case of Token reference can have no owner, so we should use Reference<> instead of Link<>
		public:
			inline Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Link<Marker>& resultSchema_);
		public:
			inline Reference<Schema> GetSchema() const;
			inline Reference<Marker> GetResultSchema() const;
		};


		class Scaner
		{
		protected:
			Reference<Token> Scan_Token(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);

			Reference<Marker> Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);
			Reference<Schema> Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);
			Reference<Marker> Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& schema_);

			Reference<Algorithm> Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_);
			Reference<Algorithm> Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_);

			bool Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_);
			Reference<Marker> Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_);
		public:
			Reference<Scope> Scan(const TokensVector& tokens_);
		};
	}

	namespace Algorithmic
	{
		class Marker:
			public Translator::Marker
		{
		protected:
			inline Marker(const Reference<Marker>& this_);
		public:
			virtual ~Marker() override = default;
		};

		class Token:
			public Marker
		{
		public:
			using Source = const Translator::Token;
		protected:
			const Reference<Source> source;
		public:
			inline Token(const Reference<Token>& this_, const Reference<Source>& source_);
			virtual ~Token() override = default;
		public:
			inline const Reference<Source> GetSource() const;
		};
		class Contentable
		{
		public:
			using Content = Vector<Reference<Marker>>;
		protected:
			Content content;
		protected:
			inline Contentable() = default;
		public:
			inline ~Contentable() = default;
		public:
			inline void Add(const Reference<Marker>& marker_);
		};

		class Unit:
			public Marker
		{
		protected:
			const Link<Scope> scope;
		protected:
			inline Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			virtual ~Unit() override = default;
		public:
			inline Reference<Scope> GetScope() const;
		};
		class Scope:
			public Unit
		{
		public:
			using Name = String;
			using Units = Dictionary<Name, Reference<Unit>>;
		protected:
			Units units;
		protected:
			friend Scaner;
			template<class Type, class...Arguments> friend Reference<Type> Translator::Make(Arguments&&...arguments_);
			inline Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			virtual ~Scope() override = default;
		public:
			inline void Add(const Name& name_, const Reference<Unit>& unit_);
			inline Reference<Unit> GetOwned(const Name& name_) const;
			inline Reference<Unit> Get(const Name& name_) const;
			inline const Units& GetUnits() const;
		};

		class Schema:
			public Scope
		{
		public:
			using AlgorithmScopes = Vector<Reference<AlgorithmScope>>;
			struct BareKey
			{
				const Reference<Schema> resultSchema;
				inline bool operator < (const BareKey& source_) const
				{
					return resultSchema.get() < source_.resultSchema.get();
				}
			};
			using BareAlgorithms = Dictionary<BareKey, Reference<Algorithms::Bare>>;
		protected:
			AlgorithmScopes algorithmScopes;
			BareAlgorithms bareAlgorithms;
		public:
			inline Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_ = nullptr);
		public:
			inline void Add(const Reference<AlgorithmScope>& algorithmScope_);
			inline const AlgorithmScopes& GetAlgorithmScopes() const;
			// inline void Add(const Index& index_, const Reference<Algorithm>& algorithm_);
			// inline Reference<Algorithm> GetOwned(const Index& index_) const;
			// inline const Algorithms& GetAlgorithms() const;
		public:
			inline void Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_);
			inline const BareAlgorithms& GetBareAlgorithms() const;
		};

		class AlgorithmScope:
			public Scope
		{
		public:
			inline AlgorithmScope(const Reference<AlgorithmScope>& this_, const Reference<Schema>& schema_ = nullptr);
		};
		class Algorithm:
			public Marker,
			public Contentable
		{
		protected:
			const Link<Schema> schema;
			const Link<Schema> resultSchema;
		public:
			const Reference<AlgorithmScope> algorithmScope; // TODO: protected + getter
		public:
			inline Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Link<Schema>& resultSchema_, const Reference<AlgorithmScope>& algorithmScope_);
		public:
			inline Reference<Schema> GetSchema() const;
			inline Reference<Schema> GetResultSchema() const;
		};
		namespace Algorithms
		{
			class Bare:
				public Algorithm
			{
			public:
				inline Bare(const Reference<Bare>& this_, const Reference<Schema>& schema_, const Link<Schema>& resultSchema_, const Reference<AlgorithmScope>& algorithmScope_);
			};
		}


		class Scaner
		{
		protected:
			Dictionary<Reference<Schema>, Reference<Schematic::Schema>> schemasTable;
			Dictionary<Reference<Schematic::Schema>, Reference<Schema>> schemasTableInverse;
			Dictionary<Reference<AlgorithmScope>, Reference<Schematic::Algorithm>> algorithmScopesTable;
			Dictionary<Reference<Algorithm>, Reference<AlgorithmScope>> algorithmsTable;
		protected:
			void Prepare(const Reference<Scope>& scope_, const Reference<Schematic::Scope>& schematicScope_);
			Reference<Unit> Prepare(const Reference<Schematic::Unit>& schematicUnit_, const Reference<Scope>& parentScope_);
			Reference<Schema> Prepare(const Reference<Schematic::Schema>& schematicSchema_, const Reference<Scope>& parentScope_);
			Reference<AlgorithmScope> Prepare(const Reference<Schematic::Algorithm>& schematicAlgorithm_, const Reference<Schema>& parentSchema_);
		protected:
			void Scan(const Reference<Unit>& unit_);
			void Scan(const Reference<Scope>& scope_);
			void Scan(const Reference<Schema>& schema_);
			void Scan(const Reference<AlgorithmScope>& algorithmScope_, const Reference<Schema>& schema_);
		// 	Reference<Token> Scan_Token(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);
		// 
		// 	Reference<Marker> Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);
		// 	Reference<Schema> Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_);
		// 	Reference<Marker> Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& schema_);
		// 
		// 	Reference<Algorithm> Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_);
		// 	Reference<Algorithm> Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_);
		// 
		// 	bool Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_);
		// 	Reference<Marker> Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_);
		public:
			Reference<Scope> Scan(const Reference<Schematic::Scope>& schematicScope_);
		};
	}
}


#pragma region Translator

#pragma region Marker

inline Translator::Marker::Marker(const Reference<Marker>& this_):
	This(this_)
{
}

#pragma endregion

#pragma region Schematic

#pragma region Marker

inline Translator::Schematic::Marker::Marker(const Reference<Marker>& this_):
	Translator::Marker(this_)
{
}

#pragma endregion

#pragma region Token

inline Translator::Schematic::Token::Token(const Reference<Token>& this_, const Reference<Source>& source_):
	Marker(this_),
	source(source_)
{
}

inline const Translator::Reference<Translator::Schematic::Token::Source> Translator::Schematic::Token::GetSource() const
{
	return source;
}

#pragma endregion

#pragma region Contentable

inline void Translator::Schematic::Contentable::Add(const Reference<Marker>& marker_)
{
	content.push_back(marker_);
}

#pragma endregion

#pragma region Unit

inline Translator::Schematic::Unit::Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_):
	Marker(this_),
	scope(scope_)
{
}

inline Translator::Reference<Translator::Schematic::Scope> Translator::Schematic::Unit::GetScope() const
{
	return MakeReference(scope);
}

#pragma endregion

#pragma region Scope

#pragma region Namer

inline void Translator::Schematic::Scope::Namer::Reset()
{
	index = 0;
}
inline Translator::Schematic::Scope::Name Translator::Schematic::Scope::Namer::Generate()
{
	return "<" + std::to_string(index++) + ">";
}

#pragma endregion


inline Translator::Schematic::Scope::Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_):
	Unit(this_, scope_)
{
}

inline void Translator::Schematic::Scope::Add(const Name& name_, const Reference<Unit>& unit_)
{
	if(units.find(name_) == units.end())
	{
		units[name_] = unit_;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Schematic::Unit> Translator::Schematic::Scope::GetOwned(const Name& name_) const
{
	auto it = units.find(name_);
	if(it != units.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception();
	}
}
inline const Translator::Schematic::Scope::Units& Translator::Schematic::Scope::GetUnits() const
{
	return units;
}
inline Translator::Schematic::Scope::Name Translator::Schematic::Scope::GenerateName()
{
	return namer->Generate();
}

#pragma endregion

#pragma region Schema

#pragma region Indexer

inline void Translator::Schematic::Schema::Indexer::Reset()
{
	index = 0;
}
inline Translator::Schematic::Schema::Index Translator::Schematic::Schema::Indexer::Generate()
{
	return index++;
}

#pragma endregion


inline Translator::Schematic::Schema::Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_):
	Scope(this_, scope_)
{
}

inline void Translator::Schematic::Schema::Add(const Index& index_, const Reference<Algorithm>& algorithm_)
{
	if(algorithms.find(index_) == algorithms.end())
	{
		algorithms[index_] = algorithm_;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Schematic::Algorithm> Translator::Schematic::Schema::GetOwned(const Index& index_) const
{
	auto it = algorithms.find(index_);
	if(it != algorithms.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception();
	}
}
inline const Translator::Schematic::Schema::Algorithms& Translator::Schematic::Schema::GetAlgorithms() const
{
	return algorithms;
}

inline Translator::Schematic::Schema::Index Translator::Schematic::Schema::GenerateIndex()
{
	return indexer->Generate();
}

#pragma endregion

#pragma region Algorithm

inline Translator::Schematic::Algorithm::Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Link<Marker>& resultSchema_):
	Scope(this_, schema_),
	schema(schema_),
	resultSchema(resultSchema_)
{
}
inline Translator::Reference<Translator::Schematic::Schema> Translator::Schematic::Algorithm::GetSchema() const
{
	return MakeReference(schema);
}
inline Translator::Reference<Translator::Schematic::Marker> Translator::Schematic::Algorithm::GetResultSchema() const
{
	return resultSchema;
}

#pragma endregion

#pragma endregion

#pragma region Algorithmic

#pragma region Marker

inline Translator::Algorithmic::Marker::Marker(const Reference<Marker>& this_):
	Translator::Marker(this_)
{
}

#pragma endregion

#pragma region Token

inline Translator::Algorithmic::Token::Token(const Reference<Token>& this_, const Reference<Source>& source_):
	Marker(this_),
	source(source_)
{
}

inline const Translator::Reference<Translator::Algorithmic::Token::Source> Translator::Algorithmic::Token::GetSource() const
{
	return source;
}

#pragma endregion

#pragma region Contentable

inline void Translator::Algorithmic::Contentable::Add(const Reference<Marker>& marker_)
{
	content.push_back(marker_);
}

#pragma endregion

#pragma region Unit

inline Translator::Algorithmic::Unit::Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_):
	Marker(this_),
	scope(scope_)
{
}

inline Translator::Reference<Translator::Algorithmic::Scope> Translator::Algorithmic::Unit::GetScope() const
{
	return MakeReference(scope);
}

#pragma endregion

#pragma region Scope

inline Translator::Algorithmic::Scope::Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_):
	Unit(this_, scope_)
{
}

inline void Translator::Algorithmic::Scope::Add(const Name& name_, const Reference<Unit>& unit_)
{
	if(units.find(name_) == units.end())
	{
		units[name_] = unit_;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Algorithmic::Unit> Translator::Algorithmic::Scope::GetOwned(const Name& name_) const
{
	auto it = units.find(name_);
	if(it != units.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Algorithmic::Unit> Translator::Algorithmic::Scope::Get(const Name& name_) const
{
	auto it = units.find(name_);
	if(it != units.end())
	{
		return (*it).second;
	}
	else
	{
		if(GetScope())
		{
			return GetScope()->Get(name_);
		}
		else
		{
			throw Exception();
		}
	}
}
inline const Translator::Algorithmic::Scope::Units& Translator::Algorithmic::Scope::GetUnits() const
{
	return units;
}

#pragma endregion

#pragma region Schema

inline Translator::Algorithmic::Schema::Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_):
	Scope(this_, scope_)
{
}

inline void Translator::Algorithmic::Schema::Add(const Reference<AlgorithmScope>& algorithmScope_)
{
	algorithmScopes.push_back(algorithmScope_);
}
inline const Translator::Algorithmic::Schema::AlgorithmScopes& Translator::Algorithmic::Schema::GetAlgorithmScopes() const
{
	return algorithmScopes;
}
inline void Translator::Algorithmic::Schema::Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_)
{
	if(bareAlgorithms.find(key_) == bareAlgorithms.end())
	{
		bareAlgorithms[key_] = bareAlgorithm_;
	}
	else
	{
		throw Exception();
	}
}
inline const Translator::Algorithmic::Schema::BareAlgorithms& Translator::Algorithmic::Schema::GetBareAlgorithms() const
{
	return bareAlgorithms;
}

/*
inline void Translator::Algorithmic::Schema::Add(const Index& index_, const Reference<Algorithm>& algorithm_)
{
	if(algorithms.find(index_) == algorithms.end())
	{
		algorithms[index_] = algorithm_;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Algorithmic::Algorithm> Translator::Algorithmic::Schema::GetOwned(const Index& index_) const
{
	auto it = algorithms.find(index_);
	if(it != algorithms.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception();
	}
}
inline const Translator::Algorithmic::Schema::Algorithms& Translator::Algorithmic::Schema::GetAlgorithms() const
{
	return algorithms;
}
*/

#pragma endregion

#pragma region AlgorithmScope

inline Translator::Algorithmic::AlgorithmScope::AlgorithmScope(const Reference<AlgorithmScope>& this_, const Reference<Schema>& schema_):
	Scope(this_, schema_)
{
}

#pragma endregion

#pragma region Algorithm

inline Translator::Algorithmic::Algorithm::Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Link<Schema>& resultSchema_, const Reference<AlgorithmScope>& algorithmScope_):
	Marker(this_),
	schema(schema_),
	resultSchema(resultSchema_),
	algorithmScope(algorithmScope_)
{
}
inline Translator::Reference<Translator::Algorithmic::Schema> Translator::Algorithmic::Algorithm::GetSchema() const
{
	return MakeReference(schema);
}
inline Translator::Reference<Translator::Algorithmic::Schema> Translator::Algorithmic::Algorithm::GetResultSchema() const
{
	return MakeReference(resultSchema);
}

#pragma endregion

#pragma region Algorithms

#pragma region Bare

inline Translator::Algorithmic::Algorithms::Bare::Bare(const Reference<Bare>& this_, const Reference<Schema>& schema_, const Link<Schema>& resultSchema_, const Reference<AlgorithmScope>& algorithmScope_):
	Algorithm(this_, schema_, resultSchema_, algorithmScope_)
{
}

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion


