#include "Receiver.h"
#include "Client.h"

#include "PositionCommand.h"
#include "RotationCommand.h"
#include "ScaleCommand.h"
#include "PivotCommand.h"
#include "NameCommand.h"
#include "TextureNumCommand.h"
#include "DeleteCommand.h"

Receiver::Receiver(void) :
	ctrl_(nullptr)
	, spriteRenderer_(nullptr)
	, name_("NoName")
	, client_(nullptr)
	, delete_(false)
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
	}

	if (!beforeData_.transform)			{ beforeData_.transform = new Transform;			}
	if (!beforeData_.name)				{ beforeData_.name		= new string;				}
	if (!beforeData_.spriteRenderer)	{ beforeData_.spriteRenderer = new SpriteRenderer;	}

	if (beforeData_.name)
	{
		name_.resize(256);
		*beforeData_.name = name_;
	}

	if (beforeData_.transform)
	{
		*beforeData_.transform = transform_;
	}

	if (beforeData_.spriteRenderer && spriteRenderer_)
	{
		beforeData_.spriteRenderer->SetPivot(spriteRenderer_->GetPivot());
		beforeData_.spriteRenderer->SetTexture(spriteRenderer_->GetTexture());
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
	if (!spriteRenderer_) { return; }

	SelectParam();

	Delete();

	if (!client_) { return; }

	const auto& list = client_->GetReceiverList();
	if (ImGui::TreeNode("sprites"))
	{
		for (auto& receiver : list)
		{
			if (!receiver) { continue; }
			if (receiver == this) { continue; }
			bool select = false;
			ImGui::MenuItem(receiver->GetName().c_str(), nullptr, &select);
			if (select)
			{
				transform_.parent = &receiver->transform_;
			}
		}
		ImGui::TreePop();
	}
}

template<class T>
bool Receiver::InvokeCommand(void)
{
	T* command = new T;
	if (command)
	{
		command->SetReceiver(this);
		command->Invoke();

		if (client_) { client_->AddCommand(command); }

		return true;
	}
	return false;
}

void Receiver::SaveData(IOFile& file)
{
	if (!spriteRenderer_) { return; }

	size_t size = name_.size();
	file.WriteParam(&size, sizeof(size_t));
	file.WriteParam(&name_[0], sizeof(char) * name_.size());
	file.WriteParam(&transform_.position, sizeof(VECTOR3));
	file.WriteParam(&transform_.rotation, sizeof(VECTOR3));
	file.WriteParam(&transform_.scale	, sizeof(VECTOR3));
	VECTOR2 pivot = spriteRenderer_->GetPivot();
	file.WriteParam(&pivot, sizeof(VECTOR2));
	int texNum = spriteRenderer_->GetTexture();
	file.WriteParam(&texNum, sizeof(int));
}

void Receiver::LoadData(IOFile& file)
{
	if (!spriteRenderer_) { return; }

	size_t size = 0;
	file.ReadParam(&size, sizeof(size_t));
	file.ReadParam(&name_[0], sizeof(char) * size);
	file.ReadParam(&transform_.position, sizeof(VECTOR3));
	file.ReadParam(&transform_.rotation, sizeof(VECTOR3));
	file.ReadParam(&transform_.scale, sizeof(VECTOR3));
	VECTOR2 pivot = VECTOR2(0);
	file.ReadParam(&pivot, sizeof(VECTOR2));
	spriteRenderer_->SetPivot(pivot);
	int texNum = 0;
	file.ReadParam(&texNum, sizeof(int));
	spriteRenderer_->SetTexture(texNum);

	if (beforeData_.name)
	{
		*beforeData_.name = name_;
	}

	if (beforeData_.transform)
	{
		*beforeData_.transform = transform_;
	}

	if (spriteRenderer_ && spriteRenderer_)
	{
		beforeData_.spriteRenderer->SetPivot(spriteRenderer_->GetPivot());
		beforeData_.spriteRenderer->SetTexture(spriteRenderer_->GetTexture());
	}
}

void Receiver::SelectParam(void)
{
	ImGui::InputText("name", &name_[0], 256);

	if (loadAdd_)
	{
		int ret = loadAdd_->SelectTexture();
		if (ret >= 0)
		{
			spriteRenderer_->SetTexture(ret);
			if (InvokeCommand<TextureNumCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Texture reflected change\" in Sprite");
				}
			}
		}
	}

	ImGui::InputFloat3("position", transform_.position, 1);
	VECTOR3 rot = transform_.rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 1);
	transform_.rotation = rot * 0.01744444f;
	ImGui::InputFloat3("scale", transform_.scale, 1);

	VECTOR2 pivot = spriteRenderer_->GetPivot();
	ImGui::InputFloat2("pivot", pivot, 1);
	spriteRenderer_->SetPivot(pivot);

	if (ctrl_->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if (beforeData_.transform->position != transform_.position)
		{
			if(InvokeCommand<PositionCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Position reflected change\" in transform");
				}
			}
		}
		if (beforeData_.transform->rotation != transform_.rotation)
		{
			if (InvokeCommand<RotationCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Rotation reflected change\" in transform");
				}
			}
		}
		if (beforeData_.transform->scale != transform_.scale)
		{
			if (InvokeCommand<ScaleCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Scale reflected change\" in transform");
				}
			}
		}
		if (beforeData_.spriteRenderer->GetPivot() != spriteRenderer_->GetPivot())
		{
			if (InvokeCommand<PivotCommand>())
			{
				if (client_)
				{
					client_->AddMessage("\"Pivot reflected change\" in Sprite");
				}
			}
		}
		if (*beforeData_.name != name_)
		{
			if (InvokeCommand<NameCommand>())
			{
				*beforeData_.name = name_;
				if (client_)
				{
					client_->AddMessage("\"Name reflected change\" in Sprite");
				}
			}
		}
	}
}

void Receiver::Delete(void)
{
	if (ImGui::Button("delete")) { delete_ = true; }
	if (delete_)
	{
		ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - 100.f, Half(Windows::HEIGHT) - 25.f - 200), ImGuiSetCond_Once);
		if (ImGui::Begin("warning"))
		{
			ImGui::Text("  really? ");

			if (ImGui::Button("yes", ImVec2(72.5f, 40)))
			{
				delete_ = false;
				DeleteCommand* command = new DeleteCommand;
				if (command)
				{
					command->SetClient(client_);
					command->SetReceiver(this);
					command->Invoke();

					if (client_)
					{
						client_->AddCommand(command);
						client_->AddMessage("\"Delete Sprite\"");
					}
				}
			}
			ImGui::SameLine(); ImGui::Text(" ");
			ImGui::SameLine();
			if (ImGui::Button("no", ImVec2(72.5f, 40))) { delete_ = false; }
			ImGui::End();
		}
	}
}
