#include "AnimationEditer.h"
#include "Receiver.h"
#include "Editer.h"

#include "Command/RangeCommand.h"
#include "Command/AddDeleteAnimDataCommand.h"

AnimationEditer::AnimationEditer(void) : Object(ObjectTag::STATIC), GUI(Systems::Instance(), this, "animation")
	, animationName_("")
	, currentFrame_(0)
	, minFrame_(0)
	, maxFrame_(120)
	, regeneration_(false)
	, receiver_(nullptr)
	, ctrl_(nullptr)
{
}

AnimationEditer::~AnimationEditer(void)
{
}

void AnimationEditer::Init(void)
{
	beforeData_.min = minFrame_;
	beforeData_.max = maxFrame_;
}

void AnimationEditer::Uninit(void)
{
}

void AnimationEditer::Update(void)
{
	if (regeneration_) 
	{
		if (receiver_) { receiver_->Animation(currentFrame_); }

		currentFrame_++; 
		if (maxFrame_ <= currentFrame_) { currentFrame_ = 0; }
	}
}

void AnimationEditer::GuiUpdate(void)
{
	if (!ctrl_) { return; }

	VECTOR2 size = VECTOR2(630, 150);
	ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - Half(size.x), Half(Windows::HEIGHT) - Half(size.y)), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiSetCond_Once);
	if (ImGui::Begin("Animation"))
	{
		ImGui::InputText("Name", animationName_, 256);

		ChangeRange(minFrame_, true);
		ImGui::SameLine();
		string temp = (regeneration_) ? "||" : ">";
		ImGui::PushItemWidth(400);
		if (ImGui::Button(temp.c_str(), ImVec2(48, 38))) { regeneration_ = !regeneration_; }
		ImGui::SameLine();
		ImGui::SliderInt("##frame", &currentFrame_, minFrame_, maxFrame_);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ChangeRange(maxFrame_, false);

		if (receiver_)
		{
			if (ImGui::Button("Add", ImVec2(68, 38)))
			{
				if (receiver_) 
				{
					AddDeleteAnimDataCommand* command = new AddDeleteAnimDataCommand;
					if (command)
					{
						command->SetAnimationEditer(this);
						command->SetReceiver(receiver_);
						command->SetFrame(currentFrame_);
						command->SetAdd(true);
						command->Invoke();

						if (editer_)
						{
							editer_->AddCommand(command);
							editer_->AddMessage("\"Add\"Animation");
						}
					}
				}
			}
			ImGui::SameLine();

			int cnt = 0;
			auto anim = receiver_->GetAnimData();
			for (auto& a : anim)
			{
				cnt++;

				string button = "pose##";
				button += static_cast<char>(cnt);
				if (ImGui::Button(button.c_str(), ImVec2(48, 38)))
				{
					currentFrame_ = a.frame;
					receiver_->Animation(currentFrame_);
				}
				ImGui::SameLine();

				ImGui::PushItemWidth(400);
				string key = "##key";
				key += static_cast<char>(cnt);
				ImGui::SliderInt(key.c_str(), &a.frame, minFrame_, maxFrame_);
				ImGui::PopItemWidth();

				ImGui::SameLine();
				string pose = "dlt##";
				pose += static_cast<char>(cnt);
				if (ImGui::Button(pose.c_str()) && receiver_)
				{
					AddDeleteAnimDataCommand* command = new AddDeleteAnimDataCommand;
					if (command)
					{
						command->SetAnimationEditer(this);
						command->SetReceiver(receiver_);
						command->SetFrame(a.frame);
						command->Invoke();

						if(editer_)
						{
							editer_->AddCommand(command);
							editer_->AddMessage("\"Delete\"Animation");
						}
					}
				}

				ImGui::TextAlign("    ");
			}

			if (ImGui::Button("CreateAnimation"))
			{
				CreateAnimation();
			}
		}
	}
	ImGui::End();
}

void AnimationEditer::ChangeRange(int& range, bool min)
{
	ImGui::PushItemWidth(68);
	float f = static_cast<float>(range);
	string label = (min) ? "##min" : "##max";
	ImGui::InputFloat(label.c_str() , &f, 0, 0, "%.0f");

	if ((min && f >= maxFrame_) ||
		!min && f <= minFrame_)
	{
		f = static_cast<float>(range);
	}

	if (ctrl_->Trigger(Input::GAMEPAD_CIRCLE, DIK_RETURN))
	{
		if((min && range != beforeData_.min) ||
		  (!min && range != beforeData_.max))
		{
			RangeCommand* command = new RangeCommand;
			if (command)
			{
				command->SetReceiver(receiver_);
				command->SetAnimationEditer(this);
				command->SetMin(min);
				command->Invoke();

				if (editer_)
				{
					editer_->AddCommand(command);
					string message = "Set ";
					message += (min) ? "\"min\" " : "\"max\" ";
					message += "value";
					editer_->AddMessage(message);
				}
			}

			int& temp = (min) ? beforeData_.min : beforeData_.max;
			temp = range;
		}
	}
	range = static_cast<int>(f);

	ImGui::PopItemWidth();
}

void AnimationEditer::HierarchyView(void)
{
	if (!receiver_) { return; }

	auto animList = receiver_->GetAnimation();

	for(auto anim : animList)
	{
		bool select = false;
		ImGui::MenuItem(anim.animationName.c_str(), nullptr, &select);

		if (select)
		{
			receiver_->ResetAnimData();
			currentFrame_ = 0;

			strcpy_s(animationName_, anim.animationName.c_str());
			minFrame_ = anim.min;
			maxFrame_ = anim.max;

			for(uint i = 0;i < anim.anim.size();++i)
			{
				receiver_->AddAnim(anim.anim[i].frame, anim, i);
			}

			break;
		}
	}
}

void AnimationEditer::SetCurrentFrame(int frame)
{
	currentFrame_ = frame; 
	if (receiver_)
	{
		receiver_->Animation(frame);
	}
}

void AnimationEditer::CreateAnimation(void)
{
	if (!receiver_) { return; }

	std::vector<SPRITE_MESH_ANIM_DATA> tempAnimData = receiver_->GetAnimData();

	SPRITE_MESH_ANIMATION tempAnimation;

	GetChildrenAnim(tempAnimation, *receiver_);

	receiver_->CreateAnimation(tempAnimation);
	strcpy_s(animationName_, "");
	receiver_->ResetAnimData();
	currentFrame_ = 0;
	minFrame_ = 0;
	maxFrame_ = 120;
}

void AnimationEditer::GetChildrenAnim(SPRITE_MESH_ANIMATION& tempAnimation, Receiver& receiver)
{
	tempAnimation.animationName = animationName_;
	tempAnimation.min = minFrame_;
	tempAnimation.max = maxFrame_;

	std::vector<SPRITE_MESH_ANIM_DATA> tempAnimData = receiver.GetAnimData();

	for (uint i = 0; i < tempAnimData.size(); ++i)
	{
		tempAnimation.anim.emplace_back(tempAnimData[i]);
	}

	auto children = receiver.GetChild();
	for (auto& child : children)
	{
		SPRITE_MESH_ANIMATION childAnimation;
		GetChildrenAnim(childAnimation, *child);
		tempAnimation.child.emplace_back(childAnimation);
	}
}
