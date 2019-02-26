#ifndef _NAME_COMMAND_H_
#define _NAME_COMMAND_H_

#include "ICommand.h"

class NameCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	NameCommand(void);
	/* @brief	デストラクタ		*/
	~NameCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	string prevName_;
	string nextName_;
};

#endif // _SCALE_COMMAND_H_
