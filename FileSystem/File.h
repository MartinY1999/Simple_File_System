#pragma once

#include <iostream>
#include <string>
#include "Type.h"

using namespace std;

class File : public Type
{

private:
	string content;
	int numeration; //This shows the ending of the file and shows if the file is copied. Examples - File, File(1), File(2)
	int copied; // This keeps how many times the file was copied
	void copy(const File& other);
public:
	File();
	File(string name);
	File(const File& other);

	using Type::getName;
	using Type::getDate;
	using Type::setName;
	using Type::setDate;

	int getNumeration() const;
	int getCopied() const;

	void setContent(const string content);
	void setNumeration(const int numeration);
	void setCopied(const int copied);

	File& operator= (File& other);
	bool operator== (File& other);

	string showFileName();
	string printContent();
};