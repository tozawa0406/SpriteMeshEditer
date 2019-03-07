#include "LookCamera.h"
#include <FrameWork/Systems/Camera/CameraManager.h>

//! �ړ����x
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
	// ���s�ړ�
	MoveTrans();

	// ����
	Input();
}

void LookCamera::MoveTrans(void)
{
	// �O�x�N�g�����Z�o
	front_ = at_ - position_;
	front_.y = 0;
	front_ = VecNorm(front_);

	// �E�x�N�g���̋��ߕ�
	right_ = VecNorm(VecCross(front_, up_));

	// ���x�� 0�łȂ���
	if (velocity_ != 0)
	{
		// z���W�ړ�
		position_	+= front_ * velocity_.z;
		at_			+= front_ * velocity_.z;
		// y���W�ړ�
		position_	+= up_ * velocity_.y;
		at_			+= up_ * velocity_.y;
		// x���W�ړ�
		position_	+= right_ * velocity_.x;
		at_			+= right_ * velocity_.x;

		// ����
		velocity_ = 0;
	}
}

// �L�[����
void LookCamera::Input(void)
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

	if (ctrl->Press(Input::GAMEPAD_R1, DIK_LSHIFT))
	{
		// ���s�ړ�
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
