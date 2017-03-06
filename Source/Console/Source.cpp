#include <iostream>
#include <fstream>

#include <Translator/Lexer.hpp>
#include <Translator/Scaner.hpp>
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

	Structure::Scaner structureScaner;
	auto structureScope = structureScaner.Scan(tokens);

	Functional::Scaner functionalScaner;
	functionalScaner.Scan(tokens, structureScope);

	std::system("pause");
}