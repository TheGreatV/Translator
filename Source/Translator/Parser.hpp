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
		public:
			Link<Schema> resultSchema = Reference<Schema>(nullptr); // TODO: protected + setter
		public:
			inline Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_);
		public:
			inline Reference<Schema> GetSchema() const;
		};
		namespace Algorithms
		{
			class Bare:
				public Algorithm
			{
			public:
				inline Bare(const Reference<Algorithm>& this_, const Reference<Schema>& schema_);
			};
		}
	}

	class Parser
	{
	protected:
		Dictionary<Reference<Algorithmic::Schema>, Reference<Instructions::Schema>> schemasTable;
		Dictionary<Reference<Instructions::Schema>, Vector<Reference<Instructions::Algorithm>>> schemasAlgorithms;
		Dictionary<Reference<Instructions::Algorithm>, Reference<Algorithmic::Algorithm>> algorithmsTable;
	protected:
		void PrepareSchematical(const Reference<Instructions::Scope>& scope_, const Reference<Algorithmic::Scope>& algorithmicScope_);
		Reference<Instructions::Unit> PrepareSchematical(const Reference<Algorithmic::Unit>& algorithmicUnit_, const Reference<Instructions::Scope>& parentScope_);
		Reference<Instructions::Schema> PrepareSchematical(const Reference<Algorithmic::Schema>& algorithmicSchema_, const Reference<Instructions::Scope>& parentScope_);
		Reference<Instructions::Algorithms::Bare> PrepareSchematical(const Reference<Algorithmic::Algorithms::Bare>& algorithmicAlgorithm_, const Reference<Instructions::Schema>& parentSchema_);
	protected:
		void PrepareAlgorithmical(const Reference<Instructions::Scope>& scope_);
		void PrepareAlgorithmical(const Reference<Instructions::Unit>& unit_);
		void PrepareAlgorithmical(const Reference<Instructions::Schema>& schema_);
		void PrepareAlgorithmical(const Reference<Instructions::Algorithm>& algorithm_);
		void PrepareAlgorithmical(const Reference<Instructions::Algorithms::Bare>& algorithm_);
	public:
		Reference<Instructions::Scope> Parse(const Reference<Algorithmic::Scope>& algorithmicScope_);
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

inline Translator::Instructions::Algorithm::Algorithm(const Reference<Algorithm>& this_, const Reference<Schema>& schema_):
	Scope(this_, Cast<Scope>(schema_)),
	schema(schema_)
{
}
inline Translator::Reference<Translator::Instructions::Schema> Translator::Instructions::Algorithm::GetSchema() const
{
	return MakeReference(schema);
}

#pragma endregion

#pragma region Algorithms

#pragma region Bare

inline Translator::Instructions::Algorithms::Bare::Bare(const Reference<Algorithm>& this_, const Reference<Schema>& schema_):
	Algorithm(this_, schema_)
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


