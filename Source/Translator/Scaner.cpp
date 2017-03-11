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

bool Translator::Structure::Scaner::Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_)
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
bool Translator::Structure::Scaner::Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_)
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

		auto schema = Make<Structure::Markers::Schema>();

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
bool Translator::Structure::Scaner::Scan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_)
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

bool Translator::Structure::Scaner::Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_)
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
bool Translator::Structure::Scaner::Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_)
{
	auto o = it_;

	if(auto keywordAlgorithm = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm)))
	{
		auto index = schema_->GenerateIndex();
		auto algorithm = Make<Markers::Algorithm>();

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

bool Translator::Structure::Scaner::Scan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_)
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
bool Translator::Structure::Scaner::Scan_BodyContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Algorithm>& algorithm_)
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
	auto scope = Make<Structure::Markers::Scope>();

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

#pragma region Scaner

void Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Scope>& structureScope_, const Reference<Markers::Scope>& scope_)
{
	for(auto &node : structureScope_->GetUnits())
	{
		auto &name = node.first;
		auto &structureUnit = node.second;

		auto unit = Prepare(structureUnit, scope_);
		scope_->Add(name, unit);
	}
}
Translator::Reference<Translator::Functional::Markers::Unit> Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Unit>& structureUnit_, const Reference<Markers::Scope>& scope_)
{
	if(auto structureSchema = UpCast<Structure::Markers::Schema>(structureUnit_))
	{
		return Prepare(structureSchema, scope_);
	}

	throw Exception();
}
Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Schema>& structureSchema_, const Reference<Markers::Scope>& scope_)
{
	auto schema = Make<Markers::Schema>(scope_);

	Prepare(Cast<Structure::Markers::Scope>(structureSchema_), Cast<Markers::Scope>(schema));

	for(auto &node : structureSchema_->GetAlgorithms())
	{
		auto &index = node.first;
		auto &structureAlgorithm = node.second;

		auto algorithmStub = Make<Markers::AlgorithmStub>();
		
		Prepare(structureAlgorithm, algorithmStub);

		schema->Add(index, algorithmStub);
	}

	return Move(schema);
}
void Translator::Functional::Scaner::Prepare(const Reference<Structure::Markers::Algorithm>& structureAlgorithm_, const Reference<Markers::AlgorithmStub>& algorithmStub_)
{
	Prepare(Cast<Structure::Markers::Scope>(structureAlgorithm_), Cast<Markers::Scope>(algorithmStub_));
}


Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Scaner::Scan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_)
{
	auto o = it_;

	if(auto schemaDefinition = Scan_SchemaDefinition(tokens_, it_, scope_, structureScope_))
	{
		return schemaDefinition;
	}
	if(auto identifier = Tokens::RequireIdentifier(tokens_, it_))
	{
		auto name = identifier->GetName();

		if(auto schema = UpCast<Markers::Schema>(scope_->Get(name)))
		{
			return schema;
		}
	}

	it_ = o;
	return nullptr;
}
Translator::Reference<Translator::Functional::Markers::Schema> Translator::Functional::Scaner::Scan_SchemaDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Scope>& scope_, const Reference<Structure::Markers::Scope>& structureScope_)
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

		// auto schema = Make<Structure::Markers::Schema>();
		if(auto structureSchema = UpCast<Structure::Markers::Schema>(structureScope_->GetOwned(name)))
		{
			structureSchema->ResetNaming();

			if(auto schema = UpCast<Markers::Schema>(scope_->GetOwned(name)))
			{
				if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				{
					return schema;
				}

				if(auto openingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::Begin)))
				{
					while(Scan_SchemaContent(tokens_, it_, schema, structureSchema));

					if(auto closingBrace = Move(Tokens::RequireBrace(tokens_, it_, Translator::Tokens::Brace::Type::Figure, Translator::Tokens::Brace::Position::End)))
					{
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

	it_ = o;
	return nullptr;
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

Translator::Reference<Translator::Functional::Markers::Algorithm> Translator::Functional::Scaner::Scan_AlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_)
{
	auto o = it_;

	if(auto bareAlgorithmDefinition = Scan_BareAlgorithmDefinition(tokens_, it_, schema_, structureSchema_))
	{
		return bareAlgorithmDefinition;
	}
	// TODO

	it_ = o;
	return false;
}
Translator::Reference<Translator::Functional::Markers::Algorithm> Translator::Functional::Scaner::Scan_BareAlgorithmDefinition(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Markers::Schema>& schema_, const Reference<Structure::Markers::Schema>& structureSchema_)
{
	auto o = it_;

	if(auto keywordAlgorithm = Move(Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm)))
	{
		auto index = structureSchema_->GenerateIndex();
		if(auto structureAlgorithm = structureSchema_->GetOwned(index))
		{
			structureAlgorithm->ResetNaming();

			if(auto algorithmStub = schema_->GetOwned(index))
			{
				// // Stub declaration
				// if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
				// {
				// 	auto algorithm = Make<Markers::Algorithm>(schema_, algorithmStub, nullptr);
				// 
				// 	// auto key = 
				// 	// 
				// 	// schema_->Add
				// 
				// 	return algorithm;
				// }
				// throw Exception(); // temporal

				if(auto resultSchema = Scan_Schema(tokens_, it_, schema_, structureSchema_))
				{
					auto key = Markers::Schema::BareKey{resultSchema};
					auto bareAlgorithm = Make<Markers::Algorithms::Bare>(schema_, algorithmStub, resultSchema);

					schema_->Add(key, bareAlgorithm);

					if(Scan_Body(tokens_, it_, bareAlgorithm, structureAlgorithm))
					{
						return bareAlgorithm;
					}
					else
					{
						// Semi-full declaration
						if(auto specialSemicolon = Move(Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon)))
						{
							return bareAlgorithm;
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

	it_ = o;
	return nullptr;
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


void Translator::Functional::Scaner::Scan(const TokensVector& tokens_, const Reference<Structure::Markers::Scope>& structureScope_)
{
	auto it = tokens_.cbegin();
	auto scope = Make<Markers::Scope>(nullptr);

	Prepare(structureScope_, scope);

	structureScope_->ResetNaming();
	while(Scan_SchemaDefinition(tokens_, it, scope, structureScope_));

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






