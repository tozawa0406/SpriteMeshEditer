#include "SpriteMeshRenderer.h"

SpriteMeshRenderer::SpriteMeshRenderer(void) : 
	transform_(nullptr)
{
}

SpriteMeshRenderer::~SpriteMeshRenderer(void)
{
}

void SpriteMeshRenderer::Init(const SPRITE_MESH_RESOURCE& info, const Transform* transform)
{
	SPRITE_MESH temp;
	temp.transform = info.transform;
	if (transform) { temp.transform.parent = transform; }

	temp.spriteRnderer = new SpriteRenderer;
	if (temp.spriteRnderer)
	{
//		temp.spriteRnderer->Init(info.texNum, &temp.transform);
		temp.spriteRnderer->SetLayer(info.layer);
		temp.spriteRnderer->SetPivot(info.pivot);

		spriteRendererList_.emplace_back(temp);

		for (auto& child : info.children)
		{
			Init(child, &temp.transform);
		}
	}

	transform_ = transform;
}

void SpriteMeshRenderer::Uninit(void)
{
	for (auto& s : spriteRendererList_)
	{
		DeletePtr(s.spriteRnderer);
	}
}
