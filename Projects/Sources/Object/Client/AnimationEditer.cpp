#include "AnimationEditer.h"
#include "Receiver.h"

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
		ImGui::PushItemWidth(68);
		float f = static_cast<float>(minFrame_);
		ImGui::InputFloat("##min", &f, 0, 0, "%.0f");
		minFrame_ = static_cast<int>(f);
		ImGui::SameLine();
		string temp = (regeneration_) ? "||" : ">";
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(400);
		if (ImGui::Button(temp.c_str(), ImVec2(48, 38))) { regeneration_ = !regeneration_; }
		ImGui::SameLine();
		ImGui::SliderInt("##frame", &currentFrame_, minFrame_, maxFrame_);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushItemWidth(68);
		f = static_cast<float>(maxFrame_);
		ImGui::InputFloat("##max", &f, 0, 0, "%.0f");
		maxFrame_ = static_cast<int>(f);
		ImGui::PopItemWidth();

		if (receiver_)
		{
			if (ImGui::Button("Add", ImVec2(68, 38)))
			{
				if (receiver_) { receiver_->AddAnim(currentFrame_); }
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
					receiver_->RemoveAnim(a.frame);
				}

				ImGui::TextAlign("    ");
			}
		}
	}
	ImGui::End();
}
