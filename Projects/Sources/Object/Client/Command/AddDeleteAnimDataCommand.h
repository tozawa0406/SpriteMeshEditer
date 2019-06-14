#ifndef _ADD_DELETE_ANIM_DATA_COMMAND_H_
#define _ADD_DELETE_ANIM_DATA_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"
#include "../AnimationEditor.h"

class AddDeleteAnimDataCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	AddDeleteAnimDataCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~AddDeleteAnimDataCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	/* @brief	�A�j���[�V�������̐ݒ�		*/
	inline void SetAnimationEditor(AnimationEditor* animationEditor) { animationEditor_ = animationEditor; }
	/* @brief	�t���[�����̐ݒ�				*/
	inline void SetFrame(int frame) { frame_	= frame;	}
	/* @brief	�ǉ��������폜������			*/
	inline void SetAdd(bool add)	{ add_		= add;		}

	/* @brief	�����t���[���ɂ���O��̃A�j���[�V�����̎擾	*/
	void GetPrevAnim(void);

private:
	/* @brief	���V�[�o�[����A�j���[�V�������̎擾
	 * @sa		Invoke()
	 * @param	(receiver)	�A�j���[�V�������擾���������V�[�o�[
	 * @param	(animData)	�擾���������i�[����		*/
	void GetAnim(Receiver* receiver, SPRITE_MESH_ANIMATION& animData);

	//! �A�j���[�V�������
	SPRITE_MESH_ANIMATION	anim_;
	//! ���t���[���̑O��̃A�j���[�V����
	SPRITE_MESH_ANIMATION	prevAnim_;
	//! �A�j���[�V�����̃t���[����
	int					frame_;
	//! �ǉ��������폜������
	bool				add_;

	//! �G�f�B�^
	AnimationEditor*	animationEditor_;
};

#endif // _ADD_DELETE_ANIM_DATA_COMMAND_H_
