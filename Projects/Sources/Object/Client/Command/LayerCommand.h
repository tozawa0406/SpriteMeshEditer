#ifndef _LAYER_COMMAND_H_
#define _LAYER_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../Client.h"

class LayerCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	LayerCommand(void);
	/* @brief	デストラクタ		*/
	~LayerCommand(void);

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
	virtual void Redo(void)		override;

private:
	uint8 prevLayer_;
	uint8 nextLayer_;
};

#endif // _LAYER_COMMAND_H_
