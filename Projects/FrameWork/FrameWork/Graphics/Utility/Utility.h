#ifndef _UTILTTY_
#define _UTILTTY_

#include "../../Define/Define.h"

class IBaseUnknown
{
public:
	IBaseUnknown(void) {}
	virtual ~IBaseUnknown(void) {}

	virtual void	Release(void) = 0;
};

class ITextureResource;
class IDevice : public IBaseUnknown
{
public:
	IDevice(void) {}
	virtual ~IDevice(void) {}

	virtual HRESULT Load(ITextureResource** resource, const string& name) = 0;
};

class IResourece : public IBaseUnknown
{
public:
	IResourece(void) : device_(nullptr), name_("") {}
	virtual ~IResourece(void) {}

	inline const string& GetName(void) { return name_; }
	virtual HRESULT Load(IDevice* device, const string& name) { device_ = device; name_ = name; return S_OK; }

protected:
	IDevice*	device_;
	string		name_;
};

class ITextureResource : public IResourece
{
public:
	ITextureResource(void) : size_(VECTOR2(0)) {}
	virtual ~ITextureResource(void) {}

	inline const VECTOR2& GetSize(void) { return size_; }

protected:
	VECTOR2 size_;
};

class IModelResource : public IResourece
{
public:
	IModelResource(void) {}
	virtual ~IModelResource(void) {}

	const std::vector<ITextureResource*>& GetTexture(void) { return texture_; }

protected:
	std::vector<ITextureResource*> texture_;
};

#endif // !_UTILTTY_
