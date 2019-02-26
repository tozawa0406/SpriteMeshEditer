//-----------------------------------------------------------------------------
//
//	eNX`Į[Texture.h]
//	Auther : ËāVãÄū
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

#include "../../../../../Sources/Object/LoadAddTexture.h"

class Loading;
class Texture : public Interface
{
	friend Systems;
public:
	~Texture(void) {};

	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);
	VECTOR2 GetTextureSize(int texNum) const;

	void GuiUpdate(void);

	LoadAddTexture* GetAddLoad(void) { return loadAdd_; }

private:
	Texture(Systems* systems);
	HRESULT Init(void)	 override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;

	LoadAddTexture* loadAdd_;
};

#endif // _TEXTURE_H_