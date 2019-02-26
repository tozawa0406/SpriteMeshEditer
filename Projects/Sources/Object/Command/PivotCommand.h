#ifndef _PIVOT_COMMAND_H_
#define _PIVOT_COMMAND_H_

#include "ICommand.h"

class PivotCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	PivotCommand(void);
	/* @brief	デストラクタ		*/
	~PivotCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(Receiver& beforeData)	override;
	/* @brief	進む		*/
	virtual void Undo(Receiver& beforeData)		override;
	/* @brief	戻る		*/
	virtual void Redo(Receiver& beforeData)		override;

private:
	VECTOR2 prevPivot_;
	VECTOR2 nextPivot_;
};

#endif // _PIVOT_COMMAND_H_
