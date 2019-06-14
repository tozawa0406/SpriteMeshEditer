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

	// 同フレームの前回のアニメーションを取得
	GetPrevAnim();

	// (追加時)現在のフレームの姿勢をアニメーションとして追加
	if (add_) { receiver_->AddAnim(frame_); }

	// アニメーション情報の取得
	GetAnim(receiver_, anim_.data);

	// (削除時)指定したフレームのアニメーションを削除
	if (!add_) { receiver_->RemoveAnim(frame_); }
}

void AddDeleteAnimDataCommand::GetAnim(Receiver* receiver, SPRITE_MESH_ANIM_DATA& animData)
{
	// 全アニメーション情報の取得
	auto receiverAnimData = receiver->GetAnimTransform();

	int size = static_cast<int>(receiverAnimData.size());
	for (int i = 0; i < size; ++i)
	{
		// 指定したフレームと同じフレームのアニメーションを取得
		if (receiverAnimData[i].frame == frame_)
		{
			animData.anim.emplace_back(receiverAnimData[i]);
		}
	}

	// 子要素にも適用
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

	// 追加時
	if (add_)	
	{
		// 削除
		receiver_->RemoveAnim(frame_);

		// 前回情報がえれば、それを追加
		if (prevAnim_.data.anim.size() > 0)
		{
			receiver_->AddAnim(frame_, prevAnim_.data, 0);
			animationEditor_->SetCurrentFrame(frame_);
		}
		// なければ0フレーム目へ
		else
		{
			animationEditor_->SetCurrentFrame(0);
		}
	}
	// 削除時
	else 
	{
		// アニメーションを戻す(追加)
		receiver_->AddAnim(frame_, anim_.data, 0);
		// 表示をそのアニメーションのフレームに
		animationEditor_->SetCurrentFrame(frame_);
	}
}

void AddDeleteAnimDataCommand::Redo(void)
{
	if (!receiver_) { return; }

	// 追加時
	if (add_)	
	{
		// 再度追加し、表示をそのアニメーションに
		receiver_->AddAnim(frame_, anim_.data, 0);
		animationEditor_->SetCurrentFrame(frame_);
	}
	// 削除時
	else
	{
		// 削除し、表示を0フレーム目に
		receiver_->RemoveAnim(frame_);
		animationEditor_->SetCurrentFrame(0);
	}
}

void AddDeleteAnimDataCommand::GetPrevAnim(void)
{
	if (!receiver_) { return; }

	GetAnim(receiver_, prevAnim_.data);
}
