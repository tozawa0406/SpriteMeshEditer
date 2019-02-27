#include "CreateSpriteCommand.h"
#include "Receiver.h"

CreateSpriteCommand::CreateSpriteCommand(void) :
	place_(-1)
	, client_(nullptr)
{
}

CreateSpriteCommand::~CreateSpriteCommand(void)
{
}

void CreateSpriteCommand::Uninit(void)
{
	if (receiver_ && place_ >= 0)
	{
		UninitDeletePtr(receiver_);
	}
}

void CreateSpriteCommand::Invoke(void)
{
}

void CreateSpriteCommand::Undo(void)
{
	if (!client_ || !receiver_) { return; }

	place_ = client_->RemoveSprite(receiver_);
}

void CreateSpriteCommand::Redo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->AddSprite(receiver_, place_);
	place_ = -1;
}
