#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <FrameWork/Define/Define.h>

class SearchFile
{
public:
	/* @brief	コンストラクタ		*/
	SearchFile(void)  {}
	/* @brief	デストラクタ		*/
	~SearchFile(void) {}

	/* @brief	検索
	 * @param	(directory)		検索するディレクトリ名
	 * @param	(extension)		検索する拡張子
	 * @param	(list)			検索結果リスト		*/
	void Search(const string& directory, const string& extension, std::vector<string>& list);

private:
	/* @brief	実際の検索
	 * @sa		Search()
	 * @param	(fileName)	検索するファイル名
	 * @param	(directory)	検索するディレクトリ名
	 * @param	(list)		検索結果リスト			*/
	void SearchAct(const string& fileName, const string& directory, std::vector<string>& list);
};

#endif // _SEARCH_H_
