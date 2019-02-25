/*
 * @file		ResourcesTexture.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_TEXTURE_H_
#define _RESOURCES_TEXTURE_H_

#include <FrameWork/Define/Define.h>

namespace Resources
{
	class Texture
	{
		const string directoryName = Define::ResourceDirectoryName + "Texture/";
	public:
		enum class Base : int8
		{
			UNOWN = -1,

			WHITE = 0,
			NUMBER,
			SSUI,
			LOAD,

			MAX
		};

		enum class Title : int8
		{
			MAX = (int)Base::MAX
		};

		const string baseFileName[(int)Base::MAX]
		{
			{ directoryName + "white.jpg"				},
			{ directoryName + "UI/number.png"			},
			{ directoryName + "UI/ssUI.jpg"				},
			{ directoryName + "load.png"				},
		};

		//const string titleFileName[(int)Title::MAX - (int)Base::MAX]
		//{
		//};

		//const string campFileName[(int)Camp::MAX - (int)Base::MAX]
		//{
		//};

		//const string buttleFileName[(int)Buttle::MAX - (int)Base::MAX]
		//{
		//};

		//const string resultFileName[(int)Result::MAX - (int)Base::MAX]
		//{
		//};
	};
}

#endif // _RESOURCES_TEXTURE_H_
