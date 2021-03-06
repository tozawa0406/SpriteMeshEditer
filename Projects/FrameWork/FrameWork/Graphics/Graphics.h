/*
 * @file		Graphics.h
 * @brief		グラフィックスAPIの処理
 * @author		戸澤翔太
 * @data		2018/08/18
 */
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../Define/Define.h"
#include "Wrapper.h"
#include "RenderTarget.h"
#include "Utility/Utility.h"

class Graphics
{
	friend class Windows;
	friend class Systems;

public:	
	//! @enum	グラフィックスの種類
	enum class Type
	{
		UNKNOWN  = -1,
		DirectX9 = 0,
		DirectX11,
		OpenGL,
		MAX
	};	

	//! @def	横解像度
	static constexpr int WIDTH  = 1920;
	//! @def	縦解像度
	static constexpr int HEIGHT = 1080;

	/* @brief	デバイス取得				*/
	inline IDevice*			GetDevice(void)			{ return device_;		}
	/* @brief	Windowsクラスの取得			*/
	inline Windows*			GetWindow(void)			{ return window_;		}
	/* @brief	Wrapperクラスの取得			*/
	inline Wrapper*			GetWrapper(void)		{ return wrapper_;		}
	/* @brief	RenderTargetクラスの取得	*/
	inline RenderTarget*	GetRenderTarget(void)	{ return renderTarget_; }

	/* @brief	後処理		*/
	virtual void	Uninit(void) = 0;

	/* @brief	Guiの更新処理		*/
	virtual void GuiUpdate(void) {}

protected:
	/* @brief	コンストラクタ		*/
	Graphics(void) : window_(nullptr), device_(nullptr), wrapper_(nullptr), renderTarget_(nullptr) {}

	/* @brief	Windowsクラスの設定処理		*/
	void SetWindows(Windows* window) { window_ = window; }

	/* @brief	初期化処理		*/
	virtual HRESULT Init(void)      = 0;
	/* @brief	描画開始処理	*/
	virtual HRESULT DrawBegin(void) = 0;
	/* @brief	描画終了処理	*/
	virtual void	DrawEnd(void)	= 0;

	//! デバイス
	IDevice* device_;

	//! Windowsクラスのポインタ
	Windows*		window_;
	//! Wrapperクラスのポインタ
	Wrapper*		wrapper_;
	//! RenderTargetクラスのポインタ
	RenderTarget*	renderTarget_;
};

#endif // _GRAPHICS_H