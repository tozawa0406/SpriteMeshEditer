/*
 * @file		ResourcesModel.h
 * @brief		リソース管理ファイル
 * @author		戸澤翔太
 * @data		2018/01/17
 */
#ifndef _RESOURCES_MODEL_H_
#define _RESOURCES_MODEL_H_

#include <FrameWork/Define/Define.h>

struct ANIMATION_INFO
{
	string	fileName;
	int		parent;
};

namespace Resources
{
	class Model
	{
		const string directoryName = Define::ResourceDirectoryName + "Model/";
	public:
		enum class Base : int8
		{
			UNOWN = -1,
			MAX,
		};

		enum class Title : int8
		{
			MAX = (int)Base::MAX,
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
			MAX = (int)Base::MAX,
		};

		class Animation
		{
		public:
			enum class Base : int8
			{
				UNOWN = -1,
				MAX,
			};

			enum class Title : int8
			{
				MAX = (int)Base::MAX,
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
				MAX = (int)Base::MAX,
			};
		};

	//const string baseFileName[(int)Base::MAX]
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

	//const ANIMATION_INFO animationCampFileName[(int)Animation::Camp::MAX]
	//{
	//};
	
	//const ANIMATION_INFO animationButtleFileName[(int)Animation::Buttle::MAX]
	//{
	//};

	};
}

#endif // _RESOURCES_MODEL_H_
