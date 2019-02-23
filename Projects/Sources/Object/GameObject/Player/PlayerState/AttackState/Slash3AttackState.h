/*
 * @file		Slash3AttackState.h
 * @brief		�U������
 * @author		���V�đ�
 * @data		2018/11/24
 */
#ifndef _SLASH3_ATTACK_STATE_H_
#define _SLASH3_ATTACK_STATE_H_

#include "AttackBaseState.h"

class Slash3AttackState : public AttackBaseState
{
public:
	/* @brief	�R���X�g���N�^		*/
	Slash3AttackState(void);
	/* @brief	�f�X�g���N�^		*/
	~Slash3AttackState(void);

	

	/* @brief	����������
	 * @param	(player)	�v���C���[�ւ̃|�C���^
	 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
	 * @return	�Ȃ�				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @breif	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃X�e�[�g		*/
	PlayerState* Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�v���C���[����Ă΂��	*/
	void GuiUpdate(void) override;
};

#endif // _SLASH3_ATTACK_STATE_H_