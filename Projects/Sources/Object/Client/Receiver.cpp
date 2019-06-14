#include "Receiver.h"
#include "ModelEditor.h"
#include "Editor.h"

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
	, loadAdd_(nullptr)
{
}

Receiver::~Receiver(void)
{
}

void Receiver::Init(ModelEditor* client)
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
								if (const auto& Editor = client_->GetEditor())
								{
									Editor->AddCommand(command);
									Editor->AddMessage("\"SetParent\"");
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
					if (const auto& Editor = client_->GetEditor())
					{
						Editor->AddCommand(command);
						Editor->AddMessage("\"RemoveParent\"");
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
			if (const auto& Editor = client_->GetEditor())
			{
				Editor->AddCommand(command);
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

	if (beforeData_.spriteRenderer && spriteRenderer_)
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
	Editor* Editor = client_->GetEditor();
	if (!Editor) { return; }

	ImGui::InputText("name", &name_[0], 256);

	int layer = static_cast<int>(spriteRenderer_->GetLayer());
	ImGui::InputInt("layer", &layer);
	if (static_cast<uint8>(layer) != spriteRenderer_->GetLayer())
	{
		spriteRenderer_->SetLayer(static_cast<uint8>(layer));
		if (InvokeCommand<LayerCommand>())
		{
			Editor->AddMessage("\"Layer reflected change\" in Sprite");
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
				Editor->AddMessage("\"Texture reflected change\" in Sprite");
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
				Editor->AddMessage("\"Position reflected change\" in transform");
			}
		}
		if (beforeData_.transform->rotation != transform_.rotation)
		{
			if (InvokeCommand<RotationCommand>())
			{
				Editor->AddMessage("\"Rotation reflected change\" in transform");
			}
		}
		if (beforeData_.transform->scale != transform_.scale)
		{
			if (InvokeCommand<ScaleCommand>())
			{
				Editor->AddMessage("\"Scale reflected change\" in transform");
			}
		}
		if (beforeData_.spriteRenderer->GetPivot() != spriteRenderer_->GetPivot())
		{
			if (InvokeCommand<PivotCommand>())
			{
				Editor->AddMessage("\"Pivot reflected change\" in Sprite");
			}
		}
		if (*beforeData_.name != name_)
		{
			if (InvokeCommand<NameCommand>())
			{
				*beforeData_.name = name_;
				Editor->AddMessage("\"Name reflected change\" in Sprite");
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
		command->SetModelEditor(client_);
		command->SetWithChild(withChild);
		command->SetReceiver(this);
		command->Invoke();

		if (client_)
		{
			if (const auto& Editor = client_->GetEditor())
			{
				Editor->AddCommand(command);
				Editor->AddMessage("\"Delete Sprite\"");
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
	if (animTransform_.size() < 1) { return; }

	size_t size = (animTransform_.size()) - 1;
	for (size_t i = 0; i < size; ++i)
	{
		if (animTransform_[i].frame <= frame && frame <= animTransform_[i + 1].frame)
		{
			animCnt_ = static_cast<int>(i);
		}
	}

	int next = animCnt_ + 1;
	if (next >= static_cast<int>(animTransform_.size())) { next = 0; }

	int diff = animTransform_[next].frame - animTransform_[animCnt_].frame;
	if (diff <= 0) { return; }
	float rate = (frame - animTransform_[animCnt_].frame) / static_cast<float>(diff);
	rate = min(1, rate);

	transform_.position = animTransform_[animCnt_].position * (1 - rate) + animTransform_[next].position * rate;
	transform_.rotation = animTransform_[animCnt_].rotation * (1 - rate) + animTransform_[next].rotation * rate;
	transform_.scale	= animTransform_[animCnt_].scale	* (1 - rate) + animTransform_[next].scale    * rate;

	for (auto& child : child_)
	{
		child->Animation(frame);
	}
}

void Receiver::AddAnim(int frame)
{
	SPRITE_MESH_TRANSFORM temp;
	temp.position		= transform_.position;
	temp.rotation		= transform_.rotation;
	temp.scale			= transform_.scale;
	temp.frame			= frame;
	temp.spriteMeshName = name_;
	temp.textureName	= textureName_;

	animTransform_.emplace_back(temp);

	int size = static_cast<int>(animTransform_.size()) - 1;
	for (int i = 0; i < size; ++i)
	{
		size_t num = static_cast<size_t>(i);
		if (animTransform_[num].frame > animTransform_[num + 1].frame)
		{
			SPRITE_MESH_TRANSFORM work;
			work = animTransform_[num];
			animTransform_[num] = animTransform_[num + 1];
			animTransform_[num + 1] = work;
			i = -1;
		}
		else if (animTransform_[num + 1].frame - animTransform_[num].frame == 0)
		{
			RemoveAnim(animTransform_[num].frame);
			size = static_cast<int>(animTransform_.size()) - 1;
			i = -1;
		}
	}

	for (auto& child : child_)
	{
		child->AddAnim(frame);
	}
}

void Receiver::AddAnim(int frame, const SPRITE_MESH_ANIM_DATA& anim, int animNum)
{
	SPRITE_MESH_TRANSFORM temp;
	temp.position		= anim.anim[animNum].position;
	temp.rotation		= anim.anim[animNum].rotation;
	temp.scale			= anim.anim[animNum].scale;
	temp.frame			= frame;
	temp.spriteMeshName = anim.anim[animNum].spriteMeshName;
	temp.textureName	= anim.anim[animNum].textureName;

	animTransform_.emplace_back(temp);

	int size = static_cast<int>(animTransform_.size()) - 1;
	for (int i = 0; i < size; ++i)
	{
		size_t num = static_cast<size_t>(i);
		if (animTransform_[num].frame > animTransform_[num + 1].frame)
		{
			SPRITE_MESH_TRANSFORM work;
			work = animTransform_[num];
			animTransform_[num] = animTransform_[num + 1];
			animTransform_[num + 1] = work;
			i = -1;
		}
		else if (animTransform_[num + 1].frame - animTransform_[num].frame == 0)
		{
			RemoveAnim(animTransform_[num].frame);
			size = static_cast<int>(animTransform_.size()) - 1;
			i = -1;
		}
	}

	int childSize		= static_cast<int>(child_.size());
	int animChildSize	= static_cast<int>(anim.child.size());
	if (childSize == animChildSize)
	{
		for (int i = 0; i < childSize; ++i)
		{
			child_[i]->AddAnim(frame, anim.child[i], animNum);
		}
	}
}

void Receiver::CreateAnimation(SPRITE_MESH_ANIMATION& animation)
{
	for (auto anim : animation_)
	{
		if (anim.animationName == animation.animationName)
		{
			anim = animation;
			return;
		}
	}
	animation_.emplace_back(animation);
}

void Receiver::RemoveAnim(int frame)
{
	int size = static_cast<int>(animTransform_.size());
	for (int i = 0;i < size;++i)
	{
		if (animTransform_[i].frame == frame)
		{
			animTransform_.erase(animTransform_.begin() + i);
			break;
		}
	}

	for (auto& child : child_)
	{
		child->RemoveAnim(frame);
	}
}
