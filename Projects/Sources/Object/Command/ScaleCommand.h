#ifndef _SCALE_COMMAND_H_
#define _SCALE_COMMAND_H_

#include "ICommand.h"

class ScaleCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	ScaleCommand(void);
	/* @brief	デストラクタ		*/
	~ScaleCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	VECTOR3 prevScale_;
	VECTOR3 nextScale_;
};

#endif // _SCALE_COMMAND_H_
