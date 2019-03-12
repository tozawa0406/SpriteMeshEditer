#ifndef _ADD_DELETE_ANIMATION_COMMAND_H_
#define _ADD_DELETE_ANIMATION_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditer.h"
#include "../AnimationEditer.h"

class AddDeleteAnimationCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	AddDeleteAnimationCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~AddDeleteAnimationCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	/* @brief	�A�j���[�V�������̐ݒ�		*/
	inline void SetAnimationEditer(AnimationEditer* animationEditer) { animationEditer_ = animationEditer; }
	inline void SetFrame(int frame) { frame_ = frame; }
	inline void SetAdd(bool add) { add_ = add; }

private:
	void GetAnim(Receiver* receiver, STORAGE_ANIMATION& animData);

	STORAGE_ANIMATION	anim_;
	int					frame_;
	bool				add_;

	AnimationEditer*	animationEditer_;
};

#endif // _ADD_DELETE_ANIMATION_COMMAND_H_
