#include "Dx11Utility.h"

void D3D11Device::Release(void) { ReleasePtr(device_); IBaseUnknown::Release(); }

HRESULT D3D11Device::Load(ITextureResource** resource, const string& name)
{
	// 派生のみfriendクラスのため、一時的に派生クラスを扱わないといけない
	if (TextureResource* temp = new TextureResource)
	{
		*resource = temp;
		return temp->Load(this, name);
	}
	return E_FAIL;
}

HRESULT D3D11Device::CreateBuffer(IVertexBuffer** vertexBuffer, const void* vertex, uint size, uint vertexNum)
{
	// 派生のみfriendクラスのため、一時的に派生クラスを扱わないといけない
	if (VertexBuffer* temp = new VertexBuffer)
	{
		*vertexBuffer = temp;
		return temp->Create(this, vertex, size, vertexNum);
	}
	return E_FAIL;
}

HRESULT D3D11Device::CreateBuffer(IIndexBuffer** indexBuffer, const WORD* index, uint indexNum)
{
	// 派生のみfriendクラスのため、一時的に派生クラスを扱わないといけない
	if (IndexBuffer* temp = new IndexBuffer)
	{
		*indexBuffer = temp;
		return temp->Create(this, index, indexNum);
	}
	return E_FAIL;

}

HRESULT VertexBuffer::Create(IDevice* device, const void* vertex, uint size, uint vertexNum)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage				= D3D11_USAGE_DEFAULT;
	bd.ByteWidth			= size * vertexNum;
	bd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags		= 0;
	bd.MiscFlags			= 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem			= vertex;
	data.SysMemPitch		= 0;
	data.SysMemSlicePitch	= 0;

	size_ = size;
	
	HRESULT hr = E_FAIL;
	D3D11Device* dev = static_cast<D3D11Device*>(device);
	if (ID3D11Device* dev11 = dev->GetDevice())
	{
		hr = dev11->CreateBuffer(&bd, &data, &buffer_);
		if (SUCCEEDED(hr))
		{
			return S_OK;
		}
	}
	ReleasePtr(buffer_);
	return hr;
}

void VertexBuffer::Release(void) { ReleasePtr(buffer_); IBaseUnknown::Release(); }

HRESULT IndexBuffer::Create(IDevice* device, const WORD* index, uint indexNum)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage			= D3D11_USAGE_DEFAULT;
	bd.ByteWidth		= sizeof(WORD) * indexNum;
	bd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags	= 0;
	bd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem			= index;
	data.SysMemPitch		= 0;
	data.SysMemSlicePitch	= 0;

	HRESULT hr = E_FAIL;
	D3D11Device* dev = static_cast<D3D11Device*>(device);
	if (ID3D11Device* dev11 = dev->GetDevice())
	{
		hr = dev11->CreateBuffer(&bd, &data, &buffer_);
		if (SUCCEEDED(hr))
		{
			return S_OK;
		}
	}
	ReleasePtr(buffer_);
	return hr;
}

void IndexBuffer::Release(void) { ReleasePtr(buffer_); IBaseUnknown::Release(); }

HRESULT TextureResource::Load(IDevice* device, const string& name)
{
	// 名前の設定不備
	if (name == "") { return E_FAIL; }
	name_ = name;

	// デバイスのチェック
	D3D11Device* dev = static_cast<D3D11Device*>(device);
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

void TextureResource::Release(void) { ReleasePtr(resource_); IBaseUnknown::Release(); }
