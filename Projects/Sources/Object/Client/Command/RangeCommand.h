#ifndef _RANGE_COMMAND_H_
#define _RANGE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditer.h"
#include "../AnimationEditer.h"

class RangeCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	RangeCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~RangeCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	/* @brief	�A�j���[�V�������̐ݒ�		*/
	void SetAnimationEditer(AnimationEditer* animationEditer) { animationEditer_ = animationEditer; }
	/* @brief	�ő�lor�ŏ��l�̐ݒ�			*/
	void SetMin(bool min) { min_ = min; }

private:
	int		prevRange_;
	int		nextRange_;
	bool	min_;

	AnimationEditer* animationEditer_;
};

#endif // _RANGE_COMMAND_H_
