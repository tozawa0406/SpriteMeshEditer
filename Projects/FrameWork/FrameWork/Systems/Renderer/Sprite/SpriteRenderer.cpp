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
	, vertexBuffer_(0)
	, indexBuffer_(0)
	, vertexNum_(4)
	, indexNum_(6)
	, texcoord_(VECTOR4(0, 0, 1, 1))
{
}

SpriteRenderer::~SpriteRenderer(void)
{
	if (wrapper_)
	{
		wrapper_->ReleaseBuffer(vertexBuffer_, Wrapper::FVF::VERTEX_3D);
	}
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

	const auto& wrapper = systems->GetGraphics()->GetWrapper();

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
	vertexBuffer_ = wrapper->CreateVertexBuffer(v, sizeof(v[0]), Wrapper::PRIMITIVE::V::FILL_RECT);
	if (vertexBuffer_ == Wrapper::R_ERROR) { return; }

	WORD index[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 2;
	index[4] = 3;
	index[5] = 1;
	indexBuffer_ = wrapper->CreateIndexBuffer(index, 6);

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
