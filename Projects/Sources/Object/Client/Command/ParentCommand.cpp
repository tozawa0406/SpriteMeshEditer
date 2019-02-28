#include "ParentCommand.h"

ParentCommand::ParentCommand(void) :
	add_(false)
	, child_()
{
}

ParentCommand::~ParentCommand(void)
{
}

void ParentCommand::Invoke(void)
{
	if (!receiver_ || !child_) { return; }

	if (add_)
	{
		child_->SetParent(receiver_);
	}
	else
	{
		child_->SetParent(nullptr);
	}
}

void ParentCommand::Undo(void)
{
	if (!receiver_ || !child_) { return; }

	if (!add_)
	{
		child_->SetParent(receiver_);
	}
	else
	{
		child_->SetParent(nullptr);
	}
}

void ParentCommand::Redo(void)
{
	if (!receiver_ || !child_) { return; }

	if (add_)
	{
		child_->SetParent(receiver_);
	}
	else
	{
		child_->SetParent(nullptr);
	}
}
