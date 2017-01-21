#include "Header.hpp"


namespace Translator
{
	class Instruction;
	namespace Instructions
	{
		class Unit;
		class Scope;
	}
	class Scaner;


	class Instruction
	{
	public:
		inline Instruction() = delete;
		inline Instruction(const Instruction&) = default;
		virtual ~Instruction() = default;
		Instruction& operator = (const Instruction&) = default;
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
				Named(name_),
				scope(scope_)
			{
			}
			inline Unit(const Unit&) = delete;
			virtual ~Unit() override = default;
			Unit& operator = (const Unit&) = delete;
		public:
			inline Reference<Scope> GetScope() const
			{
				return scope;
			}
		};
		class Scope:
			public Unit
		{
		protected:
			Dictionary<Name, Reference<Unit>> units;
		public:
			inline Scope() = delete;
			inline Scope(const Reference<Scope>& scope_, const Name& name_):
				Unit(scope_, name_),
				units()
			{
			}
			inline Scope(const Unit&) = delete;
			virtual ~Scope() override = default;
			Scope& operator = (const Scope&) = delete;
		};
	}


	class Scaner
	{
	public:
		inline Scaner() = delete;
		inline Scaner(const Scaner&) = delete;
		virtual ~Scaner() = default;
		Scaner& operator = (const Scaner&) = delete;
	};
}




