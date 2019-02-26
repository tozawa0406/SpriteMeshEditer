#ifndef _ROTATION_COMMAND_H_
#define _ROTATION_COMMAND_H_

#include "ICommand.h"

class RotationCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	RotationCommand(void);
	/* @brief	デストラクタ		*/
	~RotationCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	VECTOR3 prevRotation_;
	VECTOR3 nextRotation_;
};

#endif // _ROTATION_COMMAND_H_
