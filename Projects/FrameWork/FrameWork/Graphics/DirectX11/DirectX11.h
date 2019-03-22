//-----------------------------------------------------------------------------
//
//	DirectX11�̏���[DirectX11.h]
//	Auther : ���V�đ�
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _DIREXT_X11_H_
#define _DIRECT_X11_H_

#include "Dx11Include.h"
#include "../Graphics.h"
#include "Dx11Utility/Dx11Utility.h"

//-----------------------------------------------------------------------------
//	�N���X��`
//-----------------------------------------------------------------------------
class DirectX11 : public Graphics
{
	friend Windows;
public:
	ID3D11Device*        GetD3D11Device(void)	{ return (device_)?static_cast<D3D11Device*>(device_)->GetDevice() : nullptr; }		// �f�o�C�X�̎󂯓n��
	ID3D11DeviceContext* GetDeviceContext(void) { return pDeviceContext_; }		// �R���e�L�X�g�̎󂯓n��
	IDXGISwapChain*		 GetSwapChain(void)		{ return pSwapChain_;     }

	inline const D3D11_VIEWPORT& GetViewport(void) { return viewport_; }

	void    Uninit(void)    override;		// �㏈��
private:
	DirectX11(void);

	HRESULT Init(void)      override;		// ����������
	HRESULT DrawBegin(void) override;		// �`��J�n
	void    DrawEnd(void)   override;		// �`��I��

	HRESULT InitAll(void);			// �S�Ă̏���������
	bool    SetDevice(void);		// �f�o�C�X�̐ݒ�

	ID3D11RenderTargetView* GetRenderTargetView(void) { return pRenderTargetView_; }
	ID3D11DepthStencilView* GetDepthStencilView(void) { return pDepthStencilView_; }

	IDXGISwapChain*         pSwapChain_;
	ID3D11DeviceContext*    pDeviceContext_;		// GPU�̕�
	ID3D11RenderTargetView* pRenderTargetView_;
	ID3D11DepthStencilView* pDepthStencilView_;

	D3D11_VIEWPORT viewport_;

	friend class Dx11RenderTarget;
};

#endif // _DIRECT_X11_H