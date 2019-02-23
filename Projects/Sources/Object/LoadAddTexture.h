#ifndef _LOAD_ADD_TEXTURE_H_
#define _LOAD_ADD_TEXTURE_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/BaseManager.h>

#include "Search.h"

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

private:
	LoadAddTexture(Systems* systems);
	HRESULT Init(void)	 override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;

	SearchFile*			search_;
	std::vector<string> list_;
};

#endif // _LOAD_ADD_TEXTURE_H_