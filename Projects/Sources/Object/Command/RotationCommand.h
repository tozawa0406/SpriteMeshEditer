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
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	進む		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	戻る		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR3 prevRotation_;
	VECTOR3 nextRotation_;
};

#endif // _ROTATION_COMMAND_H_
