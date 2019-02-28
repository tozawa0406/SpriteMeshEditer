#include "TextureNumCommand.h"
#include "../Receiver.h"

TextureNumCommand::TextureNumCommand(void) : 
	prevTexNum_(0)
	, nextTexNum_(0)
{
}

TextureNumCommand::~TextureNumCommand(void)
{
}

void TextureNumCommand::Invoke(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = receiver_->GetSpriteRenderer();
	auto beforeSprite   = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	prevTexNum_ = beforeSprite->GetTexture();
	nextTexNum_ = spriteRenderer->GetTexture();

	beforeSprite->SetTexture(prevTexNum_);
}

void TextureNumCommand::Undo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer());
	auto beforeSprite   = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	spriteRenderer->SetTexture(prevTexNum_);
	beforeSprite->SetTexture(prevTexNum_);
}

void TextureNumCommand::Redo(void)
{
	if (!receiver_) { return; }
	auto spriteRenderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer());
	auto beforeSprite = receiver_->GetBeforeData().spriteRenderer;
	if (!spriteRenderer || !beforeSprite) { return; }

	spriteRenderer->SetTexture(nextTexNum_);
	beforeSprite->SetTexture(nextTexNum_);
}
