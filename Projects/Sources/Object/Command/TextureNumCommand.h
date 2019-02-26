#ifndef _TEXTURE_NUM_COMMAND_H_
#define _TEXTURE_NUM_COMMAND_H_

#include "ICommand.h"

class TextureNumCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	TextureNumCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~TextureNumCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	int prevTexNum_;
	int nextTexNum_;
};

#endif // _TEXTURE_NUM_COMMAND_H_
