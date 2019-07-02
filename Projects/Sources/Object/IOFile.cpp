#include "IOFile.h"

IOFile::IOFile(void) :
	current_(0),
	end_(0)
{
}

IOFile::~IOFile(void)
{
}

bool IOFile::OpenFile(const string& fileName, int ios)
{
	fs_.open(fileName.c_str(), ios | std::ios::binary);

	if (fs_.fail()) { return false; }
	return true;
}

void IOFile::CloseFile(void)
{
	if (fs_) { fs_.close(); }
}

void IOFile::WriteParam(const void* param, size_t size)
{
	if (fs_) { fs_.write((const char*)param, size); }
}

void IOFile::ReadParam(void* param, size_t size)
{
	if (fs_) 
	{
		fs_.read((char*)param, size); 
	}
}
