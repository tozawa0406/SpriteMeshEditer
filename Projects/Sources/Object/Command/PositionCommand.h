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
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	�i��		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	�߂�		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR3 prevPosition_;
	VECTOR3 nextPosition_;
};

#endif // _POSITION_COMMAND_H_
