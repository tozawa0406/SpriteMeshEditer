#include "Dx11Utility.h"

HRESULT TextureResource::Create(ID3D11Device* device)
{
	// �f�o�C�X��null�`�F�b�N
	if (!device) { return E_FAIL; }
	device_ = device;
	
	// ���O�̐ݒ�s��
	if (name_ == "") { return E_FAIL; }


	// �t�@�C�����J���Ċi�[
	std::wstring name(name_.begin(), name_.end());

	ID3D11Resource* descOriginal;
	HRESULT hr = DirectX::CreateWICTextureFromFile(device_, name.c_str(), &descOriginal, &resource_);
	if (FAILED(hr)) { return hr; }

	// �e�N�X�`���T�C�Y�̎擾
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
