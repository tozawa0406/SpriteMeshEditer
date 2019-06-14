#ifndef _DELETE_COMMAND_H_
#define _DELETE_COMMAND_H_

#include "ICommand.h"
#include "../Receiver.h"
#include "../ModelEditor.h"

class DeleteCommand : public ICommand
{
public:
	/* @brief	コンストラクタ		*/
	DeleteCommand(void);
	/* @brief	デストラクタ		*/
	~DeleteCommand(void);

	/* @brief	後処理		*/
	virtual void Uninit(void) override;

	/* @breif	処理		*/
	virtual void Invoke(void)	override;
	/* @brief	進む		*/
	virtual void Undo(void)		override;
	/* @brief	戻る		*/
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
