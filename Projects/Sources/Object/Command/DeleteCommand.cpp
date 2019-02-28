#include "DeleteCommand.h"
#include "Receiver.h"

DeleteCommand::DeleteCommand(void) :
	place_(-1)
	, client_(nullptr)
	, parent_(nullptr)
{
}

DeleteCommand::~DeleteCommand(void)
{
}

void DeleteCommand::Uninit(void)
{
	if (receiver_ && place_ >= 0)
	{
		UninitDeletePtr(receiver_);
	}
}

void DeleteCommand::Invoke(void)
{
	if (!client_ || !receiver_) { return; }

	place_ = client_->RemoveSprite(receiver_);
	parent_ = receiver_->GetParent();
	receiver_->SetParent(nullptr);
	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(false);
	}
}

void DeleteCommand::Undo(void)
{
	if (!client_ || !receiver_) { return; }

	client_->AddSprite(receiver_, place_);	
	receiver_->SetParent(parent_);
	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(true);
	}

	place_ = -1;
}

void DeleteCommand::Redo(void)
{
	if (!client_ || !receiver_) { return; }

	place_ = client_->RemoveSprite(receiver_);
	parent_ = receiver_->GetParent();
	receiver_->SetParent(nullptr);
	if (SpriteRenderer* renderer = const_cast<SpriteRenderer*>(receiver_->GetSpriteRenderer()))
	{
		renderer->SetEnable(false);
	}
}
