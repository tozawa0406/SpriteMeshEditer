#include "AddDeleteAnimDataCommand.h"

AddDeleteAnimDataCommand::AddDeleteAnimDataCommand(void) :
	frame_(0)
	, animationEditer_(nullptr)
	, add_(false)
{
}

AddDeleteAnimDataCommand::~AddDeleteAnimDataCommand(void)
{
}

void AddDeleteAnimDataCommand::Invoke(void)
{
	if (!receiver_) { return; }

	GetPrevAnim();

	if (add_) { receiver_->AddAnim(frame_); }

	GetAnim(receiver_, anim_);

	if (!add_) { receiver_->RemoveAnim(frame_); }
}

void AddDeleteAnimDataCommand::GetAnim(Receiver* receiver, SPRITE_MESH_ANIMATION& animData)
{
	auto receiverAnimData = receiver->GetAnimData();
	int size = static_cast<int>(receiverAnimData.size());
	for (int i = 0; i < size; ++i)
	{
		if (receiverAnimData[i].frame == frame_)
		{
			animData.anim.emplace_back(receiverAnimData[i]);
		}
	}

	auto children = receiver->GetChild();
	for (auto& child : children)
	{
		SPRITE_MESH_ANIMATION newAnim;
		GetAnim(child, newAnim);
		animData.child.emplace_back(newAnim);
	}
}

void AddDeleteAnimDataCommand::Undo(void)
{
	if (!receiver_) { return; }

	if (add_)	
	{
		receiver_->RemoveAnim(frame_);
		if (prevAnim_.anim.size() > 0)
		{
			receiver_->AddAnim(frame_, prevAnim_, 0);
			animationEditer_->SetCurrentFrame(frame_);
		}
		else
		{
			animationEditer_->SetCurrentFrame(0);
		}
	}
	else 
	{
		receiver_->AddAnim(frame_, anim_, 0);
		animationEditer_->SetCurrentFrame(frame_);
	}
}

void AddDeleteAnimDataCommand::Redo(void)
{
	if (!receiver_) { return; }

	if (add_)	
	{
		receiver_->AddAnim(frame_, anim_, 0);
		animationEditer_->SetCurrentFrame(frame_);
	}
	else
	{
		receiver_->RemoveAnim(frame_);
		animationEditer_->SetCurrentFrame(0);
	}
}

void AddDeleteAnimDataCommand::GetPrevAnim(void)
{
	if (!receiver_) { return; }

	GetAnim(receiver_, prevAnim_);
}
