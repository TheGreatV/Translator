#include "Scaner.hpp"


#pragma region Translator

#pragma region Structure

#pragma region Markers

#pragma region Scope

void Translator::Structure::Markers::Scope::ResetNaming()
{
	namer->Reset();
}

#pragma endregion

#pragma region Schema

void Translator::Structure::Markers::Schema::ResetNaming()
{
	Scope::ResetNaming();
	indexer->Reset();
}

#pragma endregion

#pragma endregion

#pragma region Scaner

bool Translator::Structure::Scaner::Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Scope>& scope_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, scope_))
	{
		return true;
	}
	if(auto identifier = Tokens::RequireIdentifier(tokens_, it_))
	{
		return true;
	}

	it_ = o;
	return false;
}
bool Translator::Structure::Scaner::Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Scope>& scope_)
{
	auto o = it_;

	if(auto keywordSchema = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema)))
	{
		Structure::Markers::Schema::Name name;
		{
			if(auto identifier = Move(Tokens::RequireIdentifier(tokens_, it_)))
			{
				name = identifier->GetName();
			}
			else
			{
				name = scope_->GenerateName();
			}
		}

		auto schema = Make<Structure::Markers::Schema>(scope_);

		if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
		{
			scope_->Add(name, schema);

			return true;
		}

		if(auto openingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::Begin)))
		{
			while(Scan_SchemaContent(tokens_, it_, schema));

			if(auto closingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::End)))
			{
				scope_->Add(name, schema);

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

	it_ = o;
	return false;
}
bool Translator::Structure::Scaner::Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Schema>& schema_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, schema_))
	{
		return true;
	}
	if(Scan_AlgorithmDefinition(tokens_, it_, schema_))
	{
		return true;
	}

	it_ = o;
	return false;
}

bool Translator::Structure::Scaner::Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Schema>& schema_)
{
	auto o = it_;

	if(Scan_BareAlgorithmDefinition(tokens_, it_, schema_))
	{
		return true;
	}
	// TODO

	it_ = o;
	return false;
}
bool Translator::Structure::Scaner::Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Schema>& schema_)
{
	auto o = it_;

	if(auto keywordAlgorithm = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm)))
	{
		auto index = schema_->GenerateIndex();
		auto algorithm = Make<Structure::Markers::Algorithm>(schema_);

		// Stub declaration
		if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
		{
			schema_->Add(index, algorithm);

			return true;
		}

		if(Scan_Schema(tokens_, it_, schema_))
		{
			if(Scan_Body(tokens_, it_, algorithm))
			{
				schema_->Add(index, algorithm);

				return true;
			}
			else
			{
				// Semi-full declaration
				if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				{
					schema_->Add(index, algorithm);

					return true;
				}
				else
				{
					// TODO: free order of algorithm properties (throw, etc.)
					throw Exception();
				}
			}
		}
		else
		{
			throw Exception();
		}
	}
	// TODO

	it_ = o;
	return false;
}

bool Translator::Structure::Scaner::Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Algorithm>& algorithm_)
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
			while(Scan_BodyContent(tokens_, it_, algorithm_));

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
bool Translator::Structure::Scaner::Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Structure::Markers::Algorithm>& algorithm_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, algorithm_))
	{
		return true;
	}

	it_ = o;
	return false;
}

Translator::Reference<Translator::Structure::Markers::Scope> Translator::Structure::Scaner::Scan(const TokensVector& tokens_)
{
	auto it = tokens_.cbegin();
	auto scope = Make<Structure::Markers::Scope>(nullptr);

	while(Scan_SchemaDefinition(tokens_, it, scope));

	if(it != tokens_.cend())
	{
		throw Exception();
	}

	return Move(scope);
}

#pragma endregion

#pragma endregion

#pragma region Functional

#pragma region Markers

#pragma endregion

#pragma region Scanner

Translator::Reference<Translator::Functional::Markers::Algorithm> Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Algorithm>& structureAlgorithm_, const Reference<Markers::Schema>& schema_)
{
	auto algorithm = Make<Markers::Algorithm>(schema_);

	for(auto &structureUnit : structureAlgorithm_->GetUnits())
	{
		auto name = structureUnit.first;
		auto unit = Prepare(structureUnit.second, algorithm);

		algorithm->Add(name, unit);
	}

	return algorithm; // TODO:
}
Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Schema>& structureSchema_, const Reference<Markers::Scope>& scope_)
{
	auto schema = Make<Functional::Markers::Schema>(scope_);

	for(auto &structureUnit : structureSchema_->GetUnits())
	{
		auto name = structureUnit.first;
		auto unit = Prepare(structureUnit.second, schema);

		// schema->Add(name, unit); // Visual Studio BUG
		schema->Scope::Add(name, unit);
	}

	for(auto &structureAlgorithm : structureSchema_->GetAlgorithms())
	{
		auto index = structureAlgorithm.first;
		auto algorithm = Prepare(structureAlgorithm.second, schema);

		schema->Add(index, algorithm);
	}
	// TODO

	return schema;
}
Translator::Reference<Translator::Functional::Markers::Unit> Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Unit>& structureUnit_, const Reference<Markers::Scope>& scope_)
{
	if(auto structureSchema = Translator::UpCast<Translator::Structure::Markers::Schema>(structureUnit_))
	{
		return Prepare(structureSchema, scope_);
	}

	// TODO

	return nullptr;
}

bool Translator::Functional::Scaner::Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, scope_, structureScope_))
	{
		return true;
	}
	if(auto identifier = Tokens::RequireIdentifier(tokens_, it_))
	{
		auto name = identifier->GetName();

		if(scope_->IsUnit(name))
		{
			auto unit = scope_->Get(name);
			auto schema = UpCast<Markers::Schema>(unit);
			return schema != nullptr;
		}
		else
		{
			return false;
		}
	}

	it_ = o;
	return false;
}
bool Translator::Functional::Scaner::Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_)
{
	auto o = it_;

	if(auto keywordSchema = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema)))
	{
		Structure::Markers::Schema::Name name;
		{
			if(auto identifier = Move(Tokens::RequireIdentifier(tokens_, it_)))
			{
				name = identifier->GetName();
			}
			else
			{
				name = structureScope_->GenerateName();
			}
		}

		auto structureUnit = structureScope_->GetOwned(name);
		if(auto structureSchema = UpCast<Structure::Markers::Schema>(structureUnit))
		{
			structureSchema->ResetNaming();

			auto unit = scope_->GetOwned(name);
			if(auto schema = UpCast<Markers::Schema>(unit))
			{

				if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				{
					return true;
				}

				if(auto openingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::Begin)))
				{
					while(Scan_SchemaContent(tokens_, it_, schema, structureSchema));

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
			else
			{
				throw Exception("There is no schema");
			}
		}
		else
		{
			throw Exception("There is no structure schema");
		}
	}

	it_ = o;
	return false;
}
bool Translator::Functional::Scaner::Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, schema_, structureSchema_))
	{
		return true;
	}
	if(Scan_AlgorithmDefinition(tokens_, it_, schema_, structureSchema_))
	{
		return true;
	}

	it_ = o;
	return false;
}

bool Translator::Functional::Scaner::Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_)
{
	auto o = it_;

	if(Scan_BareAlgorithmDefinition(tokens_, it_, schema_, structureSchema_))
	{
		return true;
	}
	// TODO

	it_ = o;
	return false;
}
bool Translator::Functional::Scaner::Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_)
{
	auto o = it_;

	if(auto keywordAlgorithm = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm)))
	{
		auto index = structureSchema_->GenerateIndex();

		if(auto structureAlgorithm = structureSchema_->GetOwned(index))
		{
			structureAlgorithm->ResetNaming();

			if(auto algorithm = schema_->GetOwned(index))
			{
				// Stub declaration
				if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				{
					return true;
				}

				if(Scan_Schema(tokens_, it_, schema_, structureSchema_))
				{
					if(Scan_Body(tokens_, it_, algorithm, structureAlgorithm))
					{
						return true;
					}
					else
					{
						// Semi-full declaration
						if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
						{
							return true;
						}
						else
						{
							// TODO: free order of algorithm properties (throw, etc.)
							throw Exception();
						}
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
		else
		{
			throw Exception();
		}
	}
	// TODO

	it_ = o;
	return false;
}
bool Translator::Functional::Scaner::Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_, const Reference<Structure::Markers::Algorithm>& structureAlgorithm_)
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
			while(Scan_BodyContent(tokens_, it_, algorithm_, structureAlgorithm_));

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
bool Translator::Functional::Scaner::Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_, const Reference<Structure::Markers::Algorithm>& structureAlgorithm_)
{
	auto o = it_;

	if(Scan_SchemaDefinition(tokens_, it_, algorithm_, structureAlgorithm_))
	{
		return true;
	}

	it_ = o;
	return false;
}


void Translator::Functional::Scaner::Scan(const TokensVector& tokens_, const Reference<Structure::Markers::Scope> structureScope_)
{
	auto it = tokens_.cbegin();
	auto scope = Make<Functional::Markers::Scope>(nullptr);

	// Prepare
	for(auto &structureUnit : structureScope_->GetUnits())
	{
		auto name = structureUnit.first;
		auto unit = Prepare(structureUnit.second, scope);

		scope->Add(name, unit);
	}

	// Scan
	structureScope_->ResetNaming();
	while(Scan_SchemaDefinition(tokens_, it, scope, structureScope_));

	// Check for errors
	if(it != tokens_.cend())
	{
		throw Exception();
	}
}

#pragma endregion

#pragma endregion

#pragma endregion


#pragma region
#pragma endregion






