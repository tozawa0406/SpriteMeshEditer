#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <FrameWork/Define/Define.h>

class SearchFile
{
public:
	/* @brief	�R���X�g���N�^		*/
	SearchFile(void)  {}
	/* @brief	�f�X�g���N�^		*/
	~SearchFile(void) {}

	/* @brief	����
	 * @param	(directory)		��������f�B���N�g����
	 * @param	(extension)		��������g���q
	 * @param	(list)			�������ʃ��X�g		*/
	void Search(const string& directory, const string& extension, std::vector<string>& list);

private:
	/* @brief	���ۂ̌���
	 * @sa		Search()
	 * @param	(fileName)	��������t�@�C����
	 * @param	(directory)	��������f�B���N�g����
	 * @param	(list)		�������ʃ��X�g			*/
	void SearchAct(const string& fileName, const string& directory, std::vector<string>& list);
};

#endif // _SEARCH_H_
