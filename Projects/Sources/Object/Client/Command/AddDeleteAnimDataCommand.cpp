#include "AddDeleteAnimDataCommand.h"

AddDeleteAnimDataCommand::AddDeleteAnimDataCommand(void) :
	frame_(0)
	, animationEditor_(nullptr)
	, add_(false)
{
}

AddDeleteAnimDataCommand::~AddDeleteAnimDataCommand(void)
{
}

void AddDeleteAnimDataCommand::Invoke(void)
{
	if (!receiver_) { return; }

	// ���t���[���̑O��̃A�j���[�V�������擾
	GetPrevAnim();

	// (�ǉ���)���݂̃t���[���̎p�����A�j���[�V�����Ƃ��Ēǉ�
	if (add_) { receiver_->AddAnim(frame_); }

	// �A�j���[�V�������̎擾
	GetAnim(receiver_, anim_.data);

	// (�폜��)�w�肵���t���[���̃A�j���[�V�������폜
	if (!add_) { receiver_->RemoveAnim(frame_); }
}

void AddDeleteAnimDataCommand::GetAnim(Receiver* receiver, SPRITE_MESH_ANIM_DATA& animData)
{
	// �S�A�j���[�V�������̎擾
	auto receiverAnimData = receiver->GetAnimTransform();

	int size = static_cast<int>(receiverAnimData.size());
	for (int i = 0; i < size; ++i)
	{
		// �w�肵���t���[���Ɠ����t���[���̃A�j���[�V�������擾
		if (receiverAnimData[i].frame == frame_)
		{
			animData.anim.emplace_back(receiverAnimData[i]);
		}
	}

	// �q�v�f�ɂ��K�p
	auto children = receiver->GetChild();
	for (auto& child : children)
	{
		SPRITE_MESH_ANIM_DATA newAnim;
		GetAnim(child, newAnim);
		animData.child.emplace_back(newAnim);
	}
}

void AddDeleteAnimDataCommand::Undo(void)
{
	if (!receiver_) { return; }

	// �ǉ���
	if (add_)	
	{
		// �폜
		receiver_->RemoveAnim(frame_);

		// �O���񂪂���΁A�����ǉ�
		if (prevAnim_.data.anim.size() > 0)
		{
			receiver_->AddAnim(frame_, prevAnim_.data, 0);
			animationEditor_->SetCurrentFrame(frame_);
		}
		// �Ȃ����0�t���[���ڂ�
		else
		{
			animationEditor_->SetCurrentFrame(0);
		}
	}
	// �폜��
	else 
	{
		// �A�j���[�V������߂�(�ǉ�)
		receiver_->AddAnim(frame_, anim_.data, 0);
		// �\�������̃A�j���[�V�����̃t���[����
		animationEditor_->SetCurrentFrame(frame_);
	}
}

void AddDeleteAnimDataCommand::Redo(void)
{
	if (!receiver_) { return; }

	// �ǉ���
	if (add_)	
	{
		// �ēx�ǉ����A�\�������̃A�j���[�V������
		receiver_->AddAnim(frame_, anim_.data, 0);
		animationEditor_->SetCurrentFrame(frame_);
	}
	// �폜��
	else
	{
		// �폜���A�\����0�t���[���ڂ�
		receiver_->RemoveAnim(frame_);
		animationEditor_->SetCurrentFrame(0);
	}
}

void AddDeleteAnimDataCommand::GetPrevAnim(void)
{
	if (!receiver_) { return; }

	GetAnim(receiver_, prevAnim_.data);
}
