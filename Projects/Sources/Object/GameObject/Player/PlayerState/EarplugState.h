/*
 * @file		EarplugState.h
 * @brief		��������
 * @author		���V�đ�
 * @data		2019/01/19
 */
#ifndef _EARPLUG_STATE_H_
#define _EARPLUG_STATE_H_

#include "PlayerState.h"

class EarplugState : public PlayerState
{
public:
	/* @brief	�R���X�g���N�^		*/
	EarplugState(void);
	/* @brief	�f�X�g���N�^		*/
	~EarplugState(void);


	
	/* @brief	����������
	 * @param	(player)	�v���C���[�ւ̃|�C���^
	 * @param	(ctrl)		�R���g���[���ւ̃|�C���^
	 * @return	�Ȃ�				*/
	void Init(Player* player, Controller* ctrl) override;
	
	/* @brief	�㏈��
	 * @param	�Ȃ�
	 * @return	�Ȃ�				*/
	void Uninit(void) override;
	
	/* @brief	�X�V����
	 * @param	�Ȃ�
	 * @return	���̃X�e�[�g		*/
	PlayerState* Update(void) override;
	
	/* @brief	Gui�̍X�V����
	 * @param	�Ȃ�
	 * @return	�Ȃ�
	 * @detail	�v���C���[����Ăяo�����	*/
	void GuiUpdate(void) override;
};

#endif // _EARPLUG_STATE_H_