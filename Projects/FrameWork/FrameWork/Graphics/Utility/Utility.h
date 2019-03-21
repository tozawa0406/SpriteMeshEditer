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

class IResourece : public IBaseUnknown
{
public:
	IResourece(void) : name_("") {}
	virtual ~IResourece(void) {}

	inline const string& GetName(void) { return name_; }
	inline void Setup(const string& name) { name_ = name; }

protected:
	string name_;
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

#endif // !_UTILTTY_
