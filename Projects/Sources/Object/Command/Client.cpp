#include "Client.h"

#include "PositionCommand.h"
#include "RotationCommand.h"
#include "ScaleCommand.h"
#include "PivotCommand.h"

Client::Client(void) :
	reciver_(nullptr)
	, beforeData_(VECTOR3(0), VECTOR3(0), VECTOR3(1))
	, beforeData2_(VECTOR2(0))
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
	if (reciver2_)
	{
		beforeData2_ = reciver2_->pivot;
	}
}

void Client::Uninit(void)
{
	for (auto& c : prevCommand_) { DeletePtr(c); }
	for (auto& c : nextCommand_) { DeletePtr(c); }
}

void Client::Update(void)
{
	if (!ctrl_) { return; }

	ImGui::InputFloat3("position", reciver_->position, 1);
	VECTOR3 rot = reciver_->rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 1);
	reciver_->rotation = rot * 0.01744444f;
	ImGui::InputFloat3("scale", reciver_->scale, 1);

	ImGui::InputFloat2("pivot", reciver2_->pivot, 1);

	if (ctrl_->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if (beforeData_.position != reciver_->position)
		{
			PositionCommand* command = new PositionCommand;
			if(command)
			{
				command->SetReciver(reciver_);
				command->SetSpriteRenderer(reciver2_);
				command->SetPosition(beforeData_.position, reciver_->position);
				SetNewCommand(command);
				beforeData_.position = reciver_->position;

				AddMessage("\"Position reflected change\" in transform");
			}
		}
		if (beforeData_.rotation != reciver_->rotation)
		{
			RotationCommand* command = new RotationCommand;
			if (command)
			{
				command->SetReciver(reciver_);
				command->SetSpriteRenderer(reciver2_);
				command->SetRotation(beforeData_.rotation, reciver_->rotation);
				SetNewCommand(command);
				beforeData_.rotation = reciver_->rotation;

				AddMessage("\"Rotation reflected change\" in transform");
			}
		}
		if (beforeData_.scale != reciver_->scale)
		{
			ScaleCommand* command = new ScaleCommand;
			if (command)
			{
				command->SetReciver(reciver_);
				command->SetSpriteRenderer(reciver2_);
				command->SetScale(beforeData_.scale, reciver_->scale);
				SetNewCommand(command);
				beforeData_.scale = reciver_->scale;

				AddMessage("\"Scale reflected change\" in transform");
			}
		}
		if (beforeData2_ != reciver2_->pivot)
		{
			PivotCommand* command = new PivotCommand;
			if (command)
			{
				command->SetReciver(reciver_);
				command->SetSpriteRenderer(reciver2_);
				command->SetPivot(beforeData2_, reciver2_->pivot);
				SetNewCommand(command);
				beforeData2_ = reciver2_->pivot;

				AddMessage("\"Pivot reflected change\" in Sprite");
			}
		}
	}

	if (ImGui::Button("Save"))
	{
		SaveData();
	}
}

void Client::SetNewCommand(ICommand* command)
{
	prevCommand_.insert(prevCommand_.begin(), command);
	for (auto& c : nextCommand_) { DeletePtr(c); }
}

void Client::Undo(void)
{
	if (prevCommand_.size() <= 0) { return; }

	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		prevCommand_.erase(prevCommand_.begin());

		if (reciver_)
		{
			beforeData_ = *reciver_;
		}
	}

	AddMessage("performed \"Undo\" process");
}

void Client::Redo(void)
{
	if (nextCommand_.size() <= 0) { return; }

	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		nextCommand_.erase(nextCommand_.begin());

		if (reciver_)
		{
			beforeData_ = *reciver_;
		}
	}

	AddMessage("performed \"Redo\" process");
}

void Client::SaveData(void)
{
	AddMessage("\"Save\" is complete");
}

void Client::AddMessage(const string& message)
{
	message_.insert(message_.begin(), message);
	int size = static_cast<int>(message_.size());
	if (size > 6)
	{
		message_.erase(message_.end() - 1);
	}
}

void Client::ConsoleWindow(void)
{

	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}
