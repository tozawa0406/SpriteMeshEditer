#include "Dx11Utility.h"

HRESULT Dx11Device::Load(ITextureResource** resource, const string& name)
{
	(*resource) = new TextureResource;
	if (resource)
	{
		return (*resource)->Load(this, name);
	}
	return E_FAIL;
}

HRESULT TextureResource::Load(IDevice* device, const string& name)
{
	// 名前の設定不備
	if (name == "") { return E_FAIL; }

	IResourece::Load(device, name);

	// デバイスのチェック
	Dx11Device* dev = static_cast<Dx11Device*>(device);
	if (!dev) { return E_FAIL; }	

	// ファイルを開いて格納
	std::wstring wname(name_.begin(), name_.end());

	ID3D11Resource* descOriginal;
	HRESULT hr = DirectX::CreateWICTextureFromFile(dev->GetDevice(), wname.c_str(), &descOriginal, &resource_);
	if (FAILED(hr)) { return hr; }

	// テクスチャサイズの取得
	D3D11_TEXTURE2D_DESC desc;
	static_cast<ID3D11Texture2D*>(descOriginal)->GetDesc(&desc);
	ReleasePtr(descOriginal);

	size_.x = (float)desc.Width;
	size_.y = (float)desc.Height;

	return S_OK;
}

void TextureResource::Release(void)
{
	ReleasePtr(resource_);
	device_ = nullptr;
}
