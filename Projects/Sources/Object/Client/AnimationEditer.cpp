#include "AnimationEditer.h"
#include "Receiver.h"
#include "Editer.h"

#include "Command/RangeCommand.h"
#include "Command/AddDeleteAnimationCommand.h"

AnimationEditer::AnimationEditer(void) : Object(ObjectTag::STATIC), GUI(Systems::Instance(), this, "animation")
	, currentFrame_(0)
	, minFrame_(0)
	, maxFrame_(120)
	, regeneration_(false)
	, receiver_(nullptr)
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
	VECTOR2 size = VECTOR2(630, 150);
	ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - Half(size.x), Half(Windows::HEIGHT) - Half(size.y)), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiSetCond_Once);
	if (ImGui::Begin("Animation"))
	{
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
					receiver_->AddAnim(currentFrame_); 
					AddDeleteAnimationCommand* command = new AddDeleteAnimationCommand;
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
					AddDeleteAnimationCommand* command = new AddDeleteAnimationCommand;
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

}
