#include "CanvasRendererImage.h"

CanvasRenderer::Image::Image(void) : CanvasRendererBase(CanvasType::Image)
	, texture_(nullptr)
	, scale_(VECTOR2(1))
	, scaleOffset_(VECTOR2(0))
	, angle_(0)
	, rotationOffset_(VECTOR2(0))
	, shader_(Shader::ENUM::UNKOUWN)
	, pattern_(0)
	, split_(VECTOR2(1))
	, primitiveType_(Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP)
	, primitiveNum_(2)
	, vertexBuffer_(nullptr)
{
}

CanvasRenderer::Image::~Image(void)
{
}

void CanvasRenderer::Image::Init(byte priority, int texNum)
{
	priority_	= priority;
	OnInit();

	if (const auto systems = Systems::Instance())
	{
		if (const auto& texture = systems->GetTexture())
		{
			texture_ = texture->GetTextureResource(texNum);
		}
	}

	CreateVertexBuffer();
}

void CanvasRenderer::Image::SetTexture(int texNum)
{
	if (const auto systems = Systems::Instance())
	{
		if (const auto& texture = systems->GetTexture())
		{
			texture_ = texture->GetTextureResource(texNum);
		}
	}
}

void CanvasRenderer::Image::Uninit(void)
{
	ReleasePtr(vertexBuffer_);

	OnUninit();
}

void CanvasRenderer::Image::CreateVertexBuffer(void)
{
	VERTEX2D v[4];

	for (int i = 0; i < 4; ++i)
	{
		v[i].position	= VECTOR4((i % 2), (i / 2), 1, 1);
		v[i].color		= COLOR(1, 1, 1, 1);
		v[i].texcoord	= VECTOR2(0, 0);
	}

	primitiveType_	= Wrapper::PRIMITIVE::TYPE::TRIANGLE_STRIP;
	primitiveNum_	= 2;

	if (manager_)
	{
		if (const auto& systems = manager_->GetSystems())
		{
			if (const auto& graphics = systems->GetGraphics())
			{
				if (const auto& dev = graphics->GetDevice())
				{
					dev->CreateBuffer(&vertexBuffer_, v, sizeof(VERTEX2D), 4);
				}
			}
		}
	}
}

bool CanvasRenderer::Image::Animation(float add)
{
	pattern_ += add;	
	if (pattern_ >= split_.x * split_.y)
	{
		pattern_ = 0;
		return true;
	}
	return false;
}
