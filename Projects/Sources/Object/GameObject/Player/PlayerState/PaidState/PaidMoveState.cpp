#include "PaidMoveState.h"

#include "PaidWaitState.h"
#include "../AvoidanceState.h"
#include "../EmergencyAvoidanceState.h"
#include "../AttackState/SetupAttackState.h"
#include "HealState.h"

//! @def	����X�^�~�i
static constexpr float DEC_STAMINA = 0.1f;

PaidMoveState::PaidMoveState(void) :
	dash_(false)
	, breath_(false)
{
}

PaidMoveState::~PaidMoveState(void)
{
}

void PaidMoveState::Init(Player* player, Controller* ctrl)
{
	PlayerState::Init(player, ctrl);
}

void PaidMoveState::Uninit(void)
{
}

PlayerState* PaidMoveState::Update(void)
{
	if (!player_ || !ctrl_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	// ����
	VECTOR2 inputDir;
	// Input
	inputDir.x = (float)ctrl_->PressRange(Input::AXIS_LX, DIK_A, DIK_D);
	inputDir.y = (float)ctrl_->PressRange(Input::AXIS_LY, DIK_S, DIK_W);
	// ���K��
	inputDir = VecNorm(inputDir);

	if (ctrl_->Trigger(Input::GAMEPAD_L3, DIK_LSHIFT)) { dash_ = true; }

	float inputDash = 1.0f;
	float stamina = player_->GetStamina();
	if (!breath_)
	{
		if (ctrl_->Press(Input::GAMEPAD_R1, DIK_LSHIFT) || dash_)
		{
			if (stamina > 0)
			{
				inputDash = 2.5f;
				player_->SetStamina(stamina - (Player::ADD_STAMINA + DEC_STAMINA));
			}
			else { breath_ = true; }
		}
	}
	else
	{
		if (stamina > 25) { breath_ = false; }
	}

	if (static_cast<int>(stamina) % 15 == 0)
	{
		if (const auto& systems = Systems::Instance())
		{
			if (const auto& sound = systems->GetSound())
			{
				sound->Play(static_cast<int>(Resources::Sound::Camp::UNITYCHAN_BREATH));
			}
		}
	}

	inputDir *= inputDash;

	// �A�j���[�V�����؂�ւ�
	if (fabs(inputDir.x) + fabs(inputDir.y) > 0)
	{
		meshAnim.animSpeed = 0.55f;

		int p = static_cast<int>(meshAnim.mesh.GetPattern());
		Player::Animation anim = Player::Animation::Walk;
		int frame = ANIMATION_CHANGE_FRAME30;
		bool se = (p == 15 || p == 35) ? true : false;
		if (inputDash > 1)
		{
			anim = Player::Animation::Run;
			frame = ANIMATION_CHANGE_FRAME15;
			se = (p == 12 || p == 24) ? true : false;

		}

		meshAnim.animation = static_cast<int>(anim);
		meshAnim.mesh.ChangeAnimation(meshAnim.animation, frame);

		if (se)
		{
			if (const auto& systems = Systems::Instance())
			{
				if (const auto& sound = systems->GetSound())
				{
					sound->Play(static_cast<int>(Resources::Sound::Camp::UNITYCHAN_WALK));
				}
			}
		}
	}
	// ���͂��Ȃ��Ƒҋ@���[�V������
	else
	{
		return new PaidWaitState;
	}

	// �ړ����x
	if (const auto& camera = player_->GetCamera())
	{
		VECTOR3 velocity = player_->GetVelocity();
		velocity += camera->GetFrontXPlane() * inputDir.y * MOVE_SPEED;
		velocity -= camera->GetRightXPlane() * inputDir.x * MOVE_SPEED;
		player_->SetVelocity(velocity);
	}

	// ����R�}���h�ŉ���X�e�[�g��
	if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
	{
		if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
		{
			if (inputDash == 2.5f)
			{
				return new EmergencyAvoidanceState;
			}
			else
			{
				return new AvoidanceState;
			}
		}
	}

	// �����R�}���h�Ŕ����X�e�[�g��
	if (ctrl_->Trigger(Input::GAMEPAD_TRIANGLE, DIK_U))
	{
		return new SetupAttackState;
	}

	// �A�C�e���g�p
	if (ctrl_->Trigger(Input::GAMEPAD_SQUARE, DIK_H))
	{
		if (!ctrl_->Press(Input::GAMEPAD_L1, DIK_R))
		{
			if (const auto& itemList = player_->GetItemList())
			{
				if (itemList->GetCurrentItem().itemID != ItemID::UNKNOWN)
				{
					return new HealState;
				}
			}
		}
	}
	
	return nullptr;
}

void PaidMoveState::GuiUpdate(void)
{
	ImGui::Text("PaidMove");
}