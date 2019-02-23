#ifndef _SET_POSITION_COMMAND_H_
#define _SET_POSITION_COMMAND_H_

#include "ICommand.h"

class SetPositionCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	SetPositionCommand(void);
	/* @brief	デストラクタ		*/
	~SetPositionCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	void SetPosition(const VECTOR3& prevPositoin, const VECTOR3& newPosition);

private:
	VECTOR3 prevPosition_;
	VECTOR3 newPosition_;
};

#endif // _SET_POSITION_COMMAND_H_
