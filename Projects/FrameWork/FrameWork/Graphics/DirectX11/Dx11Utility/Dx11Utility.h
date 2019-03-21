#ifndef _DX11_UTILITY_
#define _DX11_UTILITY_

#include "../../Utility/Utility.h"
#include "../Dx11Include.h"

class IDx11Base
{
public:
	IDx11Base(void) : device_(nullptr) {}
	virtual ~IDx11Base(void) {}

	virtual HRESULT Create(ID3D11Device* device) = 0;

protected:
	ID3D11Device* device_;
};

class TextureResource : public ITextureResource, IDx11Base
{
public:
	TextureResource(void) : resource_(nullptr) {}
	virtual ~TextureResource(void) {}

	inline ID3D11ShaderResourceView* GetResource(void) { return resource_; }

	virtual HRESULT Create(ID3D11Device* device) override;
	virtual void	Release(void)	override;

private:
	ID3D11ShaderResourceView* resource_;
};

#endif // !_DX11_UTILITY_
