#include "File.h"

File::File(std::string path, FileMode mode)
{
	this->path = path;
	this->mode = mode;
}

File::~File()
{
	Close();
}

std::string File::GetPath() const
{
	return path;
}

bool File::Open()
{
	std::string streamMode;

	switch (mode)
	{
	case FileMode::APPEND:
		streamMode = "a";
		break;
	case FileMode::READ:
		streamMode = "r";
		break;
	case FileMode::WRITE:
		streamMode = "w";
		break;
	}

	stream = fopen(path.c_str(), streamMode.c_str());
	return stream != nullptr;
}

void File::Close()
{
	fclose(stream);
}

void File::Write(const std::string& str)
{
	fwrite(str.c_str(), str.length(), 1, stream);
}

void File::Write(const std::vector<uint8_t>& buffer)
{
	fwrite((const char*)buffer.data(), buffer.size(), 1, stream);
}

std::string File::ReadString()
{
	std::string str;
	fseek(stream, 0, SEEK_END);
	size_t length = ftell(stream);
	rewind(stream);
	str.resize(length + 1);
	fread(str.data(), length, 1, stream);
	str[length] = '\0';
	return str;
}

std::vector<uint8_t> File::ReadRaw()
{
	std::vector<uint8_t> buffer;
	fseek(stream, 0, SEEK_END);
	size_t length = ftell(stream);
	rewind(stream);
	buffer.resize(length);
	fread((char*)buffer.data(), length, 1, stream);
	return buffer;
}