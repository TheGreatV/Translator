#include <iostream>
#include <fstream>

#include <Translator/Lexer.hpp>
#include <Translator/Scaner.hpp>
#include <Translator/Parser.hpp>
using namespace Translator;


void main()
{
	Lexer::Glyphs source;
	{
		std::ifstream file("../../../../Media/Source.e"); // relative to project (.vcxproj)
		std::getline(file, source, '\0');
		file.close();
	}

	Lexer lexer;
	auto tokens = Move(lexer.Parse(source));
	
	Schematic::Scaner schematicScaner;
	auto schematicScope = schematicScaner.Scan(tokens);

	Algorithmic::Scaner algorithmicScaner;
	auto algorithmicScope = algorithmicScaner.Scan(schematicScope);

	// Structure::Scaner structureScaner;
	// auto structureScope = structureScaner.Scan(tokens);
	// 
	// Functional::Scaner functionalScaner;
	// auto functionalScope = functionalScaner.Scan(tokens, structureScope);
	// 
	// Parser parser;
	// auto scope = parser.Parse(tokens, functionalScope);

	std::system("pause");
}