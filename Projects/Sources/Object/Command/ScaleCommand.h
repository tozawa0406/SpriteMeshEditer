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
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	�i��		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	�߂�		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR3 prevScale_;
	VECTOR3 nextScale_;
};

#endif // _SCALE_COMMAND_H_
