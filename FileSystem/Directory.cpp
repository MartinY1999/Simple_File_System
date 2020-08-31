#include "Directory.h"
#include <algorithm>

void Directory::copy(Directory& other)
{
	setName(other.name);
	this->dateOfCreate = other.dateOfCreate;

	for (size_t i = 0; i < other.fileChildren.size(); i++)
	{
		this->fileChildren[i] = other.fileChildren[i];
	}

	for (size_t i = 0; i < other.directoriesChildren.size(); i++)
	{
		Directory* current = new Directory();
		*current = *other.directoriesChildren[i];
		this->directoriesChildren.push_back(current);
	}
}

void Directory::destroy()
{
	for (size_t i = 0; i < this->directoriesChildren.size(); i++)
	{
		delete this->directoriesChildren[i];
	}
}

Directory::Directory()
{
	Type();

	setName("");
	setDate();
}

Directory::Directory(string name)
{
	Type();

	setName(name);
	setDate();
}

Directory::Directory(Directory& other)
{
	this->copy(other);
}

Directory::~Directory()
{
	this->destroy();
}

vector<File> Directory::getFileChildren()
{
	return this->fileChildren;
}

vector<Directory*> Directory::getDirectoriesChildren()
{
	return this->directoriesChildren;
}

Directory& Directory::operator=(Directory& other)
{
	if (this != &other)
	{
		this->destroy();
		this->copy(other);
	}

	return *this;
}

void Directory::addFile(const File& file)
{
	this->fileChildren.push_back(file);
}

void Directory::addDirectory(Directory* directory)
{
	this->directoriesChildren.push_back(directory);
}

//Return directory we are looking for with recursion
Directory* Directory::findDirectory(string name, Directory* current)
{
	if (current->getName() == name)
		return current;

	for (size_t i = 0; i < current->directoriesChildren.size(); i++) 
	{
		if (current->directoriesChildren[i]->getName() == name)
			return current->directoriesChildren[i];
		else
		{
			Directory* child = findDirectory(name, current->directoriesChildren[i]);

			if (child != nullptr)
				return child;
			else
				continue;
		}
	}

	return nullptr;
}

string Directory::copyFile(File& file)
{
	File copy;

	copy.setNumeration(file.getCopied() + 1);
	copy.setName(file.getName() + '(' + to_string(copy.getNumeration()) + ')'); // Example: Name of file: File. After copy : File(1)
	copy.setContent(file.printContent());

	this->fileChildren.push_back(copy);
	
	for (size_t i = 0; i < this->fileChildren.size(); i++)
	{
		if (this->fileChildren[i] == file)
		{
			//This way we are keeping how many times the original file was copied
			this->fileChildren[i].setCopied(this->fileChildren[i].getCopied() + 1);
			break;
		}
	}

	return copy.getName();
}

void Directory::deleteFile(File& file)
{
	vector<File> resultVector;

	for (size_t i = 0; i < this->fileChildren.size(); i++)
	{
		if (this->fileChildren[i].showFileName() != file.showFileName())
			resultVector.push_back(this->fileChildren[i]);

		continue;
	}

	if (this->fileChildren.size() != resultVector.size())
	{
		this->fileChildren.clear();

		for (size_t i = 0; i < resultVector.size(); i++)
		{
			this->fileChildren.push_back(resultVector[i]);
		}
	}
}

void Directory::resetFile(int index, string newContent)
{
	this->fileChildren[index].setContent(newContent);
}

void Directory::appendTextToFile(int index, string toAdd)
{
	File current;
	current = this->fileChildren[index];

	this->fileChildren[index].setContent(current.printContent() + toAdd);
}

//Return the index of the file we are looking for
int Directory::returnFileIndex(string fileName)
{
	int indexAt = -1;

	if (this == nullptr)
	{
		//Invalid path
		return indexAt;
	}

	for (size_t i = 0; i < this->getFileChildren().size(); i++)
	{
		if (this->getFileChildren()[i].getName() == fileName)
		{
			indexAt = i;
			break;
		}
	}

	return indexAt;
}

void Directory::clearFileChildren()
{
	this->fileChildren.clear();
}

void Directory::eraseFromDirectories(Directory* directory)
{
	int index = -1;

	for (size_t i = 0; i < this->directoriesChildren.size(); i++)
	{
		if (this->directoriesChildren[i]->getName() == directory->getName())
		{
			index = i;
			break;
		}
	}

	this->directoriesChildren.erase(this->directoriesChildren.begin() + index);
	directory->~Directory();
}
