#pragma once
#pragma warning( disable : 4996 )


#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

class Type
{

protected:
	string name;
	string dateOfCreate;
public:
	string getName() const 
	{
		return this->name;
	}
	string getDate() const
	{
		return this->dateOfCreate;
	}

	void setName(string name)
	{
		this->name = name;
	}
	void setDate()
	{
		auto t = time(nullptr);
		auto tm = *localtime(&t);

		ostringstream result;
		result << put_time(&tm, "%d-%m-%Y %H:%M:%S");

		this->dateOfCreate = result.str();
	}
};
