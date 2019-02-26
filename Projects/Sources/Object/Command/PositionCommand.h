#ifndef _POSITION_COMMAND_H_
#define _POSITION_COMMAND_H_

#include "ICommand.h"

class PositionCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	PositionCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~PositionCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	VECTOR3 prevPosition_;
	VECTOR3 nextPosition_;
};

#endif // _POSITION_COMMAND_H_
