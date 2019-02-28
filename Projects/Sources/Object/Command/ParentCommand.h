#ifndef _PARENT_COMMAND_H_
#define _PARENT_COMMAND_H_

#include "ICommand.h"
#include "Receiver.h"
#include "Client.h"

class ParentCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	ParentCommand(void);
	/* @brief	デストラクタ		*/
	~ParentCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	inline void SetChiled(Receiver* chiled, bool add) { chiled_ = chiled; add_ = add; }

private:
	bool		add_;
	Receiver*	chiled_;
};

#endif // _PARENT_COMMAND_H_
