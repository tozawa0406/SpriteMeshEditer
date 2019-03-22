#ifndef _DX11_UTILITY_
#define _DX11_UTILITY_

#include "../../Utility/Utility.h"
#include "../Dx11Include.h"

class Dx11Device : public IDevice
{
public:
	Dx11Device(void) : device_(nullptr) {}
	virtual ~Dx11Device(void) {}

	void Release(void) {}
	ID3D11Device* GetDevice(void) { return device_; }
	void SetDevice(ID3D11Device* device) { device_ = device; }

	virtual HRESULT Load(ITextureResource** resource, const string& name) override;

private:
	ID3D11Device* device_;
};

class TextureResource : public ITextureResource
{
public:
	TextureResource(void) : resource_(nullptr) {}
	virtual ~TextureResource(void) {}

	inline ID3D11ShaderResourceView* GetResource(void) { return resource_; }

	virtual HRESULT Load(IDevice* device, const string& name) override;
	virtual void	Release(void)	override;

private:
	ID3D11ShaderResourceView* resource_;
};

#endif // !_DX11_UTILITY_
