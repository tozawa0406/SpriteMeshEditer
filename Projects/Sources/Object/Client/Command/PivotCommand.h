#ifndef _PIVOT_COMMAND_H_
#define _PIVOT_COMMAND_H_

#include "ICommand.h"

class PivotCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	PivotCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~PivotCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	VECTOR2 prevPivot_;
	VECTOR2 nextPivot_;
};

#endif // _PIVOT_COMMAND_H_
