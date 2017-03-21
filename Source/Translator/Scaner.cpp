#include "Scaner.hpp"


#pragma region Translator

#pragma region Schematic

#pragma region Scope

void Translator::Schematic::Scope::ResetNaming()
{
	namer->Reset();
}

#pragma endregion

#pragma region Schema

void Translator::Schematic::Schema::ResetNaming()
{
	Scope::ResetNaming();
	indexer->Reset();
}

#pragma endregion

#pragma region Scaner

Translator::Reference<Translator::Schematic::Token> Translator::Schematic::Scaner::Scan_Token(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_)
{
	auto o = it_;

	if(auto identifier = Move(Tokens::RequireIdentifier(tokens_, it_)))
	{
		auto token = Move(Make<Token>(identifier));
		return token;
	}

	it_ = o;
	return nullptr;
}

Translator::Reference<Translator::Schematic::Marker> Translator::Schematic::Scaner::Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_)
{
	auto o = it_;

	if(auto schema = Move(Scan_SchemaDefinition(tokens_, it_, parentScope_)))
	{
		return schema;
	}
	if(auto identifier = Move(Tokens::RequireIdentifier(tokens_, it_)))
	{
		auto token = Move(Make<Token>(identifier));
		return token;
	}

	it_ = o;
	return nullptr;
}
Translator::Reference<Translator::Schematic::Schema> Translator::Schematic::Scaner::Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Scope>& parentScope_)
{
	auto o = it_;

	if(auto keywordSchema = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema)))
	{
		auto name = ([&]()->Schema::Name{
			if(auto identifier = Move(Tokens::RequireIdentifier(tokens_, it_)))
			{
				return identifier->GetName();
			}
			else
			{
				return parentScope_->GenerateName();
				// return "";
			}
		})();

		auto schema = Move(Make<Schema>(parentScope_));

		if(auto semicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
		{
			parentScope_->Add(name, schema);

			return schema;
		}
		else
		{
			if(auto openingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::Begin)))
			{
				while(auto token = Move(Scan_SchemaContent(tokens_, it_, schema)))
				{
					// schema->Contentable::Add(token);
				}

				if(auto closingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::End)))
				{
					parentScope_->Add(name, schema);

					return schema;
				}
				else
				{
					throw Exception("No closing brace");
				}
			}
			else
			{
				throw Exception("No opeing brace");
			}
		}
	}

	it_ = o;
	return nullptr;
}
Translator::Reference<Translator::Schematic::Marker> Translator::Schematic::Scaner::Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& schema_)
{
	auto o = it_;

	if(auto schema = Move(Scan_SchemaDefinition(tokens_, it_, schema_)))
	{
		return schema;
	}
	if(auto algorithm = Move(Scan_AlgorithmDefinition(tokens_, it_, schema_)))
	{
		return algorithm;
	}
	// if(auto token = Scan_Token(tokens_, it_, schema_))
	// {
	// 	return token;
	// }

	it_ = o;
	return nullptr;
}

Translator::Reference<Translator::Schematic::Algorithm> Translator::Schematic::Scaner::Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_)
{
	auto o = it_;

	if(auto bareAlgorithm = Move(Scan_BareAlgorithmDefinition(tokens_, it_, parentSchema_)))
	{
		return bareAlgorithm;
	}

	it_ = o;
	return nullptr;
}
Translator::Reference<Translator::Schematic::Algorithm> Translator::Schematic::Scaner::Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Schema>& parentSchema_)
{
	auto o = it_;

	if(auto keywordAlgorithm = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm)))
	{
		if(auto resultSchema = Move(Scan_Schema(tokens_, it_, parentSchema_)))
		{
			auto algorithm = Make<Algorithm>(parentSchema_, resultSchema);

			if(Scan_Body(tokens_, it_, algorithm))
			{
				auto index = parentSchema_->GenerateIndex();
				parentSchema_->Add(index, algorithm);

				return algorithm;
			}
			else
			{
				// Semi-full declaration
				if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				{
					auto index = parentSchema_->GenerateIndex();
					parentSchema_->Add(index, algorithm);

					return algorithm;
				}
				else
				{
					// TODO: free order of algorithm properties (throw, etc.)
					throw Exception();
				}
			}
		}
	}

	it_ = o;
	return nullptr;
}

bool Translator::Schematic::Scaner::Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_)
{
	auto o = it_;

	if(auto keywordBody = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Body)))
	{
		// Stub declaration
		if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
		{
			return true;
		}

		// Full declaration
		if(auto openingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::Begin)))
		{
			while(auto marker = Move(Scan_BodyContent(tokens_, it_, algorithm_)))
			{
				algorithm_->Contentable::Add(marker);
			}

			if(auto closingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::End)))
			{
				return true;
			}
			else
			{
				throw Exception("No closing brace");
			}
		}
		else
		{
			throw Exception("No opeing brace");
		}
	}
	// TODO

	it_ = o;
	return false;
}
Translator::Reference<Translator::Schematic::Marker> Translator::Schematic::Scaner::Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Algorithm>& algorithm_)
{
	auto o = it_;

	if(auto schema = Move(Scan_SchemaDefinition(tokens_, it_, algorithm_)))
	{
		return schema;
	}

	it_ = o;
	return nullptr;
}

Translator::Reference<Translator::Schematic::Scope> Translator::Schematic::Scaner::Scan(const TokensVector& tokens_)
{
	auto it = tokens_.cbegin();
	auto scope = Make<Schematic::Scope>();
	
	while(auto schema = Scan_SchemaDefinition(tokens_, it, scope));

	// if(it != tokens_.cend())
	// {
	// 	throw Exception();
	// }

	return Move(scope);
}

#pragma endregion

#pragma endregion

#pragma region Algorithmic

void Translator::Algorithmic::Scaner::Prepare(const Reference<Scope>& scope_, const Reference<Schematic::Scope>& schematicScope_)
{
	for(auto &node : schematicScope_->GetUnits())
	{
		auto &name = node.first;
		auto &schematicUnit = node.second;

		auto unit = Prepare(schematicUnit, scope_);

		scope_->Add(name, unit);
	}
}
Translator::Reference<Translator::Algorithmic::Unit> Translator::Algorithmic::Scaner::Prepare(const Reference<Schematic::Unit>& schematicUnit_, const Reference<Scope>& parentScope_)
{
	if(auto schematicSchema = UpCast<Schematic::Schema>(schematicUnit_))
	{
		auto schema = Prepare(schematicSchema, parentScope_);
		return Move(schema);
	}

	throw Exception();
}
Translator::Reference<Translator::Algorithmic::Schema> Translator::Algorithmic::Scaner::Prepare(const Reference<Schematic::Schema>& schematicSchema_, const Reference<Scope>& parentScope_)
{
	auto schema = Make<Schema>(parentScope_);

	schemasTable[schema] = schematicSchema_;
	schemasTableInverse[schematicSchema_] = schema;

	Prepare(Cast<Scope>(schema), Cast<Schematic::Scope>(schematicSchema_));

	for(auto &node : schematicSchema_->GetAlgorithms())
	{
		auto &index = node.first;
		auto &schematicAlgorithm = node.second;

		auto algorithmScope = Prepare(schematicAlgorithm, schema);

		schema->Add(algorithmScope);
	}

	return Move(schema);
}
Translator::Reference<Translator::Algorithmic::AlgorithmScope> Translator::Algorithmic::Scaner::Prepare(const Reference<Schematic::Algorithm>& schematicAlgorithm_, const Reference<Schema>& parentSchema_)
{
	auto algorithmScope = Make<AlgorithmScope>(parentSchema_);

	algorithmScopesTable[algorithmScope] = schematicAlgorithm_;

	Prepare(Cast<Scope>(algorithmScope), Cast<Schematic::Scope>(schematicAlgorithm_));

	return algorithmScope;
}

void Translator::Algorithmic::Scaner::Scan(const Reference<Unit>& unit_)
{
	if(auto schema = UpCast<Schema>(unit_))
	{
		Scan(schema);
	}
}
void Translator::Algorithmic::Scaner::Scan(const Reference<Scope>& scope_)
{
	for(auto &node : scope_->GetUnits())
	{
		auto &name = node.first;
		auto &unit = node.second;

		Scan(unit);
	}
}
void Translator::Algorithmic::Scaner::Scan(const Reference<Schema>& schema_)
{
	Scan(Cast<Scope>(schema_));

	for(auto &algorithmScope : schema_->GetAlgorithmScopes())
	{
		Scan(algorithmScope, schema_);
	}
}
void Translator::Algorithmic::Scaner::Scan(const Reference<AlgorithmScope>& algorithmScope_, const Reference<Schema>& schema_)
{
	auto schematicAlgorithm = algorithmScopesTable[algorithmScope_];

	Scan(Cast<Scope>(algorithmScope_));

	auto resultSchemaMarker = schematicAlgorithm->GetResultSchema();
	if(auto resultSchematicSchema = UpCast<Schematic::Schema>(resultSchemaMarker))
	{
		auto resultSchema = schemasTableInverse[resultSchematicSchema];

		auto algorithm = Make<Algorithms::Bare>(schema_, resultSchema, algorithmScope_);

		schema_->Add({resultSchema}, algorithm);
	}
	else
	{
		if(auto token = UpCast<Schematic::Token>(resultSchemaMarker))
		{
			if(auto identifier = UpCast<Tokens::Identifier>(token->GetSource()))
			{
				auto scope = algorithmScope_->GetScope();

				if(auto resultSchema = UpCast<Schema>(scope->Get(identifier->GetName())))
				{
					auto algorithm = Make<Algorithms::Bare>(schema_, resultSchema, algorithmScope_);

					schema_->Add({resultSchema}, algorithm);
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
		else
		{
			throw Exception();
		}
	}
}


Translator::Reference<Translator::Algorithmic::Scope> Translator::Algorithmic::Scaner::Scan(const Reference<Schematic::Scope>& schematicScope_)
{
	auto scope = Make<Scope>();

	Prepare(scope, schematicScope_);

	Scan(scope);

	return Move(scope);
}

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion






