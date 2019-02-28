#include "Receiver.h"
#include "Client.h"

#include "Command/PositionCommand.h"
#include "Command/RotationCommand.h"
#include "Command/ScaleCommand.h"
#include "Command/PivotCommand.h"
#include "Command/NameCommand.h"
#include "Command/TextureNumCommand.h"
#include "Command/DeleteCommand.h"
#include "Command/ParentCommand.h"
#include "Command/LayerCommand.h"

Receiver::Receiver(void) :
	ctrl_(nullptr)
	, spriteRenderer_(nullptr)
	, name_("NoName")
	, client_(nullptr)
	, delete_(false)
	, parent_(nullptr)
	, isHierarchy_(false)
	, textureName_("")
	, isHierarchyChild_(false)
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
		beforeData_.spriteRenderer->SetLayer(spriteRenderer_->GetLayer());
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

	if (!client_) { return; }

	if (!parent_)
	{
		if (ImGui::CollapsingHeader("SetParent"))
		{
			if (ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(400, 200)))
			{
				const auto& list = client_->GetReceiverList();
				for (auto& receiver : list)
				{
					if (!receiver) { continue; }
					if (receiver == this) { continue; }
					bool select = false;
					ImGui::MenuItem(receiver->GetName().c_str(), nullptr, &select);
					if (select)
					{
						ParentCommand* command = new ParentCommand;
						if (command)
						{
							command->SetReceiver(receiver);
							command->SetChiled(this, true);
							command->Invoke();

							if (client_)
							{
								client_->AddCommand(command);
								client_->AddMessage("\"SetParent\"");
							}
						}
					}
				}
				ImGui::EndChild();
			}
		}
	}
	else
	{
		if (ImGui::Button("RemoveParent"))
		{
			ParentCommand* command = new ParentCommand;
			if (command)
			{
				command->SetReceiver(parent_);
				command->SetChiled(this, false);
				command->Invoke();

				if (client_) 
				{
					client_->AddCommand(command); 
					client_->AddMessage("\"RemoveParent\"");
				}
			}
		}
	}

	Delete();
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

void Receiver::SaveData(IOFile& file, bool parentCall)
{
	if (!spriteRenderer_) { return; }

	bool isParent = (parent_) ? true : false;
	if (isParent && !parentCall) { return; }
	file.WriteParam(&isParent, sizeof(bool));

	size_t size = name_.size();
	file.WriteParam(&size, sizeof(size_t));
	file.WriteParam(&name_[0], sizeof(char) * name_.size());
	file.WriteParam(&transform_.position, sizeof(VECTOR3));
	file.WriteParam(&transform_.rotation, sizeof(VECTOR3));
	file.WriteParam(&transform_.scale	, sizeof(VECTOR3));
	VECTOR2 pivot = spriteRenderer_->GetPivot();
	file.WriteParam(&pivot, sizeof(VECTOR2));
	uint8 layer = spriteRenderer_->GetLayer();
	file.WriteParam(&layer, sizeof(uint8));
	size = textureName_.size();
	file.WriteParam(&size, sizeof(size_t));
	file.WriteParam(&textureName_[0], sizeof(char) * textureName_.size());

	size = child_.size();
	file.WriteParam(&size, sizeof(size_t));
	for (int i = 0; i < size; ++i)
	{
		child_[i]->SaveData(file, true);
	}
}

bool Receiver::LoadData(IOFile& file, bool parentCall)
{
	if (!spriteRenderer_) { return false; }

	bool isParent = false;
	file.ReadParam(&isParent, sizeof(bool));
	if (!parentCall && isParent) { return false; }

	size_t size = 0;
	file.ReadParam(&size, sizeof(size_t));
	file.ReadParam(&name_[0], sizeof(char) * size);
	file.ReadParam(&transform_.position, sizeof(VECTOR3));
	file.ReadParam(&transform_.rotation, sizeof(VECTOR3));
	file.ReadParam(&transform_.scale, sizeof(VECTOR3));
	VECTOR2 pivot = VECTOR2(0);
	file.ReadParam(&pivot, sizeof(VECTOR2));
	spriteRenderer_->SetPivot(pivot);
	uint8 layer = 0;
	file.ReadParam(&layer, sizeof(uint8));
	spriteRenderer_->SetLayer(layer);

	size = 0;
	file.ReadParam(&size, sizeof(size_t));
	textureName_.resize(size);
	file.ReadParam(&textureName_[0], sizeof(char) * size);

	if (loadAdd_) { spriteRenderer_->SetTexture(loadAdd_->SetTexture(textureName_)); }

	size = 0;
	file.ReadParam(&size, sizeof(size_t));
	for (int i = 0; i < size; ++i)
	{
		Receiver* receiver = new Receiver;
		if (receiver)
		{
			receiver->SetCtrl(ctrl_);
			receiver->Init(client_);

			receiver->LoadData(file, true);
			receiver->SetParent(this);

			client_->AddSprite(receiver, 0);
		}
	}

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
		beforeData_.spriteRenderer->SetLayer(spriteRenderer_->GetLayer());
		beforeData_.spriteRenderer->SetTexture(spriteRenderer_->GetTexture());
	}
	return true;
}

void Receiver::SelectParam(void)
{
	if (!spriteRenderer_) { return; }

	ImGui::InputText("name", &name_[0], 256);

	int layer = static_cast<int>(spriteRenderer_->GetLayer());
	ImGui::InputInt("layer", &layer);
	if (static_cast<uint8>(layer) != spriteRenderer_->GetLayer())
	{
		spriteRenderer_->SetLayer(static_cast<uint8>(layer));
		if (InvokeCommand<LayerCommand>())
		{
			if (client_)
			{
				client_->AddMessage("\"Layer reflected change\" in Sprite");
			}
		}
	}

	if (loadAdd_)
	{
		string name;
		int ret = loadAdd_->SelectTexture(name);
		if (ret >= 0)
		{
			textureName_ = name;
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

	ImGui::InputFloat3("position", transform_.position, 2);
	VECTOR3 rot = transform_.rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 1);
	transform_.rotation = rot * 0.01744444f;
	ImGui::InputFloat3("scale", transform_.scale, 2);

	VECTOR2 pivot = spriteRenderer_->GetPivot();
	ImGui::InputFloat2("pivot", pivot, 2);
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
			ImGui::SameLine(); 
			ImGui::TextAlign(" ");
			if (ImGui::Button("no", ImVec2(72.5f, 40))) { delete_ = false; }
			ImGui::End();
		}
	}
}

void Receiver::SetParent(Receiver* parent)
{
	if (parent) 
	{
		parent_ = parent;
		transform_.parent = &parent->GetTransform();
		parent->SetChild(this, true);
	}
	else
	{
		parent_ = nullptr;
		transform_.parent = nullptr;
	}
}

void Receiver::SetChild(Receiver* child, bool add)
{
	int size = static_cast<int>(child_.size());
	for (int i = 0; i < size; ++i)
	{
		if (child_[i] == child)
		{
			if (!add)
			{
				child_.erase(child_.begin() + i);
			}
			return;
		}
	}

	child_.emplace_back(child);
}
