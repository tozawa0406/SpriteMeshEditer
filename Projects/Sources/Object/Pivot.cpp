#include "Pivot.h"

Pivot::Pivot(void) : Object(ObjectTag::STATIC)
	, pivot_(nullptr)
{
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
		transform_.position.z = -1;

		MATERIAL m;
		m.diffuse = COLOR(1, 0, 0, 1);
		pivot_->SetMaterial(m);

		transform_.scale = VECTOR3(0.25f);
	}
}

void Pivot::Uninit(void)
{
	DeletePtr(pivot_);
}

void Pivot::Update(void)
{
}
