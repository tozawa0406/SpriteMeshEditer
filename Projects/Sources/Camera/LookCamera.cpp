#include "LookCamera.h"
#include <FrameWork/Systems/Camera/CameraManager.h>

//! 移動速度
static constexpr float MOVE_SPEED = 1;

LookCamera::LookCamera(void) : 
	dir_(VECTOR2(0))
	, velocity_(VECTOR3(0))
{
}

LookCamera::~LookCamera(void)
{
}

void LookCamera::Update(void)
{
	// 平行移動
	MoveTrans();

	// 入力
	Input();
}

void LookCamera::MoveTrans(void)
{
	// 前ベクトルを算出
	front_ = at_ - position_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// 右ベクトルの求め方
	right_ = VecNorm(VecCross(front_, up_));

	// 速度が 0でない時
	if (velocity_ != 0)
	{
		// z座標移動
		position_	+= front_ * velocity_.z;
		at_			+= front_ * velocity_.z;
		// y座標移動
		position_	+= up_ * velocity_.y;
		at_			+= up_ * velocity_.y;
		// x座標移動
		position_	+= right_ * velocity_.x;
		at_			+= right_ * velocity_.x;

		// 慣性
		velocity_ = 0;
	}
}

// キー入力
void LookCamera::Input(void)
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

	if (ctrl->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	{
		// 平行移動
		velocity_.x += static_cast<float>(ctrl->PressRange(Input::AXIS_LX, DIK_A, DIK_D)) * MOVE_SPEED;
		velocity_.y -= static_cast<float>(ctrl->PressRange(Input::AXIS_LY, DIK_S, DIK_W)) * MOVE_SPEED;
		velocity_.z += static_cast<float>(ctrl->PressRange(Input::AXIS_RY, DIK_Q, DIK_E)) * MOVE_SPEED;
	}
}

// GUI
void LookCamera::GuiUpdate(void)
{
	ImGui::SetNextWindowSize(ImVec2(450, 160), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(Windows::WIDTH - 450 - 50, Half(Windows::HEIGHT) + 40), ImGuiSetCond_Once);
	if (ImGui::Begin("camera"))
	{
		ImGui::InputFloat3("position", position_);
	}
	ImGui::End();
}
