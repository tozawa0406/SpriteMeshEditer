#include "PivotCommand.h"
#include "../Receiver.h"

PivotCommand::PivotCommand(void) : 
	prevPivot_(0)
	, nextPivot_(0)
{
}

PivotCommand::~PivotCommand(void)
{
}

void PivotCommand::Invoke(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	prevPivot_ = beforeSprite->GetPivot();
	nextPivot_ = spriteRenderer->GetPivot();

	beforeSprite->SetPivot(nextPivot_);
}

void PivotCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	const_cast<SpriteRenderer*>(spriteRenderer)->SetPivot(prevPivot_);
	beforeSprite->SetPivot(prevPivot_);
}

void PivotCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	const_cast<SpriteRenderer*>(spriteRenderer)->SetPivot(nextPivot_);
	beforeSprite->SetPivot(nextPivot_);
}
