#include "PaidGoodsBox.h"

//! @def	�����ʒu
static const VECTOR3 POSITION = VECTOR3(-30, 9.5f, 55);
//! @def	������]
static const VECTOR3 ROTATION = VECTOR3(0, -1.3f, 0);
//! @def	�����X�P�[��
static const VECTOR3 SCALE = VECTOR3(0.05f, 0.05f * 0.8f, 0.05f);

//! @def	�F
static const COLOR MESH_COLOR = COLOR(0.25f, 0.25f, 1, 1);

//! @def	�����蔻��̃T�C�Y
static const VECTOR3 COLLIDER_SIZE = VECTOR3(15, 12.5f, 31);
//! @def	�����蔻��̃I�t�Z�b�g
static const VECTOR3 COLLIDER_OFFSET = VECTOR3(0, -2, 1);

PaidGoodsBox::PaidGoodsBox(void) : Object(ObjectTag::STATIC)
	, collider_(nullptr)
{
}

PaidGoodsBox::~PaidGoodsBox(void)
{
}

void PaidGoodsBox::Init(void)
{
	// �����p���̐ݒ�
	transform_ = Transform(POSITION, ROTATION, SCALE);

	// ���b�V��
	mesh_.Init(Systems::Instance(), (int)Resources::Model::Camp::BOX, &transform_);
	mesh_.material.diffuse = MESH_COLOR;

	// �����蔻��
	collider_ = new Collider3D::OBB(this);
	if (collider_)
	{
		collider_->SetSize(COLLIDER_SIZE);
		collider_->SetOffsetPosition(COLLIDER_OFFSET);
		collider_->Update();
	}
}

void PaidGoodsBox::Uninit(void)
{
	DeletePtr(collider_);
}