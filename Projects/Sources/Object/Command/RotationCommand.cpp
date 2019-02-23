#include "RotationCommand.h"

RotationCommand::RotationCommand(void) : 
	prevRotation_(0)
	, newRotation_(0)
{
}

RotationCommand::~RotationCommand(void)
{
}

void RotationCommand::Invoke(void)
{
}

void RotationCommand::Undo(void)
{
	reciver_->rotation = prevRotation_;
}

void RotationCommand::Redo(void)
{
	reciver_->rotation = newRotation_;
}

void RotationCommand::SetRotation(const VECTOR3& prevRotation, const VECTOR3& newRotation)
{
	prevRotation_ = prevRotation;
	newRotation_  = newRotation;
}
