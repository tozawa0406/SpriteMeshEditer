#include "MoveCamera.h"
#include "../../Graphics/DirectX11/DirectX11.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"

//! �ړ����x
static constexpr float MOVE_SPEED = 0.06f;
//! ��]���x
static constexpr float ROTATION_SPEED = 0.002f;

MoveCamera::MoveCamera(void) : 
	dir_(VECTOR2(0))
	, velocity_(VECTOR2(0))
	, rotVelocity_(VECTOR2(0))
	, rotAtVelocity_(VECTOR2(0))
{
}

MoveCamera::~MoveCamera(void)
{
}

void MoveCamera::Update(void)
{
	// ���s�ړ�
	MoveTrans();

	// �����𒆐S�ɉ�]
	MoveRotation();

	// �����_�𒆐S�ɉ�]
	MoveAtRotation();

	// ����
	Input();
}

void MoveCamera::MoveTrans(void)
{
	// �O�x�N�g�����Z�o
	front_ = at_ - position_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// �E�x�N�g���̋��ߕ�
	right_ = VecNorm(VecCross(front_, up_));

	// ���x�� 0�łȂ���
	if (velocity_.x != 0 || velocity_.y != 0)
	{
		// y���W�ړ�
		position_	+= front_ * velocity_.y;
		at_			+= front_ * velocity_.y;
		// x���W�ړ�
		position_	+= right_ * velocity_.x;
		at_			+= right_ * velocity_.x;

		// ����
		velocity_ *= 0.9f;
	}
}

void MoveCamera::MoveRotation(void)
{
	// ���x�� 0�łȂ���
	if ( rotVelocity_.x != 0 || rotVelocity_.y != 0 )
	{
		// ��]�s��
		MATRIX rotY, rotX;
		// y����]
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotVelocity_.y);
		// x����]
		rotX.Identity().Rotation(right_, rotVelocity_.x);

		VECTOR3 vec = at_ - position_;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// ���ςɂ�鐧��
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			at_ = position_ + vec;
		}
		else
		{
			// �����Ȃ瑬�x�� 0�ɂ���
			rotVelocity_ = VECTOR2(0); 
		}

		// ����
		rotVelocity_ *= 0.9f;
	}
}

void MoveCamera::MoveAtRotation(void)
{
	// ���x�� 0�łȂ���
	if (rotAtVelocity_.x != 0 || rotAtVelocity_.y != 0)
	{
		// ��]�s��
		MATRIX rotY, rotX;
		// y����]
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotAtVelocity_.x);
		// x����]
		rotX.Identity().Rotation(right_, rotAtVelocity_.y);

		VECTOR3 vec = position_ - at_;		// �����x�N�g��
		// �x�N�g���̍��W�ϊ�	
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// �P�ʃx�N�g��
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// ���ςɂ�鐧��
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// ��]
			position_ = at_ + vec;
		}
		else
		{
			// �����Ȃ瑬�x�� 0�ɂ���
			rotAtVelocity_ = VECTOR2(0);
		}

		// ����
		rotAtVelocity_ *= 0.9f;
	}
}

// �L�[����
void MoveCamera::Input(void)
{
	// �����������C���J�����łȂ��Ȃ瑀��s��
	if (manager_->GetMainCamera() != this) { return; }

	Controller* ctrl = nullptr;
	if (const auto& systems = Systems::Instance())
	{
		if (const auto& input = systems->GetInput())
		{
			ctrl = input->GetCtrl(0);
		}
	}
	if (!ctrl) { return; }

	// ���s�ړ�
	velocity_.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D)) * MOVE_SPEED;
	velocity_.y += static_cast<float>(ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W)) * MOVE_SPEED;

	// �����𒆐S�ɉ�]
	if      (ctrl->Press(Input::GAMEPAD_LEFT , DIK_J)) { rotVelocity_.y +=  ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_RIGHT, DIK_L)) { rotVelocity_.y += -ROTATION_SPEED; }
	if      (ctrl->Press(Input::GAMEPAD_UP   , DIK_I)) { rotVelocity_.x += -ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_DOWN , DIK_K)) { rotVelocity_.x +=  ROTATION_SPEED; }

	// �����_�𒆐S�ɉ�]
	rotAtVelocity_.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_RX, DIK_Q, DIK_E)) * ROTATION_SPEED;
	rotAtVelocity_.y -= static_cast<float>(ctrl->PressRange(Input::AXIS_RY, DIK_G, DIK_T)) * ROTATION_SPEED;
}

// GUI
void MoveCamera::GuiUpdate(void)
{
}
