#ifndef _SPRITE_MESH_H_
#define _SPRITE_MESH_H_

#include <FrameWork/Define/Define.h>
#include <FrameWork/Systems/BaseManager.h>

// 1つのメッシュデータ
struct SPRITE_MESH_RESOURCE
{
	string		name;
	Transform	transform;
	VECTOR2		pivot;
	uint8		layer;
	string		textureName;

	std::vector<SPRITE_MESH_RESOURCE> children;

	SPRITE_MESH_RESOURCE(void) : name(""), transform(VECTOR3(0), VECTOR3(0), VECTOR3(1)), pivot(VECTOR2(0)), layer(0), textureName("") {}
};

// 1つのメッシュの1つのキーフレーム
struct SPRITE_MESH_TRANSFORM
{
	string	spriteMeshName;
	int		frame;
	string	textureName;
	VECTOR3 position;
	VECTOR3 rotation;
	VECTOR3 scale;

	SPRITE_MESH_TRANSFORM(void) : spriteMeshName(""), frame(0), textureName(""), position(0), rotation(0), scale(0) {}
	void operator = (const SPRITE_MESH_TRANSFORM& data)
	{
		spriteMeshName = data.spriteMeshName;
		frame = data.frame;
		textureName = data.textureName;
		position = data.position;
		rotation = data.rotation;
		scale = data.scale;
	}
};


struct SPRITE_MESH_ANIM_DATA
{
	std::vector<SPRITE_MESH_TRANSFORM> anim;
	std::vector<SPRITE_MESH_ANIM_DATA> child;
};

struct SPRITE_MESH_ANIMATION
{
	string	animationName;
	uint	min;
	uint	max;
	SPRITE_MESH_ANIM_DATA data;
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
