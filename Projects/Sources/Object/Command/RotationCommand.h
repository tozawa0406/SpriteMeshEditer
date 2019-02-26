#ifndef _ROTATION_COMMAND_H_
#define _ROTATION_COMMAND_H_

#include "ICommand.h"

class RotationCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	RotationCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~RotationCommand(void);

	/* @breif	����		*/
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	�i��		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	�߂�		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR3 prevRotation_;
	VECTOR3 nextRotation_;
};

#endif // _ROTATION_COMMAND_H_
