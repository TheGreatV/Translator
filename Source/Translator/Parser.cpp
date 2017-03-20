#include "Parser.hpp"


#pragma region Translator

#pragma region Parser

void Translator::Parser::PrepareStructure(const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_)
{
	for(auto &node : functionalScope_->GetUnits())
	{
		auto &name = node.first;
		auto &functionalUnit = node.second;

		auto unit = PrepareStructure(functionalUnit, functionalScope_, scope_);
		
		scope_->Add(name, unit);
	}
}
Translator::Reference<Translator::Instructions::Unit> Translator::Parser::PrepareStructure(const Reference<Functional::Markers::Unit>& functionalUnit_, const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_)
{
	if(auto functionalSchema = UpCast<Functional::Markers::Schema>(functionalUnit_))
	{
		auto schema = PrepareStructure(functionalSchema, functionalScope_, scope_);
		return schema;
	}

	throw Exception();
}
Translator::Reference<Translator::Instructions::Schema> Translator::Parser::PrepareStructure(const Reference<Functional::Markers::Schema>& functionalSchema_, const Reference<Functional::Markers::Scope>& functionalScope_, const Reference<Instructions::Scope>& scope_)
{
	auto schema = Make<Instructions::Schema>(scope_);

	schemaBindingTable[functionalSchema_] = schema;

	PrepareStructure(Cast<Functional::Markers::Scope>(functionalSchema_), Cast<Instructions::Scope>(schema));

	return schema;
}

void Translator::Parser::PrepareFunctional(const Reference<Instructions::Scope>& scope_, const Reference<Functional::Markers::Scope>& functionalScope_)
{
	for(auto &node : scope_->GetUnits())
	{
		auto &name = node.first;
		auto &unit = node.second;

		auto functionalUnit = functionalScope_->GetOwned(name);

		PrepareFunctional(unit, functionalUnit);
	}
}
void Translator::Parser::PrepareFunctional(const Reference<Instructions::Unit>& unit_, const Reference<Functional::Markers::Unit>& functionalUnit_)
{
	if(auto schema = UpCast<Instructions::Schema>(unit_))
	{
		if(auto functionalSchema = UpCast<Functional::Markers::Schema>(functionalUnit_))
		{
			PrepareFunctional(schema, functionalSchema);
		}
		else
		{
			throw Exception();
		}
	}
	else
	{
		throw Exception();
	}
}
void Translator::Parser::PrepareFunctional(const Reference<Instructions::Schema>& schema_, const Reference<Functional::Markers::Schema>& functionalSchema_)
{
	PrepareFunctional(Cast<Instructions::Scope>(schema_), Cast<Functional::Markers::Scope>(functionalSchema_));

	for(auto &node : functionalSchema_->GetBareAlgorithms())
	{
		auto &key = node.first;
		auto &functionalAlgorithm = node.second;
		
		PrepareFunctional(functionalAlgorithm, schema_, functionalSchema_);
	}
}
void Translator::Parser::PrepareFunctional(const Reference<Functional::Markers::Algorithms::Bare>& functionalAlgorithm_, const Reference<Instructions::Schema>& schema_, const Reference<Functional::Markers::Schema>& functionalSchema_)
{
	auto resultSchema = schemaBindingTable[functionalAlgorithm_->GetResultSchema()];

	auto algorithm = Make<Instructions::Algorithms::Bare>(schema_, resultSchema);

	PrepareStructure(Cast<Functional::Markers::Scope>(functionalAlgorithm_), Cast<Instructions::Scope>(algorithm));

	schema_->Add({resultSchema}, algorithm);
}

Translator::Reference<Translator::Instructions::Scope> Translator::Parser::Parse(const TokensVector& tokens_, const Reference<Functional::Markers::Scope>& functionalScope_)
{
	auto it = tokens_.cbegin();
	auto scope = Make<Instructions::Scope>(nullptr);

	PrepareStructure(functionalScope_, scope);
	PrepareFunctional(scope, functionalScope_);
	// 

	return Move(scope);
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion






