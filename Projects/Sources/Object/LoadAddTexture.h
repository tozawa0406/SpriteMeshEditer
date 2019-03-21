#ifndef _LOAD_ADD_TEXTURE_H_
#define _LOAD_ADD_TEXTURE_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Graphics/Utility/Utility.h>
#include <FrameWork/Systems/BaseManager.h>

#include "Search.h"

struct ADD_TEXTURE_DATA
{
	string	name;
	int		texNum;
};

class Loading;
class LoadAddTexture : public Interface
{
	friend Systems;
	friend class Texture;
public:
	~LoadAddTexture(void) {};

	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);

	void GuiUpdate(void);

	ITextureResource* SelectTexture(string& textureName);
	ITextureResource* SetTexture(const string& texName);

private:
	LoadAddTexture(Systems* systems);
	HRESULT Init(void)	 override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;

	SearchFile*						search_;
	std::vector<ADD_TEXTURE_DATA>	list_;
	std::vector<ITextureResource*>	textureList_;
};

#endif // _LOAD_ADD_TEXTURE_H_