#include "Editer.h"
#include "ModelEditer.h"
#include <FrameWork/Object/ObjectManager.h>
#include "../Pivot.h"

Editer::Editer(void) : Object(ObjectTag::STATIC), GUI(Systems::Instance(), this, "editer")
	, modelEditer_(nullptr)
{
}

Editer::~Editer(void)
{
}

void Editer::Init(void)
{
}

void Editer::Setup(void)
{
	modelEditer_ = manager_->Create<ModelEditer>();
	if (modelEditer_)
	{
		modelEditer_->SetCtrl(ctrl_);
		modelEditer_->SetPivot(manager_->Create<Pivot>());
		modelEditer_->SetEditer(this);
		modelEditer_->Load();
	}
}

void Editer::Uninit(void)
{
	for (auto& c : prevCommand_) { UninitDeletePtr(c); }
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }
}

void Editer::Update(void)
{
	// �L�[�{�[�h�Ή�
	if (ctrl_->Press(Input::GAMEPAD_L1, DIK_LCONTROL))
	{
		if (ctrl_->Press(Input::GAMEPAD_L2, DIK_LSHIFT))
		{
			if (ctrl_->Trigger(Input::GAMEPAD_L1, DIK_Z))
			{
				Redo();
			}
		}
		else if (ctrl_->Trigger(Input::GAMEPAD_R1, DIK_Z))
		{
			Undo();
		}
		else if (ctrl_->Trigger(Input::GAMEPAD_START, DIK_S))
		{
			if (modelEditer_) { modelEditer_->SaveData(); }
		}
	}
}

void Editer::GuiUpdate(void)
{
}

void Editer::AddCommand(ICommand* command)
{
	// �߂鏈���̒ǉ�
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { UninitDeletePtr(c); }

	// �i�ޏ����̃��Z�b�g
	nextCommand_.clear();
}

void Editer::Undo(void)
{
	// �߂�R�}���h�����邩
	if (prevCommand_.size() <= 0) { return; }

	// �擪�����s
	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		// �i�ޏ����ɒǉ�
		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		// �߂鏈������͍폜
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Editer::Redo(void)
{
	// �i�ރR�}���h�͂��邩
	if (nextCommand_.size() <= 0) { return; }

	// �擪�����s
	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		// �߂鏈���ɒǉ�
		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		// �i�ޏ�������͍폜
		nextCommand_.erase(nextCommand_.begin());
	}

	AddMessage("performed \"Redo\" process");
}
