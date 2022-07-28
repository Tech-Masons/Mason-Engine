#pragma once
#include "cdefs.h"
#include "cpch.h"

enum FileMode : uint8_t
{
	READ,
	WRITE,
	APPEND
};

enum FileType
{
	UNDEFINED = -1,
	DATA_FILE,
	FONT_FILE,
	RAW_DATA_FILE,
	IMAGE_FILE,
	SOUND_FILE,
	SHADER_FILE,
};

class File
{
private:
	std::string actualPath, relativePath;
	FileMode mode;
	FILE* stream;
	FileType fileType;
public:
	File(std::string actualPath, FileMode mode);
	File(std::string actualPath, std::string relativePath, FileMode mode);
	~File();

	FileType GetFileType() const;
	std::string GetActualPath() const;
	std::string GetRelativePath() const;
	bool Open();
	void Close();
	size_t GetFileSize();
	std::string ReadString();
	std::vector<uint8_t> ReadRaw();
	template <typename T>
	void Write(T data);
};

template<typename T>
inline void File::Write(T data)
{
	fwrite(&data[0], sizeof(T), 1, stream);
}