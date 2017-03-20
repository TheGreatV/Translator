#pragma once


#include "Header.hpp"
#include "Scaner.hpp"


namespace Translator
{
	class Instruction;
	namespace Instructions
	{
		class Unit;
		class Scope;

		class Schema;

		class Algorithm;
		namespace Algorithms
		{
			class Bare;
		}
	}

	class Parser;


	class Instruction:
		public This<Instruction>
	{
	public:
		inline Instruction(const Reference<Instruction>& this_);
		virtual ~Instruction() = default;
	};
	namespace Instructions
	{
		class Unit:
			public Instruction
		{
		protected:
			const Link<Scope> scope;
		public:
			inline Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_);
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
		public:
			inline void Add(const Name& name_, const Reference<Unit>& unit_);
			inline const Units& GetUnits() const;
		};

		class Schema:
			public Scope
		{
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
			BareAlgorithms bareAlgorithms;
		public:
			inline Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_);
		public:
			inline void Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_);
		};

		class Algorithm:
			public Scope
		{
		protected:
			const Link<Schema> schema;
			const Link<Schema> resultSchema;
		public:
			inline Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<Schema>& resultSchema_);
		};
		namespace Algorithms
		{
			class Bare:
				public Algorithm
			{
			public:
				inline Bare(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<Schema>& resultSchema_);
			};
		}
	}

	class Parser
	{
	protected:
		Dictionary<Reference<Functional::Markers::Schema>, Reference<Instructions::Schema>> schemaBindingTable;
	protected:
		void PrepareStructure(const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Unit> PrepareStructure(const Reference<Functional::Markers::Unit>& functionalUnit_, const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Schema> PrepareStructure(const Reference<Functional::Markers::Schema>& functionalSchema_, const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_);
	protected:
		void PrepareFunctional(const Reference<Instructions::Scope>& scope_, const Reference<Functional::Markers::Scope>& functionalScope_);
		void PrepareFunctional(const Reference<Instructions::Unit>& unit_, const Reference<Functional::Markers::Unit>& functionalUnit_);
		void PrepareFunctional(const Reference<Instructions::Schema>& schema_, const Reference<Functional::Markers::Schema>& functionalSchema_);
		void PrepareFunctional(const Reference<Functional::Markers::Algorithms::Bare>& functionalAlgorithm_, const Reference<Instructions::Schema>& schema_, const Reference<Functional::Markers::Schema>& functionalSchema_);
	public:
		Reference<Instructions::Scope> Parse(const TokensVector& tokens_, const Reference<Functional::Markers::Scope>& functionalScope_);
	};
}


#pragma region Translator

#pragma region Instruction

inline Translator::Instruction::Instruction(const Reference<Instruction>& this_):
	This(this_)
{
}

#pragma endregion

#pragma region Instructions

#pragma region Unit

inline Translator::Instructions::Unit::Unit(const Reference<Unit>& this_, const Reference<Scope>& scope_):
	Instruction(this_),
	scope(scope_)
{
}

#pragma endregion

#pragma region Scope

inline Translator::Instructions::Scope::Scope(const Reference<Scope>& this_, const Reference<Scope>& scope_):
	Unit(this_, scope_)
{
}

inline void Translator::Instructions::Scope::Add(const Name& name_, const Reference<Unit>& unit_)
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
inline const Translator::Instructions::Scope::Units& Translator::Instructions::Scope::GetUnits() const
{
	return units;
}

#pragma endregion

#pragma region Schema

inline Translator::Instructions::Schema::Schema(const Reference<Schema>& this_, const Reference<Scope>& scope_):
	Scope(this_, scope_)
{
}

inline void Translator::Instructions::Schema::Add(const BareKey& key_, const Reference<Algorithms::Bare>& bareAlgorithm_)
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

#pragma region Algorithm

inline Translator::Instructions::Algorithm::Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<Schema>& resultSchema_):
	Scope(this_, Cast<Scope>(schema_)),
	schema(schema_)
{
}

#pragma endregion

#pragma region Algorithms

#pragma region Bare

inline Translator::Instructions::Algorithms::Bare::Bare(const Reference<Algorithm>& this_, const Reference<Schema>& schema_, const Reference<Schema>& resultSchema_):
	Algorithm(this_, schema_, resultSchema_)
{
}

#pragma endregion

#pragma endregion

#pragma endregion

#pragma region Parser

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion


