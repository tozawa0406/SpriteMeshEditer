#include "Client.h"

#include "SetPositionCommand.h"

Client::Client(void) :
	reciver_(nullptr)
	, beforeData_(VECTOR3(0), VECTOR3(0), VECTOR3(1))
	, ctrl_(nullptr)
{
}

Client::~Client(void)
{
}

void Client::Init(void)
{
	if (reciver_)
	{
		beforeData_.position	= reciver_->position;
		beforeData_.rotation	= reciver_->rotation;
		beforeData_.scale		= reciver_->scale;
	}
}

void Client::Uninit(void)
{
	for (auto& c : prevCommand_) { DeletePtr(c); }
	for (auto& c : newCommand_)  { DeletePtr(c); }
}

void Client::Update(void)
{
	if (!ctrl_) { return; }

	ImGui::InputFloat3("position", reciver_->position, 1);

	if (ctrl_->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if (beforeData_.position != reciver_->position)
		{
			SetPositionCommand* command = new SetPositionCommand;
			if(command)
			{
				command->SetReciver(reciver_);
				command->SetPosition(beforeData_.position, reciver_->position);
				SetNewCommand(command);
				beforeData_.position = reciver_->position;
			}
		}
	}
}

void Client::SetNewCommand(ICommand* command)
{
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : newCommand_) { DeletePtr(c); }
}

void Client::Undo(void)
{
	if (prevCommand_.size() <= 0) { return; }

	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		newCommand_.insert(newCommand_.begin(), prevCommand_[0]);
		prevCommand_.erase(prevCommand_.begin());

		if (reciver_)
		{
			beforeData_ = *reciver_;
		}
	}
}

void Client::Redo(void)
{
	if (newCommand_.size() <= 0) { return; }

	if (newCommand_[0])
	{
		newCommand_[0]->Redo();

		prevCommand_.insert(prevCommand_.begin(), newCommand_[0]);
		newCommand_.erase(newCommand_.begin());

		if (reciver_)
		{
			beforeData_ = *reciver_;
		}
	}
}
