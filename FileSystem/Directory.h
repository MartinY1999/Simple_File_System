#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Type.h"
#include "File.h"

using namespace std;

class Directory : Type
{

private:
	vector<File> fileChildren;
	vector<Directory*> directoriesChildren;
	void copy(Directory& other);
	void destroy();
public:
	Directory();
	Directory(string name);
	Directory(Directory& other);
	~Directory();

	using Type::getName;
	using Type::getDate;
	using Type::setName;
	using Type::setDate;

	vector<File> getFileChildren();
	vector<Directory*> getDirectoriesChildren();

	Directory& operator= (Directory& other);

	void addFile(const File& file);
	void addDirectory(Directory* directory);
	Directory* findDirectory(string name, Directory* current);
	string copyFile(File& file);
	void deleteFile(File& file);
	void resetFile(int index, string newContent);
	void appendTextToFile(int index, string toAdd);
	int returnFileIndex(string fileName);
	void clearFileChildren();
	void eraseFromDirectories(Directory* directory);
};