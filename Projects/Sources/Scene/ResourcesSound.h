/*
 * @file		ResourcesSound.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_SOUND_H_
#define _RESOURCES_SOUND_H_

#include <FrameWork/Define/Define.h>

struct SOUNDPARAM
{
	string pFilename;	// ファイル名
	int cntLoop;		// ループカウント
};

namespace Resources
{
	class Sound
	{
		const string directoryName = Define::ResourceDirectoryName + "Sound/";
		const string BGMDirectoryName = directoryName + "BGM/";
		const string SEDirectoryName = directoryName + "SE/";
	public:
		enum class Base : int8
		{
			UNOWN = -1,
			SE_SELECT = 0,
			SE_ENTER,
			SE_CANCEL,

			MAX
		};

		enum class Title : int8
		{
			MAX = (int)Base::MAX
		};

		enum class Camp : int8
		{
			MAX = (int)Base::MAX
		};

		enum class Buttle : int8
		{
			MAX = (int)Base::MAX
		};

		enum class Result : int8
		{
			MAX = (int)Base::MAX
		};

		const SOUNDPARAM baseFileName[(int)Base::MAX]
		{
			{ SEDirectoryName + "select.wav"	, 0 },
			{ SEDirectoryName + "enter.wav"		, 0 },
			{ SEDirectoryName + "cancel.wav"	, 0 },
		};

		//const SOUNDPARAM titleFileName[(int)Title::MAX - (int)Base::MAX]
		//{
		//};

		//const SOUNDPARAM campFileName[(int)Camp::MAX - (int)Base::MAX]
		//{
		//};

		//const SOUNDPARAM buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		//{
		//};

		//const SOUNDPARAM resultFileName[(int)Result::MAX - (int)Base::MAX]
		//{
		//};
	};
}

#endif // _RESOURCES_SOUND_H_
