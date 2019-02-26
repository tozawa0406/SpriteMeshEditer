#include "NameCommand.h"

NameCommand::NameCommand(void) : 
	prevName_("")
	, nextName_("")
{
}

NameCommand::~NameCommand(void)
{
}

void NameCommand::Invoke(void)
{
	if (!beforeData_->name || !receiver_.name) { return; }

	prevName_ = *beforeData_->name;
	nextName_ = *receiver_.name;

	if (beforeData_->name) { *beforeData_->name = nextName_; }
}

void NameCommand::Undo(void)
{
	if (receiver_.name)
	{
		*receiver_.name = prevName_; 
		if (beforeData_->name)
		{
			*beforeData_->name = prevName_;
		}
	}
}

void NameCommand::Redo(void)
{
	if (receiver_.name)
	{
		*receiver_.name = nextName_; 
		if (beforeData_->name)
		{
			*beforeData_->name = nextName_;
		}
	}
}
