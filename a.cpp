#include "a.hpp"

char FooBar::getByte(char *c)
{
	std::cout << c[0] << std::endl;
	return c[0];
}

void FooBar::writeByte(const std::string &str)
{
	std::ofstream file_wrtr;
	file_wrtr.open("dump.log");
	file_wrtr << str;
	file_wrtr.close();
}

std::string FooBar::readByte()
{
	std::string line;
	std::ifstream file_rdr("dump.log");
	if (file_rdr.is_open())
	{
		while (std::getline(file_rdr, line))
		{
			std::cout << line << '\n';
		}
		file_rdr.close();
	}
	return line;
}
