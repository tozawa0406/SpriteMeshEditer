#include "SpriteRenderer.h"
#include "../../../Windows/Windows.h"
#include "../../GameSystems.h"

#include "../Shader/Default.h"

SpriteRenderer::SpriteRenderer(void) : ObjectRenderer(ObjectRenderer::RendererType::SPRITE)
	, texture_(nullptr)
	, pattern_(0)
	, split_(VECTOR2(1))
	, flagBillboard_(0)
	, pivot_(VECTOR2(0.5f))
	, layer_(0)
	, vertexBuffer_(nullptr)
	, indexBuffer_(nullptr)
	, vertexNum_(4)
	, indexNum_(6)
	, texcoord_(VECTOR4(0, 0, 1, 1))
{
}

SpriteRenderer::~SpriteRenderer(void)
{
	ReleasePtr(indexBuffer_);
	ReleasePtr(vertexBuffer_);
}

void SpriteRenderer::Init(int texNum, const Transform* transform)
{
	const auto& systems = Systems::Instance();
	if (!systems) { return; }

	ObjectRenderer::Init(systems->GetObjectRenderer(), transform);

	if (const auto& texture = systems->GetTexture())
	{
		texture_ = texture->GetTextureResource(texNum);
	}

	VECTOR2 inv		= { 1 / split_.x, 1 / split_.y };
	int		splitX	= static_cast<int>(split_.x);
	int		p		= static_cast<int>(pattern_);

	// 左上座標
	texcoord_.x = inv.x *  (p % splitX);
	texcoord_.y = inv.y *  (p / splitX);

	// サイズ
	texcoord_.z = inv.x;
	texcoord_.w = inv.y;

	VERTEX v[4];
	for (int i = 0; i < 4; ++i)
	{
		int x = i % 2, y = i / 2;
		v[i].position	= VECTOR3(-0.5f + (1 * x), 0.5f - (1 * y), 0.0f);
		v[i].normal		= VECTOR3(0, 0, -1);
		v[i].tangent	= VECTOR3(0, 0, 0);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2((float)x, (float)y);
		v[i].boneIndex	= VECTOR4(0, 0, 0, 0);
		v[i].weight		= VECTOR4(0, 0, 0, 0);
	}

	WORD index[6] = { 0, 1, 2, 2, 3, 1 };

	if (const auto& graphics = systems->GetGraphics())
	{
		if (const auto& dev = graphics->GetDevice())
		{
			HRESULT hr = dev->CreateBuffer(&vertexBuffer_, v, sizeof(v[0]), Wrapper::PRIMITIVE::V::FILL_RECT);
			if (FAILED(hr)) { return; }

			hr = dev->CreateBuffer(&indexBuffer_, index, 6);
			if (FAILED(hr)) { return; }
		}
	}

	shader_ = Shader::ENUM::DEFAULT;
}

void SpriteRenderer::SetTexture(int texNum)
{
	if (const auto systems = Systems::Instance())
	{
		if (const auto& texture = systems->GetTexture())
		{
			texture_ = texture->GetTextureResource(texNum);
		}
	}
}

bool SpriteRenderer::Animation(float add)
{
	pattern_ += add;

	bool zero = false;
	if (pattern_ >= split_.x * split_.y)
	{
		pattern_ = 0;
		zero = true;
	}

	VECTOR2 inv		= { 1 / split_.x, 1 / split_.y };
	int		splitX	= static_cast<int>(split_.x);
	int		p		= static_cast<int>(pattern_);

	// 左上座標
	texcoord_.x = inv.x *  (p % splitX);
	texcoord_.y = inv.y *  (p / splitX);

	// サイズ
	texcoord_.z = inv.x;
	texcoord_.w = inv.y;

	return zero;
}
