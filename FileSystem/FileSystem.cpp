#include "FileSystem.h"
#include <ostream>
#include <regex>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

void FileSystem::copy(const FileSystem& fileSystem)
{
	this->root = new Directory();
	*this->root = *fileSystem.root;
}

void FileSystem::destroy()
{
	delete this->root;
}

//Method for splitting the path into elements
vector<string> FileSystem::paths(string path)
{
	string delimiter = "\\";

	size_t start = 0, end;
	string token;
	vector<string> result;

	while ((end = path.find(delimiter, start)) != string::npos) 
	{
		token = path.substr(start, end - start);
		start = end + delimiter.length();
		result.push_back(token);
	}

	result.push_back(path.substr(start));
	
	return result;
}

bool FileSystem::validatePath(vector<string> splitted)
{
	if (splitted[0] != this->root->getName())
	{
		std::cerr << "Root does not match." << std::endl;
		return false;
	}

	return true;
}

//Method for checking if we have already encountered this path
bool FileSystem::containsPath(string path)
{
	const bool is_in = objectPaths.find(path) != objectPaths.end();
	if (is_in)
		return true;

	return false;
}

//Method for building the content information of a file
string FileSystem::buildText(vector<string> content)
{
	stringstream stringBuilder;

	for (int i = 0; i < content.size(); i++)
	{
		if (i != content.size() - 1)
			stringBuilder << content[i] << " ";
		else
			stringBuilder << content[i] << "\n";
	}

	return stringBuilder.str();
}

//Method for building the new path
string FileSystem::buildPath(vector<string> pathElements, string newElement)
{
	stringstream stringBuilder;

	for (int i = 0; i < pathElements.size() - 1; i++)
	{
		stringBuilder << pathElements[i] << "\\";
	}

	stringBuilder << newElement;

	return stringBuilder.str();
}

Directory* FileSystem::returnCurrentDirectory(Directory* directory, vector<string> splitted)
{
	int currentIndex = 0;

	//While the index of the directory is not found or has reached the end
	while (currentIndex != splitted.size() - 1)
	{
		string directoryName = splitted[currentIndex];
		directory = directory->findDirectory(directoryName, directory);

		currentIndex++;
	}

	return directory;
}

//Method for deleting the file and directory children of the directory we are passing as parameter
void FileSystem::deleteDirectory(Directory*& directory)
{
	if (directory->getFileChildren().size() == 0 &&
		directory->getDirectoriesChildren().size() == 0) {
		return;
	}
	
	directory->clearFileChildren();

	for (int i = 0; i < directory->getDirectoriesChildren().size(); i++)
	{
		deleteDirectory(directory->getDirectoriesChildren()[i]);
		directory->~Directory();
	}
}

FileSystem::FileSystem()
{
	this->root = new Directory("root");
}

FileSystem::FileSystem(Directory& directory)
{
	this->root = new Directory(directory);
	
}

FileSystem::FileSystem(const FileSystem& other)
{
	this->copy(other);
}

FileSystem::~FileSystem()
{
	this->destroy();
}

Directory* FileSystem::getFileSystem()
{
	return this->root;
}

set<string> FileSystem::getObjectPaths()
{
	return this->objectPaths;
}

void FileSystem::mkdir(string path)
{
	//Elements of the path
	vector<string> splitted = this->paths(path);

	if (!validatePath(splitted) || containsPath(path))
	{
		std::cerr << "Invalid path! Try again." << std::endl;
		return;
	}
	
	//We add the new path to set of unique paths
	this->objectPaths.insert(path);

	//When creating a folder at the start for example : root\D:, root\C:
	if (splitted.size() == 2)
	{
		Directory* newDir = new Directory(splitted[1]);
		this->root->addDirectory(newDir);
		return;
	}

	int startIndex = 1;
	Directory* currentDirectory = this->root;

	while (startIndex != splitted.size() - 1)
	{
		bool found = false;
		vector<Directory*> currentDirectoryChildren = currentDirectory->getDirectoriesChildren();

		//Searching for the parent folder in which we are going to create the new one
		for (size_t i = 0; i < currentDirectoryChildren.size(); i++)
		{
			if (currentDirectoryChildren[i]->getName() == splitted[startIndex]) {
				currentDirectory = currentDirectoryChildren[i];
				found = true;
				break;
			}
		}

		if (!found)
		{
			std::cout << "Parent folder not found!" << std::endl;
			return;
		}

		startIndex++;
	}

	currentDirectory->addDirectory(new Directory(splitted[startIndex]));
}

void FileSystem::rmdir(string path)
{
	vector<string> splitted = this->paths(path);

	if (!validatePath(splitted) || !containsPath(path))
	{
		std::cerr << "Invalid path. Try again." << std::endl;
		return;
	}

	//We delete from the set of unique paths
	this->objectPaths.erase(path);

	Directory* directory = this->root;

	int currentIndex = 0;
	while (currentIndex != splitted.size())
	{
		string directoryName = splitted[currentIndex];
		directory = directory->findDirectory(directoryName, directory);

		currentIndex++;
	}

	string parentName = splitted[currentIndex - 2];
	Directory* parent = directory->findDirectory(parentName, this->root);

	deleteDirectory(directory);

	//We call this method to fully delete the directory from the parent after we have deleted its children
	parent->eraseFromDirectories(directory);
}

string FileSystem::ls(string path)
{
	vector<string> splitted = this->paths(path);

	if (!validatePath(splitted) || !containsPath(path))
	{
		return "Invalid path";
	}

	Directory* directory = this->root;

	int currentIndex = 0;
	while (currentIndex != splitted.size())
	{
		string directoryName = splitted[currentIndex];
		directory = directory->findDirectory(directoryName, directory);

		currentIndex++;
	}

	if (directory == nullptr)
		return "Invalid path!";

	stringstream stringBuilder;

	for (int i = 0; i < directory->getDirectoriesChildren().size(); i++)
	{
		Directory* current = directory->getDirectoriesChildren()[i];

		stringBuilder << current->getName() << " "
			<< current->getDate() << "\n";

	}

	for (int i = 0; i < directory->getFileChildren().size(); i++)
	{
		File current = directory->getFileChildren()[i];

		stringBuilder << current.showFileName() << " "
			<< current.getDate() << "\n";
	}

	return stringBuilder.str();
}

void FileSystem::cp(string path)
{
	vector<string> splitted = this->paths(path);
	//We call buildPath so we can change the path and pass it to the set with ending .txt
	string refactoredPath = buildPath(splitted, splitted[splitted.size() - 1] + ".txt");

	if (!validatePath(splitted) || !containsPath(refactoredPath))
	{
		std::cerr << "Invalid path. Try again!" << std::endl;
		return;
	}

	string fileName = splitted[splitted.size() - 1];

	Directory* directory = returnCurrentDirectory(this->root, splitted);

	int indexAt = directory->returnFileIndex(fileName);

	if (indexAt == -1)
	{
		//File not found
		return;
	}
	else
	{
		string copyName = directory->copyFile(directory->getFileChildren()[indexAt]);
		vector<string> pathElements;

		for (int i = 0; i < splitted.size(); i++)
		{
			pathElements.push_back(splitted[i]);
		}

		//we insert the copied file path to the unique paths
		this->objectPaths.insert(buildPath(pathElements, copyName + ".txt"));
	}
}

string FileSystem::rm(string path)
{
	vector<string> splitted = this->paths(path);
	string refactoredPath = buildPath(splitted, splitted[splitted.size() - 1] + ".txt");


	if (!validatePath(splitted) || !containsPath(refactoredPath))
		return "File not found!";

	this->objectPaths.erase(refactoredPath);

	string fileName = splitted[splitted.size() - 1];

	Directory* directory = returnCurrentDirectory(this->root, splitted);

	int indexAt = directory->returnFileIndex(fileName);

	directory->deleteFile(directory->getFileChildren()[indexAt]);

	return "File deleted!";
}

string FileSystem::cat(string path)
{
	vector<string> splitted = this->paths(path);
	string refactoredPath = buildPath(splitted, splitted[splitted.size() - 1] + ".txt");

	if (!validatePath(splitted) || !containsPath(refactoredPath))
		return "Invalid path!";

	string fileName = splitted[splitted.size() - 1];

	Directory* directory = returnCurrentDirectory(this->root, splitted);

	int indexAt = directory->returnFileIndex(fileName);

	if (indexAt == -1)
		return "File not found!";
	else
		return directory->getFileChildren()[indexAt].printContent();
}

void FileSystem::write(string path, vector<string> content)
{
	vector<string> splitted = this->paths(path);
	string refactoredPath = buildPath(splitted, splitted[splitted.size() - 1] + ".txt");

	if (!validatePath(splitted))
		return;

	if (!containsPath(refactoredPath))
	{
		//If we haven't created the file, we insert its path to the set of unique paths
		this->objectPaths.insert(refactoredPath);
	}

	string fileName = splitted[splitted.size() - 1];

	Directory* directory = returnCurrentDirectory(this->root, splitted);

	int indexAt = directory->returnFileIndex(fileName);
	string text = buildText(content);

	if (indexAt != -1)
	{
		//If the file is already existing, we reset its content
		directory->resetFile(indexAt, text);
	}
	else
	{
		//Here we create the new file and pass it as a child to its parent directory

		File newFile;
		newFile.setName(fileName);
		newFile.setContent(text);

		directory->addFile(newFile);
	}
}

void FileSystem::append(string path, vector<string> content)
{
	vector<string> splitted = this->paths(path);
	string refactoredPath = buildPath(splitted, splitted[splitted.size() - 1] + ".txt");

	if (!validatePath(splitted) || !containsPath(refactoredPath))
		return;

	string fileName = splitted[splitted.size() - 1];

	Directory* directory = returnCurrentDirectory(this->root, splitted);

	int indexAt = directory->returnFileIndex(fileName);
	string text = buildText(content);

	if (indexAt == -1)
	{
		//If not existing, we create the file
		directory->addFile(File(fileName));
	}
	else
	{
		//We append the new content
		directory->appendTextToFile(indexAt, text);
	}
}

FileSystem& FileSystem::operator=(FileSystem& other)
{
	if (this != &other)
	{
		this->destroy();
		this->copy(other);
	}

	return *this;
}
