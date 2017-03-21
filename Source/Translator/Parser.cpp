#include "Parser.hpp"


#pragma region Translator

#pragma region Parser

/*
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
}*/

void Translator::Parser::PrepareSchematical(const Reference<Instructions::Scope>& scope_, const Reference<Algorithmic::Scope>& algorithmicScope_)
{
	for(auto &node : algorithmicScope_->GetUnits())
	{
		auto &name = node.first;
		auto &algorithmicUnit = node.second;

		auto unit = Move(PrepareSchematical(algorithmicUnit, scope_));

		scope_->Add(name, unit);
	}
}
Translator::Reference<Translator::Instructions::Unit> Translator::Parser::PrepareSchematical(const Reference<Algorithmic::Unit>& algorithmicUnit_, const Reference<Instructions::Scope>& parentScope_)
{
	if(auto algorithmicSchema = Move(UpCast<Algorithmic::Schema>(algorithmicUnit_)))
	{
		auto schema = PrepareSchematical(algorithmicSchema, parentScope_);
		return schema;
	}

	throw Exception();
}
Translator::Reference<Translator::Instructions::Schema> Translator::Parser::PrepareSchematical(const Reference<Algorithmic::Schema>& algorithmicSchema_, const Reference<Instructions::Scope>& parentScope_)
{
	auto schema = Make<Instructions::Schema>(parentScope_);

	schemasTable[algorithmicSchema_] = schema;

	PrepareSchematical(Cast<Instructions::Scope>(schema), Cast<Algorithmic::Scope>(algorithmicSchema_));

	for(auto &node : algorithmicSchema_->GetBareAlgorithms())
	{
		auto &key = node.first;
		auto &algorithmicAlgorithm = node.second;

		auto algorithm = Move(PrepareSchematical(algorithmicAlgorithm, schema));

		schemasAlgorithms[schema].push_back(algorithm);
	}

	return schema;
}
Translator::Reference<Translator::Instructions::Algorithms::Bare> Translator::Parser::PrepareSchematical(const Reference<Algorithmic::Algorithms::Bare>& algorithmicAlgorithm_, const Reference<Instructions::Schema>& parentSchema_)
{
	auto algorithm = Move(Make<Instructions::Algorithms::Bare>(parentSchema_));

	algorithmsTable[algorithm] = algorithmicAlgorithm_;

	PrepareSchematical(Cast<Instructions::Scope>(algorithm), algorithmicAlgorithm_->algorithmScope);

	return algorithm;
}

void Translator::Parser::PrepareAlgorithmical(const Reference<Instructions::Scope>& scope_)
{
	for(auto &node : scope_->GetUnits())
	{
		auto &name = node.first;
		auto &unit = node.second;

		PrepareAlgorithmical(unit);
	}
}
void Translator::Parser::PrepareAlgorithmical(const Reference<Instructions::Unit>& unit_)
{
	if(auto schema = Move(UpCast<Instructions::Schema>(unit_)))
	{
		PrepareAlgorithmical(schema);
	}
	else
	{
		throw Exception();
	}
}
void Translator::Parser::PrepareAlgorithmical(const Reference<Instructions::Schema>& schema_)
{
	PrepareAlgorithmical(Cast<Instructions::Scope>(schema_));

	auto &bareAlgorithms = schemasAlgorithms[schema_];

	for(auto &bareAlgorithm : bareAlgorithms)
	{
		PrepareAlgorithmical(bareAlgorithm);
	}
}
void Translator::Parser::PrepareAlgorithmical(const Reference<Instructions::Algorithm>& algorithm_)
{
	if(auto bareAlgorithm = UpCast<Instructions::Algorithms::Bare>(algorithm_))
	{
		PrepareAlgorithmical(bareAlgorithm);
	}
	else
	{
		throw Exception();
	}
}
void Translator::Parser::PrepareAlgorithmical(const Reference<Instructions::Algorithms::Bare>& algorithm_)
{
	auto algorithmicAlgorithm = algorithmsTable[algorithm_];

	auto algorithmicResultSchema = algorithmicAlgorithm->GetResultSchema();
	auto resultSchema = schemasTable[algorithmicResultSchema];

	algorithm_->resultSchema = resultSchema;

	auto parentSchema = algorithm_->GetSchema();
	parentSchema->Add(Instructions::Schema::BareKey{resultSchema}, algorithm_);

	PrepareAlgorithmical(Cast<Instructions::Scope>(algorithm_));
}

Translator::Reference<Translator::Instructions::Scope> Translator::Parser::Parse(const Reference<Algorithmic::Scope>& algorithmicScope_)
{
	auto scope = Move(Make<Instructions::Scope>(nullptr));

	PrepareSchematical(scope, algorithmicScope_);
	PrepareAlgorithmical(scope);

	// TODO

	return Move(scope);
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion






