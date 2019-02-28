#ifndef _NAME_COMMAND_H_
#define _NAME_COMMAND_H_

#include "ICommand.h"

class NameCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	NameCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~NameCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	string prevName_;
	string nextName_;
};

#endif // _SCALE_COMMAND_H_
