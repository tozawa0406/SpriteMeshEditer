#ifndef _IO_FILE_H_
#define _IO_FILE_H_

#include <FrameWork/Define/Define.h>

#include <iostream>
#include <fstream>

class IOFile
{
public:
	/* @brief	�R���X�g���N�^		*/
	IOFile(void);
	/* @brief	�f�X�g���N�^		*/
	~IOFile(void);

	/* @brief	�t�@�C���I�[�v��
	 * @param	(fileName)	�t�@�C����
	 * @param	(ios)		std::ios::in or std::ios::out
	 * @return	����				*/
	bool OpenFile(const string& fileName, int ios);

	/* @brief	�t�@�C���N���[�Y	*/
	void CloseFile(void);

	/* @brief	�t�H�C���ւ̏�������
	 * @param	(param)		�������ޓ��e�̃|�C���^
	 * @param	(size)		�o�C�g��	*/
	void WriteParam(void* param, size_t size);
	
	/* @brief	�t�H�C������̓Ǎ�
	 * @param	(param)		�ǂݍ��ޓ��e�̃|�C���^
	 * @param	(size)		�o�C�g��	*/
	void ReadParam(void* param, size_t size);

private:
	//! �t�@�C���|�C���^
	std::fstream fs_;
};

#endif // _IO_FILE_H_
