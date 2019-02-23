#include "Search.h"

void SearchFile::Search(const string& directory, const string& extension, std::vector<string>& list)
{
	string searchFile = directory;
	searchFile += "*.";
	searchFile += extension;

	SearchAct(searchFile, directory, list);
}

void SearchFile::SearchAct(const string& fileName, const string& directory, std::vector<string>& list)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	hFind = FindFirstFile(fileName.c_str(), &win32fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				MessageBox(NULL, "Error", "Error", MB_OK);
			}
			else
			{
				string search = directory;
				search += win32fd.cFileName;
				list.emplace_back(search);
			}
		} while (FindNextFile(hFind, &win32fd));
	}
	FindClose(hFind);
}
