#include "DrawnWaitState.h"
#include <random>

#include "DrawnMoveState.h"
#include "../AvoidanceState.h"
#include "../SetupState.h"
#include "../AttackState/Slash1AttackState.h"

DrawnWaitState::DrawnWaitState(void)
{
}

DrawnWaitState::~DrawnWaitState(void)
{
}

void DrawnWaitState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }

	PlayerState::Init(player, ctrl);

	auto& meshAnim = player->GetMeshAnimation();

	// �[����ԂƔ�����ԂŃA�j���[�V�����̐؂�ւ�
	meshAnim.animation = static_cast<int>(Player::Animation::SetupWait);
	// �Đ����x�̐ݒ�
	meshAnim.animSpeed = ANIMATION_DEFAULT;

	// �A�j���[�V�����̕ύX
	meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME30, true);
}

void DrawnWaitState::Uninit(void)
{
}

PlayerState* DrawnWaitState::Update(void)
{
#ifdef _SELF_DEBUG
	if (!player_->IsDebugCtrl())
#endif
	{
		// ���͂ňړ��X�e�[�g��
		if (ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D) || ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W))
		{
			return new DrawnMoveState;
		}

		// ����R�}���h�ŉ���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}

		// �[���R�}���h�Ŕ[���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H) || ctrl_->Trigger(Input::GAMEPAD_R1, DIK_H))
		{
			if (!ctrl_->Press(Input::GAMEPAD_L1, DIK_R))
			{
				return new SetupState;
			}
		}

		// �U���R�}���h�ōU���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
		{
			return new Slash1AttackState;
		}
	}
	return nullptr;
}

void DrawnWaitState::GuiUpdate(void)
{
	ImGui::Text("DrawnWait");
}