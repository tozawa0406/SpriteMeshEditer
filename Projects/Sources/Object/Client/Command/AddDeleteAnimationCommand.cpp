#include "AddDeleteAnimationCommand.h"

AddDeleteAnimationCommand::AddDeleteAnimationCommand(void) :
	frame_(0)
	, animationEditer_(nullptr)
	, add_(false)
{
	anim_.anim.frame		= 0;
	anim_.anim.position		= VECTOR3(0);
	anim_.anim.rotation		= VECTOR3(0);
	anim_.anim.scale		= VECTOR3(0);
	anim_.anim.spriteMeshName	= "";
	anim_.anim.textureName		= "";
}

AddDeleteAnimationCommand::~AddDeleteAnimationCommand(void)
{
}

void AddDeleteAnimationCommand::Invoke(void)
{
	if (!receiver_) { return; }

	GetAnim(receiver_, anim_);

	if (!add_) { receiver_->RemoveAnim(frame_); }
}

void AddDeleteAnimationCommand::GetAnim(Receiver* receiver, STORAGE_ANIMATION& animData)
{
	auto receiverAnimData = receiver->GetAnimData();
	int size = static_cast<int>(receiverAnimData.size());
	for (int i = 0; i < size; ++i)
	{
		if (receiverAnimData[i].frame == frame_)
		{
			animData.anim = receiverAnimData[i];
		}
	}

	auto children = receiver->GetChild();
	for (auto& child : children)
	{
		STORAGE_ANIMATION newAnim;
		GetAnim(child, newAnim);
		animData.child.emplace_back(newAnim);
	}
}

void AddDeleteAnimationCommand::Undo(void)
{
	if (!receiver_) { return; }

	if (add_)	{ receiver_->RemoveAnim(frame_);		}
	else		{ receiver_->AddAnim(frame_, anim_);	}
}

void AddDeleteAnimationCommand::Redo(void)
{
	if (!receiver_) { return; }

	if (add_)	{ receiver_->AddAnim(frame_, anim_);	}
	else		{ receiver_->RemoveAnim(frame_);		}
}
