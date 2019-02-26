#include "NameCommand.h"

NameCommand::NameCommand(void) : 
	prevName_("")
	, nextName_("")
{
}

NameCommand::~NameCommand(void)
{
}

void NameCommand::Invoke(Receiver& beforeData)
{
	if (!beforeData.name || !receiver_.name) { return; }

	prevName_ = *beforeData.name;
	nextName_ = *receiver_.name;

	if (beforeData.name) { *beforeData.name = nextName_; }
}

void NameCommand::Undo(Receiver& beforeData)
{
	if (receiver_.name)
	{
		*receiver_.name = prevName_; 
		if (beforeData.name)
		{
			*beforeData.name = prevName_;
		}
	}
}

void NameCommand::Redo(Receiver& beforeData)
{
	if (receiver_.name)
	{
		*receiver_.name = nextName_; 
		if (beforeData.name)
		{
			*beforeData.name = nextName_;
		}
	}
}
