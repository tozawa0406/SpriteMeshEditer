#include "ParentCommand.h"
#include "Receiver.h"

ParentCommand::ParentCommand(void) :
	add_(false)
	, chiled_()
{
}

ParentCommand::~ParentCommand(void)
{
}

void ParentCommand::Invoke(void)
{
	if (!receiver_ || !chiled_) { return; }

	if (add_)
	{
		chiled_->SetParent(receiver_);
	}
	else
	{
		chiled_->SetParent(nullptr);
	}
}

void ParentCommand::Undo(void)
{
	if (!receiver_ || !chiled_) { return; }

	if (!add_)
	{
		chiled_->SetParent(receiver_);
	}
	else
	{
		chiled_->SetParent(nullptr);
	}
}

void ParentCommand::Redo(void)
{
	if (!receiver_ || !chiled_) { return; }

	if (add_)
	{
		chiled_->SetParent(receiver_);
	}
	else
	{
		chiled_->SetParent(nullptr);
	}
}
