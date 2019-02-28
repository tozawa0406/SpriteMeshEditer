#ifndef _DELETE_COMMAND_H_
#define _DELETE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../Client.h"

class DeleteCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	DeleteCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~DeleteCommand(void);

	/* @brief	�㏈��		*/
	virtual void Uninit(void) override;

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

	inline void SetClient(Client* client) { client_ = client; }

private:
	int			place_;
	Client*		client_;
	Receiver*	parent_;
	std::vector<Receiver*> children_;
};

#endif // _DELETE_COMMAND_H_
