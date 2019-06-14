#ifndef _DELETE_COMMAND_H_
#define _DELETE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"

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

	inline void SetModelEditor(ModelEditor* client)		{ client_ = client;			}
	inline void SetWithChild(bool withChild)	{ withChild_ = withChild;	}

private:
	void UninitChild(Receiver* child);

	void ChildCtrl(Receiver* child, bool add);

	ModelEditor*	client_;
	Receiver*		parent_;
	bool			withChild_;
	std::vector<Receiver*> children_;
};

#endif // _DELETE_COMMAND_H_
