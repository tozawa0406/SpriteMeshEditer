#ifndef _SET_POSITION_COMMAND_H_
#define _SET_POSITION_COMMAND_H_

#include "ICommand.h"

class SetPositionCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	SetPositionCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~SetPositionCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	void SetPosition(const VECTOR3& prevPositoin, const VECTOR3& newPosition);

private:
	VECTOR3 prevPosition_;
	VECTOR3 newPosition_;
};

#endif // _SET_POSITION_COMMAND_H_
