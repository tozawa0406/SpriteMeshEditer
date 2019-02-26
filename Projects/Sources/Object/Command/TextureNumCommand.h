#ifndef _TEXTURE_NUM_COMMAND_H_
#define _TEXTURE_NUM_COMMAND_H_

#include "ICommand.h"

class TextureNumCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	TextureNumCommand(void);
	/* @brief	デストラクタ		*/
	~TextureNumCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	int prevTexNum_;
	int nextTexNum_;
};

#endif // _TEXTURE_NUM_COMMAND_H_
