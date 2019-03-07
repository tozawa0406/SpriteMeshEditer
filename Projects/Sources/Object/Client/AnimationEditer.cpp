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
	VECTOR2 size = VECTOR2(620, 150);
	ImGui::SetNextWindowPos(ImVec2(Half(Windows::WIDTH) - Half(size.x), Half(Windows::HEIGHT) - Half(size.y)), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiSetCond_Once);
	if (ImGui::Begin("Animation"))
	{
		ImGui::PushItemWidth(63);
		float f = static_cast<float>(minFrame_);
		ImGui::InputFloat("##min", &f, 0, 0, "%.0f");
		minFrame_ = static_cast<int>(f);
		ImGui::SameLine();
		string temp = (regeneration_) ? "||" : ">";
		ImGui::PopItemWidth();
		ImGui::PushItemWidth(400);
		if (ImGui::Button(temp.c_str(), ImVec2(48, 32))) { regeneration_ = !regeneration_; }
		ImGui::SameLine();
		ImGui::SliderInt("##frame", &currentFrame_, minFrame_, maxFrame_);
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushItemWidth(63);
		f = static_cast<float>(maxFrame_);
		ImGui::InputFloat("##max", &f, 0, 0, "%.0f");
		maxFrame_ = static_cast<int>(f);
		ImGui::PopItemWidth();

		if (receiver_)
		{
			int cnt = 1;
			auto anim = receiver_->GetAnimData();
			for (auto& a : anim)
			{
				ImGui::TextAlign("       ");
				ImGui::PushItemWidth(400);
				string key = "##key";
				key += cnt;
				ImGui::SliderInt(key.c_str(), &a.frame, minFrame_, maxFrame_);
				cnt++;
				ImGui::PopItemWidth();
			}
		}

		if(ImGui::Button("AddKey"))
		{
			if (receiver_) { receiver_->AddAnim(currentFrame_); }
		}
	}
	ImGui::End();
}
