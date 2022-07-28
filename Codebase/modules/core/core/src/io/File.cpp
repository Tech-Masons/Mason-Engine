#include "File.h"

File::File(std::string actualPath, FileMode mode)
{
	this->actualPath = actualPath;
	this->relativePath = "";
	this->mode = mode;
}

File::File(std::string actualPath, std::string relativePath, FileMode mode)
{
	this->actualPath = actualPath;
	this->relativePath = relativePath;
	this->mode = mode;
}

File::~File()
{
	Close();
}

FileType File::GetFileType() const
{
	return fileType;
}

std::string File::GetActualPath() const
{
	return actualPath;
}

std::string File::GetRelativePath() const
{
	return relativePath;
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

	// Ugly if statements, no way around them
	// TODO: Add more file type extension designations
	if (actualPath.contains(".txt") || actualPath.contains(".xml"))
		fileType = FileType::DATA_FILE;
	else if (actualPath.contains(".ttf") || actualPath.contains(".otf"))
		fileType = FileType::FONT_FILE;
	else if (actualPath.contains(".bin") || actualPath.contains(".raw"))
		fileType = FileType::RAW_DATA_FILE;
	else if (actualPath.contains(".bmp") || actualPath.contains(".png") || actualPath.contains(".jpg") || actualPath.contains(".hdr"))
		fileType = FileType::IMAGE_FILE;
	else if (actualPath.contains(".wav") || actualPath.contains(".ogg") || actualPath.contains(".mp3"))
		fileType = FileType::SOUND_FILE;
	if (actualPath.contains(".glsl") || actualPath.contains(".hlsl"))
		fileType = FileType::SHADER_FILE;
	else
		fileType = FileType::UNDEFINED;

	stream = fopen(actualPath.c_str(), streamMode.c_str());
	return stream != nullptr;
}

void File::Close()
{
	fclose(stream);
}

size_t File::GetFileSize()
{
	fseek(stream, 0, SEEK_END);
	size_t size = ftell(stream);
	rewind(stream);
	return size;
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