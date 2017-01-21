#include <iostream>
#include <fstream>

#include <Translator/Lexer.hpp>
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


	std::system("pause");
}