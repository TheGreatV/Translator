#include "Scaner.hpp"


#pragma region Algorithm

bool Translator::Instructions::Algorithm::IsEqual(const Reference<const Algorithm>& source_) const
{
	throw std::exception(); // TODO
}

#pragma endregion

#pragma region Algorithms

#pragma region Bare

bool Translator::Instructions::Algorithms::Bare::IsEqual(const Reference<const Algorithm>& source_) const
{
	auto bare = UpCast<Bare>(source_);
	if(bare)
	{
		return result == bare->GetResult();
	}

	throw std::exception(); // TODO
}

#pragma endregion

#pragma endregion

#pragma region Scaner

#pragma region SchemaScan

Translator::Reference<Translator::Instructions::Block> Translator::Scaner::SchemaScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	Instructions::Block::Name name;
	{
		auto keywordBlock = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Block);
		if(keywordBlock)
		{
			auto identifier = Tokens::RequireIdentifier(tokens_, it_);
			if(identifier)
			{
				name = identifier->GetName();
			}
			else
			{
				throw std::exception();
			}
		}
	}

	auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
	if(openingBrace)
	{
		if(name.empty())
		{
			name = scope_->GenerateName();
		}

		auto block = MakeReference(new Instructions::Block(scope_, name));
		{
			scope_->Add(name, block);
		}

		SchemaScan_BlockContent(tokens_, it_, block);

		// Search for closing brace
		auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
		if(closingBrace)
		{
			return block;
		}
		else
		{
			throw std::exception(); // TODO
		}
	}


	it_ = o;
	return false;
}
void Translator::Scaner::SchemaScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_)
{
	while(true)
	{
		if(SchemaScan_SchemaDeclaration(tokens_, it_, block_))
		{
			continue;
		}

		auto block = SchemaScan_Block(tokens_, it_, block_);
		if(block)
		{
			continue;
		}

		break;
	}
}

bool Translator::Scaner::SchemaScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	if(SchemaScan_SchemaDeclaration(tokens_, it_, scope_))
	{
		return true;
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::SchemaScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	if(SchemaScan_SchemaDeclaration(tokens_, it_, scope_))
	{
		return true;
	}

	auto identifier = Tokens::RequireIdentifier(tokens_, it_);
	if(identifier)
	{
		return true;
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::SchemaScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto keywordSchema = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema);
	if(keywordSchema)
	{
		auto identifier = Tokens::RequireIdentifier(tokens_, it_);
		Instructions::Schema::Name name = identifier ? identifier->GetName() : scope_->GenerateName();

		auto schema = MakeReference(new Instructions::Schema(scope_, name));

		auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
		if(openingBrace)
		{
			// Scan content
			SchemaScan_SchemaContent(tokens_, it_, schema);
		
			// Search for closing brace
			auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
			if(closingBrace)
			{
				scope_->Add(name, schema);
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
		else
		{
			throw std::exception();
		}
	}


	it_ = o;
	return false;
}
void Translator::Scaner::SchemaScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	while(true)
	{
		if(SchemaScan_SchemaDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		if(SchemaScan_AlgorithmDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		break;
	}
}


bool Translator::Scaner::SchemaScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto keywordBody = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Body);
	if(keywordBody)
	{
		auto semicolon = Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon);
		if(semicolon)
		{
			return true;
		}
		else
		{
			// TODO: scan 1 instruction
			if(SchemaScan_Block(tokens_, it_, scope_))
			{
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::SchemaScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	auto o = it_;


	auto keywordAlgorithm = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm);
	if(keywordAlgorithm)
	{
		auto name = schema_->GenerateName();
		auto localScope = MakeReference(new Instructions::Scope(schema_, name));
		{
			schema_->Scope::Add(name, localScope);
		}

		if(SchemaScan_Schema(tokens_, it_, localScope))
		{
			if(SchemaScan_BareAlgorithmDeclaration(tokens_, it_, localScope))
			{
				return true;
			}
			// TODO: brace/placement algorithms

			throw std::exception(); // TODO
		}
		else
		{
			throw std::exception(); // TODO
		}
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::SchemaScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& localScope_)
{
	auto o = it_;


	if(SchemaScan_Body(tokens_, it_, localScope_))
	{
		return true;
	}


	it_ = o;
	return false;
}

#pragma endregion

#pragma region AlgorithmScan

Translator::Reference<Translator::Instructions::Block> Translator::Scaner::AlgorithmScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	Instructions::Block::Name name;
	{
		auto keywordBlock = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Block);
		if(keywordBlock)
		{
			auto identifier = Tokens::RequireIdentifier(tokens_, it_);
			if(identifier)
			{
				name = identifier->GetName();
			}
			else
			{
				throw std::exception();
			}
		}
	}

	auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
	if(openingBrace)
	{
		if(name.empty())
	{
		name = scope_->GenerateName();
	}

		auto block = UpCast<Instructions::Block>(scope_->Get(name));
		if(block)
		{
			block->ResetGenerator();

			AlgorithmScan_BlockContent(tokens_, it_, block);

			// Search for closing brace
			auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
			if(closingBrace)
			{
				return block;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
		else
		{
			throw std::exception(); // TODO
		}

	}


	it_ = o;
	return false;
}
void Translator::Scaner::AlgorithmScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_)
{
	while(true)
	{
		if(AlgorithmScan_SchemaDeclaration(tokens_, it_, block_))
		{
			continue;
		}

		auto block = AlgorithmScan_Block(tokens_, it_, block_);
		if(block)
		{
			// block_->Add(block);
			continue;
		}

		break;
	}
}

bool Translator::Scaner::AlgorithmScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	if(AlgorithmScan_SchemaDeclaration(tokens_, it_, scope_))
	{
		return true;
	}


	it_ = o;
	return false;
}
Translator::Reference<Translator::Instructions::Schema> Translator::Scaner::AlgorithmScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto keywordSchema = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema);
	if(keywordSchema)
	{
		auto identifier = Tokens::RequireIdentifier(tokens_, it_);
		Instructions::Schema::Name name = identifier ? identifier->GetName() : scope_->GenerateName();

		auto unit = scope_->GetOwned(name);
		auto schema = UpCast<Instructions::Schema>(unit);
		if(schema)
		{
			schema->ResetGenerator();

			auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
			if(openingBrace)
			{
				// Scan content
				AlgorithmScan_SchemaContent(tokens_, it_, schema);

				// Search for closing brace
				auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
				if(closingBrace)
				{
					return schema;
				}
				else
				{
					throw std::exception(); // TODO
				}
			}
			else
			{
				throw std::exception();
			}
		}
	}


	it_ = o;
	return false;
}
void Translator::Scaner::AlgorithmScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	while(true)
	{
		if(AlgorithmScan_SchemaDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		if(AlgorithmScan_AlgorithmDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		break;
	}
}

bool Translator::Scaner::AlgorithmScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto keywordBody = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Body);
	if(keywordBody)
	{
		// TODO: replace with block scanning
		auto semicolon = Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon);
		if(semicolon)
		{
			return true;
		}
		else
		{
			// TODO: scan 1 instruction
			if(AlgorithmScan_Block(tokens_, it_, scope_))
			{
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}

		/*auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
		if(openingBrace)
		{
			// TODO: scan content

			// Search for closing brace
			auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
			if(closingBrace)
			{
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}*/
	}


	it_ = o;
	return false;
}
Translator::Reference<Translator::Instructions::Schema> Translator::Scaner::AlgorithmScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto declaration = AlgorithmScan_SchemaDeclaration(tokens_, it_, scope_);
	if(declaration)
	{
		return declaration;
	}

	auto identifier = Tokens::RequireIdentifier(tokens_, it_);
	if(identifier)
	{
		auto unit = scope_->Get(identifier->GetName());
		auto schema = UpCast<Instructions::Schema>(unit);
		if(schema)
		{
			return schema;
		}
	}


	it_ = o;
	return nullptr;
}
bool Translator::Scaner::AlgorithmScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	auto o = it_;


	auto keywordAlgorithm = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm);
	if(keywordAlgorithm)
	{
		auto name = schema_->GenerateName();
		auto localScope = UpCast<Instructions::Scope>(schema_->GetOwned(name));
		{
			localScope->ResetGenerator();
		}

		if(localScope)
		{
			auto resultSchema = AlgorithmScan_Schema(tokens_, it_, localScope);
			if(resultSchema)
			{
				if(AlgorithmScan_BareAlgorithmDeclaration(tokens_, it_, localScope, resultSchema))
				{
					return true;
				}
				// TODO: brace/placement algorithms

				throw std::exception(); // TODO
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
		else
		{
			throw std::exception(); // TODO
		}
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::AlgorithmScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& localScope_, const Reference<const Instructions::Schema>& resultSchema_)
{
	auto o = it_;


	if(AlgorithmScan_Body(tokens_, it_, localScope_))
	{
		auto algorithm = MakeReference(new Instructions::Algorithms::Bare(localScope_, resultSchema_));
		auto schema = UpCast<Instructions::Schema>(localScope_->GetScope());
		if(schema)
		{
			schema->Add(algorithm);
			return true;
		}
		else
		{
			throw std::exception(); // TODO
		}
	}
	else
	{
		throw std::exception(); // TODO
	}


	it_ = o;
	return false;
}

#pragma endregion

#pragma region ExpressionScan

Translator::Reference<Translator::Instructions::Block> Translator::Scaner::ExpressionScan_Block(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	Instructions::Block::Name name;
	{
		auto keywordBlock = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Block);
		if(keywordBlock)
		{
			auto identifier = Tokens::RequireIdentifier(tokens_, it_);
			if(identifier)
			{
				name = identifier->GetName();
			}
			else
			{
				throw std::exception();
			}
		}
	}

	auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
	if(openingBrace)
	{
		if(name.empty())
	{
		name = scope_->GenerateName();
	}

		auto block = UpCast<Instructions::Block>(scope_->Get(name));
		if(block)
		{
			block->ResetGenerator();

			ExpressionScan_BlockContent(tokens_, it_, block);

			// Search for closing brace
			auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
			if(closingBrace)
			{
				return block;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
		else
		{
			throw std::exception(); // TODO
		}

	}


	it_ = o;
	return false;
}
void Translator::Scaner::ExpressionScan_BlockContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Block>& block_)
{
	while(true)
	{
		if(ExpressionScan_SchemaDeclaration(tokens_, it_, block_))
		{
			continue;
		}

		auto block = ExpressionScan_Block(tokens_, it_, block_);
		if(block)
		{
			block_->Add(block);
			continue;
		}

		break;
	}
}

bool Translator::Scaner::ExpressionScan_Declaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	if(ExpressionScan_SchemaDeclaration(tokens_, it_, scope_))
	{
		return true;
	}


	it_ = o;
	return false;
}
Translator::Reference<Translator::Instructions::Schema> Translator::Scaner::ExpressionScan_SchemaDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto keywordSchema = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Schema);
	if(keywordSchema)
	{
		auto identifier = Tokens::RequireIdentifier(tokens_, it_);
		Instructions::Schema::Name name = identifier ? identifier->GetName() : scope_->GenerateName();

		auto unit = scope_->GetOwned(name);
		auto schema = UpCast<Instructions::Schema>(unit);
		if(schema)
		{
			schema->ResetGenerator();

			auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
			if(openingBrace)
			{
				// Scan content
				ExpressionScan_SchemaContent(tokens_, it_, schema);

				// Search for closing brace
				auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
				if(closingBrace)
				{
					return schema;
				}
				else
				{
					throw std::exception(); // TODO
				}
			}
			else
			{
				throw std::exception();
			}
		}
	}


	it_ = o;
	return false;
}
void Translator::Scaner::ExpressionScan_SchemaContent(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	while(true)
	{
		if(ExpressionScan_SchemaDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		if(ExpressionScan_AlgorithmDeclaration(tokens_, it_, schema_))
		{
			continue;
		}

		break;
	}
}

bool Translator::Scaner::ExpressionScan_Body(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Algorithm>& algorithm_)
{
	auto o = it_;


	auto keywordBody = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Body);
	if(keywordBody)
	{
		// TODO: replace with block scanning
		auto semicolon = Tokens::RequireSpecial(tokens_, it_, Tokens::Special::Type::Semicolon);
		if(semicolon)
		{
			return true;
		}
		else
		{
			// TODO: scan 1 instruction
			auto block = ExpressionScan_Block(tokens_, it_, algorithm_->GetLocalScope());
			if(block)
			{
				algorithm_->SetInstruction(block);
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}

		/*auto openingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::Begin);
		if(openingBrace)
		{
			// TODO: scan content

			// Search for closing brace
			auto closingBrace = Tokens::RequireBrace(tokens_, it_, Tokens::Brace::Type::Figure, Tokens::Brace::Position::End);
			if(closingBrace)
			{
				return true;
			}
			else
			{
				throw std::exception(); // TODO
			}
		}*/
	}


	it_ = o;
	return false;
}
Translator::Reference<Translator::Instructions::Schema> Translator::Scaner::ExpressionScan_Schema(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Scope>& scope_)
{
	auto o = it_;


	auto declaration = ExpressionScan_SchemaDeclaration(tokens_, it_, scope_);
	if(declaration)
	{
		return declaration;
	}

	auto identifier = Tokens::RequireIdentifier(tokens_, it_);
	if(identifier)
	{
		auto unit = scope_->Get(identifier->GetName());
		auto schema = UpCast<Instructions::Schema>(unit);
		if(schema)
		{
			return schema;
		}
	}


	it_ = o;
	return nullptr;
}
bool Translator::Scaner::ExpressionScan_AlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Schema>& schema_)
{
	auto o = it_;


	auto keywordAlgorithm = Tokens::RequireKeyword(tokens_, it_, Tokens::Keyword::Type::Algorithm);
	if(keywordAlgorithm)
	{
		auto name = schema_->GenerateName();
		auto localScope = UpCast<Instructions::Scope>(schema_->GetOwned(name));
		{
			localScope->ResetGenerator();
		}

		if(localScope)
		{
			auto resultSchema = ExpressionScan_Schema(tokens_, it_, localScope);
			if(resultSchema)
			{
				auto algorithm = schema_->Get(resultSchema);

				if(ExpressionScan_BareAlgorithmDeclaration(tokens_, it_, algorithm))
				{
					return true;
				}
				// TODO: brace/placement algorithms

				throw std::exception(); // TODO
			}
			else
			{
				throw std::exception(); // TODO
			}
		}
		else
		{
			throw std::exception(); // TODO
		}
	}


	it_ = o;
	return false;
}
bool Translator::Scaner::ExpressionScan_BareAlgorithmDeclaration(const TokensVector& tokens_, TokensVector::const_iterator& it_, const Reference<Instructions::Algorithms::Bare>& algorithm_)
{
	auto o = it_;


	if(ExpressionScan_Body(tokens_, it_, algorithm_))
	{
		return true;
	}
	else
	{
		throw std::exception(); // TODO
	}


	it_ = o;
	return false;
}

#pragma endregion


Translator::Reference<Translator::Instructions::Scope> Translator::Scaner::Parse(const TokensVector& tokens_)
{
	auto scope = Reference<Instructions::Scope>(new Instructions::Scope(nullptr, "<header>"));

	// Type scanning
	scope->ResetGenerator();
	auto it = tokens_.begin();
	while(it != tokens_.end())
	{
		auto o = it;

		if(SchemaScan_Declaration(tokens_, it, scope))
		{
			continue;
		}

		if(it == o)
		{
			throw std::exception("Tokens not parsed");
		}
	}

	// Algorithm scan
	scope->ResetGenerator();
	it = tokens_.begin();
	while(it != tokens_.end())
	{
		auto o = it;

		if(AlgorithmScan_Declaration(tokens_, it, scope))
		{
			continue;
		}

		if(it == o)
		{
			throw std::exception("Tokens not parsed");
		}
	}

	// Algorithm scan
	scope->ResetGenerator();
	it = tokens_.begin();
	while(it != tokens_.end())
	{
		auto o = it;

		if(ExpressionScan_Declaration(tokens_, it, scope))
		{
			continue;
		}

		if(it == o)
		{
			throw std::exception("Tokens not parsed");
		}
	}

	return scope;
}

#pragma endregion


#pragma region
#pragma endregion






