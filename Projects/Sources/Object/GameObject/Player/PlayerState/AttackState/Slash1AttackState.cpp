#include "Slash1AttackState.h"
#include <random>

#include "Slash2AttackState.h"
#include "../DrawnState/DrawnWaitState.h"
#include "../AvoidanceState.h"

//! @def	�R���W�����J�n
static constexpr int COLLISION_START = 18;
//! @def	�R���W�����I��
static constexpr int COLLISION_END = 26;
//! @def	�ړ�����
static constexpr float MOVE = 0.1f;
//! @def	�G�t�F�N�g�̉�]�p�x
static constexpr float EFFECT_ROTATION = 0.8f;
//! @def	�ړ��J�n
static constexpr int MOVE_START = 10;
//! @def	�ړ��I��
static constexpr int MOVE_END = 15;

Slash1AttackState::Slash1AttackState(void)
{
}

Slash1AttackState::~Slash1AttackState(void)
{
}

void Slash1AttackState::Init(Player* player, Controller* ctrl)
{
	if (!player) { return; }
	
	animSpeed_		= ANIM_SPEED;
	animation_		= Player::Animation::Slash_1;
	changeFrame_	= ANIMATION_CHANGE_FRAME30;
	collisionStart_ = COLLISION_START;
	collisionEnd_	= COLLISION_END;
	effectRot_		= EFFECT_ROTATION;
	moveStart_		= MOVE_START;
	moveEnd_		= MOVE_END;
	move_			= MOVE;
	voice_			= static_cast<int>(Resources::Sound::Camp::UNITYCHAN_ATTACK01);

	AttackBaseState::Init(player, ctrl);

	if (auto* wapon = player_->GetWapon())
	{
		wapon->SetOffensivePower(50);
	}
}

void Slash1AttackState::Uninit(void)
{
	AttackBaseState::Uninit();
}

PlayerState* Slash1AttackState::Update(void)
{
	if (!player_) { return nullptr; }
	auto& meshAnim = player_->GetMeshAnimation();

	if (auto temp = AttackBaseState::Update())
	{
		return temp;
	}

	float pattern = meshAnim.mesh.GetPattern();
	// �A�j���[�V�����̏��
	int animMax = meshAnim.mesh.GetMaxAnimation();
	// �I���O��
	if (pattern > (Quarter(animMax) * 3.0f))
	{
		if (next_)
		{
			// ���̍U�����s��
			next_ = false;
			// �A�j���[�V�������Ō�܂ōs������ŏ��ɖ߂�
			meshAnim.animation = static_cast<int>(Player::Animation::Slash_2);
			meshAnim.mesh.ChangeAnimation(meshAnim.animation, ANIMATION_CHANGE_FRAME15);

			return new Slash2AttackState;
		}
	}

	Wapon* wapon = player_->GetWapon();
	if (!wapon) { return nullptr; }
	if (!wapon->IsAttack() && pattern > Half(animMax))
	{	
		// ����R�}���h�ŉ���X�e�[�g
		if (ctrl_->Trigger(Input::GAMEPAD_CROSS, DIK_M))
		{
			if (player_->GetStamina() > AvoidanceState::DEC_STAMINA)
			{
				return new AvoidanceState;
			}
		}
	}

	return nullptr;
}

void Slash1AttackState::GuiUpdate(void)
{
	ImGui::Text("Slash1Attack");

	AttackBaseState::GuiUpdate();
}