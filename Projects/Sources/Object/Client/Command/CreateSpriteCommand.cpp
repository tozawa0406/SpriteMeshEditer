#include "CreateSpriteCommand.h"

CreateSpriteCommand::CreateSpriteCommand(void) :
	client_(nullptr)
{
}

CreateSpriteCommand::~CreateSpriteCommand(void)
{
}

void CreateSpriteCommand::Uninit(void)
{
	UninitDeletePtr(receiver_);
}

void CreateSpriteCommand::Invoke(void)
{
}

void CreateSpriteCommand::Undo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->RemoveSprite(receiver_);
}

void CreateSpriteCommand::Redo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->AddSprite(receiver_);
}
