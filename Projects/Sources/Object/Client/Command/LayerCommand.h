#ifndef _LAYER_COMMAND_H_
#define _LAYER_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../Client.h"

class LayerCommand : public ICommand
{
public:
	/* @brief	�R���X�g���N�^		*/
	LayerCommand(void);
	/* @brief	�f�X�g���N�^		*/
	~LayerCommand(void);

	/* @breif	����		*/
	virtual void Invoke(void)	override;
	/* @brief	�i��		*/
	virtual void Undo(void)		override;
	/* @brief	�߂�		*/
	virtual void Redo(void)		override;

private:
	uint8 prevLayer_;
	uint8 nextLayer_;
};

#endif // _LAYER_COMMAND_H_
