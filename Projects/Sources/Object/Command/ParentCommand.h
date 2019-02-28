#ifndef _PARENT_COMMAND_H_
#define _PARENT_COMMAND_H_

#include "ICommand.h"
#include "Receiver.h"
#include "Client.h"

class ParentCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	ParentCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~ParentCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	inline void SetChiled(Receiver* chiled, bool add) { chiled_ = chiled; add_ = add; }

private:
	bool		add_;
	Receiver*	chiled_;
};

#endif // _PARENT_COMMAND_H_
