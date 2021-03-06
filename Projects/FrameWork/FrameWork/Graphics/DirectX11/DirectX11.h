//-----------------------------------------------------------------------------
//
//	DirectX11の処理[DirectX11.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIREXT_X11_H_
#define _DIRECT_X11_H_

#include "Dx11Include.h"
#include "../Graphics.h"
#include "Dx11Utility/Dx11Utility.h"

//-----------------------------------------------------------------------------
//	クラス定義
//-----------------------------------------------------------------------------
class DirectX11 : public Graphics
{
	friend Windows;
public:
	ID3D11Device*        GetD3D11Device(void)	{ return (device_)?static_cast<D3D11Device*>(device_)->GetDevice() : nullptr; }		// デバイスの受け渡し
	ID3D11DeviceContext* GetDeviceContext(void) { return pDeviceContext_; }		// コンテキストの受け渡し
	IDXGISwapChain*		 GetSwapChain(void)		{ return pSwapChain_;     }

	inline const D3D11_VIEWPORT& GetViewport(void) { return viewport_; }

	void    Uninit(void)    override;		// 後処理
private:
	DirectX11(void);

	HRESULT Init(void)      override;		// 初期化処理
	HRESULT DrawBegin(void) override;		// 描画開始
	void    DrawEnd(void)   override;		// 描画終了

	HRESULT InitAll(void);			// 全ての初期化処理
	bool    SetDevice(void);		// デバイスの設定

	ID3D11RenderTargetView* GetRenderTargetView(void) { return pRenderTargetView_; }
	ID3D11DepthStencilView* GetDepthStencilView(void) { return pDepthStencilView_; }

	IDXGISwapChain*         pSwapChain_;
	ID3D11DeviceContext*    pDeviceContext_;		// GPUの方
	ID3D11RenderTargetView* pRenderTargetView_;
	ID3D11DepthStencilView* pDepthStencilView_;

	D3D11_VIEWPORT viewport_;

	friend class Dx11RenderTarget;
};

#endif // _DIRECT_X11_H