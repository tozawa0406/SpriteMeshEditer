#include "MoveCamera.h"
#include "../../Graphics/DirectX11/DirectX11.h"
#include "../../Scene/SceneManager.h"
#include "CameraManager.h"
#include "../GameSystems.h"
#include "../Input/KeyInput.h"

//! 移動速度
static constexpr float MOVE_SPEED = 0.06f;
//! 回転速度
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
	// 平行移動
	MoveTrans();

	// 自分を中心に回転
	MoveRotation();

	// 注視点を中心に回転
	MoveAtRotation();

	// 入力
	Input();
}

void MoveCamera::MoveTrans(void)
{
	// 前ベクトルを算出
	front_ = at_ - position_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// 右ベクトルの求め方
	right_ = VecNorm(VecCross(front_, up_));

	// 速度が 0でない時
	if (velocity_.x != 0 || velocity_.y != 0)
	{
		// y座標移動
		position_	+= front_ * velocity_.y;
		at_			+= front_ * velocity_.y;
		// x座標移動
		position_	+= right_ * velocity_.x;
		at_			+= right_ * velocity_.x;

		// 慣性
		velocity_ *= 0.9f;
	}
}

void MoveCamera::MoveRotation(void)
{
	// 速度が 0でない時
	if ( rotVelocity_.x != 0 || rotVelocity_.y != 0 )
	{
		// 回転行列
		MATRIX rotY, rotX;
		// y軸回転
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotVelocity_.y);
		// x軸回転
		rotX.Identity().Rotation(right_, rotVelocity_.x);

		VECTOR3 vec = at_ - position_;		// 方向ベクトル
		// ベクトルの座標変換
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// 内積による制限
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			at_ = position_ + vec;
		}
		else
		{
			// 制限なら速度を 0にする
			rotVelocity_ = VECTOR2(0); 
		}

		// 慣性
		rotVelocity_ *= 0.9f;
	}
}

void MoveCamera::MoveAtRotation(void)
{
	// 速度が 0でない時
	if (rotAtVelocity_.x != 0 || rotAtVelocity_.y != 0)
	{
		// 回転行列
		MATRIX rotY, rotX;
		// y軸回転
		rotY.Identity().Rotation(VECTOR3(0, 1, 0), rotAtVelocity_.x);
		// x軸回転
		rotX.Identity().Rotation(right_, rotAtVelocity_.y);

		VECTOR3 vec = position_ - at_;		// 方向ベクトル
		// ベクトルの座標変換	
		vec = VecTransform(vec, rotY);
		vec = VecTransform(vec, rotX);

		// 単位ベクトル
		VECTOR3 check = vec;
		check = VecNorm(check);
		front_ = VecNorm(front_);

		// 内積による制限
		float limit = VecDot(front_, check);
		if (Abs(limit) >= 0.1f)
		{
			// 回転
			position_ = at_ + vec;
		}
		else
		{
			// 制限なら速度を 0にする
			rotAtVelocity_ = VECTOR2(0);
		}

		// 慣性
		rotAtVelocity_ *= 0.9f;
	}
}

// キー入力
void MoveCamera::Input(void)
{
	// 今自分がメインカメラでないなら操作不可
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

	// 平行移動
	velocity_.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D)) * MOVE_SPEED;
	velocity_.y += static_cast<float>(ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W)) * MOVE_SPEED;

	// 自分を中心に回転
	if      (ctrl->Press(Input::GAMEPAD_LEFT , DIK_J)) { rotVelocity_.y +=  ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_RIGHT, DIK_L)) { rotVelocity_.y += -ROTATION_SPEED; }
	if      (ctrl->Press(Input::GAMEPAD_UP   , DIK_I)) { rotVelocity_.x += -ROTATION_SPEED; }
	else if (ctrl->Press(Input::GAMEPAD_DOWN , DIK_K)) { rotVelocity_.x +=  ROTATION_SPEED; }

	// 注視点を中心に回転
	rotAtVelocity_.x -= static_cast<float>(ctrl->PressRange(Input::AXIS_RX, DIK_Q, DIK_E)) * ROTATION_SPEED;
	rotAtVelocity_.y -= static_cast<float>(ctrl->PressRange(Input::AXIS_RY, DIK_G, DIK_T)) * ROTATION_SPEED;
}

// GUI
void MoveCamera::GuiUpdate(void)
{
}
