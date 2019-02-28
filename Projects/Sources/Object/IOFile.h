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

	/* @brief	フォイルへの書き込み
	 * @param	(param)		書き込む内容のポインタ
	 * @param	(size)		バイト数	*/
	void WriteParam(void* param, size_t size);
	
	/* @brief	フォイルからの読込
	 * @param	(param)		読み込む内容のポインタ
	 * @param	(size)		バイト数	*/
	void ReadParam(void* param, size_t size);

private:
	//! ファイルポインタ
	std::fstream fs_;
};

#endif // _IO_FILE_H_
