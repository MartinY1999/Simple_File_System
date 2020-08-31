#include <iostream>
#include "Directory.h"
#include "File.h"
#include "FileSystem.h"
#include "User.h"

using namespace std;

vector<string> split(string input);

int main()
{
    Directory* root = new Directory("PC");
    FileSystem fileSystem = FileSystem(*root);
    User user;

    string input;

    while (input != "close")
    {
        getline(cin, input);

        vector<string> splitted = split(input);
        vector<string> content;

        string command = splitted[0];

        if (command == "close")
            continue;

        if (splitted.size() <= 1)
        {
            std::cout << "Invalid input! Try again." << std::endl;
            continue;
        }

        string path = splitted[1];

        if (command == "mkdir")
            user.makeDirectory(fileSystem, path);
        if (command == "rmdir")
            user.removeDirectory(fileSystem, path);
        if (command == "ls")
            user.printDirectoryContent(fileSystem, path);
        if (command == "cp")
            user.copyFile(fileSystem, path);
        if (command == "rm")
            user.deleteFile(fileSystem, path);
        if (command == "cat")
            user.printFileContent(fileSystem, path);
        if (command == "write")
        {
            for (int i = 2; i < splitted.size(); i++)
                content.push_back(splitted[i]);

            user.writeTextToFile(fileSystem, path, content);
        }
        if (command == "append")
        {
            for (int i = 2; i < splitted.size(); i++)
                content.push_back(splitted[i]);

            user.appendTextToFile(fileSystem, path, content);
        }
    }

    user.close(fileSystem);
}

vector<string> split(string input)
{
    string delimiter = " ";

    size_t start = 0, end;
    string token;
    vector<string> result;

    while ((end = input.find(delimiter, start)) != string::npos)
    {
        token = input.substr(start, end - start);
        start = end + delimiter.length();
        result.push_back(token);
    }

    result.push_back(input.substr(start));

    return result;
}