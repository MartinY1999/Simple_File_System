#pragma once
#include "FileSystem.h"

class User
{	
private:
	void save(ofstream &of, FileSystem& fileSystem);


public:
	User();

	void makeDirectory(FileSystem& fileSystem, string path);
	void removeDirectory(FileSystem& fileSystem, string path);
	void printDirectoryContent(FileSystem& fileSystem, string path);
	void copyFile(FileSystem& fileSystem, string path);
	void deleteFile(FileSystem& fileSystem, string path);
	void printFileContent(FileSystem& fileSystem, string path);
	void writeTextToFile(FileSystem& fileSystem, string path, vector<string> text);
	void appendTextToFile(FileSystem& fileSystem, string path, vector<string> text);
	void close(FileSystem& fileSystem);
};

