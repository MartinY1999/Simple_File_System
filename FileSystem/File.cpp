#include "File.h"

using namespace std;

void File::copy(const File& other)
{
	setName(other.name);
	setNumeration(other.numeration);
	setCopied(other.copied);
	this->dateOfCreate = other.dateOfCreate;
	setContent(other.content);
}

File::File()
{
	setName("");
	setNumeration(0);
	setCopied(0);
	setDate();
	setContent("");
}

File::File(string name)
{
	setName(name);
	setNumeration(0);
	setCopied(0);
	setDate();
	setContent("");
}

File::File(const File& other)
{
	this->copy(other);
}

int File::getNumeration() const
{
	return this->numeration;
}

int File::getCopied() const
{
	return this->copied;
}

void File::setContent(const string content)
{
	this->content = content;
}

void File::setNumeration(const int numeration)
{
	this->numeration = numeration;
}

void File::setCopied(const int copied)
{
	this->copied = copied;
}

File& File::operator=(File& other)
{
	if (this != &other)
		this->copy(other);

	return *this;
}

bool File::operator==(File& other)
{
	if (this->showFileName() == other.showFileName())
		return true;

	return false;
}

//We use this method to pass the file name as {Name}.txt as when we create it we pass it only as {Name}
string File::showFileName()
{
	stringstream stringBuilder;

	if (this->getNumeration() == 0)
	{
		stringBuilder << this->getName() << ".txt";

		return stringBuilder.str();
	}
	
	stringBuilder << this->getName() << ".txt";

	return stringBuilder.str();
}

//Returning the content (probably will be the same if i went with getContent())
string File::printContent()
{
	stringstream stringBuilder;

	stringBuilder << this->content;

	return stringBuilder.str();
}


