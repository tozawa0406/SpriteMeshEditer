#ifndef _CREATE_SPRITE_COMMAND_H_
#define _CREATE_SPRITE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"

class CreateSpriteCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	CreateSpriteCommand(void);
	/* @brief	デストラクタ		*/
	~CreateSpriteCommand(void);

	/* @brief	後処理		*/
	virtual void Uninit(void) override;

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

	inline void SetModelEditor(ModelEditor* client) { client_ = client; }

private:
	ModelEditor*		client_;
};

#endif // _CREATE_SPRITE_COMMAND_H_
