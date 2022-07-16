#pragma once
#include "cdefs.h"
#include "cpch.h"

enum FileMode : uint8_t
{
	READ,
	WRITE,
	APPEND
};

class File
{
private:
	std::string path;
	FileMode mode;
	FILE* stream;
public:
	File(std::string path, FileMode mode);
	~File();

	std::string GetPath() const;
	bool Open();
	void Close();
	void Write(const std::string& str);
	void Write(const std::vector<uint8_t>& buffer);
	std::string ReadString();
	std::vector<uint8_t> ReadRaw();
};