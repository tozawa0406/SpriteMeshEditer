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
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	�i��		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	�߂�		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR2 prevPivot_;
	VECTOR2 nextPivot_;
};

#endif // _PIVOT_COMMAND_H_
