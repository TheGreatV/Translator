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
			const Reference<AlgorithmScope> algorithmScope;
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


	/*namespace Functional
	{
		class Marker;

		namespace Markers
		{
			class Unit;
			class Scope;

			class Schema;

			class AlgorithmStub;
			class Algorithm;
			namespace Algorithms
			{
				class Bare;
			}
		}

		class Scaner;
	}*/
	/*namespace Structure
	{
		namespace Markers
		{
			class Algorithm:
				public Scope
			{
			public:
				inline Algorithm(const Reference<Algorithm>& this_);
			};
		}

		class Scaner
		{
		protected:
			bool Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_);
			bool Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_);
			bool Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_);

			bool Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_);
			bool Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_);

			bool Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_);
			bool Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_);
		public:
			Reference<Markers::Scope> Scan(const TokensVector& tokens_);
		};
	}*/
	/*namespace Functional
	{
		class Marker:
			public Translator::Marker
		{
		public:
			inline Marker(const Reference<Marker>& this_);
		};

		namespace Markers
		{
			class Unit:
				public Marker
			{
			protected:
				const Reference<Scope> scope;
			public:
				inline Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_);
				virtual ~Unit() override = default;
			};
			class Scope:
				public Unit
			{
			public:
				using Name = String;
				using Units = Dictionary<Name, Reference<Unit>>;
			protected:
				Units units;
			public:
				inline Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_);
				virtual ~Scope() override = default;
			public:
				inline void Add(const Name& name_, const Reference<Unit>& unit_);
				inline bool Is(const Name& name_) const;
				inline Reference<Unit> GetOwned(const Name& name_) const;
				inline Reference<Unit> Get(const Name& name_) const;
				inline const Units& GetUnits() const;
			};
		
			class Schema:
				public Scope
			{
			public:
				using Index = Size;
				using AlgorithmStubs = Dictionary<Index, Reference<AlgorithmStub>>;
			public:
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
				AlgorithmStubs algorithmStubs;
				BareAlgorithms bareAlgorithms;
			public:
				inline Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_);
			public:
				inline void Add(const Index& index_, const Reference<AlgorithmStub>& algorithmStub_);
				inline Reference<AlgorithmStub> GetOwned(const Index& index_) const;
				inline const BareAlgorithms& GetBareAlgorithms() const;
			public:
				inline void Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_);
			};

			class AlgorithmStub:
				public Scope
			{
			public:
				inline AlgorithmStub(const Reference<AlgorithmStub>& this_);
			};
			class Algorithm:
				public Scope
			{
			protected:
				const Reference<Schema> schema;
				const Reference<Schema> resultSchema;
			public:
				inline Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<AlgorithmStub>& algorithmStub_, const Reference<Schema> resultSchema_);
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
					inline Bare(const Reference<Bare>& this_, const Reference<Schema>& schema_, const Reference<AlgorithmStub>& algorithmStub_, const Reference<Schema> resultSchema_);
				};
			}
		}

		class Scaner
		{
		protected:
			void Prepare(const Reference<Structure::Markers::Scope>& structureScope_, const Reference<Markers::Scope>& scope_);
			Reference<Markers::Unit> Prepare(const Reference<Structure::Markers::Unit>& structureUnit_, const Reference<Markers::Scope>& scope_);
			Reference<Markers::Schema> Prepare(const Reference<Structure::Markers::Schema>& structureSchema_, const Reference<Markers::Scope>& scope_);
			void Prepare(const Reference<Structure::Markers::Algorithm>& structureAlgorithm_, const Reference<Markers::AlgorithmStub>& algorithmStub_);
		protected:
			Reference<Markers::Schema> Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_);
			Reference<Markers::Schema> Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_);
			bool Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_);
			
			Reference<Markers::Algorithm> Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_);
			Reference<Markers::Algorithm> Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_);
			
			bool Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_, const Reference<Structure::Markers::Algorithm>& structureAlgorithm_);
			bool Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_, const Reference<Structure::Markers::Algorithm>& structureAlgorithm_);
		public:
			Reference<Markers::Scope> Scan(const TokensVector& tokens_, const Reference<Structure::Markers::Scope>& structureScope_);
		};
	}
	*/
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

/*#pragma region Functional

#pragma region Marker

inline Translator::Functional::Marker::Marker(const Reference<Marker>& this_):
	Translator::Marker(this_)
{
}

#pragma endregion

#pragma region Markers

#pragma region Unit

inline Translator::Functional::Markers::Unit::Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_):
	Marker(this_),
	scope(scope_)
{
}

#pragma endregion

#pragma region Scope

inline Translator::Functional::Markers::Scope::Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_):
	Unit(this_, scope_)
{
}

inline void Translator::Functional::Markers::Scope::Add(const Name& name_, const Reference<Unit>& unit_)
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
inline bool Translator::Functional::Markers::Scope::Is(const Name& name_) const
{
	auto it = units.find(name_);
	return it != units.end();
}
inline Translator::Reference<Translator::Functional::Markers::Unit> Translator::Functional::Markers::Scope::GetOwned(const Name& name_) const
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
inline Translator::Reference<Translator::Functional::Markers::Unit> Translator::Functional::Markers::Scope::Get(const Name& name_) const
{
	return Is(name_) ?
		GetOwned(name_) :
		scope ? scope->Get(name_) : throw Exception();
}
inline const Translator::Functional::Markers::Scope::Units& Translator::Functional::Markers::Scope::GetUnits() const
{
	return units;
}

#pragma endregion

#pragma region Schema

inline Translator::Functional::Markers::Schema::Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_):
	Scope(this_, scope_)
{
}

inline void Translator::Functional::Markers::Schema::Add(const Index& index_, const Reference<AlgorithmStub>& algorithm_)
{
	if(algorithmStubs.find(index_) == algorithmStubs.end())
	{
		algorithmStubs[index_] = algorithm_;
	}
	else
	{
		throw Exception();
	}
}
inline Translator::Reference<Translator::Functional::Markers::AlgorithmStub> Translator::Functional::Markers::Schema::GetOwned(const Index& index_) const
{
	auto it = algorithmStubs.find(index_);
	if(it != algorithmStubs.end())
	{
		return (*it).second;
	}
	else
	{
		throw Exception();
	}
}
inline const Translator::Functional::Markers::Schema::BareAlgorithms& Translator::Functional::Markers::Schema::GetBareAlgorithms() const
{
	return bareAlgorithms;
}

inline void Translator::Functional::Markers::Schema::Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_)
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

#pragma endregion

#pragma region AlgorithmStub

inline Translator::Functional::Markers::AlgorithmStub::AlgorithmStub(const Reference<AlgorithmStub>& this_):
	Scope(this_, nullptr)
{
}

#pragma endregion

#pragma region Algorithm

inline Translator::Functional::Markers::Algorithm::Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<AlgorithmStub>& algorithmStub_, const Reference<Schema> resultSchema_):
	Scope(this_, schema_),
	schema(schema_),
	resultSchema(resultSchema_)
{
	for(auto &node : algorithmStub_->GetUnits())
	{
		auto &name = node.first;
		auto &unit = node.second;

		Add(name, unit);
	}
}
inline Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Markers::Algorithm::GetSchema() const
{
	return schema;
}
inline Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Markers::Algorithm::GetResultSchema() const
{
	return resultSchema;
}

#pragma endregion

#pragma region Algorithms

#pragma region Bare

inline Translator::Functional::Markers::Algorithms::Bare::Bare(const Reference<Bare>& this_, const Reference<Schema>& schema_, const Reference<AlgorithmStub>& algorithmStub_, const Reference<Schema> resultSchema_):
	Algorithm(this_, schema_, algorithmStub_, resultSchema_)
{
}

#pragma endregion

#pragma endregion

#pragma endregion

#pragma endregion
*/

#pragma endregion


#pragma region
#pragma endregion


