#include "RotationCommand.h"

RotationCommand::RotationCommand(void) : 
	prevRotation_(0)
	, nextRotation_(0)
{
}

RotationCommand::~RotationCommand(void)
{
}

void RotationCommand::Invoke(void)
{
	if (!beforeData_->transform || !receiver_.transform) { return; }

	prevRotation_ = beforeData_->transform->rotation;
	nextRotation_ = receiver_.transform->rotation;

	if (beforeData_->transform) { beforeData_->transform->rotation = nextRotation_; }
}

void RotationCommand::Undo(void)
{
	if (receiver_.transform) 
	{
		receiver_.transform->rotation = prevRotation_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->rotation = prevRotation_;
		}
	}
}

void RotationCommand::Redo(void)
{
	if (receiver_.transform)
	{
		receiver_.transform->rotation = nextRotation_; 
		if (beforeData_->transform)
		{
			beforeData_->transform->rotation = nextRotation_;
		}
	}
}
