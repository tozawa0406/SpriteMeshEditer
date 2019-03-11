#include "DeleteAnimationCommand.h"

DeleteAnimationCommand::DeleteAnimationCommand(void) :
	frame_(0)
	, animationEditer_(nullptr)
{
	anim_.anim.frame		= 0;
	anim_.anim.position		= VECTOR3(0);
	anim_.anim.rotation		= VECTOR3(0);
	anim_.anim.scale		= VECTOR3(0);
	anim_.anim.spriteMeshName	= "";
	anim_.anim.textureName		= "";
}

DeleteAnimationCommand::~DeleteAnimationCommand(void)
{
}

void DeleteAnimationCommand::Invoke(void)
{
	if (!receiver_) { return; }

	GetAnim(receiver_, anim_);
	receiver_->RemoveAnim(frame_);
}

void DeleteAnimationCommand::GetAnim(Receiver* receiver, STORAGE_ANIMATION& animData)
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

void DeleteAnimationCommand::Undo(void)
{
	if (!receiver_) { return; }

	receiver_->AddAnim(frame_, anim_);
}

void DeleteAnimationCommand::Redo(void)
{
	if (!receiver_) { return; }

	receiver_->RemoveAnim(frame_);
}
