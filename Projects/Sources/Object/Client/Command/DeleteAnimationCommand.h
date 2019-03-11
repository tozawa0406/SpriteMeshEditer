#ifndef _DELETE_ANIMATION_COMMAND_H_
#define _DELETE_ANIMATION_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditer.h"
#include "../AnimationEditer.h"

class DeleteAnimationCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	DeleteAnimationCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~DeleteAnimationCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	/* @brief	�A�j���[�V�������̐ݒ�		*/
	inline void SetAnimationEditer(AnimationEditer* animationEditer) { animationEditer_ = animationEditer; }
	inline void SetFrame(int frame) { frame_ = frame; }

private:
	void GetAnim(Receiver* receiver, STORAGE_ANIMATION& animData);

	STORAGE_ANIMATION	anim_;
	int					frame_;

	AnimationEditer*	animationEditer_;
};

#endif // _DELETE_ANIMATION_COMMAND_H_
