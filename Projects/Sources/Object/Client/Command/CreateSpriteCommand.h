#ifndef _CREATE_SPRITE_COMMAND_H_
#define _CREATE_SPRITE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../Client.h"

class CreateSpriteCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	CreateSpriteCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~CreateSpriteCommand(void);

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
	Client*		client_;
};

#endif // _CREATE_SPRITE_COMMAND_H_
