#ifndef _IO_FILE_H_
#define _IO_FILE_H_

#include <FrameWork/Define/Define.h>

#include <iostream>
#include <fstream>

class IOFile
{
public:
	/* @brief	コンストラクタ		*/
	IOFile(void);
	/* @brief	デストラクタ		*/
	~IOFile(void);

	/* @brief	ファイルオープン
	 * @param	(fileName)	ファイル名
	 * @param	(ios)		std::ios::in or std::ios::out
	 * @return	成否				*/
	bool OpenFile(const string& fileName, int ios);

	/* @brief	ファイルクローズ	*/
	void CloseFile(void);

	void WriteParam(void* param, size_t size);

	void ReadParam(void* param, size_t size);



private:
	std::fstream fs_;
};

#endif // _IO_FILE_H_
