#pragma once
#include "Directory.h"
#include <set>

class FileSystem
{
private:
	Directory* root;
	set<string> objectPaths;

	void copy(const FileSystem& fileSystem);
	void destroy();
	vector<string> paths(string path);
	bool validatePath(vector<string> splitted);
	bool containsPath(string path);
	string buildText(vector<string> content);
	string buildPath(vector<string> pathElements, string newElement);
	Directory* returnCurrentDirectory(Directory* directory, vector<string> splitted);
	void deleteDirectory(Directory*& directory);
public:

	FileSystem();
	FileSystem(Directory& directory);
	FileSystem(const FileSystem& other);
	~FileSystem();

	Directory* getFileSystem();
	set<string> getObjectPaths();

	void mkdir(string path);
	void rmdir(string path);
	string ls(string path);
	void cp(string path);
	string rm(string path);
	string cat(string path);
	void write(string path, vector<string> content);
	void append(string path, vector<string> content);

	FileSystem& operator= (FileSystem& other);
};
