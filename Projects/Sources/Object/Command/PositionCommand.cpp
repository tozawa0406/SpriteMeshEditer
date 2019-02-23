#include "PositionCommand.h"

PositionCommand::PositionCommand(void) : 
	prevPosition_(0)
	, newPosition_(0)
{
}

PositionCommand::~PositionCommand(void)
{
}

void PositionCommand::Invoke(void)
{
}

void PositionCommand::Undo(void)
{
	reciver_->position = prevPosition_;
}

void PositionCommand::Redo(void)
{
	reciver_->position = newPosition_;
}

void PositionCommand::SetPosition(const VECTOR3& prevPosition, const VECTOR3& newPosition)
{
	prevPosition_ = prevPosition;
	newPosition_  = newPosition;
}
