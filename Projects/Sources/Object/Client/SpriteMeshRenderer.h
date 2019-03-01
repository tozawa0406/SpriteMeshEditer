#ifndef _SPRITE_MESH_RENDERER_H_
#define _SPRITE_MESH_RENDERER_H_

#include <FrameWork/Systems/Renderer/Sprite/SpriteRenderer.h>
#include "SpriteMesh.h"

struct SPRITE_MESH
{
	Transform		transform;
	SpriteRenderer* spriteRnderer;
};

class SpriteMeshRenderer
{
public:
	SpriteMeshRenderer(void);
	~SpriteMeshRenderer(void);

	void Init(const SPRITE_MESH_RESOURCE& info, const Transform* transform);
	void Uninit(void);

private:
	std::vector<SPRITE_MESH> spriteRendererList_;
	const Transform* transform_;
};

#endif // _SPRITE_MESH_RENDERER_H_
