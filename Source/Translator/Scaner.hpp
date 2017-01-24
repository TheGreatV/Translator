#pragma once


#include "Header.hpp"
#include "Lexer.hpp"


namespace Translator
{
	class Instruction;
	namespace Instructions
	{
		// Intermediate
		class Unit;
		class Scope;
		class Executable;
		// ???
		class Algorithm;
		// Terminal
		class Block;
		class Schema;
	}
	class Scaner;


	class Instruction
	{
	public:
		inline Instruction() = default;
		inline Instruction(const Instruction&) = default;
		virtual ~Instruction() = default;
		inline Instruction& operator = (const Instruction&) = default;
	};
	namespace Instructions
	{

		class Unit:
			public Instruction,
			public Named
		{
		protected:
			const Reference<Scope> scope;
		public:
			inline Unit() = delete;
			inline Unit(const Reference<Scope>& scope_, const Name& name_):
				Instruction(),
				Named(name_),
				scope(scope_)
			{
			}
			inline Unit(const Unit&) = delete;
			virtual ~Unit() override = default;
			inline Unit& operator = (const Unit&) = delete;
		public:
			inline Reference<Scope> GetScope() const
			{
				return scope;
			}
		};
		class Scope:
			public Unit
		{
		public:
			using Units = Dictionary<Name, Reference<Unit>>;
		protected:
			Size generatorId = 0;
			Units units;
		public:
			inline Scope() = delete;
			inline Scope(const Reference<Scope>& scope_, const Name& name_):
				Unit(scope_, name_),
				units()
			{
			}
			inline Scope(const Unit&) = delete;
			virtual ~Scope() override = default;
			inline Scope& operator = (const Scope&) = delete;
		public:
			inline void ResetGenerator()
			{
				generatorId = 0;
			}
			inline Name GenerateName()
			{
				return "<" + std::to_string(generatorId++) + ">";
			}
		public:
			inline void Add(const Name& name_, const Reference<Unit>& unit_)
			{
				if(units.find(name_) == units.end())
				{
					units.insert({name_, unit_});
				}
				else
				{
					throw std::exception(); // TODO
				}
			}
			inline Reference<Unit> GetOwned(const Name& name_)
			{
				auto it = units.find(name_);
				if(it != units.end())
				{
					auto &unit = (*it).second;
					return unit;
				}
				else
				{
					throw std::exception(); // TODO
				}
			}
			inline Reference<Unit> Get(const Name& name_)
			{
				auto it = units.find(name_);
				if(it != units.end())
				{
					auto &unit = (*it).second;
					return unit;
				}
				else
				{
					return scope ?
						scope->Get(name_) :
						throw std::exception(); // TODO
				}
			}
		};

		class Algorithm
		{
		protected:
			const Reference<const Schema> result;
			const Reference<Scope> localScope;
			Reference<Instruction> instruction;
		public:
			inline Algorithm() = delete;
			inline Algorithm(const Algorithm&) = delete;
			virtual ~Algorithm() = default;
			inline Algorithm& operator = (const Algorithm&) = delete;
		protected:
			inline Algorithm(const Reference<Scope>& localScope_, const Reference<const Schema>& result_);
		public:
			inline Reference<const Schema> GetResult() const
			{
				return result;
			}
			inline Reference<Scope> GetLocalScope() const
			{
				return localScope;
			}
			inline void SetInstruction(const Reference<Instruction>& instruction_)
			{
				instruction = instruction_;
			}
		public:
			virtual bool IsEqual(const Reference<const Algorithm>& source_) const;
		};
		namespace Algorithms
		{
			class Bare:
				public Algorithm
			{
			public:
				inline Bare() = delete;
				inline Bare(const Reference<Scope>& localScope_, const Reference<const Schema>& result_):
					Algorithm(localScope_, result_)
				{
				}
				inline Bare(const Bare&) = delete;
				virtual ~Bare() override = default;
				inline Bare& operator = (const Bare&) = delete;
			public:
				virtual bool IsEqual(const Reference<const Algorithm>& source_) const override;
			};
		}

		class Block:
			public Scope
		{
		protected:
			Vector<Reference<const Instruction>> instructions;
		public:
			inline Block() = delete;
			inline Block(const Reference<Scope>& scope_, const Name& name_):
				Scope(scope_, name_)
			{
			}
			inline Block(const Block&) = delete;
			virtual ~Block() override = default;
			inline Block& operator = (const Block&) = delete;
		public:
			inline void Add(const Reference<const Instruction>& instruction_)
			{
				instructions.push_back(instruction_);
			}
		};
		class Schema:
			public Scope
		{
		protected:
			Vector<Reference<Algorithm>> algorithms;
		public:
			inline Schema() = delete;
			inline Schema(const Reference<Scope>& scope_, const Name& name_):
				Scope(scope_, name_)
			{
			}
			inline Schema(const Schema&) = delete;
			virtual ~Schema() override = default;
			inline Schema& operator = (const Schema&) = delete;
		public:
			inline void Add(const Reference<Algorithm>& algorithm_)
			{
				auto it = std::find_if(algorithms.begin(), algorithms.end(), [&](const Reference<const Algorithm>& source_){
					return algorithm_->IsEqual(source_);
				});
				
				if(it == algorithms.end())
				{
					algorithms.push_back(algorithm_);
				}
				else
				{
					throw std::exception(); // TODO
				}
			}
			inline Reference<Algorithms::Bare> Get(const Reference<Instructions::Schema>& resultSchema_) const
			{
				for(auto &algorithm : algorithms)
				{
					auto bare = UpCast<Instructions::Algorithms::Bare>(algorithm);
					if(bare)
					{
						if(bare->GetResult() == resultSchema_)
						{
							return bare;
						}
					}
				}

				throw std::exception(); // TODO
			}
		};
	}


	class Scaner
	{
	public:
		inline Scaner() = default;
		inline Scaner(const Scaner&) = delete;
		virtual ~Scaner() = default;
		Scaner& operator = (const Scaner&) = delete;
	protected: // Type scan
		Reference<Instructions::Block> SchemaScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		/* NEW */ void SchemaScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_);
		bool SchemaScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		bool SchemaScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		void SchemaScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool SchemaScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		bool SchemaScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		bool SchemaScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool SchemaScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& localScope_);
	protected: // Algorithm scan
		Reference<Instructions::Block> AlgorithmScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		/* NEW */ void AlgorithmScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_);
		bool AlgorithmScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Schema> AlgorithmScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Schema> AlgorithmScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		void AlgorithmScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool AlgorithmScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		bool AlgorithmScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool AlgorithmScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& localScope_, const Reference<const Instructions::Schema>& resultSchema_);
	protected: // Expression scan
		Reference<Instructions::Block> ExpressionScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		/* NEW */ void ExpressionScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_);
		bool ExpressionScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Schema> ExpressionScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		Reference<Instructions::Schema> ExpressionScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_);
		void ExpressionScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool ExpressionScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Algorithm>& algorithm_);
		bool ExpressionScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_);
		bool ExpressionScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Algorithms::Bare>& algorithm_);
	public:
		Reference<Instructions::Scope> Parse(const TokensVector& tokens_);
	};
}


#pragma region Instructions

#pragma region Algorithm

inline Translator::Instructions::Algorithm::Algorithm(const Reference<Scope>& localScope_, const Reference<const Schema>& result_):
	localScope(localScope_),
	result(result_)
{
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion


