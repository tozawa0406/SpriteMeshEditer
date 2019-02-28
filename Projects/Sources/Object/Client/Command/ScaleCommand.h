#ifndef _SCALE_COMMAND_H_
#define _SCALE_COMMAND_H_

#include "ICommand.h"

class ScaleCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	ScaleCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~ScaleCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	VECTOR3 prevScale_;
	VECTOR3 nextScale_;
};

#endif // _SCALE_COMMAND_H_
