#include "TextureNumCommand.h"

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
	if (!beforeData_->spriteRenderer || !receiver_.spriteRenderer) { return; }

	prevTexNum_ = beforeData_->spriteRenderer->GetTexture();
	nextTexNum_ = receiver_.spriteRenderer->GetTexture();

	beforeData_->spriteRenderer->SetTexture(nextTexNum_);
}

void TextureNumCommand::Undo(void)
{
	if (receiver_.spriteRenderer)
	{
		receiver_.spriteRenderer->SetTexture(prevTexNum_);
		if (beforeData_->spriteRenderer)
		{
			beforeData_->spriteRenderer->SetTexture(prevTexNum_);
		}
	}
}

void TextureNumCommand::Redo(void)
{
	if (receiver_.spriteRenderer)
	{
		receiver_.spriteRenderer->SetTexture(nextTexNum_); 
		if (beforeData_->spriteRenderer)
		{
			beforeData_->spriteRenderer->SetTexture(nextTexNum_);
		}
	}
}
