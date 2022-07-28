#include "FileSystem.h"
#include <filesystem>

namespace fs = std::filesystem;

FileSystem::FileSystem(std::string head)
{
	this->head = head;
	Setup();
}

FileSystem::~FileSystem()
{
	Close();
}

void FileSystem::Setup()
{
	for (FileSystem* folder : folders)
	{
		folder->Setup();
	}

	for (fs::directory_entry entry : fs::directory_iterator(head.c_str()))
	{
		std::string path = entry.path().string();
		if (entry.is_directory()) {
			FileSystem* fileSystem = new FileSystem(path.c_str());
			folders.push_back(fileSystem);
		} else if (entry.is_regular_file()) {
			File* file = new File(path, path.substr(head.length() + 1, path.length()), FileMode::READ);
			file->Open();
			files.push_back(file);
		}
	}
}

void FileSystem::Close()
{
	for (File* file : files)
	{
		delete file;
	}

	for (FileSystem* folder : folders)
	{
		delete folder;
	}
}

std::string FileSystem::GetHead() const
{
	return head;
}

std::vector<File*> FileSystem::GetFiles() const
{
	return files;
}

std::vector<FileSystem*> FileSystem::GetFolders() const
{
	return folders;
}

std::string FileSystem::GetCompletePath(std::string path)
{
	return head + "\\" + path;
}

FileSystem* FileSystem::AddFolder(std::string name)
{
	std::string path = GetCompletePath(name);
	fs::create_directory(path);
	FileSystem* fileSystem = new FileSystem(path);
	folders.push_back(fileSystem);
	return fileSystem;
}

FileSystem* FileSystem::GetFolder(std::string name)
{
	for (FileSystem* folder : folders)
	{
		if (folder->GetHead() == GetCompletePath(name))
			return folder;
	}
	return nullptr;
}

File* FileSystem::AddFile(std::string name, FileMode mode)
{
	File* file = new File(GetCompletePath(name), mode);
	file->Open();
	files.push_back(file);
	return file;
}

File* FileSystem::GetFile(std::string name)
{
	for (File* file : files)
	{
		if (file->GetActualPath() == GetCompletePath(name))
			return file;
	}
	return nullptr;
}

void FileSystem::DebugPrint()
{
	for (FileSystem* folder : folders)
	{
		std::cout << folder->GetHead() << std::endl;
		folder->DebugPrint();
	}

	for (File* file : files)
	{
		std::cout << file->GetActualPath() << " " << file->GetRelativePath() << " " << file->GetFileType() << " " << file->GetFileSize() << std::endl;
	}
}