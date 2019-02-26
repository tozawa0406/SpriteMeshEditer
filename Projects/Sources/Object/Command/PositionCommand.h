#ifndef _POSITION_COMMAND_H_
#define _POSITION_COMMAND_H_

#include "ICommand.h"

class PositionCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	PositionCommand(void);
	/* @brief	デストラクタ		*/
	~PositionCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	VECTOR3 prevPosition_;
	VECTOR3 nextPosition_;
};

#endif // _POSITION_COMMAND_H_
