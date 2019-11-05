#include "Pivot.h"

Pivot::Pivot(void) : Object(ObjectTag::STATIC)
	, pivot_(nullptr)
{
	transform_.parent = nullptr;
}

Pivot::~Pivot(void)
{
}

void Pivot::Init(void)
{
	pivot_ = new SpriteRenderer;
	if (pivot_)
	{
		pivot_->Init(static_cast<int>(Resources::Texture::Base::PIVOT), &transform_);
		pivot_->SetLayer(255);
		transform_.scale = 0.1f;

		MATERIAL m;
		m.diffuse = COLOR(1, 0, 0, 1);
		pivot_->SetMaterial(m);
	}
}

void Pivot::Uninit(void)
{
	DeletePtr(pivot_);
}

void Pivot::Update(void)
{
}
