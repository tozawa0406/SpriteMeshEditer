//-----------------------------------------------------------------------------
//
//	テクスチャ管理[Texture.h]
//	Auther : 戸澤翔太
//																	2018/08/18
//-----------------------------------------------------------------------------
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "../../../Define/Define.h"
#include "../../BaseManager.h"

#include "../../../../../Sources/Object/LoadAddTexture.h"

#include "../../../Graphics/Utility/Utility.h"

class Loading;
class Texture : public Interface
{
	friend Systems;
public:
	~Texture(void) {};

	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);
	ITextureResource* GetTextureResource(int texNum) { if (list_.size() > static_cast<size_t>(texNum)) { return list_[texNum]; } return nullptr; }
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

	std::vector<ITextureResource*> list_;
};

#endif // _TEXTURE_H_