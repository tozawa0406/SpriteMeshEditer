#include "PositionCommand.h"

PositionCommand::PositionCommand(void) : 
	prevPosition_(0)
	, nextPosition_(0)
{
}

PositionCommand::~PositionCommand(void)
{
}

void PositionCommand::Invoke(Receiver& beforeData)
{
	if (!beforeData.transform || !receiver_.transform) { return; }

	prevPosition_ = beforeData.transform->position;
	nextPosition_ = receiver_.transform->position;

	if (beforeData.transform) { beforeData.transform->position = nextPosition_; }
}

void PositionCommand::Undo(Receiver& beforeData)
{
	if (receiver_.transform) 
	{
		receiver_.transform->position = prevPosition_; 
		if (beforeData.transform)
		{
			beforeData.transform->position = prevPosition_;
		}
	}
}

void PositionCommand::Redo(Receiver& beforeData)
{
	if (receiver_.transform) 
	{
		receiver_.transform->position = nextPosition_; 
		if (beforeData.transform)
		{
			beforeData.transform->position = nextPosition_;
		}
	}
}
