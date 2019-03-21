#include "Receiver.h"
#include "ModelEditer.h"
#include "Editer.h"

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
	, prevDelete_(nullptr)
	, parent_(nullptr)
	, textureName_("")
	, flag_(0)
	, animCnt_(0)
{
}

Receiver::~Receiver(void)
{
}

void Receiver::Init(ModelEditer* client)
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

	if (spriteRenderer_->IsEnable())
	{
		ImGui::TextAlign("true ");
	}
	else
	{
		ImGui::TextAlign("false");
	}
	if (ImGui::Button("enable")) 
	{
		SetEnable(!spriteRenderer_->IsEnable());
	}

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
								if (const auto& editer = client_->GetEditer())
								{
									editer->AddCommand(command);
									editer->AddMessage("\"SetParent\"");
								}
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
					if (const auto& editer = client_->GetEditer())
					{
						editer->AddCommand(command);
						editer->AddMessage("\"RemoveParent\"");
					}
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

		if (client_) 
		{
			if (const auto& editer = client_->GetEditer())
			{
				editer->AddCommand(command);
			}
		}

		return true;
	}
	return false;
}

void Receiver::SetEnable(bool enable)
{
	if (spriteRenderer_)
	{
		spriteRenderer_->SetEnable(enable);
	}

	for(auto& child : child_)
	{
		child->SetEnable(enable);
	}
}

void Receiver::SaveData(SPRITE_MESH_RESOURCE& resource)
{
	if (!spriteRenderer_) { return; }

	resource.name			= name_;
	resource.transform		= transform_;
	resource.pivot			= spriteRenderer_->GetPivot();
	resource.layer			= spriteRenderer_->GetLayer();
	resource.textureName	= textureName_;

	size_t size = child_.size();
	for (size_t i = 0; i < size; ++i)
	{
		SPRITE_MESH_RESOURCE child;
		child_[i]->SaveData(child);
		resource.children.emplace_back(child);
	}
}

bool Receiver::LoadData(SPRITE_MESH_RESOURCE& resource)
{
	if (!spriteRenderer_) { return false; }

	name_			= resource.name;
	transform_		= resource.transform;
	textureName_	= resource.textureName;
	spriteRenderer_->SetPivot(resource.pivot);
	spriteRenderer_->SetLayer(resource.layer);

	if (loadAdd_) { spriteRenderer_->SetTexture(loadAdd_->SetTexture(textureName_)); }

	int size = static_cast<int>(resource.children.size());
	for (int i = 0; i < size; ++i)
	{
		Receiver* receiver = new Receiver;
		if (receiver)
		{
			receiver->SetCtrl(ctrl_);
			receiver->Init(client_);

			receiver->LoadData(resource.children[i]);
			receiver->SetParent(this);

			client_->AddSprite(receiver);
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
	if (!client_) { return; }
	Editer* editer = client_->GetEditer();
	if (!editer) { return; }

	ImGui::InputText("name", &name_[0], 256);

	int layer = static_cast<int>(spriteRenderer_->GetLayer());
	ImGui::InputInt("layer", &layer);
	if (static_cast<uint8>(layer) != spriteRenderer_->GetLayer())
	{
		spriteRenderer_->SetLayer(static_cast<uint8>(layer));
		if (InvokeCommand<LayerCommand>())
		{
			editer->AddMessage("\"Layer reflected change\" in Sprite");
		}
	}

	if (loadAdd_)
	{
		string name;
		ITextureResource* ret = loadAdd_->SelectTexture(name);
		if (ret)
		{
			textureName_ = name;
			spriteRenderer_->SetTexture(ret);
			if (InvokeCommand<TextureNumCommand>())
			{
				editer->AddMessage("\"Texture reflected change\" in Sprite");
			}
		}
	}

	ImGui::InputFloat3("position", transform_.position, 2);
	VECTOR3 rot = transform_.rotation / 0.01744444f;
	ImGui::InputFloat3("rotation", rot, 2);
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
				editer->AddMessage("\"Position reflected change\" in transform");
			}
		}
		if (beforeData_.transform->rotation != transform_.rotation)
		{
			if (InvokeCommand<RotationCommand>())
			{
				editer->AddMessage("\"Rotation reflected change\" in transform");
			}
		}
		if (beforeData_.transform->scale != transform_.scale)
		{
			if (InvokeCommand<ScaleCommand>())
			{
				editer->AddMessage("\"Scale reflected change\" in transform");
			}
		}
		if (beforeData_.spriteRenderer->GetPivot() != spriteRenderer_->GetPivot())
		{
			if (InvokeCommand<PivotCommand>())
			{
				editer->AddMessage("\"Pivot reflected change\" in Sprite");
			}
		}
		if (*beforeData_.name != name_)
		{
			if (InvokeCommand<NameCommand>())
			{
				*beforeData_.name = name_;
				editer->AddMessage("\"Name reflected change\" in Sprite");
			}
		}
	}
}

void Receiver::Delete(void)
{
	if (ImGui::Button("delete")) { BitSetFlag(true, flag_, FLAG_DELETE); }
	if (BitCheck(flag_, FLAG_DELETE))
	{
		ImGui::SetNextWindowSize(ImVec2(200, 130), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - 100.f, Half(Windows::HEIGHT) - 25.f - 200), ImGuiSetCond_Once);
		if (ImGui::Begin("warning"))
		{
			ImGui::Text("  really? ");

			if (ImGui::Button("yes", ImVec2(72.5f, 40)))
			{
				BitSetFlag(false, flag_, FLAG_DELETE);

				if (child_.size() > 0)	{ BitSetFlag(true, flag_, FLAG_WITH_CHILD); }
				else					{ InvokeDeleteCommand(false);				}
			}
			ImGui::SameLine();
			ImGui::TextAlign(" ");
			if (ImGui::Button("no", ImVec2(72.5f, 40))) { BitSetFlag(false, flag_, FLAG_DELETE); }
		}
		ImGui::End();
	}

	if (BitCheck(flag_, FLAG_WITH_CHILD))
	{
		ImGui::SetNextWindowSize(ImVec2(400, 130), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - 100.f, Half(Windows::HEIGHT) - 25.f - 200), ImGuiSetCond_Once);
		if (ImGui::Begin("warning"))
		{
			ImGui::Text("with child?");

			if (ImGui::Button("yes", ImVec2(72.5f, 40)))
			{
				BitSetFlag(false, flag_, FLAG_WITH_CHILD);
				InvokeDeleteCommand(true);
			}

			ImGui::SameLine();
			ImGui::TextAlign(" ");
			if (ImGui::Button("no", ImVec2(72.5f, 40))) 
			{
				BitSetFlag(false, flag_, FLAG_WITH_CHILD);
				InvokeDeleteCommand(false);
			}
		}
		ImGui::End();
	}
}

void Receiver::InvokeDeleteCommand(bool withChild)
{
	DeleteCommand* command = new DeleteCommand;
	if (command)
	{
		command->SetModelEditer(client_);
		command->SetWithChild(withChild);
		command->SetReceiver(this);
		command->Invoke();

		if (client_)
		{
			if (const auto& editer = client_->GetEditer())
			{
				editer->AddCommand(command);
				editer->AddMessage("\"Delete Sprite\"");
			}
		}
	}
}

void Receiver::SetParent(Receiver* parent)
{
	if (parent_)
	{
		parent_->SetChild(this, false);
	}

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

void Receiver::Animation(int frame)
{
	if (anim_.size() < 1) { return; }

	int size = static_cast<int>(anim_.size()) - 1;
	for (int i = 0; i < size; ++i)
	{
		if (anim_[i].frame <= frame && frame <= anim_[i + 1].frame)
		{
			animCnt_ = i;
		}
	}

	int next = animCnt_ + 1;
	if (next >= static_cast<int>(anim_.size())) { next = 0; }

	int diff = anim_[next].frame - anim_[animCnt_].frame;
	if (diff <= 0) { return; }
	float rate = (frame - anim_[animCnt_].frame) / static_cast<float>(diff);
	rate = min(1, rate);

	transform_.position = anim_[animCnt_].position * (1 - rate) + anim_[next].position * rate;
	transform_.rotation = anim_[animCnt_].rotation * (1 - rate) + anim_[next].rotation * rate;
	transform_.scale	= anim_[animCnt_].scale	   * (1 - rate) + anim_[next].scale    * rate;

	for (auto& child : child_)
	{
		child->Animation(frame);
	}
}

void Receiver::AddAnim(int frame)
{
	SPRITE_MESH_ANIM_DATA temp;
	temp.position		= transform_.position;
	temp.rotation		= transform_.rotation;
	temp.scale			= transform_.scale;
	temp.frame			= frame;
	temp.spriteMeshName = name_;
	temp.textureName	= textureName_;

	anim_.emplace_back(temp);

	int size = static_cast<int>(anim_.size()) - 1;
	for (int i = 0; i < size; ++i)
	{
		if (anim_[i].frame > anim_[i + 1].frame) 
		{
			SPRITE_MESH_ANIM_DATA work;
			work = anim_[i];
			anim_[i] = anim_[i + 1];
			anim_[i + 1] = work;
			i = -1;
		}
		else if (anim_[i + 1].frame - anim_[i].frame == 0)
		{
			RemoveAnim(anim_[i].frame);
			size = static_cast<int>(anim_.size()) - 1;
			i = -1;
		}
	}

	for (auto& child : child_)
	{
		child->AddAnim(frame);
	}
}

void Receiver::AddAnim(int frame, const STORAGE_ANIMATION& anim)
{
	SPRITE_MESH_ANIM_DATA temp;
	temp.position		= anim.anim.position;
	temp.rotation		= anim.anim.rotation;
	temp.scale			= anim.anim.scale;
	temp.frame			= frame;
	temp.spriteMeshName = anim.anim.spriteMeshName;
	temp.textureName	= anim.anim.textureName;

	anim_.emplace_back(temp);

	int size = static_cast<int>(anim_.size()) - 1;
	for (int i = 0; i < size; ++i)
	{
		if (anim_[i].frame > anim_[i + 1].frame)
		{
			SPRITE_MESH_ANIM_DATA work;
			work = anim_[i];
			anim_[i] = anim_[i + 1];
			anim_[i + 1] = work;
			i = -1;
		}
		else if (anim_[i + 1].frame - anim_[i].frame == 0)
		{
			RemoveAnim(anim_[i].frame);
			size = static_cast<int>(anim_.size()) - 1;
			i = -1;
		}
	}

	int childSize		= static_cast<int>(child_.size());
	int animChildSize	= static_cast<int>(anim.child.size());
	if (childSize == animChildSize)
	{
		for (int i = 0; i < childSize; ++i)
		{
			child_[i]->AddAnim(frame, anim.child[i]);
		}
	}
}

void Receiver::RemoveAnim(int frame)
{
	int size = static_cast<int>(anim_.size());
	for (int i = 0;i < size;++i)
	{
		if (anim_[i].frame == frame)
		{
			anim_.erase(anim_.begin() + i);
			break;
		}
	}

	for (auto& child : child_)
	{
		child->RemoveAnim(frame);
	}
}
