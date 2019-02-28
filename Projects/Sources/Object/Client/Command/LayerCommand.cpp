#include "LayerCommand.h"

LayerCommand::LayerCommand(void) :
	prevLayer_(0)
	, nextLayer_(0)
{
}

LayerCommand::~LayerCommand(void)
{
}

void LayerCommand::Invoke(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	prevLayer_ = beforeSprite->GetLayer();
	nextLayer_ = spriteRenderer->GetLayer();

	beforeSprite->SetLayer(nextLayer_);
}

void LayerCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	const_cast<SpriteRenderer*>(spriteRenderer)->SetLayer(prevLayer_);
	beforeSprite->SetLayer(prevLayer_);
}

void LayerCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	const_cast<SpriteRenderer*>(spriteRenderer)->SetLayer(nextLayer_);
	beforeSprite->SetLayer(nextLayer_);
}
