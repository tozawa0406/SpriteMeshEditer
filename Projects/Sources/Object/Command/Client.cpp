#include "Client.h"

#include "PositionCommand.h"
#include "RotationCommand.h"
#include "ScaleCommand.h"
#include "PivotCommand.h"

Client::Client(void) :
	ctrl_(nullptr)
{
	receiver_.transform		= nullptr;
	receiver_.pivot			= nullptr;

	beforeData_.transform	= nullptr;
	beforeData_.pivot		= nullptr;
}

Client::~Client(void)
{
}

void Client::Init(void)
{
	if (!beforeData_.transform)	{ beforeData_.transform = new Transform;	}
	if (!beforeData_.pivot)		{ beforeData_.pivot		= new VECTOR2;		}

	if (receiver_.transform && beforeData_.transform)
	{
		*beforeData_.transform = *receiver_.transform;
	}

	if (receiver_.pivot && beforeData_.pivot)
	{
		*beforeData_.pivot = *receiver_.pivot;
	}
}

void Client::Uninit(void)
{
	for (auto& c : prevCommand_) { DeletePtr(c); }
	for (auto& c : nextCommand_) { DeletePtr(c); }

	DeletePtr(beforeData_.transform);
	DeletePtr(beforeData_.pivot);
}

void Client::Update(void)
{
	if (!ctrl_) { return; }

	ImGui::InputFloat3("position", receiver_.transform->position, 1);
	VECTOR3 rot = receiver_.transform->rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 1);
	receiver_.transform->rotation = rot * 0.01744444f;
	ImGui::InputFloat3("scale", receiver_.transform->scale, 1);

	ImGui::InputFloat2("pivot", *receiver_.pivot, 1);

	if (ctrl_->Press(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if (beforeData_.transform->position != receiver_.transform->position)
		{
			if(InvokeCommand<PositionCommand>())
			{
				AddMessage("\"Position reflected change\" in transform");
			}
		}
		if (beforeData_.transform->rotation != receiver_.transform->rotation)
		{
			if (InvokeCommand<RotationCommand>())
			{
				AddMessage("\"Rotation reflected change\" in transform");
			}
		}
		if (beforeData_.transform->scale != receiver_.transform->scale)
		{
			if (InvokeCommand<ScaleCommand>())
			{
				AddMessage("\"Scale reflected change\" in transform");
			}
		}
		if (*beforeData_.pivot != *receiver_.pivot)
		{
			if (InvokeCommand<PivotCommand>())
			{
				AddMessage("\"Pivot reflected change\" in Sprite");
			}
		}
	}

	if (ImGui::Button("Save"))
	{
		SaveData();
	}
}

template<class T>
bool Client::InvokeCommand(void)
{
	T* command = new T;
	if (command)
	{
		command->SetReceiver(receiver_);
		command->Invoke(beforeData_);

		prevCommand_.insert(prevCommand_.begin(), command);
		for (auto& c : nextCommand_) { DeletePtr(c); }

		nextCommand_.clear();

		return true;
	}
	return false;
}

void Client::Undo(void)
{
	if (prevCommand_.size() <= 0) { return; }

	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo(beforeData_);

		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Client::Redo(void)
{
	if (nextCommand_.size() <= 0) { return; }

	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo(beforeData_);

		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		nextCommand_.erase(nextCommand_.begin());
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
//	ImGui::Text("prevSize %d, nextSize %d", prevCommand_.size(), nextCommand_.size());
	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}

void Client::SetReceiver(SpriteRenderer* spriteRenderer)
{
	receiver_.transform = const_cast<Transform*>(spriteRenderer->GetTransform());
	receiver_.pivot		= const_cast<VECTOR2*>(&spriteRenderer->GetPivot());
}
