#pragma once


#include "Header.hpp"
#include "Lexer.hpp"


namespace Translator
{
	class Marker;

	namespace Structure
	{
		class Marker;

		namespace Markers
		{
			class Unit;
			class Scope;

			class Schema;

			class Algorithm;
		}

		class Scaner;
	}
	namespace Functional
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
	}


	class Marker:
		public This<Marker>
	{
	protected:
		inline Marker(const Reference<Marker>& this_);
	public:
		virtual ~Marker() = default;
	};

	namespace Structure
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
			public:
				inline Unit(const Reference<Unit>& this_);
				virtual ~Unit() override = default;
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
			public:
				inline Scope(const Reference<Scope>& this_);
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
				inline Schema(const Reference<Schema>& this_);
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
	}
	namespace Functional
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
				const Reference<Scope>& scope;
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
				// inline const Algorithms& GetAlgorithms() const;
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
			void Scan(const TokensVector& tokens_, const Reference<Structure::Markers::Scope>& structureScope_);
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

#pragma region Structure

#pragma region Marker

inline Translator::Structure::Marker::Marker(const Reference<Marker>& this_):
	Translator::Marker(this_)
{
}

#pragma endregion

#pragma region Markers

#pragma region Unit

inline Translator::Structure::Markers::Unit::Unit(const Reference<Unit>& this_):
	Marker(this_)
{
}

#pragma endregion

#pragma region Scope

#pragma region Namer

inline void Translator::Structure::Markers::Scope::Namer::Reset()
{
	index = 0;
}
inline Translator::Structure::Markers::Scope::Name Translator::Structure::Markers::Scope::Namer::Generate()
{
	return "<" + std::to_string(index++) + ">";
}

#pragma endregion


inline Translator::Structure::Markers::Scope::Scope(const Reference<Scope>& this_):
	Unit(this_)
{
}

inline void Translator::Structure::Markers::Scope::Add(const Name& name_, const Reference<Unit>& unit_)
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
inline Translator::Reference<Translator::Structure::Markers::Unit> Translator::Structure::Markers::Scope::GetOwned(const Name& name_) const
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
inline const Translator::Structure::Markers::Scope::Units& Translator::Structure::Markers::Scope::GetUnits() const
{
	return units;
}
inline Translator::Structure::Markers::Scope::Name Translator::Structure::Markers::Scope::GenerateName()
{
	return namer->Generate();
}

#pragma endregion

#pragma region Schema

#pragma region Indexer

inline void Translator::Structure::Markers::Schema::Indexer::Reset()
{
	index = 0;
}
inline Translator::Structure::Markers::Schema::Index Translator::Structure::Markers::Schema::Indexer::Generate()
{
	return index++;
}

#pragma endregion


inline Translator::Structure::Markers::Schema::Schema(const Reference<Schema>& this_):
	Scope(this_)
{
}

inline void Translator::Structure::Markers::Schema::Add(const Index& index_, const Reference<Algorithm>& algorithm_)
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
inline Translator::Reference<Translator::Structure::Markers::Algorithm> Translator::Structure::Markers::Schema::GetOwned(const Index& index_) const
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
inline const Translator::Structure::Markers::Schema::Algorithms& Translator::Structure::Markers::Schema::GetAlgorithms() const
{
	return algorithms;
}

inline Translator::Structure::Markers::Schema::Index Translator::Structure::Markers::Schema::GenerateIndex()
{
	return indexer->Generate();
}

#pragma endregion

#pragma region Algorithm

inline Translator::Structure::Markers::Algorithm::Algorithm(const Reference<Algorithm>& this_):
	Scope(this_)
{
}

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region Functional

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
/*inline const Translator::Structure::Markers::Schema::Algorithms& Translator::Structure::Markers::Schema::GetAlgorithms() const
{
	return algorithms;
}*/

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
// inline void Translator::Functional::Markers::Schema::Add(const AlgorithmKey& key_, const Reference<Algorithm>& algorithm_)
// {
// 	if(algorithms.find(key_) == algorithms.end())
// 	{
// 		algorithms[key_] = algorithm_;
// 	}
// 	else
// 	{
// 		throw Exception();
// 	}
// }

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

#pragma endregion


#pragma region
#pragma endregion


