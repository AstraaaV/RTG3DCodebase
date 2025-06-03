#include "stringHelp.h"

void StringHelp::Int(std::ifstream& file, const std::string& label, int& value)
{
	std::string line;
	while (std::getline(file, line))
	{
		size_t found = line.find(label + " :");
		if (found != std::string::npos)
		{
			std::stringstream ss(line.substr(found + label.length() + 2));
			ss >> value;
			return;
		}
	}
}
