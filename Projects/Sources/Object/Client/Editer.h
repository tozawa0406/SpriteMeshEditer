/*
 * @file		Editer.h
 * @brief		�G�f�B�^
 * @author		���V�đ�
 * @data		2019/03/07
 */
#ifndef _EDITER_H_
#define _EDITER_H_

#include "Command/ICommand.h"
#include <FrameWork/Object/Object.h>
#include <FrameWork/Systems/DebugSetting/GUI.h>

class ModelEditer;
class Editer : public Object, public GUI
{
public:
	Editer(void);
	~Editer(void);

	virtual void Init(void)		override;
	virtual void Uninit(void)	override;
	virtual void Update(void)	override;

	virtual void GuiUpdate(void)	override;

	void AddCommand(ICommand* command);
	void AddMessage(const string& message) { message_.emplace_back(message); }
	const std::vector<string>& GetMessageList(void) { return message_; }

	void SetCtrl(Controller* ctrl) { ctrl_ = ctrl; }

	void Setup(void);

	void Undo(void);
	void Redo(void);

private:
	ModelEditer*	modelEditer_;
	//! �R���g���[���̃|�C���^
	Controller*		ctrl_;

	std::vector<ICommand*>	prevCommand_;
	std::vector<ICommand*>	nextCommand_;
	std::vector<string>		message_;
};

#endif // _EDITER_H_
