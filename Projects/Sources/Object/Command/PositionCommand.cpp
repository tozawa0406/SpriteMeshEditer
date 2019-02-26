#include "PositionCommand.h"

PositionCommand::PositionCommand(void) : 
	prevPosition_(0)
	, nextPosition_(0)
{
}

PositionCommand::~PositionCommand(void)
{
}

void PositionCommand::Invoke(void)
{
	if (!beforeData_->transform || !receiver_.transform) { return; }

	prevPosition_ = beforeData_->transform->position;
	nextPosition_ = receiver_.transform->position;

	if (beforeData_->transform) { beforeData_->transform->position = nextPosition_; }
}

void PositionCommand::Undo(void)
{
	if (receiver_.transform) 
	{
		receiver_.transform->position = prevPosition_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->position = prevPosition_;
		}
	}
}

void PositionCommand::Redo(void)
{
	if (receiver_.transform) 
	{
		receiver_.transform->position = nextPosition_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->position = nextPosition_;
		}
	}
}
