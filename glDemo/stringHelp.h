#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//a bunch of helper functions to load in sets of values from the manifest
//and output the values that have been read in to the console
class StringHelp {
public:

	static void Int(std::ifstream& file, const std::string& label, int& value);

	static void String(ifstream& _file, string _message, string& _out)
	{
		string dummy;
		_file >> dummy >> _out; _file.ignore(255, '\n');
		std::cout << _message << " : " << _out << endl;
	}

	static void Float3(ifstream& _file, string _message, float& _out1, float& _out2, float& _out3)
	{
		string line;
		if (getline(_file, line))
		{
			size_t colonPos = line.find(':');
			if (colonPos != string::npos)
			{
				istringstream iss(line.substr(colonPos + 1));
				iss >> _out1 >> _out2 >> _out3;
				std::cout << _message << " : " << _out1 << " " << _out2 << " " << _out3 << endl;
			}
		}
	}

	static void Float(ifstream& _file, string _message, float& _out)
	{
		string dummy;
		_file >> dummy >> _out; _file.ignore(255, '\n');
		std::cout << _message << " : " << _out << endl;
	}
};
