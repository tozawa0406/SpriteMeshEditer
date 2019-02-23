#include "SetPositionCommand.h"

SetPositionCommand::SetPositionCommand(void) : 
	prevPosition_(0)
	, newPosition_(0)
{
}

SetPositionCommand::~SetPositionCommand(void)
{
}

void SetPositionCommand::Invoke(void)
{
}

void SetPositionCommand::Undo(void)
{
	reciver_->position = prevPosition_;
}

void SetPositionCommand::Redo(void)
{
	reciver_->position = newPosition_;
}

void SetPositionCommand::SetPosition(const VECTOR3& prevPositoin, const VECTOR3& newPosition)
{
	prevPosition_ = prevPositoin;
	newPosition_  = newPosition;
}
