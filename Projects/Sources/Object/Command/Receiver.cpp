#include "Receiver.h"

#include "PositionCommand.h"
#include "RotationCommand.h"
#include "ScaleCommand.h"
#include "PivotCommand.h"
#include "NameCommand.h"
#include "TextureNumCommand.h"

Receiver::Receiver(void) :
	ctrl_(nullptr)
	, spriteRenderer_(nullptr)
	, name_("NoName")
{
}

Receiver::~Receiver(void)
{
}

void Receiver::Init(void)
{
	spriteRenderer_ = new SpriteRenderer;
	if (spriteRenderer_)
	{
		spriteRenderer_->Init(static_cast<int>(Resources::Texture::Base::WHITE), &transform_);

		receiver_.name				= &name_;
		receiver_.transform			= &transform_;
		receiver_.spriteRenderer	= spriteRenderer_;
	}

	if (!beforeData_.transform)			{ beforeData_.transform = new Transform;			}
	if (!beforeData_.name)				{ beforeData_.name		= new string;				}
	if (!beforeData_.spriteRenderer)	{ beforeData_.spriteRenderer = new SpriteRenderer;	}

	if (receiver_.name && beforeData_.name)
	{
		name_.resize(256);
		*beforeData_.name = *receiver_.name;
	}

	if (receiver_.transform && beforeData_.transform)
	{
		*beforeData_.transform = *receiver_.transform;
	}

	if (receiver_.spriteRenderer && beforeData_.spriteRenderer)
	{
		beforeData_.spriteRenderer->SetPivot(receiver_.spriteRenderer->GetPivot());
		beforeData_.spriteRenderer->SetTexture(receiver_.spriteRenderer->GetTexture());
	}


	if (const auto& systems = Systems::Instance())
	{
		if (const auto& texture = systems->GetTexture())
		{
			loadAdd_ = texture->GetAddLoad();
		}
	}
}

void Receiver::Uninit(void)
{
	for (auto& c : prevCommand_) { DeletePtr(c); }
	for (auto& c : nextCommand_) { DeletePtr(c); }

	DeletePtr(beforeData_.spriteRenderer);
	DeletePtr(beforeData_.transform);
	DeletePtr(beforeData_.name);

	DeletePtr(spriteRenderer_);
}

void Receiver::Update(void)
{
	if (!ctrl_) { return; }
	if (!receiver_.spriteRenderer) { return; }

	ImGui::InputText("name", &name_[0], 256);

	ImGui::InputFloat3("position", receiver_.transform->position, 1);
	VECTOR3 rot = receiver_.transform->rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 1);
	receiver_.transform->rotation = rot * 0.01744444f;
	ImGui::InputFloat3("scale", receiver_.transform->scale, 1);

	VECTOR2 pivot = receiver_.spriteRenderer->GetPivot();
	ImGui::InputFloat2("pivot", pivot, 1);
	receiver_.spriteRenderer->SetPivot(pivot);

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
		if (beforeData_.spriteRenderer->GetPivot() != receiver_.spriteRenderer->GetPivot())
		{
			if (InvokeCommand<PivotCommand>())
			{
				AddMessage("\"Pivot reflected change\" in Sprite");
			}
		}
		if (*beforeData_.name != *receiver_.name)
		{
			if (InvokeCommand<NameCommand>())
			{
				*beforeData_.name = *receiver_.name;
				AddMessage("\"Name reflected change\" in Sprite");
			}
		}
	}

	if (ImGui::Button("Save"))
	{
		SaveData();
	}

	if (loadAdd_)
	{
		int ret = loadAdd_->SelectTexture();
		if (ret >= 0)
		{
			receiver_.spriteRenderer->SetTexture(ret);
			if (InvokeCommand<TextureNumCommand>())
			{
				AddMessage("\"Texture reflected change\" in Sprite");
			}
		}
	}
}

template<class T>
bool Receiver::InvokeCommand(void)
{
	T* command = new T;
	if (command)
	{
		command->SetReceiver(receiver_);
		command->SetBeforeData(&beforeData_);
		command->Invoke();

		prevCommand_.insert(prevCommand_.begin(), command);
		for (auto& c : nextCommand_) { DeletePtr(c); }

		nextCommand_.clear();

		return true;
	}
	return false;
}

void Receiver::Undo(void)
{
	if (prevCommand_.size() <= 0) { return; }

	if (prevCommand_[0])
	{
		prevCommand_[0]->Undo();

		nextCommand_.insert(nextCommand_.begin(), prevCommand_[0]);
		prevCommand_.erase(prevCommand_.begin());
	}

	AddMessage("performed \"Undo\" process");
}

void Receiver::Redo(void)
{
	if (nextCommand_.size() <= 0) { return; }

	if (nextCommand_[0])
	{
		nextCommand_[0]->Redo();

		prevCommand_.insert(prevCommand_.begin(), nextCommand_[0]);
		nextCommand_.erase(nextCommand_.begin());
	}

	AddMessage("performed \"Redo\" process");
}

void Receiver::SaveData(void)
{
	AddMessage("\"Save\" is complete");
}

void Receiver::AddMessage(const string& message)
{
	message_.insert(message_.begin(), message);
	int size = static_cast<int>(message_.size());
	if (size > 6)
	{
		message_.erase(message_.end() - 1);
	}
}

void Receiver::ConsoleWindow(void)
{
//	ImGui::Text("prevSize %d, nextSize %d", prevCommand_.size(), nextCommand_.size());
	for (auto& m : message_)
	{
		ImGui::Text(m.c_str());
	}
}
