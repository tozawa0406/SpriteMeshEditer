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
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	void SetRotation(const VECTOR3& prevRotation, const VECTOR3& newRotation);

private:
	VECTOR3 prevRotation_;
	VECTOR3 newRotation_;
};

#endif // _ROTATION_COMMAND_H_
