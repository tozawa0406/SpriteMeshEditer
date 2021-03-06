/*
 * @file	Define.h
 * @brief	定数、構造体、関数定義ヘッダ
 * @author	戸澤翔太
 * @date	2018/10/12
 */
#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <Windows.h>

#include <type_traits>
#include <XInput.h>
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h> 
#include <crtdbg.h>

//-----------------------------------------------------------------------------
//	定数定義
//-----------------------------------------------------------------------------
namespace Define
{
	// @def リソースディレクトリへのパス
	static const string ResourceDirectoryName = "./Resource/";
};

typedef char int8;
typedef unsigned char uint8;
typedef unsigned int  uint;

#ifdef _INC_LIMITS
#else
static constexpr uint UINT_MAX = 0xffffffff;
#endif
static constexpr float PI = 3.14f;

static constexpr float LAYER_DIST = -0.0001f;

//-----------------------------------------------------------------------------
//	構造体定義
//-----------------------------------------------------------------------------
struct VERTEX2D
{
	VECTOR4 position;
	COLOR   color;
	VECTOR2 texcoord;
};

struct VERTEX3D
{
	VECTOR3 position;
	VECTOR3 normal;
	COLOR   color;
	VECTOR2 texcoord;
};
struct VERTEXPOINT
{
	VECTOR3	position;
	float	size;
	COLOR	color;
};
struct VERTEX
{
	VECTOR3 position;
	VECTOR3 normal;
	VECTOR3 tangent;
	COLOR   color;
	VECTOR2 texcoord;
	VECTOR4 boneIndex;
	VECTOR4 weight;
};

//-----------------------------------------------------------------------------
//	関数定義
//-----------------------------------------------------------------------------
/* @fn		Half
 * @brief	半分
 * @param	(origin)	元の値
 * @return	半分にした値				*/
template<typename Num>
inline Num Half(Num origin) { return (Num)(origin * 0.5f); };
/* @fn		Quarter
 * @brief	1/4倍
 * @param	(origin)	元の値
 * @return	計算結果					*/
template<typename Num>
inline Num Quarter(Num origin) { return (Num)(origin * 0.25f); };
/* @fn		Abs
 * @brief	絶対値
 * @param	(origin)	元の値
 * @return	絶対値						*/
template<typename Num>
inline Num Abs(Num origin) { return origin >= 0 ? origin : -origin; };

/* @fn		DeletePtr
 * @brief	セーフdelete 
 * @param	(ptr)	ポインタ
 * @return	成功か失敗か		*/
template<class Ptr>
inline bool DeletePtr(Ptr& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		UninitDeletePtr
 * @brief	セーフdelete、&その前に後処理
 * @param	(ptr)	ポインタ
 * @return	成功か失敗か		*/
template<class Ptr>
inline bool UninitDeletePtr(Ptr& ptr)
{
	if (ptr)
	{
		ptr->Uninit();
		delete ptr;
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		ReleaseDeletePtr
 * @brief	セーフdelete、&その前にRelease処理
 * @param	(ptr)	ポインタ
 * @return	成功か失敗か		*/
template<class Ptr>
inline bool ReleasePtr(Ptr& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
		return true;
	}
	return false;
};

/* @fn		RemoveVectorPtr
 * @brief	vectorからerase、&その後delete処理
 * @param	(obj)		vector
 * @param	(_this)		取り出す配列
 * @return	成功か失敗か		*/
template<class T, class U>
inline bool RemoveVectorPtr(T& obj, U& _this)
{
	for (auto itr = obj.begin(); itr != obj.end();)
	{
		if ((*itr) == _this)
		{
 			auto temp = *itr;
			itr = obj.erase(itr);		//配列削除
			delete temp;
			temp = nullptr;
			break;
		}
		else
		{
			itr++;
		}
	}
	if (_heapchk() != _HEAPOK)
		DebugBreak();

	obj.shrink_to_fit();
	return true;
}

/* @fn		RemoveVector
 * @brief	vectorからerase、deleteはしない
 * @param	(obj)		vector
 * @param	(_this)		取り出す配列
 * @return	成功か失敗か		*/
template<class T, class U>
inline bool RemoveVector(T& obj, U& _this)
{
	for (auto itr = obj.begin(); itr != obj.end();)
	{
		if ((*itr) == _this)
		{
			itr = obj.erase(itr);		//配列削除
			break;
		}
		else
		{
			itr++;
		}
	}

	obj.shrink_to_fit();
	return true;
}

/* @fn		Swap
 * @brief	配列入れ替え処理
 * @param	(obj1, obj2)	入れ替える値		*/
template<class T>
inline void Swap(T& obj1, T& obj2)
{
	T temp = obj1;
	obj1 = obj2;
	obj2 = temp;
}

/* @brief	＆演算
 * @param	(var)	フラグ
 * @param	(bit)	チェックフラグ
 * @return	真ならtrue				*/
template<class T>
inline bool BitCheck(const T& var, const T& bit) { return (var & bit) ? true : false; }

/* @brief	XOR演算
 * @param	(var)	フラグ
 * @param	(bit)	チェックフラグ
 * @return	真ならtrue				*/
template<class T>
inline void BitSub(T& var, const T& bit) { (var & bit) ? var ^= bit : var; }

/* @brief	OR演算
 * @param	(var)	フラグ
 * @param	(bit)	チェックフラグ
 * @return	真ならtrue				*/
template<class T>
inline void BitAdd(T& var, const T& bit) { var |= bit; }

/* @brief	ビット演算によるboolの設定
 * @param	(judge)		判定するbool
 * @param	(flag)		値を設定するフラグ
 * @param	(bit)		フラグの値	*/
inline void BitSetFlag(bool judge, uint8& flag, uint8 bit) { (judge) ? BitAdd(flag, bit) : BitSub(flag, bit); }


#endif //_DEFINE_H_
