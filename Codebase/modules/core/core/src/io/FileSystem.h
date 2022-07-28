#pragma once
#include "File.h"

class FileSystem
{
private:
	std::vector<File*> files;
	std::vector<FileSystem*> folders;
	std::string head;
public:
	FileSystem(std::string head = "./");
	~FileSystem();
	
	void Setup();
	void Close();
	std::string GetHead() const;
	std::vector<File*> GetFiles() const;
	std::vector<FileSystem*> GetFolders() const;
	std::string GetCompletePath(std::string path);
	FileSystem* AddFolder(std::string name);
	FileSystem* GetFolder(std::string name);
	File* AddFile(std::string name, FileMode mode = FileMode::WRITE);
	File* GetFile(std::string name);

	void DebugPrint();
};