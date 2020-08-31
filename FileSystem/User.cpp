#include "User.h"
#include <fstream>
#include <iostream>

/*
root
root/D:
root/D:/Tv Series
root/D:/C++
root/D:/C++/Java
root/D:/C++/C#
root/D:/Games
root/D:/Movies
root/C:
root/C:/Program Files
root/C:/Users
*/

//Here we save the information in the text file
void User::save(ofstream& of, FileSystem& fileSystem)
{
	std::set<string>::iterator iterator;
	std::set<string> paths = fileSystem.getObjectPaths();

	for (iterator = paths.begin(); iterator != paths.end(); ++iterator)
	{
		string path = *iterator;
		of << path;
		of << "\n";
	}
}

User::User()
{
}

void User::makeDirectory(FileSystem& fileSystem, string path)
{
	fileSystem.mkdir(path);
}

void User::removeDirectory(FileSystem& fileSystem, string path)
{
	fileSystem.rmdir(path);
}

void User::printDirectoryContent(FileSystem& fileSystem, string path)
{
	std::cout << fileSystem.ls(path) << std::endl;
}

void User::copyFile(FileSystem& fileSystem, string path)
{
	fileSystem.cp(path);
}

void User::deleteFile(FileSystem& fileSystem, string path)
{
	std:: cout << fileSystem.rm(path) << std:: endl;
}

void User::printFileContent(FileSystem& fileSystem, string path)
{
	std::cout << fileSystem.cat(path) << std::endl;
}

void User::writeTextToFile(FileSystem& fileSystem, string path, vector<string> text)
{
	fileSystem.write(path, text);
}

void User::appendTextToFile(FileSystem& fileSystem, string path, vector<string> text)
{
	fileSystem.append(path, text);
}

void User::close(FileSystem& fileSystem)
{
	ofstream fs;
	fs.open("File System.txt", ios::out);

	if (!fs.is_open())
	{
		std::cerr << "Cannot open file!" << std::endl;
	}
	else
	{
		this->save(fs, fileSystem);
	}

	fs.close();
}
