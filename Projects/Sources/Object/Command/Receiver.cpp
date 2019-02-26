#include "Receiver.h"
#include "Client.h"

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
	, client_(nullptr)
{
}

Receiver::~Receiver(void)
{
}

void Receiver::Init(Client* client)
{
	client_ = client;

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
				if (client_)
				{
					client_->AddMessage("\"Position reflected change\" in transform");
				}
			}
		}
		if (beforeData_.transform->rotation != receiver_.transform->rotation)
		{
			if (InvokeCommand<RotationCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Rotation reflected change\" in transform");
				}
			}
		}
		if (beforeData_.transform->scale != receiver_.transform->scale)
		{
			if (InvokeCommand<ScaleCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Scale reflected change\" in transform");
				}
			}
		}
		if (beforeData_.spriteRenderer->GetPivot() != receiver_.spriteRenderer->GetPivot())
		{
			if (InvokeCommand<PivotCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Pivot reflected change\" in Sprite");
				}
			}
		}
		if (*beforeData_.name != *receiver_.name)
		{
			if (InvokeCommand<NameCommand>())
			{
				*beforeData_.name = *receiver_.name;
				if (client_)
				{
					client_->AddMessage("\"Name reflected change\" in Sprite");
				}
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
				if (client_)
				{
					client_->AddMessage("\"Texture reflected change\" in Sprite");
				}
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

		if (client_) { client_->AddCommand(command); }

		return true;
	}
	return false;
}

void Receiver::SaveData(void)
{
	if (client_)
	{
		client_->AddMessage("\"Save\" is complete");
	}
}
