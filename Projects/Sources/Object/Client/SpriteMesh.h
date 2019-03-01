#ifndef _SPRITE_MESH_H_
#define _SPRITE_MESH_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/BaseManager.h>

struct SPRITE_MESH_RESOURCE
{
	string		name;
	Transform	transform;
	VECTOR2		pivot;
	uint8		layer;
	string		textureName;

	std::vector<SPRITE_MESH_RESOURCE> children;
};

class Loading;
class SpriteMesh :public Interface
{
public:
	~SpriteMesh(void) {}
	int		SetUpLoading(Loading* loading, int sceneNum);
	HRESULT Load(int sceneNum);
	void	Release(bool uninit = false);

private:
	SpriteMesh(Systems* systems);
	HRESULT Init(void)   override;
	void	Uninit(void) override;

	int sceneNum_;
	Loading* loading_;
};

#endif // _SPRITE_MESH_H_
